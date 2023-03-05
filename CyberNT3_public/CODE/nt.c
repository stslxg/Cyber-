/** ###################################################################
**     Filename  : nt.C
**     Project   : CyberNT3
**     Processor : 56F8366
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/11, 15:22
**     Contents  :
**         User source code
**
** ###################################################################*/

/* MODULE nt */

#include "world.h"

#define THRESHOLD_MIN (32)
#define INITIAL_LINE_WIDTH (11)
#define SCORE_MAX (COLS / 3)
#define LINE_WIDTH_MAX (25)
#define LOSE_LIMIT (15)

#define MAX_STARTLINE_ROW (20)
#define STARTLINE_WIDTH_MIN (23)
#define STARTLINE_WIDTH_MAX (48)
#define STARTLINE_STEP_MIN (8)
#define STARTLINE_STEP_MAX (18)

// global variables
static volatile U8 NtpIsRunning;
static volatile U8 NtpIsStopping;
static S32 NtpCurrentDistance;
#define CURRENT_DISTANCE (S16)(NtpCurrentDistance / 3400)
static S16 NtpStopDistance;
static U16 NtpPreviousAccumulator[2];
static U8 NtpFrameIndex;
static volatile AFE_FRAME *NtpFrame;
static U8 NtpSendFrame;
static U8 NtpRow;
static S16 NtpPredictPosition;
static U8 NtpServoHistoryIndex;
static S16 NtpServoHistory[3];

// memorial variables
static S16 NtpLineWidth;
static S16 NtpLineFirstDerivative;
static U8 NtpLastValidRow;
static S16 NtpLosingCounter;

// result variables
static U8 NtpWidth[ROWS];
static U8 NtpPosition[ROWS];

// startline merge
static U8 NtpLeftStartLineCount, NtpRightStartLineCount;
static U8 NtpLeftStartLine, NtpRightStartLine;
static U8 NtpMaybeLeftStartLineCount, NtpMaybeRightStartLineCount;
static U8 NtpMaybeLeftStartLine, NtpMaybeRightStartLine;

// speed merge
static S16 NtpRowHistory[4];
static U8 NtpRowHistoryIndex;
static U8 NtpIsHighSpeed;
static S16 NtpBrakeCount;

// controller context
static mc_sPIDparams NtpServoPidCtx;

// tables
S16 NtpTransformTable[ROWS];

static void NtpKeyHandler(U8 key)
{
	if (key == HMI_KEY_A) {
		NtpIsRunning = 0;
	}
}

static inline U8 NtpCheckforStartLine(void)
{
#define MAX_STARTLINE_LENGTH (5)
#define MAX_STARTLINE_GAP (3)
#define MAX_CONFIRM_STARTLINE_ROW (15)
	if(NtpLeftStartLineCount && NtpRightStartLineCount
		&& NtpLeftStartLineCount <= MAX_STARTLINE_LENGTH && NtpRightStartLineCount <= MAX_STARTLINE_LENGTH) {
		if (RtlAbsS16(NtpLeftStartLine - NtpRightStartLine) <= MAX_STARTLINE_GAP) {
			return 1;
		}
	}
	if (NtpMaybeLeftStartLineCount && NtpRightStartLineCount
		&& NtpMaybeLeftStartLineCount <= MAX_STARTLINE_LENGTH && NtpRightStartLineCount <= MAX_STARTLINE_LENGTH
		&& NtpMaybeLeftStartLine < MAX_CONFIRM_STARTLINE_ROW) {
		if (RtlAbsS16(NtpMaybeLeftStartLine - NtpRightStartLine) <= MAX_STARTLINE_GAP) {
			return 1;
		}
	}
	if (NtpLeftStartLineCount && NtpMaybeRightStartLineCount
		&& NtpLeftStartLineCount <= MAX_STARTLINE_LENGTH && NtpMaybeRightStartLineCount <= MAX_STARTLINE_LENGTH
		&& NtpMaybeRightStartLine < MAX_CONFIRM_STARTLINE_ROW) {
		if (RtlAbsS16(NtpLeftStartLine - NtpMaybeRightStartLine) <= MAX_STARTLINE_GAP) {
			return 1;
		}
	}
	return 0;
}


static inline void NtpInitRunning(void)
{
	S16 row;
	
	NtpIsRunning = 1;
	NtpIsStopping = 0;
	NtpCurrentDistance = 0;
	NtpStopDistance = 32767;
	RtlQueryPulseAccumulator(NtpPreviousAccumulator);
	NtpFrameIndex = AfeFrameIndex;
	NtpPredictPosition = COLS / 2 - 1;
	NtpServoHistoryIndex = 0;
	NtpServoHistory[0] = 0;
	NtpServoHistory[1] = 0;
	NtpServoHistory[2] = 0;
	NtpRowHistory[0] = 0;
	NtpRowHistory[1] = 0;
	NtpRowHistory[2] = 0;
	NtpRowHistory[3] = 0;
	NtpRowHistoryIndex = 0;
	NtpIsHighSpeed = 0;
	NtpBrakeCount = 0;

	for (row = 0; row < ROWS; ++row) {
		NtpTransformTable[row] = RtlScaleS16(RtlSquareS16(ROWS - row),
			0, ROWS * ROWS, 1000, CFG(TRANSFORM_NEAR) * 100);
	}

	NtpServoPidCtx.ProportionalGain = CFG(SERVO_P);
	NtpServoPidCtx.ProportionalGainScale = 1;
	NtpServoPidCtx.IntegralGain = 0;
	NtpServoPidCtx.IntegralGainScale = 0;
	NtpServoPidCtx.DerivativeGain = CFG(SERVO_D);
	NtpServoPidCtx.DerivativeGainScale = 0;
	NtpServoPidCtx.PositivePIDLimit = SERVO_RANGE;
	NtpServoPidCtx.NegativePIDLimit = -SERVO_RANGE;
	NtpServoPidCtx.IntegralPortionK_1 = 0;
	NtpServoPidCtx.InputErrorK_1 = 0;
}

static inline void NtpProcessRow(void)
{
	static S16 edge[COLS];
	U32 sum;
	U8 *data;
	U8 col;
	S16 t;
	S16 threshold;
	U8 rising[5];
	U8 risingIndex;
	U8 i;
	
#define STARTLINE_MAX (20)
#define MAYBE_STARTLINE_MAX (5)
	U8 startLine[STARTLINE_MAX + 1][2];
	U8 startLineIndex;
	U8 maybeLeftStartLine[MAYBE_STARTLINE_MAX + 1], maybeRightStartLine[MAYBE_STARTLINE_MAX + 1];
	U8 maybeLeftStartLineIndex, maybeRightStartLineIndex;
	static U8 noStartLine = 0;
	
	S16 width, pos, firstDeriv;
	S32 score;
	S32 bestScore = SCORE_MAX;
	S16 bestWidth, bestPos, bestFirstDeriv;
	S16 row;
	
	if (NtpLosingCounter > LOSE_LIMIT) {
		goto NtpProcessRow_SkipScanning;
	}
	
	sum = 0;
	data = (U8 *)&NtpFrame->data[NtpRow][0];
	while (NtpFrame->ready <= NtpRow) { /* spin wait */ }
	for (col = 1; col < COLS - 2; ++col) {
		t = (S16)data[col - 1] - (S16)data[col + 2];
		edge[col] = t;
		if (t >= 0) {
			sum += t;
		} else {
			sum -= t;
		}
	}
	threshold = (S16)(sum / (COLS - 3));
	if (threshold < THRESHOLD_MIN) {
		threshold = THRESHOLD_MIN;
	}
	
	rising[0] = 0;
	rising[1] = 0;
	rising[2] = 0;
	rising[3] = 0;
	rising[4] = 0; // end mark
	risingIndex = 0;
	
	for (i = 0; i < STARTLINE_MAX + 1; ++i) {
		startLine[i][0] = 0;
		startLine[i][1] = 0;
	}
	startLineIndex = 0;
	
	for (i = 0; i < MAYBE_STARTLINE_MAX + 1; ++i) {
		maybeLeftStartLine[i] = 0;
		maybeRightStartLine[i] = 0;
	}
	maybeLeftStartLineIndex = 0;
	maybeRightStartLineIndex = 0;
	
	edge[0] = 0;
	edge[COLS - 2] = 0;
	
	for (col = 1; col < COLS - 2; ++col) {
		if (edge[col] >= threshold) {
			if (edge[col] >= edge[col - 1] && edge[col] > edge[col + 1]) {
				rising[risingIndex] = col;
				if (++risingIndex == 4) {
					risingIndex = 0;
				}
				
				// maybe right startline
				maybeRightStartLine[maybeRightStartLineIndex] = col;
				if (++maybeRightStartLineIndex == MAYBE_STARTLINE_MAX) {
					maybeRightStartLineIndex = 0;
				}	
			}
		} else if (edge[col] <= -threshold) {
			if (edge[col] < edge[col - 1] && edge[col] <= edge[col + 1]) {
				// maybe left startline
				if (maybeLeftStartLineIndex < MAYBE_STARTLINE_MAX) {
					maybeLeftStartLine[maybeLeftStartLineIndex] = col;
					++maybeLeftStartLineIndex;
				}				
				for (i = 0; rising[i] != 0; ++i) {
					width = (S16)col - (S16)rising[i];
					// startline
					if (STARTLINE_WIDTH_MIN <= width && width <= STARTLINE_WIDTH_MAX){
						startLine[startLineIndex][0] = rising[i];
						startLine[startLineIndex][1] = col;
						if (++startLineIndex == STARTLINE_MAX) {
							startLineIndex = 0;
						}
					}

					if (width > LINE_WIDTH_MAX) {
						continue;
					}
					pos = ((S16)rising[i] + (S16)col) / 2;
					
					// scoring
					if (NtpLastValidRow == 0xff) {
						firstDeriv = 0;
						score = RtlSquareS32(width - NtpLineWidth) * 2
							+ RtlAbsS16(pos - NtpPredictPosition);
					} else {
						firstDeriv = (pos - NtpPosition[NtpLastValidRow]) / (NtpRow - NtpLastValidRow);
						score = RtlSquareS32(width - NtpLineWidth) * 2
							+ RtlAbsS16(firstDeriv)
							+ RtlSquareS32(firstDeriv - NtpLineFirstDerivative);
					}
					
					if (score < bestScore) {
						bestScore = score;
						bestWidth = width;
						bestPos = pos;
						bestFirstDeriv = firstDeriv;
					}
				}
			}
		}
	}
	
NtpProcessRow_SkipScanning:
	if (bestScore < SCORE_MAX) {
		// current row found
		NtpLineWidth = (NtpLineWidth + bestWidth) / 2;
		NtpLineFirstDerivative = (NtpLineFirstDerivative + bestFirstDeriv) / 2;
		
		// interpolation
		if (NtpLastValidRow != 0xff) {
			for (row = NtpLastValidRow + 1; row < NtpRow; ++row) {
				NtpWidth[row] = (U8)RtlScaleS16(row, NtpLastValidRow, NtpRow,
					NtpWidth[NtpLastValidRow], bestWidth);
				NtpPosition[row] = (U8)RtlScaleS16(row, NtpLastValidRow, NtpRow,
					NtpPosition[NtpLastValidRow], bestPos);
			}
		}
		
		NtpWidth[NtpRow] = (U8)bestWidth;
		NtpPosition[NtpRow] = (U8)bestPos;
		NtpLastValidRow = NtpRow;
		
		// bestPos: [1, COLS-3 = 169]
		
		NtpLosingCounter = RtlScaleS16(NtpRow, 0, ROWS - 1, 0, LOSE_LIMIT);
	} else {
		// current row lost
		NtpWidth[NtpRow] = 0;
		NtpPosition[NtpRow] = 0;
		++NtpLosingCounter;
	}
	
#define MAX_NO_STARTLINE (4)
	// startline
	if (NtpPosition[NtpRow] == 0) {
		noStartLine = MAX_NO_STARTLINE;
	} else {
		if (noStartLine > 0) {
			--noStartLine;
		}
	}
	if (NtpRow < MAX_STARTLINE_ROW && NtpPosition[NtpRow] != 0 && noStartLine == 0) {
		// left
		for (i = 0; startLine[i][0] != 0; ++i) {
			if ((S16)NtpPosition[NtpRow] - (S16)NtpWidth[NtpRow] / 2 - (S16)STARTLINE_STEP_MAX <= (S16)startLine[i][1]
			&& (S16)startLine[i][1] <= (S16)NtpPosition[NtpRow] - (S16)NtpWidth[NtpRow] / 2 - (S16)STARTLINE_STEP_MIN) {
				++NtpLeftStartLineCount;
				if (NtpLeftStartLineCount == 1) {
					NtpLeftStartLine = NtpRow;
				}
				goto HasLeftStartLine;
			}
		}
		//maybe left
		for (i = 0;maybeLeftStartLine[i] != 0; ++i) {
			if ((S16)NtpPosition[NtpRow] - (S16)NtpWidth[NtpRow] / 2 - (S16)STARTLINE_STEP_MAX <= (S16)maybeLeftStartLine[i]
			&& (S16)maybeLeftStartLine[i] <= (S16)NtpPosition[NtpRow] - (S16)NtpWidth[NtpRow] / 2 - (S16)STARTLINE_STEP_MIN
			&& (S16)maybeLeftStartLine[i] - (S16)STARTLINE_WIDTH_MAX <= 1) {
				++NtpMaybeLeftStartLineCount;
				if (NtpMaybeLeftStartLineCount == 1) {
					NtpMaybeLeftStartLine = NtpRow;
				}
				break;
			}
		}
HasLeftStartLine:
		// right
		for (i = 0; startLine[i][0] != 0; ++i) {
			if ((S16)NtpPosition[NtpRow] + (S16)NtpWidth[NtpRow] / 2 + (S16)STARTLINE_STEP_MIN <= (S16)startLine[i][0]
			&& (S16)startLine[i][0] <= (S16)NtpPosition[NtpRow] + (S16)NtpWidth[NtpRow] / 2 + (S16)STARTLINE_STEP_MAX) {
				++NtpRightStartLineCount;
				if (NtpRightStartLineCount == 1) {
					NtpRightStartLine = NtpRow;
				}
				goto HasRightStartLine;
			}
		}
		//maybe right
		for (i = 0; maybeRightStartLine[i] != 0; ++i) {
			if ((S16)NtpPosition[NtpRow] + (S16)NtpWidth[NtpRow] / 2 + (S16)STARTLINE_STEP_MIN <= (S16)maybeRightStartLine[i]
			&& (S16)maybeRightStartLine[i] <= (S16)NtpPosition[NtpRow] + (S16)NtpWidth[NtpRow] / 2 + (S16)STARTLINE_STEP_MAX
			&& (S16)maybeRightStartLine[i] + (S16)STARTLINE_WIDTH_MAX >= COLS - 2) {
				++NtpMaybeRightStartLineCount;
				if (NtpMaybeRightStartLineCount == 1) {
					NtpMaybeRightStartLine = NtpRow;
				}
				break;
			}
		}
HasRightStartLine:
		;
	}
	
	switch (NtpSendFrame) {
	case 'g':
		SciSendData((U8 *)&NtpFrame->data[NtpRow], COLS);
		break;
	case 'b':
		SciSendByte(NtpPosition[NtpRow]);
		SciSendByte((U8)bestScore);
		break;
	case 'r':
		SciSendData((U8 *)&NtpFrame->data[NtpRow], COLS);
		SciSendByte(NtpPosition[NtpRow]);
		break;
	}
}

static inline S16 NtpGetScaledPosition(S16 row)
{
	if (NtpPosition[row] == 0) {
		return 0;
	}
	return (S16)((S32)(NtpPosition[row] - (COLS/2 - 1)) * NtpTransformTable[row] / 1000) + (COLS/2 - 1);
}

static void NtpProcessFrame(void)
{
	S16 servo = 0;
	U8 frameIndex;
	U8 predictCount;
	S16 predictSum;
	S16 leftSpeed, rightSpeed;
	S16 position;
	S16 row;
	S16 servoRow;
	S32 fixSum, fixCount;
	S16 maxRow;
	
	NtpLineWidth = INITIAL_LINE_WIDTH;
	NtpLineFirstDerivative = 0;
	NtpLastValidRow = 0xff;
	NtpLosingCounter = 0;
	
	NtpLeftStartLineCount = 0;
	NtpRightStartLineCount = 0;
	
	NtpMaybeLeftStartLineCount = 0;
	NtpMaybeRightStartLineCount = 0;
		
	while ((frameIndex = AfeFrameIndex) == NtpFrameIndex) { /* spin wait */ }
	NtpFrameIndex = frameIndex;
	NtpFrame = &AfeFrameBuffer[frameIndex & 1];
	NtpSendFrame = SciReadEvent();
	
	predictSum = 0;
	predictCount = 0;
	maxRow = 0;
	for (NtpRow = 0; NtpRow < ROWS; ++NtpRow) {
		NtpProcessRow();
#define PREDICT_ROW (10)
		if (NtpPosition[NtpRow] != 0) {
			if (NtpRow < PREDICT_ROW) {
				predictSum += NtpPosition[NtpRow];
				++predictCount;
			}
			maxRow = NtpRow;
		}
	}
	
	if (++NtpRowHistoryIndex > 3) {
		NtpRowHistoryIndex = 0;
	}
	NtpRowHistory[NtpRowHistoryIndex] = maxRow;
	
	if (predictCount != 0) {
		NtpPredictPosition = (NtpPredictPosition + predictSum / predictCount) / 2;
	}
	
	servoRow = RtlScaleS16(RtlLimitS16(DrvQueryCurrentSpeed(), 0, 500),
		0, 500, CFG(SERVO_ROW_LOW), CFG(SERVO_ROW_HIGH));
	position = NtpGetScaledPosition(servoRow);
	if (position != 0) {
		fixSum = fixCount = 0;
		for (row = servoRow + 1; row < ROWS; ++row) {
			if (NtpPosition[row] != 0) {
				fixSum += (S32)NtpGetScaledPosition(row) * row * row;
				fixCount += (S32)row * row;
			}
		}
		if (fixCount != 0) {
			position = RtlLimitS16((S16)(fixSum / fixCount),
				position - CFG(TRACK_WIDTH), position + CFG(TRACK_WIDTH));
		}
		position -= (COLS / 2 - 1);
	} else {
		position = (NtpServoHistory[0] + NtpServoHistory[1] + NtpServoHistory[2]) / 2;
	}
	if (position < -COLS/2) {
		position = -COLS/2;
	} else if (position > COLS/2) {
		position = COLS/2;
	}
	NtpServoHistory[NtpServoHistoryIndex] = position;
	if (++NtpServoHistoryIndex == 3) {
		NtpServoHistoryIndex = 0;
	}
	
	if (!NtpIsStopping) {
		if (CURRENT_DISTANCE > CFG(STARTLINE_DISTANCE) * 10 && NtpCheckforStartLine()) {
			NtpIsStopping = 1;
			NtpStopDistance = CURRENT_DISTANCE + 8;
		}
	}
	
	if (CURRENT_DISTANCE >= NtpStopDistance) {
		leftSpeed = rightSpeed = 0;
		NtpIsHighSpeed = 0;
		NtpBrakeCount = 0;
	} else if (NtpPosition[CFG(HIGHSPEED_ROW)] != 0) {
		leftSpeed = rightSpeed = CFG(SPEED_HIGH) * 20;
		NtpIsHighSpeed = 1;
		NtpBrakeCount = 0;
	} else if (((NtpRowHistory[NtpRowHistoryIndex] - NtpRowHistory[(NtpRowHistoryIndex + 3) % 4]) >= CFG(ACC_LM))
		&& ((NtpRowHistory[(NtpRowHistoryIndex + 3) % 4] - NtpRowHistory[(NtpRowHistoryIndex + 2) % 4]) >= CFG(ACC_LM))
		&& ((NtpRowHistory[(NtpRowHistoryIndex + 2) % 4] - NtpRowHistory[(NtpRowHistoryIndex + 1) % 4]) >= CFG(ACC_LM))) {
		leftSpeed = rightSpeed = CFG(SPEED_HIGH) * 20;
		NtpIsHighSpeed = 0;
		NtpBrakeCount = 0;
	} else if (NtpPosition[CFG(MIDSPEED_ROW)] != 0) {
		if (NtpIsHighSpeed) {
			leftSpeed = rightSpeed = CFG(SPEED_LOW) * 20;
			NtpIsHighSpeed = 0;
			NtpBrakeCount = CFG(BRAKE_COUNT);
		} else if (--NtpBrakeCount < 0) {
			leftSpeed = rightSpeed = CFG(SPEED_MID) * 20;
			NtpBrakeCount = 0;
		} else {
			leftSpeed = rightSpeed = CFG(SPEED_LOW) * 20;
			NtpIsHighSpeed = 0;
		}
	} else {
		leftSpeed = rightSpeed = CFG(SPEED_LOW) * 20;
		NtpIsHighSpeed = 0;
		NtpBrakeCount = 0;
	}
	
	servo = PID_controllerPIDtype1(0, position, &NtpServoPidCtx);
	if (servo > 0) {
		rightSpeed = RtlScaleS16(servo, 0, SERVO_RANGE, rightSpeed,
			rightSpeed / 2);
	} else if (servo < 0) {
		leftSpeed = RtlScaleS16(-servo, 0, SERVO_RANGE, leftSpeed,
			leftSpeed / 2);
	}
	
	if (CFG(CONTROL_LEVEL) >= 1) {
		DrvEmitPulseServo(servo);
	}
	DrvSetSpeedMotor(leftSpeed, rightSpeed);
}

static void NtpMeasureDistance(void)
{
	U16 data[2];
	volatile S16 delta;
	
	RtlQueryPulseAccumulator(data);
	delta = (S16)(data[0] - NtpPreviousAccumulator[0]);
	NtpCurrentDistance += delta;
	delta = (S16)(data[1] - NtpPreviousAccumulator[1]);
	NtpCurrentDistance += delta;
	
	NtpPreviousAccumulator[0] = data[0];
	NtpPreviousAccumulator[1] = data[1];	
}

void NtSystemEntry(void)
{
	HMI_HANDLER *oldHandler;
	
	NtpInitRunning();
	if (CFG(CONTROL_LEVEL) >= 2) {
		DrvEnableMotorControl();
	}
	oldHandler = HmiAttachHandler(&NtpKeyHandler);
	while (NtpIsRunning) {
		NtpProcessFrame();
		NtpMeasureDistance();
		HmiShowNumberEx((U16)CURRENT_DISTANCE, 0x20);
	}
	HmiDetachHandler(oldHandler);
	DrvDisableMotorControl();
}

/* END nt */

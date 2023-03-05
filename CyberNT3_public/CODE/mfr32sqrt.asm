;********************************************************************
;
; (c) Freescale Semiconductor
; 2004 All Rights Reserved
;
;********************************************************************
    SECTION rtlib

    include "portasm.h"

    GLOBAL  Fmfr32Sqrt

; Frac16 mfr32Sqrt (Frac32 x);

;    Register usage:
;       A  - Frac32 input value
;       B  - Temp
;       Y0 - Frac16 output value
;       X0 - Temp
;
        ORG  P:
Fmfr32Sqrt:
        move.w    #$4000,Y0
        mpy     Y0,Y0,B
        cmp     A,B
        nop
        ble     TooSmall14
        bfclr   #$4000,Y0
TooSmall14:
        bfset   #$2000,Y0
        mpy     Y0,Y0,B
        cmp     A,B
        nop
        ble     TooSmall13
        bfclr   #$2000,Y0
TooSmall13:
        bfset   #$1000,Y0
        mpy     Y0,Y0,B
        cmp     A,B
        nop
        ble     TooSmall12
        bfclr   #$1000,Y0
TooSmall12:
        bfset   #$0800,Y0
        mpy     Y0,Y0,B
        cmp     A,B
        nop
        ble     TooSmall11
        bfclr   #$0800,Y0
TooSmall11:
        bfset   #$0400,Y0
        mpy     Y0,Y0,B
        cmp     A,B
        nop
        ble     TooSmall10
        bfclr   #$0400,Y0
TooSmall10:
        bfset   #$0200,Y0
        mpy     Y0,Y0,B
        cmp     A,B
        nop
        ble     TooSmall9
        bfclr   #$0200,Y0
TooSmall9:
        bfset   #$0100,Y0
        mpy     Y0,Y0,B
        cmp     A,B
        nop
        ble     TooSmall8
        bfclr   #$0100,Y0
TooSmall8:
        bfset   #$0080,Y0
        mpy     Y0,Y0,B
        cmp     A,B
        nop
        ble     TooSmall7
        bfclr   #$0080,Y0
TooSmall7:
        bfset   #$0040,Y0
        mpy     Y0,Y0,B
        cmp     A,B
        nop
        ble     TooSmall6
        bfclr   #$0040,Y0
TooSmall6:
        bfset   #$0020,Y0
        mpy     Y0,Y0,B
        cmp     A,B
        nop
        ble     TooSmall5
        bfclr   #$0020,Y0
TooSmall5:
        bfset   #$0010,Y0
        mpy     Y0,Y0,B
        cmp     A,B
        nop
        ble     TooSmall4
        bfclr   #$0010,Y0
TooSmall4:
        bfset   #$0008,Y0
        mpy     Y0,Y0,B
        cmp     A,B
        nop
        ble     TooSmall3
        bfclr   #$0008,Y0
TooSmall3:
        bfset   #$0004,Y0
        mpy     Y0,Y0,B
        cmp     A,B
        nop
        ble     TooSmall2
        bfclr   #$0004,Y0
TooSmall2:
        bfset   #$0002,Y0
        mpy     Y0,Y0,B
        cmp     A,B
        nop
        ble     TooSmall1
        bfclr   #$0002,Y0
TooSmall1:
        bfset   #$0001,Y0
        mpy     Y0,Y0,B
        cmp     A,B
        nop
        ble     TooSmall0
        bfclr   #$0001,Y0
TooSmall0:
        mpy     Y0,Y0,B
        sub     A,B
        abs     B
        adda    #2,SP
        move.l  B10,x:(SP)
        move.w  Y0,X0
        inc.w   Y0
        mpy     Y0,Y0,B
        sub     A,B
        abs     B
        move.l  x:(SP)-,A
        cmp     A,B
        blt     UseEstR
        move.w  X0,Y0
UseEstR:
        rts

    ENDSEC
    END

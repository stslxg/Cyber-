/** ###################################################################
**
**     (c) Freescale Semiconductor
**     2004 All Rights Reserved
**
**
**     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* File: mempx.h */

#ifndef __MEMPX_H
#define __MEMPX_H

#include "port.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************
* Types of Memory
*******************************************************/

typedef enum
{
  XData,
  PData
} mem_eMemoryType;


/*******************************************************
* Routines for P <-> X Memory Access
*
* - All routines can be used in pragma interrupt ISRs
*
*******************************************************/

#define memset   memMemset
#define memSetP  memMemsetP
#define memcpy   memMemcpy

UWord16 memReadP16 (UWord16 *pSrc);
UWord32 memReadP32 (UWord32 *pSrc);

void   memWriteP16 (UWord16 Data, Word16 *pDest);
void   memWriteP32 (UWord32 Data, Word32 *pDest);

void * memCopyXtoP (void *pDest, const void *pSrc, ssize_t Count);
void * memCopyPtoX (void *pDest, const void *pSrc, ssize_t Count);
void * memCopyPtoP (void *pDest, const void *pSrc, ssize_t Count);

void * memMemset   (void *dest, int c, ssize_t count);
void * memMemcpy   (void *dest, const void *src, ssize_t count);

void * memMemsetP  (void *dest, int c, ssize_t count);

/*******************************************************/
#ifdef __cplusplus
}
#endif

#endif

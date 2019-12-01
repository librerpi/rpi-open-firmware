/*=============================================================================
Copyright (c) 2007 Broadcom Europe Limited. All rights reserved.

Project  :  BCM2707
Module   :  CRYPTO wrapper hardware header
File     :  $RCSfile: cryptohw.h,v $
Revision :  $Revision: 1.2 $

FILE DESCRIPTION
Definition of bits within CRYPTO hardware registers.
=============================================================================*/

#ifndef CRYPTOHW_H
#define CRYPTOHW_H

#define CRYPTO_READFIELD(_w,_f) (((unsigned long)(_w) / _f) & (_f##_MASK))

enum
{
// BCM2707 has no crypto hardware but supports RNG interrupt
//   CRYPTO_ISR_RNG_INT = (1 << 2),
   CRYPTO_ISR_PKA_INT = (1 << 1),
   CRYPTO_ISR_SPU_INT = (1 << 0),
   
//   CRYPTO_IMR_RNG_INT_EN = (1 << 2),
   CRYPTO_IMR_PKA_INT_EN = (1 << 1),
   CRYPTO_IMR_SPU_INT_EN = (1 << 0),
   
   CRYPTO_CLK_CFG_PKA_CLK        = (1 << 8),
   CRYPTO_CLK_CFG_PKA_CLK_MASK   = 3,
   CRYPTO_CLK_CFG_PKA_CLK_FULL   = 0,
   CRYPTO_CLK_CFG_PKA_CLK_HALF   = 1,
   CRYPTO_CLK_CFG_PKA_CLK_THIRD  = 2,
   CRYPTO_CLK_CFG_PKA_CLK_DISABLED  = 3,

   CRYPTO_CLK_CFG_SPU_CLK        = (1 << 4),
   CRYPTO_CLK_CFG_SPU_CLK_MASK   = 3,
   CRYPTO_CLK_CFG_SPU_CLK_FULL   = 0,
   CRYPTO_CLK_CFG_SPU_CLK_HALF   = 1,
   CRYPTO_CLK_CFG_SPU_CLK_THIRD  = 2,
   CRYPTO_CLK_CFG_SPU_CLK_DISABLED  = 3,
 
   CRYPTO_CLK_CFG_OTP_CLKDIV      = (1 << 0),
   CRYPTO_CLK_CFG_OTP_CLKDIV_MASK = 0xf,
   
   CRYPTO_CLK_CFG_OTP_CLKDIV_2    = 0x0,
   CRYPTO_CLK_CFG_OTP_CLKDIV_4    = 0x1,
   CRYPTO_CLK_CFG_OTP_CLKDIV_6    = 0x2,
   CRYPTO_CLK_CFG_OTP_CLKDIV_8    = 0x3,
   CRYPTO_CLK_CFG_OTP_CLKDIV_10   = 0x4,
   CRYPTO_CLK_CFG_OTP_CLKDIV_12   = 0x5,
   CRYPTO_CLK_CFG_OTP_CLKDIV_14   = 0x6,
   CRYPTO_CLK_CFG_OTP_CLKDIV_16   = 0x7,
   CRYPTO_CLK_CFG_OTP_CLKDIV_18   = 0x8,
   CRYPTO_CLK_CFG_OTP_CLKDIV_20   = 0x9,
   CRYPTO_CLK_CFG_OTP_CLKDIV_22   = 0xa,
   CRYPTO_CLK_CFG_OTP_CLKDIV_24   = 0xb,
   CRYPTO_CLK_CFG_OTP_CLKDIV_26   = 0xc,
   CRYPTO_CLK_CFG_OTP_CLKDIV_28   = 0xd,
   CRYPTO_CLK_CFG_OTP_CLKDIV_30   = 0xe,
   CRYPTO_CLK_CFG_OTP_CLKDIV_32   = 0xf,
   
   CRYPTO_SIMCTRL_DEBUG  = (1 << 0),
   CRYPTO_SIMCTRL_SECURE = (1 << 1)
};

#endif

/*=============================================================================
Copyright (c) 2007 Broadcom Europe Limited. All rights reserved.

Project  :  BCM2707
Module   :  RNG hardware header
File     :  $RCSfile: rnghw.h,v $
Revision :  $Revision: 1.2 $

FILE DESCRIPTION
Definition of bits within RNG hardware registers.
=============================================================================*/



#ifndef RNGHW_H
#define RNGHW_H

#define RNG_READFIELD(_w,_f) (((unsigned long)(_w) / _f) & (_f##_MASK))

enum
{
   RNG_CTRL_RBG_2X            = (1 << 1),
   RNG_CTRL_RBG_EN            = (1 << 0),
   
   RNG_STATUS_WARM_CNT        = (1 << 0),
   RNG_STATUS_WARM_CNT_MASK   = 0xfffff,
   RNG_STATUS_VAL             = (1 << 24),
   RNG_STATUS_VAL_MASK        = 0xff,

   RNG_FF_THRESHOLD_MAX       = 0x04,

// BCM2707_B0:
// RNG interrupt no longer uses Crypto local interrupt register 
// It is connected to interrupt bit 52 : ISRC1_0 bit 52-32=20 (0x0100000)
// Mask has 6 IRQ/reg => floor(52/8)=6  Bits = 16:19 

   RNG_INT_MASK_ENABLE        = (1 << 16),
   RNG_INT_MASK_DISABLE       = (0 << 16),
   
   RNG_SIMCTRL_DEBUG          = (1 << 0),
   RNG_SIMCTRL_SECURE         = (1 << 1)
};

#endif

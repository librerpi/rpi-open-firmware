/*=============================================================================
Copyright (c) 2006 Broadcom Europe Limited.
All rights reserved.

Project  :  VideoCore III
Module   :  Video codec specific header (vcodec)
File     :  $RCSfile: vcodec.h,v $
Revision :  $Revision: 1.14 $

FILE DESCRIPTION
Chip I/O for the video codec.
=============================================================================*/

#ifndef  __VCODEC_H__
#define  __VCODEC_H__

#define VCODEC_IO_BASE 0x7f000000


typedef enum
{
   WIDTH_MASK0 = 0,
   WIDTH_MASK1 = 0x00000001,
   WIDTH_MASK2 = 0x00000003,
   WIDTH_MASK3 = 0x00000007,
   WIDTH_MASK4 = 0x0000000f,
   WIDTH_MASK5 = 0x0000001f,
   WIDTH_MASK6 = 0x0000003f,
   WIDTH_MASK7 = 0x0000007f,
   WIDTH_MASK8 = 0x000000ff,
   WIDTH_MASK9 = 0x000001ff,
   WIDTH_MASK10 = 0x000003ff,
   WIDTH_MASK11 = 0x000007ff,
   WIDTH_MASK12 = 0x00000fff,
   WIDTH_MASK13 = 0x00001fff,
   WIDTH_MASK14 = 0x00003fff,
   WIDTH_MASK15 = 0x00007fff,
   WIDTH_MASK16 = 0x0000ffff,
   WIDTH_MASK17 = 0x0001ffff,
   WIDTH_MASK18 = 0x0003ffff,
   WIDTH_MASK19 = 0x0007ffff,
   WIDTH_MASK20 = 0x000fffff,
   WIDTH_MASK21 = 0x001fffff,
   WIDTH_MASK22 = 0x003fffff,
   WIDTH_MASK23 = 0x007fffff,
   WIDTH_MASK24 = 0x00ffffff,
   WIDTH_MASK25 = 0x01ffffff,
   WIDTH_MASK26 = 0x03ffffff,
   WIDTH_MASK27 = 0x07ffffff,
   WIDTH_MASK28 = 0x0fffffff,
   WIDTH_MASK29 = 0x1fffffff,
   WIDTH_MASK30 = 0x3fffffff,
   WIDTH_MASK31 = 0x7fffffff,
   WIDTH_MASK32 = 0xffffffff 
} WIDTH_MASKS;


/* Pull the register addresses from 'RegFld.h' and create an enum;
 * this gives us a set of constants that we can use to access the
 * registers in IO statements.
 */
 #define CHIP "sv_chip_regmap.h"
 #define CHIP2 "sv_enc_chip_regmap.h"
 
enum
{
#define RegAreaFunc(a,b,c) a = b, a##End = c, a##Max = c-b,
#define RegFunc(mod, modbase, regname, offset, name, addr)  name = addr,
#define FldFunc(a,b,c,d)

#include CHIP
//#include CHIP2

#undef  FldFunc
#undef  RegFunc
#undef  RegAreaFunc
};

// Enum of FIELD_Reg 
typedef enum  {
#define FldFunc(reg,   field,     lsb, width) FIELD_ ##reg ##_ ##field = (WIDTH_MASK ##width << lsb),
#include CHIP
//#include CHIP2
}RegAddrField;
#undef  FldFunc

// Enum of POS_Reg 
typedef enum  {
#define FldFunc(reg,   field,     lsb, width) POS_ ##reg ##_ ##field = lsb,
#include CHIP
//#include CHIP2
}RegAddrPos;
#undef  FldFunc



/* Don't use _VCODEC_IO directly - prefer Reg{Rd|Wt} below */
#define  _VCODEC_IO(x) (* (volatile unsigned long *) (VCODEC_IO_BASE | (x)))

/* Use these rather than _IO() directly so that it's easy to replace the macros with simulation functions */
#define  RegWt( addr, value ) _VCODEC_IO( addr ) = ( value )
#define  RegRd( addr )        _VCODEC_IO( addr )



#define RegValFieldGet(RegVal, Reg, Field) ((RegVal & FIELD_ ##Reg ##_ ##Field) >>  POS_ ##Reg ##_ ##Field)
#define RegValFieldSetTo(RegVal, Reg, Field, To) ( (RegVal) += -((RegVal)&FIELD_ ##Reg ##_ ##Field) + (To<<POS_ ##Reg ##_ ##Field))

// Allows video codec to access memory
#define VCODECCTL 0x10000c40
#define VCODECDQNT8X8BASE 0x500


// Taken from mpeg_symdec.h
/* GetSymbol Register (WO) ----------------------------------------------------
---------------------------------------------------------------------------------
  Write this register to specify the next symbol to be extracted from the stream.
  The symbol value is also read from the GetSymbol register.
  The codestream pointer is advanced by the size of the symbol.

  Register Fields:
  31:11 Reserved
  15:12 Type
  11:08 SubType
  07:00 N
*/

/* 15:12 Type for */
typedef enum
{
   GET_N_BITS,
   GET_EXP_GOL,
   GET_MP2_INFO,
   GET_MP2_COEF,
   GET_264_INFO,
   GET_264_COEF,
   GET_4X4_SPACE_PRED_MODE
} GS_TYPE;

/* 11:08 SubType GET_MP2_INFO */
typedef enum
{
   MB_TYPE_I,
   MB_TYPE_P,
   MB_TYPE_B,
   MB_ADDRESS,
   CBP,
   MOTION_CODE,
   DM_VECTOR
} GS_SUBTYPE_MP2_INFO;

/* 11:08 SubType GET_MP2_COEF */
typedef enum
{
   DCT_COEF0_DC,
   DCT_COEF0_AC,
   DCT_COEF1_AC,
   DCT_COEF1_DC,
   DC_SIZE_LUM,
   DC_SIZE_CHR
} GS_SUBTYPE_MP2_COEF;



// From sv_symb_int.vh

/*
// Syntax element decoder function selection
parameter DCD_TYPE_N_BITS    = 'd0,
          DCD_TYPE_EXPGOL    = 'd1,
          DCD_TYPE_MPEG_INFO = 'd2,
          DCD_TYPE_MPEG_COEF = 'd3,
          DCD_TYPE_H264_INFO = 'd4,
          DCD_TYPE_H264_COEF = 'd5,
          DCD_TYPE_SPAT_PRED = 'd6,
          DCD_TYPE_VC1       = 'd7;


// Table identification


// MPEG "Info" tables
*/
enum
{
    MINFO_MBTYPE_I_TABLE  = 0,
          MINFO_MBTYPE_P_TABLE  = 1,
          MINFO_MBTYPE_B_TABLE  = 2,
          MINFO_MBADDR_TABLE    = 3,
          MINFO_CBP_TABLE       = 4,
          MINFO_MOTION_TABLE    = 5,
          MINFO_DM_VECTOR_TABLE = 6,
          MINFO_261_MTYPE       = 7,
          MINFO_263_MCBPC       = 8,
          MINFO_263_CBPY        = 9};

// MPEG "Coef" tables
enum
{ 
   MCOEF_DCT_COEF0_DC   = 0,
          MCOEF_DCT_COEF0_AC   = 1,
          MCOEF_DCT_COEF1_DC   = 2,
          MCOEF_DCT_COEF1_AC   = 3,
          MCOEF_DC_SIZE_LUMA   = 4,
          MCOEF_DC_SIZE_CHROMA = 5,
          MCOEF_261_COEF0      = 6,
          MCOEF_261_COEF1      = 7,
          MCOEF_263            = 8
         };
/*
// H264 Info tables
parameter HINFO_MBTYPE_I    = 'd0,
          HINFO_MBTYPE_P    = 'd1,
          HINFO_MBTYPE_B    = 'd2,
          HINFO_SUBMBTYPE_P = 'd3,
          HINFO_SUBMBTYPE_B = 'd4,
          HINFO_MVD         = 'd5,
          HINFO_TU          = 'd6,
          HINFO_CBP         = 'd7,
          HINFO_TE          = 'd8,
          HINFO_SU          = 'd9;

// H264 coef
parameter HCOEF_CAVLC_TOKEN       = 'd0,
          HCOEF_CAVLC_TOT_ZERO    = 'd1,
          HCOEF_CAVLC_TOT_ZERO_CR = 'd2,
          HCOEF_CAVLC_RUN_BEFORE  = 'd3,
          HCOEF_CAVLC_LEVEL       = 'd4,
          HCOEF_CABAC_LEVEL       = 'd5,
          HCOEF_CABAC_SIGMAP      = 'd6;

// VC-1
parameter VC1_CBPCY             = 'd0,
          VC1_MBMODE            = 'd1,
          VC1_MVEC              = 'd2,
          VC1_MVBP              = 'd3,
          VC1_TTMB              = 'd4,
          VC1_SUBTYPE           = 'd5,
          VC1_TTBLK             = 'd6,
          VC1_DCDIFF            = 'd7,
          VC1_BMVTYPE           = 'd8;

parameter VC1_LEVEL_SIZE = 'd11;   // Unsigned
parameter VC1_RUN_SIZE = 'd6;
parameter VC1_COEF_MSB = VC1_LEVEL_SIZE + 1 + VC1_RUN_SIZE - 1;  // Run, level, sign


// Constructed symbol types
parameter CONSTR_UE    = 'd0,
          CONSTR_SE    = 'd1,
          CONSTR_UEG0  = 'd2,
          CONSTR_UEG3  = 'd3,
          CONSTR_CAVLC = 'd4,
          CONSTR_SU    = 'd5;


// Xfer symb types
parameter XFER_MVD      = 'd0,
          XFER_REFID    = 'd1,
          XFER_SMODE    = 'd2,
          XFER_XFORM8X8 = 'd3,
          XFER_MVD_HW   = 'd4,
          XFER_SUBTYPE  = 'd5;
          



// Coef generator
parameter CG_MPEG_TYPE  = 'd0,
          CG_CAVLC_TYPE = 'd1,
          CG_CABAC_TYPE = 'd2,
          CG_VC1_TYPE   = 'd3,
          CG_PCM_TYPE   = 'd4;

parameter FAST_DCD_MPEG  = 'd0,
          FAST_DCD_CAVLC = 'd1,
          FAST_DCD_CABAC = 'd2,
          FAST_DCD_VC1   = 'd3;

parameter CG_SUBTYPE_MPEG2 = 'd0,
          CG_SUBTYPE_H261  = 'd1,
          CG_SUBTYPE_H263  = 'd2,
          CG_SUBTYPE_MPEG1 = 'd3;

parameter CG_IXFM_TYPE_4X4   = 'd0,
          CG_IXFM_TYPE_16X16 = 'd1,
          CG_IXFM_TYPE_8X8   = 'd2,
          CG_IXFM_TYPE_PCM   = 'd3;

// MPEG coefficient escape code is a fixed-length representation
// of run & level., preceeded by an 'escape' code
*/
#define MPEG_ESC_CODE 1 
#define MPEG_ESC_CODE_SIZE  6
#define MPEG_LEVEL_SIZE 11
#define MPEG_RUN_SIZE  6
#define MPEG_ESC_SIZE  (MPEG_ESC_CODE_SIZE + MPEG_LEVEL_SIZE + MPEG_RUN_SIZE + 1)
#define MPEG_RUN_SHIFT (MPEG_LEVEL_SIZE + 1)
#define MPEG_ESC_SHIFT (MPEG_RUN_SHIFT + MPEG_RUN_SIZE)
#define H263_LAST_SHIFT (MPEG_ESC_SHIFT + 1)

// Escape code - same as MPEG
#define H261_LEVEL_SIZE 7
#define H261_RUN_SIZE 6
#define H261_ESC_SIZE (MPEG_ESC_CODE_SIZE + H261_LEVEL_SIZE + H261_RUN_SIZE + 1)

#define H263_ESC_CODE 3
#define H263_ESC_CODE_SIZE 7
#define H263_LEVEL_SIZE 7
#define H263_RUN_SIZE 6
#define H263_ESC_SIZE (H263_ESC_CODE_SIZE + 1 + 1 + H263_LEVEL_SIZE + H263_RUN_SIZE)

// MPEG escape: [Run],[Sign],[Level]
#define MPEG_ESC_RUN_START   MPEG_ESC_CODE_SIZE
#define MPEG_ESC_RUN_END     (MPEG_ESC_RUN_START + MPEG_RUN_SIZE - 1)
#define MPEG_ESC_SIGN_BIT    (MPEG_ESC_RUN_END + 1)
#define MPEG_ESC_LEVEL_START (MPEG_ESC_SIGN_BIT + 1)
#define MPEG_ESC_LEVEL_END   (MPEG_ESC_LEVEL_START + MPEG_LEVEL_SIZE - 1)


#define MPEG_LEVEL_MSB (MPEG_LEVEL_SIZE - 1)
#define MPEG_COEF_MSB  (MPEG_LEVEL_MSB + 1)  
#define MPEG_RUN_MSB   (MPEG_RUN_SIZE - 1)

// H261 escape: [Run],[Sign],[Level]
#define H261_ESC_RUN_START   (MPEG_ESC_CODE_SIZE)
#define H261_ESC_RUN_END     (H261_ESC_RUN_START + H261_RUN_SIZE - 1)
#define H261_ESC_SIGN_BIT    (H261_ESC_RUN_END + 1)
#define H261_ESC_LEVEL_START (H261_ESC_SIGN_BIT + 1)
#define H261_ESC_LEVEL_END   (H261_ESC_LEVEL_START + H261_LEVEL_SIZE - 1)

// H263 escape: [Last],[Run],[Sign],[Level]
#define H263_ESC_LAST_BIT    (H263_ESC_CODE_SIZE)
#define H263_ESC_RUN_START   (H263_ESC_CODE_SIZE + 1)
#define H263_ESC_RUN_END     (H263_ESC_RUN_START + H263_RUN_SIZE - 1)
#define H263_ESC_SIGN_BIT    (H263_ESC_RUN_END + 1)
#define H263_ESC_LEVEL_START (H263_ESC_SIGN_BIT + 1)
#define H263_ESC_LEVEL_END   (H263_ESC_LEVEL_START + H263_LEVEL_SIZE - 1)

#define H264_LEVEL_SIZE      15;  // Unsigned
#define H264_LEVEL_MSB       (H264_LEVEL_SIZE - 1)
/*
// MPEG 1 escape.  Basically the same as H261, except has a 'double' escape if the 
// level in the regular escape = 0
parameter MPEG1_ESC_DBL_START = H261_ESC_LEVEL_END + 1;
parameter MPEG1_ESC_DBL_END   = MPEG1_ESC_DBL_START + 8 - 1;
parameter MPEG1_ESC_DBL_SIZE  = H261_ESC_SIZE + 8;


// VC1 defines
parameter VC1_PTYPE_I = 0,
          VC1_PTYPE_P = 1,
          VC1_PTYPE_B = 2;

parameter VC1_FCM_PROG     = 0,
          VC1_FCM_INTL_FRM = 1,
          VC1_FCM_INTL_FLD = 2;

// Paramters for coef. generator buffer
parameter BUF_COEF_SIZE = 'd15;
parameter BUF_RUN_SIZE = 'd6;  // For MPEG


// H264 vector types
parameter H264_P_L0_16X16   = 'd0,
          H264_P_L0_L0_16X8 = 'd1,
          H264_P_L0_L0_8X16 = 'd2,
          H264_P_8X8        = 'd3,
          H264_P_8X8REF0    = 'd4;

parameter H264_B_DIRECT_16X16 = 'd0,
          H264_B_8X8 = 'd22;



// VC-1 Inverse coef block sizes
parameter VC1_IX_BLK_8X8 = 0,
          VC1_IX_BLK_8X4 = 1,
          VC1_IX_BLK_4X8 = 2,
          VC1_IX_BLK_4X4 = 3;          


// Constants used in vector generation
parameter VEC_TYPE_16X16 = 'd0,
          VEC_TYPE_16X8  = 'd1,
          VEC_TYPE_8X16  = 'd2,
          VEC_TYPE_8X8   = 'd3;

parameter VEC_SUBTYPE_8X8 = 'd0,
          VEC_SUBTYPE_8X4 = 'd1,
          VEC_SUBTYPE_4X8 = 'd2,
          VEC_SUBTYPE_4X4 = 'd3;

parameter VEC_COMPUTE   = 'd0,
          VEC_COPY_LEFT = 'd1,
          VEC_COPY_TOP  = 'd2;

parameter VEC_SEL_A = 'd0,
          VEC_SEL_B = 'd1,
          VEC_SEL_C = 'd2,
          VEC_SEL_D = 'd3;

parameter VEC_VC1_1MV = 'd0,
          VEC_VC1_2MV = 'd1,
          VEC_VC1_4MV = 'd3;


// Vector generator ALU
parameter ALU_IN_SEL_A     = VEC_SEL_A, 
          ALU_IN_SEL_B     = VEC_SEL_B,
          ALU_IN_SEL_C     = VEC_SEL_C,
          ALU_IN_SEL_CONST = 'd3;

parameter ALU_LDSEL_A = 'd0,
          ALU_LDSEL_B = 'd1,
          ALU_LDSEL_C = 'd2;

parameter ALU_OUT_DIFF     = 'd0,
          ALU_OUT_SUM      = 'd1,   
          ALU_OUT_C        = 'd2,
          ALU_OUT_ENC_DIFF = 'd3;

parameter ALU_LDSRC_MEM = 'd0,
          ALU_LDSRC_ALU = 'd1;

// Conditional load
parameter ALU_CLD_LT_M32 = 'd0,
          ALU_CLD_GE_M32 = 'd1,
          ALU_CLD_LT_0   = 'd2,
          ALU_CLD_GE_0   = 'd3;  
*/

#define REG_STATUS         0x00000110

#define STATUS_MOCOMP_RDY  (1 << 2)
#define STATUS_MOCOMP_DONE (1 << 3)
#define STATUS_SPAT_RDY    (1 << 4)
#define STATUS_SPAT_DONE   (1 << 5)
#define STATUS_XFM_RDY     (1 << 6)
#define STATUS_XFM_DONE    (1 << 7)
#define STATUS_RECON_RDY   (1 << 8)
#define STATUS_RECON_DONE  (1 << 9)
#define STATUS_DBLK_RDY    (1 << 10)
#define STATUS_DBLK_DONE   (1 << 11)

// Updated bits for new status register...
#define STATUS_FLUSHCTX (1<<17)
#define STATUS_RESET (1<<16)
#define STATUS_CTXDMAACT (1<<11)

#define STATUS_ALL_RDY (STATUS_MOCOMP_RDY | STATUS_SPAT_RDY | \
                    STATUS_XFM_RDY    | STATUS_RECON_RDY | \
                    STATUS_DBLK_RDY)

#define STATUS_ALL_DONE (STATUS_MOCOMP_DONE | STATUS_SPAT_DONE | \
                     STATUS_XFM_DONE     | STATUS_RECON_DONE | \
                     STATUS_DBLK_DONE)

#define STATUS_ALL_RDY_DONE (STATUS_ALL_DONE | STATUS_ALL_RDY)

#define REG_SINT_STRM_POS  0x00000c10

//    Use base and offsets for symbol interpreter registers.
#define REGBASE_SINT             0x00000c00
#define REGOFF_SINT_DMA_ADDR      0x00
#define REGOFF_SINT_DMA_LEN       0x04
#define REGOFF_SINT_STRM_POS      0x10
#define REGOFF_SINT_STRM_STAT     0x14
#define REGOFF_SINT_STRM_BITS     0x1c
#define REGOFF_SINT_GET_SYMB      0x20
#define SINT_STRM_STAT_DCD_ERR  (1 << 9)
#define SINT_STRM_STAT_STRM_ERR (1 << 8)
#define SINT_STRM_STAT_VALID    (1 << 0)
#define REGOFF_SINT_DO_RESID      0x28
#define DO_RESID_INTRA16        (1 << 6)
#define RESID_WR_XNZERO         (1 << 10)
#define DO_RESID_AFETCH         (1 << 16)
#define DO_RESID_INTRA          (1 << 17)
#define DO_RESID_SKIP           (1 << 18)
#define REGOFF_VEC_MBTYPE         0x30
#define REGOFF_SINT_VEC_DMODE     0x38
#define REGOFF_SINT_VEC_TOP_LD    0x3c
#define REGOFF_SINT_SMODE_DATA    0xa0
#define REGOFF_SINT_SMODE_LEFT    0xa4
#define REGOFF_SINT_SMODE_TOP     0xa8
#define REGOFF_SINT_SMODE_VALID   0xac

#define REGOFF_TOPCTX_WRADDR      0x90
#define REGOFF_DIRCTX_WRADDR      0x94
#define REGOFF_TOPCTX_WRDAT       0x98
#define REGOFF_XFER_SYMB          0x9c

#define REGOFF_VEC_DO_CONST       0x40
#define REGOFF_VEC_REFIDX         0x48

#define REGOFF_SINT_VEC_COLTYPE   0x54
#define REGOFF_SINT_VEC_COLREFID  0x58
#define REGOFF_SINT_CTL           0x80
#define REGOFF_SINT_VLC_TOP_CTX   0x84
#define REGOFF_SINT_QP            0x8c

#define STRM_STAT_CG_ACTIVE         (1 << 1)
#define STRM_STAT_VG_ACTIVE         (1 << 4)
#define STRM_STAT_CG_PARSE          (1 << 7)
#define STRM_STAT_STRM_ERR          (1 << 8)
#define STRM_STAT_DCD_ERR           (1 << 9)
#define STRM_STAT_CTX_BUSY          (1 << 11)
#define STRM_STAT_FLUSH_CTX         (1 << 17)

#define REG_MCOM_CTL       0x00000300
#define SHIFT_REG_MCOM_CTL_BREF 24
#define SHIFT_REG_MCOM_CTL_AREF 16
#define SHIFT_REG_MCOM_CTL_SUBBLOCK  12
#define SHIFT_REG_MCOM_CTL_YSIZE 6
#define SHIFT_REG_MCOM_CTL_XSIZE 4
#define SHIFT_REG_MCOM_CTL_BFLD 3
#define SHIFT_REG_MCOM_CTL_AFLD 2
#define SHIFT_REG_MCOM_CTL_MODE 0

#define REG_MCOM_SRCA      0x00000304
#define SHIFT_REG_MCOM_SRCA_Y 16
#define REG_MCOM_SRCB      0x00000308
#define SHIFT_REG_MCOM_SRCB_Y 16

#define REG_SPRE_CTL       0x00000320
#define SPRE_CTL_CNST_INTRA   (1 << 4)
#define REG_SPRE_MODE      0x00000324

#define REG_DEQUANT_MAP1   0x400
#define REG_DEQUANT_MAP2   0x500

#define REG_IXFM_CTL       0x00000700
#define IXFM_XFM_I16      (1 << 30)
#define IXFM_XFM_8X8      (2 << 30)
#define IXFM_FIELD_SCAN   (1 << 29)
#define IXFM_QS_TABLE     (1 << 28)
#define IXFM_8X8_SUB_ORD  (1 << 27)
#define IXFM_INTRA        (1 << 24)

#define REG_IXFM_COEF      0x00000704
#define REG_IXFM_PCM       0x0000070C

#define REG_DBLK_CTL       0x00000720
#define DBLK_CTL_8X8         (1 << 7)
#define DBLK_CTL_MONO        (1 << 6)
#define DBLK_CTL_ISINTRA     (1 << 4)
#define DBLK_CTL_FLEFT       (1 << 3)
#define DBLK_CTL_FTOP        (1 << 2)
#define DBLK_CTL_FINT        (1 << 1)
#define DBLK_ALL_AVAIL      (DBLK_CTL_FINT | \
                      DBLK_CTL_FTOP  | DBLK_CTL_FLEFT)
#define DBLK_NO_TOP_LEFT    (DBLK_CTL_FINT)
#define DBLK_NO_TOP   (DBLK_CTL_FINT | DBLK_CTL_FLEFT)
#define DBLK_NO_LEFT  (DBLK_CTL_FINT | DBLK_CTL_FTOP)

#define REG_DBLK_OUT       0x00000724
#define REG_DBLK_EDGE      0x00000728
#define REG_DBLK_QNT       0x0000072C
#define REG_DBLK_OFFSET    0x00000730
#define REG_DBLK_TOP_CTX   0x00000734
#define REG_DBLK_TOP_INTRA   (1 << 0)
#define REG_DBLK_TOP_FIELD   (1 << 1)
#define SHIFT_REG_DBLK_TOP_CTX_ISINTRA 0
#define SHIFT_REG_DBLK_TOP_CTX_ISFIELD 1
#define SHIFT_REG_DBLK_TOP_CTX_B10 2
#define SHIFT_REG_DBLK_TOP_CTX_B11 3
#define SHIFT_REG_DBLK_TOP_CTX_B14 4
#define SHIFT_REG_DBLK_TOP_CTX_B15 5
#define REG_DBLK_XZERO     0x00000738
#define REG_DBLK_MVDIFF    0x0000073C

#define REG_MB_CTL         0x00000740
#define SHIFT_REG_MB_CTL_TOP_FIELD 17
#define SHIFT_REG_MB_CTL_FIELD_MODE 16

#define SYMB_GET_NBITS        (0)
#define SYMB_GET_UE           (1 << 12)
#define SYMB_GET_SE           ((1 << 12) | (1 << 8))
#define SYMB_GET_H264_INFO    (4 << 12)
#define SYMB_GET_SPATIAL      (6 << 12)

#define SYMB_H264_INFO_I_MBTYPE (0 << 8)
#define SYMB_H264_INFO_P_MBTYPE (1 << 8)
#define SYMB_H264_INFO_B_MBTYPE (2 << 8)
#define SYMB_H264_INFO_SUB_P_SLICE (3 << 8)
#define SYMB_H264_INFO_SUB_B_SLICE (4 << 8)
#define SYMB_H264_INFO_MVD      (5 << 8)
#define SYMB_H264_INFO_TU      (6 << 8)
#define SYMB_H264_INFO_CBP     (7 << 8)
#define SYMB_H264_INFO_TE      (8 << 8)
#define SYMB_H264_INFO_SU      (9 << 8)

#define SYMB_GET_TE          (SYMB_GET_H264_INFO | SYMB_H264_INFO_TE)


#define REG_RVC_CTL        0xe00
#define REG_RVC_PUT        0xe04
#define REG_RVC_GET        0xe08
#define REG_RVC_BASE       0xe0c
#define REG_RVC_END        0xe10

#define REG_HST2CPU_MBX    0xf00
#define REG_CPU2HST_MBX    0xf04
#define REG_MBX_STAT       0xf08

#define REG_VEC_MBTYPE  0x0C30
#define VEC_MBTYPE_IS_B   (1 << 0)

#define REG_VEC_RESID   0x0C34

#define REG_VEC_DO_CONST 0x0C40
#define SHIFT_REG_VEC_DO_CONST_ULFT_MBAFF_FIELD 6
#define REG_VEC_DO_CONST_INTRA (1 << 4)
#define REG_VEC_DO_CONST_PSKIP (1 << 5)
#define REG_VEC_DO_CONST_ULFT_MBAFF_FIELD (1 << 6)
#define REG_VEC_DO_CONST_LEFTCOPY (1 << 7)
#define REG_VEC_REF0_LTERM       (1 << 8)
#define REG_VEC_DO_CONST_MVDIFF (1 << 9)

#define REG_VEC_DIFF 0x0C44
#define REG_SINT_XNZERO 0x0C2C
#define SHIFT_REG_SINT_XNZERO_B10  10
#define REG_SINT_XNZERO_B10  (1 << 10)
#define REG_SINT_XNZERO_B11  (1 << 11)
#define REG_SINT_XNZERO_B14  (1 << 14)
#define SHIFT_REG_SINT_XNZERO_B14  14
#define REG_SINT_XNZERO_B15  (1 << 15)

#define REG_VEC_REFIDX  0x0C48
#define SHIFT_REG_VEC_REFIDX0 0
#define SHIFT_REG_VEC_REFIDX1 8
#define SHIFT_REG_VEC_REFIDX2 16
#define SHIFT_REG_VEC_REFIDX3 24
#define SHIFT_REG_VEC_REFIDX_DELTA SHIFT_REG_VEC_REFIDX1

#define REG_VEC_TOPREF  0x0C4C
#define SHIFT_REG_VEC_TOPREFL0_B0 0
#define SHIFT_REG_VEC_TOPREFL0_B1 5
#define SHIFT_REG_VEC_TOPREFL0_C  10
#define SHIFT_REG_VEC_TOPREFL1_B0 16
#define SHIFT_REG_VEC_TOPREFL1_B1 21
#define SHIFT_REG_VEC_TOPREFL1_C  26
#define SHIFT_REG_VEC_TOPREF_DELTA SHIFT_REG_VEC_TOPREFL0_B1

#define REG_VEC_TOPTOPREF  0x0C50

#define REG_VEC_COLTYPE        0x0C54
#define MASK_REG_VEC_COLTYPE_TYPE 0x3
#define MASK_REG_VEC_COLTYPE_SLICE_ID 0xfff
#define SHIFT_REG_VEC_COLTYPE_TYPE 0
#define SHIFT_REG_VEC_COLTYPE_SUBTYPE0 2
#define SHIFT_REG_VEC_COLTYPE_SUBTYPE1 4
#define SHIFT_REG_VEC_COLTYPE_SUBTYPE2 6
#define SHIFT_REG_VEC_COLTYPE_SUBTYPE3 8
#define SHIFT_REG_VEC_COLTYPE_SLICE_ID 16
#define SHIFT_REG_VEC_COLTYPE_IS_FIELD 29
#define SHIFT_REG_VEC_COLTYPE_TOP_FIELD 30
#define SHIFT_REG_VEC_COLTYPE_MBAFF 31

#define COLTYPE_FLD_TOP  (1<<SHIFT_REG_VEC_COLTYPE_IS_FIELD) | (1<<SHIFT_REG_VEC_COLTYPE_TOP_FIELD)
#define COLTYPE_AFF_FLD  (1<<SHIFT_REG_VEC_COLTYPE_MBAFF) | (1<<SHIFT_REG_VEC_COLTYPE_IS_FIELD)
#define COLTYPE_FLD      (1<<SHIFT_REG_VEC_COLTYPE_IS_FIELD)

#define REG_VEC_COLREFID       0x0C58
#define SHIFT_REG_VEC_COLREFID0 0
#define SHIFT_REG_VEC_COLREFID1 8
#define SHIFT_REG_VEC_COLREFID2 16
#define SHIFT_REG_VEC_COLREFID3 24
#define SHIFT_REG_VEC_COLREFID_DELTA SHIFT_REG_VEC_COLREFID1
#define MASK_REG_VEC_COLREFID 0x7

#define REG_VEC_TOPPIC         0x0c5c

#define REG_VEC_OUTPIC_MAP     0x0cc0
#define REGIO_VEC_OUTPIC_MAP   (REG_VEC_OUTPIC_MAP | IO_BASE)

#define REG_VEC_MEM_BASE       0x0d00
#define REG_VEC_MEM_END        0x0e00

#define REG_VEC_MEM_MBAFF_L0_D 0x0d7c
#define REG_VEC_MEM_MBAFF_L1_D 0x0dfc

#define REG_VEC_MEM_SIZE (REG_VEC_MEM_END - REG_VEC_MEM_BASE)


#define REG_DMA0_SD_ADDR      0x1800
#define REG_DMA0_LCL_ADDR     0x1804
#define REG_DMA0_LENGTH       0x1808
#define REG_DMA1_SD_ADDR      0x1810
#define REG_DMA1_LCL_ADDR     0x1814
#define REG_DMA1_LENGTH       0x1818
#define REG_DMA2_SD_ADDR      0x1820
#define REG_DMA2_LCL_ADDR     0x1824
#define REG_DMA2_LENGTH       0x1828
#define REG_DMA3_SD_ADDR      0x1830
#define REG_DMA3_LCL_ADDR     0x1834
#define REG_DMA3_LENGTH       0x1838
#define REG_DMA_STATUS        0x1840
#define REG_DMA0_ACTIVE       (1 << 0)
#define REG_DMA1_ACTIVE       (1 << 1)
#define REG_DMA2_ACTIVE       (1 << 2)
#define REG_DMA3_ACTIVE       (1 << 3)

#define REG_DMA_ALL_ACTIVE    REG_DMA0_ACTIVE | REG_DMA1_ACTIVE | \
                       REG_DMA2_ACTIVE | REG_DMA3_ACTIVE

#define DMA_MEM_BASE         0x1A00
#define DMA_MEM_SIZE         0x200            
//; In bytes
#define MASK_DMA0_STATUS       (1<<0)
#define MASK_DMA1_STATUS       (1<<1)
#define MASK_DMA2_STATUS       (1<<2)
#define MASK_DMA3_STATUS       (1<<3)

#define DEBUG_LED             0x910


// Offsets to MVDATA
#define MVDATA_A              0x0
#define MVDATA_B              0xC
#define MVDATA_C              0x30
#define MVDATA_D              0x3C
#define MVDATA_LD_A           0x0
#define MVDATA_LD_B           0x8
#define MVDATA_LD_C           0x20
#define MVDATA_LD_D           0x28

#define MASK_MVDATA_X         0xFFFF
#define MASK_MVDATA_Y         0x7FFF0000
#define SHIFT_MVDATA_Y        16
#define MASK_MVDATA_VALID     0x80000000

#define REGIO_SINT_VEC_TOPPIC  (REG_VEC_TOPPIC | IO_BASE)

#define REGIO_SINT_VEC_TOPREF  (REG_VEC_TOPREF | IO_BASE)

#define REGIO_DEBUG_LED       (DEBUG_LED | IO_BASE)


#define REG_WPRED_CTL                 0x0340
#define REG_WPRED_SEL                 0x0314



/*
** The following do not appear in sv_chip_regmap.h
**
** From code_in_pump.v:
*/

#define DECODE_CIP_CIRC_START  0x00000A40
#define DECODE_CIP_CIRC_END    0x00000A44
#define DECODE_CIP_START       0x00000A48
#define DECODE_CIP_END         0x00000A4C
#define DECODE_CIP_CTL         0x00000A50

/* We define the range of the CIP block register address space... */
#define DECODE_CIP_BASE        0x00000A40
#define DECODE_CIP_BASEEND     0x00000A53

/* ...and the three flags in CIP_CTL. */
#define CIP_CTL_ENABLE         0x00000001
#define CIP_CTL_CMD_Q_EMPTY    0x00000002
#define CIP_CTL_DATA_Q_EMPTY   0x00000004
/*
** These two macros are used to determine whether the CIP block has finished sending
** data to the CI block.  It is then safe to flush the CI block if required.
*/
#define CIP_CTL_MASK           ( CIP_CTL_ENABLE | CIP_CTL_CMD_Q_EMPTY | CIP_CTL_DATA_Q_EMPTY )
#define CIP_CTL_EMPTY          ( CIP_CTL_CMD_Q_EMPTY | CIP_CTL_DATA_Q_EMPTY )


/*
** Defines for the CodeIn control register:
*/
#define CI_CTL_ENA                   0x00000001
#define CI_CTL_EMU                   0x00000002
#define CI_CTL_MCHN                  0x00000004
#define CI_CTL_OWRT                  0x00000008

#define CI_CTL_FLUSH                 0x00000010
#define CI_CTL_NOBD                  0x00000020

#define CI_CTL_ACT                   0x00010000
#define CI_CTL_FULL                  0x00020000
#define CI_CTL_ERR                   0x00040000

#define CI_CTL_GET_ERRTYPE( x )      ( ( (x) >> 19U ) & 0x1F )

#define CI_CTL_BFULL                 0x01000000


#endif

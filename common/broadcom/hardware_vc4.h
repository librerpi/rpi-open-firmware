/*=============================================================================
Copyright (c) 2006 Broadcom Europe Limited.
All rights reserved.

Project  :  VideoCore
Module   :  VideoCore hardware headers
File     :  $Id$

FILE DESCRIPTION
Public interface definition file for hardware specified registers.

=============================================================================*/

#ifndef _HARDWARE_VC4_H
#define _HARDWARE_VC4_H


/********************************************************
* VideoCore IV support
********************************************************/

/*
VC4 Processor Control Register usage

p0        PRFPXCS    - See "Scalar Floating Point Exception Control" in the VCIV Architecture Specification.
p1        PRCANARY   - If stack protection is enabled, this register holds the canary.
p2-p9     Unassigned
p10 [B0]  PRPOWCTL   - Closely Coupled Power Control (VPU clock gating)
p11 [B0]  PRTIMCTL   - Closely Coupled Timer Control (core and sleep timers)
p12 [B0]  PRCORTIM   - Core Timer Result
p13 [B0]  PRSLPTIM   - Sleep Timer Result
p14       PROWCNT    - Count of outstanding writes. See "Scalar Memory Engine" in the VC4AS.
p15       PRORCNT    - Count of outstanding reads. See "Scalar Memory Engine" in the VC4AS.

p16-p31 are single-bit mutexes, shared between the two VPUs. See "P-Reg Semaphore" in the VC4AS.
p16       PRSPINL    - Used by spinlock, a lightweight mutex.
p17                  - Used by vcos_quickslow_mutex on ThreadX. (TODO: could we just use p16 instead?)
p18       Unassigned
p19       Unassigned
p20       Unassigned
p21       Unassigned
p22       Unassigned
p23       Unassigned
p24       Unassigned
p25       Unassigned
p26       Unassigned
p27       Unassigned
p28       Unassigned
p29       Unassigned
p30       Unassigned
p31       Unassigned
*/


   //#define FORCE_SECOND_CORE

   #include "bcm2708_chip/arm_control.h"
   #include "bcm2708_chip/apb_async_bridge_ctrl.h"
   #include "bcm2708_chip/axi_dma0.h"
#ifdef __BCM2708A0__
   #include "bcm2708_chip/axi_dma8.h"
#else
   #include "bcm2708_chip/axi_dma15.h"
#endif
#ifdef __BCM2708A0__
   #include "bcm2708_chip/cam0_a0.h"
   #include "bcm2708_chip/cam1_a0.h"
   #include "bcm2708_chip/ccp2tx_a0.h"
#else
   #include "bcm2708_chip/cam0.h"
   #include "bcm2708_chip/cam1.h"
   #include "bcm2708_chip/ccp2tx.h"   
#endif
   #include "bcm2708_chip/clkman_image.h"
//   #include "bcm2708_chip/clkman_audio.h"
//   #include "bcm2708_chip/clkman_run.h"
   #include "bcm2708_chip/cpg.h"
#ifdef __BCM2708A0__
   #include "bcm2708_chip/cpr_clkman_a0.h"
   #include "bcm2708_chip/cpr_powman_a0.h"
   #include "bcm2708_chip/cpr_apb2wtap_a0.h"
#else
   #include "bcm2708_chip/cpr_clkman.h"
   #include "bcm2708_chip/cpr_powman.h"
   #include "bcm2708_chip/cpr_apb2wtap.h"
#endif
   #include "bcm2708_chip/dpi.h"
   #include "bcm2708_chip/dsi.h"
   #include "bcm2708_chip/dsi4.h"
   #include "bcm2708_chip/gpio.h"
   #include "bcm2708_chip/hdcp.h"
   #include "bcm2708_chip/hdmi.h"
   #include "bcm2708_chip/hdmicore.h"
   #include "bcm2708_chip/hvs.h"
   #include "bcm2708_chip/i2c0.h"
   #include "bcm2708_chip/i2c1.h"
   #include "bcm2708_chip/i2c2.h"
   #include "bcm2708_chip/intctrl0.h"
   #include "bcm2708_chip/intctrl1.h"
   #include "bcm2708_chip/isp.h"
   #include "bcm2708_chip/l2_cache_ctrl.h"
   #include "bcm2708_chip/jpeg_top.h"   
   #include "bcm2708_chip/mphi.h"
   #include "bcm2708_chip/multicore_sync.h"
   #include "bcm2708_chip/nexus_uba.h"
   #include "bcm2708_chip/otp.h"
   #include "bcm2708_chip/pcm.h"
   #include "bcm2708_chip/perfmon.h"
   #include "bcm2708_chip/pixel_valve0.h"
   #include "bcm2708_chip/pixel_valve1.h"
   #include "bcm2708_chip/pixel_valve2.h"
   #include "bcm2708_chip/pwm.h"
//   #include "bcm2708_chip/reset_ctrl.h"
   #include "bcm2708_chip/sdc_ctrl.h"
   #include "bcm2708_chip/sdc_addr_front.h"
   #include "bcm2708_chip/sdc_dq_front.h"
   #include "bcm2708_chip/sdhost.h"
#if defined(__BCM2708A0__)
   #include "bcm2708_chip/slimbus_a0.h"
#else
   #include "bcm2708_chip/slimbus.h"
#endif
   #include "bcm2708_chip/spi_master.h"
   #include "bcm2708_chip/system_arbiter_ctrl.h"
   #include "bcm2708_chip/vpu_arb_ctrl.h"
   #include "bcm2708_chip/peri_image_arb_ctrl.h"
#ifdef __BCM2708A0__
   #include "bcm2708_chip/tectl_a0.h"
#else
   #include "bcm2708_chip/tectl.h"
#endif
   #include "bcm2708_chip/timer.h"
   #include "bcm2708_chip/tempsens.h"
   #include "bcm2708_chip/txp.h"   
   #include "bcm2708_chip/uart.h"
   #include "bcm2708_chip/v3d.h"
   #include "bcm2708_chip/vec.h"
   #include "bcm2708_chip/vpu_l1_cache_ctrl.h"
   #include "bcm2708_chip/mphi.h"
   #include "bcm2708_chip/usb.h"
#ifdef __BCM2708A0__
   #include "bcm2708_chip/rng_a0.h"
#else
   #include "bcm2708_chip/rng.h"
#endif

   // Note: these macro evaluate argument twice - beware of side effects
   #define ALIAS_NORMAL(x)               ((void*)(((unsigned)(x)&~0xc0000000)|0x00000000)) // normal cached data (uses main 128K L2 cache)
   #define ALIAS_L1_NONALLOCATING(x)     ((void*)(((unsigned)(x)&~0xc0000000)|0x40000000)) // Doesn't allocate in L1 cache, will allocate in L2
#if defined(__BCM2708__)
   // HW-2827 workaround
   #define ALIAS_L1L2_NONALLOCATING(x)   ALIAS_L1_NONALLOCATING(x)
   #define ALIAS_L1L2_NONALLOCATING_READ(x) ((void*)(((unsigned)(x)&~0xc0000000)|0x80000000)) // cache coherent but non-allocating in L1 and L2
#else
   #define ALIAS_L1L2_NONALLOCATING(x)   ((void*)(((unsigned)(x)&~0xc0000000)|0x80000000)) // cache coherent but non-allocating in L1 and L2
   #define ALIAS_L1L2_NONALLOCATING_READ(x) ALIAS_L1L2_NONALLOCATING(x)
#endif
   #define ALIAS_COHERENT(x)             ALIAS_L1L2_NONALLOCATING(x)
   #define ALIAS_DIRECT(x)               ((void*)(((unsigned)(x)&~0xc0000000)|0xc0000000)) // uncached
   #define ALIAS_ANY_NONALLOCATING(x)    (IS_ALIAS_DIRECT(x)?ALIAS_DIRECT(x):ALIAS_L1L2_NONALLOCATING(x)) // eliminate L1+L2 allocation from whatever alias is supplied
   #define ALIAS_ANY_NONALLOCATING_READ(x) (IS_ALIAS_DIRECT(x)?ALIAS_DIRECT(x):ALIAS_L1L2_NONALLOCATING_READ(x))
   #define ALIAS_ANY_L1_NONALLOCATING(x) (IS_ALIAS_DIRECT(x)?ALIAS_DIRECT(x):ALIAS_L1_NONALLOCATING(x)) // eliminate L1 allocation from whatever alias is supplied

   #define IS_ALIAS_NORMAL(x)    ((((unsigned)(x)>>30)&0x3)==0)
   #define IS_ALIAS_L1_NONALLOCATING(x) ((((unsigned)(x)>>30)&0x3)==1)
#if defined(__BCM2708__)
   // HW-2827 workaround
   #define IS_ALIAS_L1L2_NONALLOCATING(x) IS_ALIAS_L1_NONALLOCATING(x)
#else
   #define IS_ALIAS_L1L2_NONALLOCATING(x) ((((unsigned)(x)>>29)&0x7)==4)  // make sure we are not considering peripherals
#endif
   #define IS_ALIAS_DIRECT(x)             ((((unsigned)(x)>>30)&0x3)==3)
   #define IS_ALIAS_NONALLOCATING(x)      (((unsigned)(x)>>29)>=3)
   #define IS_ALIAS_PERIPHERAL(x)         (((unsigned)(x)>>29)==0x3)
   #define IS_ALIAS_COHERENT(x)  IS_ALIAS_L1L2_NONALLOCATING(x)
   #define IS_ALIAS_NOT_L1(p)             (IS_ALIAS_L1_NONALLOCATING(p) || IS_ALIAS_NONALLOCATING(p))

   //number of cores
   #define VIDEOCORE_NUM_CORES   2

   //The size of the stacked SDRAM
   #define SDRAM_SIZE            (1024 * 1024 * 128) //32MBytes
   #define SDRAM_START_ADDRESS   0 //starts at 0 in our memory space

   //The size of the L2 cache
   #define L2CACHE_SIZE          (1024 * 128) //starts at 0 in our memory space

   //default interrupt vector table base address
   #define INTERRUPT_VECTOR_BASE 0

   //common interrupts
   #define INTERRUPT_EXCEPTION_OFFSET  0
   #define INTERRUPT_EXCEPTION_NUM     32

   #define INTERRUPT_SOFTINT_OFFSET    32
   #define INTERRUPT_SOFTINT_NUM       32

   #define INTERRUPT_HARDINT_OFFSET    64
   #define INTERRUPT_HARDINT_NUM       64

   #define MAX_TIMER_NUM 4
   #define MAX_EXCEPTION_NUM 8

   // In A0 address order (ie RESET_CONTROLLER for B0 is later in the list)

   #define BOOTROM_BASE_ADDRESS           0x60000000

   #define L2CACHE_BASE                   L2_BASE
   #define I0CACHE_BASE                   L1_BASE
   #define D0CACHE_BASE                   (L1_BASE+0x100)

   #define SDRAM_BASE_ADDRESS             SD_BASE
   #define DEBUG_MASTER_BASE              NU_BASE
   #define ARBITER_CTRL_BASE              SYSAC_BASE
   #define VPU0_THREAD_CTRL_BASE_ADDRESS  0x18011000


   // vc_run APB Bridge           - 0x1A00_0000 - 0x1A0F_FFFF
   #define RUN_ARBITER_CTRL_BASE_ADDRESS  0x1A003000
   #define V3D_BASE_ADDRESS               0x1A005000
   #define VPU1_THREAD_CTRL_BASE_ADDRESS  0x1A008000
   #define VPU1_UNIFORM_MEM_BASE_ADDRESS  0x1A00A000
   #define V3D_MEM1_BASE_ADDRESS          0x1A00B000
   #define V3D_MEM2_BASE_ADDRESS          0x1A00C000
   #define VIDEOCODEC_BASE_ADDRESS        0x7f000000

   // peri_audio APB Bridge        - 0x7e20_0000 - 0x7E21_FFFF
   #define UART_BASE_ADDRESS              UART_BASE
   #define I2C_BASE_0                     I2C0_BASE
   #define PIXELVALVE_0_BASE_ADDRESS      PIXELVALVE0_BASE
   #define PIXELVALVE_1_BASE_ADDRESS      PIXELVALVE1_BASE
   #define DSI_BASE                       DSI0_BASE
   #define PWM_BASE_ADDRESS               PWM_BASE
   #define PERFMON_BASE_ADDRESS           PRM_BASE
//   #define SPI_BASE_ADDRESS               SPI_BASE
//   #define DSI1_BASE_ADDRESS              DSI1_BASE
   #define OTP_BASE_ADDRESS               OTP_BASE
//   #define CPG_BASE_ADDRESS               CPG_BASE
//   #define TEMP_SENS_BASE_ADDRESS         TS_BASE


   // cprman Audio APB bridge
   #define POWERMAN_BASE_ADDRESS          PM_BASE
   #define RESET_CONTROLLER_BASE          RS_BASE

   #define JPEG_BASE                      JP_BASE
   #define TRANSPOSER_BASE_ADDRESS        TXP_BASE
   //#define CCP2TX_BASE                    CCP2TX_BASE   // definition in ccp2tx.h

   // what to do with these ??
   #define DISPC_BASE_ADDRESS             0x1C009000
   #define CDP_BASE                       0x1C00E000
   #define ACIS_BASE_ADDRESS              0x1C004800
   #define ADC_BASE_ADDRESS               0x1C00E000

   // The AXI bus to the SMI      - 0x1C20_0000 - 0x1C2F_FFFF
   #define SMI_BASE                       0x7E600000
   #define SMI_BASE_DIRECT                0x7E601000

   // perp run APB  bridge

   // peri_image APB Bridge        - 0x7e80_0000 - 0x7E81_FFFF
   // Camera - now have two Unicam modules at
   // CAM 0 : 0x7e800000 (CAM0_BASE)
   // CAM 1 : 0x7e801000 (CAM1_BASE)

   #define I2C_BASE_1                     I2C1_BASE
   #define I2C_BASE_2                     I2C2_BASE
   #define PIXELVALVE_2_BASE_ADDRESS      PIXELVALVE2_BASE
   #define VEC_BASE_ADDRESS               VEC_BASE

   //interrupt definitions
   #define INTERRUPT_HW_NUM               (64)
   #define INTERRUPT_HW_OFFSET            (64)
   #define INTERRUPT_SW_OFFSET            (32)
   #define INTERRUPT_SW_NUM               (32)

   #define INTERRUPT_TIMER0               (INTERRUPT_HW_OFFSET + 0 )
   #define INTERRUPT_TIMER1               (INTERRUPT_HW_OFFSET + 1 )
   #define INTERRUPT_TIMER2               (INTERRUPT_HW_OFFSET + 2 )
   #define INTERRUPT_TIMER3               (INTERRUPT_HW_OFFSET + 3 )
   #define INTERRUPT_CODEC0               (INTERRUPT_HW_OFFSET + 4 )
   #define INTERRUPT_CODEC1               (INTERRUPT_HW_OFFSET + 5 )
   #define INTERRUPT_CODEC2               (INTERRUPT_HW_OFFSET + 6 )
   #define INTERRUPT_JPEG                 (INTERRUPT_HW_OFFSET + 7 )
   #define INTERRUPT_ISP                  (INTERRUPT_HW_OFFSET + 8 )
   #define INTERRUPT_USB                  (INTERRUPT_HW_OFFSET + 9 )
   #define INTERRUPT_3D                   (INTERRUPT_HW_OFFSET + 10 )
   #define INTERRUPT_TRANSPOSER           (INTERRUPT_HW_OFFSET + 11 )
   #define INTERRUPT_MULTICORESYNC0       (INTERRUPT_HW_OFFSET + 12 )
   #define INTERRUPT_MULTICORESYNC1       (INTERRUPT_HW_OFFSET + 13 )
   #define INTERRUPT_MULTICORESYNC2       (INTERRUPT_HW_OFFSET + 14 )
   #define INTERRUPT_MULTICORESYNC3       (INTERRUPT_HW_OFFSET + 15 )
   #define INTERRUPT_DMA0                 (INTERRUPT_HW_OFFSET + 16 )
   #define INTERRUPT_DMA1                 (INTERRUPT_HW_OFFSET + 17 )
   #define INTERRUPT_DMA2                 (INTERRUPT_HW_OFFSET + 18 )
   #define INTERRUPT_DMA3                 (INTERRUPT_HW_OFFSET + 19 )
   #define INTERRUPT_DMA4                 (INTERRUPT_HW_OFFSET + 20 )
   #define INTERRUPT_DMA5                 (INTERRUPT_HW_OFFSET + 21 )
   #define INTERRUPT_DMA6                 (INTERRUPT_HW_OFFSET + 22 )
   #define INTERRUPT_DMA7                 (INTERRUPT_HW_OFFSET + 23 )
   #define INTERRUPT_DMA8                 (INTERRUPT_HW_OFFSET + 24 )
#if defined(__BCM2708A0__)
   // A0 only has 9 dma interrupts
   //#define INTERRUPT_DMA9                 (INTERRUPT_HW_OFFSET + 25 )
   //#define INTERRUPT_DMA10                (INTERRUPT_HW_OFFSET + 26 )
   //#define INTERRUPT_DMA11                (INTERRUPT_HW_OFFSET + 27 )
   //#define INTERRUPT_DMA12                (INTERRUPT_HW_OFFSET + 28 )
   //#define INTERRUPT_DMA13                (INTERRUPT_HW_OFFSET + 29 )
   //#define INTERRUPT_DMA14                (INTERRUPT_HW_OFFSET + 30 )
   //#define INTERRUPT_DMA15                (INTERRUPT_HW_OFFSET + 31 )
#else
   #define INTERRUPT_DMA9                 (INTERRUPT_HW_OFFSET + 25 )
   #define INTERRUPT_DMA10                (INTERRUPT_HW_OFFSET + 26 )
   #define INTERRUPT_DMA11_12_13_14       (INTERRUPT_HW_OFFSET + 27 )
   #define INTERRUPT_DMA_ALL              (INTERRUPT_HW_OFFSET + 28 )
   #define INTERRUPT_UART_SPI0_SPI1       (INTERRUPT_HW_OFFSET + 29 )
   #define INTERRUPT_AUXIO                INTERRUPT_UART_SPI0_SPI1
   #define INTERRUPT_ARM                  (INTERRUPT_HW_OFFSET + 30 )
   #define INTERRUPT_DMA_VPU              (INTERRUPT_HW_OFFSET + 31 )
#endif
   #define INTERRUPT_HOSTPORT             (INTERRUPT_HW_OFFSET + 32 )
   #define INTERRUPT_VIDEOSCALER          (INTERRUPT_HW_OFFSET + 33 )
   #define INTERRUPT_CCP2TX               (INTERRUPT_HW_OFFSET + 34 )
   #define INTERRUPT_SDC                  (INTERRUPT_HW_OFFSET + 35 )
   #define INTERRUPT_DSI0                 (INTERRUPT_HW_OFFSET + 36 )
   #define INTERRUPT_AVE                  (INTERRUPT_HW_OFFSET + 37 )
   #define INTERRUPT_CAM0                 (INTERRUPT_HW_OFFSET + 38 )
   #   define INTERRUPT_CCP2              INTERRUPT_CAM0 // backward compatibility
   #define INTERRUPT_CAM1                 (INTERRUPT_HW_OFFSET + 39 )
   #   define INTERRUPT_CSI2              INTERRUPT_CAM1 // backward compatibility
   #define INTERRUPT_HDMI0                (INTERRUPT_HW_OFFSET + 40 )
   #define INTERRUPT_HDMI1                (INTERRUPT_HW_OFFSET + 41 )
   #define INTERRUPT_PIXELVALVE1          (INTERRUPT_HW_OFFSET + 42 )
   #define INTERRUPT_I2C_SLV              (INTERRUPT_HW_OFFSET + 43 )
   #define INTERRUPT_DSI1                 (INTERRUPT_HW_OFFSET + 44 )
   #define INTERRUPT_PWA0                 (INTERRUPT_HW_OFFSET + 45 )
   #define INTERRUPT_PWA1                 (INTERRUPT_HW_OFFSET + 46 )
   #define INTERRUPT_CPR                  (INTERRUPT_HW_OFFSET + 47 )
   #define INTERRUPT_SMI                  (INTERRUPT_HW_OFFSET + 48 )
   #define INTERRUPT_GPIO0                (INTERRUPT_HW_OFFSET + 49 )
   #define INTERRUPT_GPIO1                (INTERRUPT_HW_OFFSET + 50 )
   #define INTERRUPT_GPIO2                (INTERRUPT_HW_OFFSET + 51 )
   #define INTERRUPT_GPIO3                (INTERRUPT_HW_OFFSET + 52 )
   #define INTERRUPT_I2C                  (INTERRUPT_HW_OFFSET + 53 )
   #define INTERRUPT_SPI                  (INTERRUPT_HW_OFFSET + 54 )
   #define INTERRUPT_I2SPCM               (INTERRUPT_HW_OFFSET + 55 )
   #define INTERRUPT_SDIO                 (INTERRUPT_HW_OFFSET + 56 )
   #define INTERRUPT_UART                 (INTERRUPT_HW_OFFSET + 57 )
   #define INTERRUPT_SLIMBUS              (INTERRUPT_HW_OFFSET + 58 )
   #define INTERRUPT_VEC                  (INTERRUPT_HW_OFFSET + 59 )
   #define INTERRUPT_CPG                  (INTERRUPT_HW_OFFSET + 60 )
   #define INTERRUPT_RNG                  (INTERRUPT_HW_OFFSET + 61 )
#if defined(__BCM2708A0__)
   // FIXME: see middleware/rpc/rpc.c
   #define INTERRUPT_SPARE4               (INTERRUPT_HW_OFFSET + 62 )
   #define INTERRUPT_SPARE5               (INTERRUPT_HW_OFFSET + 63 )
#else
   #define INTERRUPT_ASDIO                (INTERRUPT_HW_OFFSET + 62 )
   #define INTERRUPT_AVSPMON              (INTERRUPT_HW_OFFSET + 63 )
#endif
   #define INTERRUPT_DUMMY                (INTERRUPT_HW_OFFSET + 63 )

   // aliases
   #define INTERRUPT_HOSTINTERFACE INTERRUPT_HOSTPORT
   #define INTERRUPT_SDCARDHOST    INTERRUPT_SDIO


   // temporary dummy register definitions to avoid compile errors
   #define DUMMYREG HW_REGISTER_RW( 0x7C ) //software exception vector 15

   /*---------------------------------------------------------------------------*/
   /* TODO FIXME ETC... VCII Clock Manager defs */
   #define CMPREC              DUMMYREG
   #define CMPRE1              DUMMYREG
   #define CMPRE2              DUMMYREG
   #define CMPRE3              DUMMYREG
   #define CMPLLC              DUMMYREG
   #define CMPLL1              DUMMYREG
   #define CMPLL2              DUMMYREG
   #define CMPLL3              DUMMYREG
   #define CMCORE              DUMMYREG
   #define CMCAM               DUMMYREG
   #define CMLCD               DUMMYREG
   #define CMACIS              DUMMYREG
   #define CMPCM               DUMMYREG
   #define CMUSB               DUMMYREG
   #define CMGEN               DUMMYREG
   #define CMMSP               DUMMYREG
   #define CMUART              DUMMYREG
   #define CMTIMER             DUMMYREG
   #define CMUARTF             DUMMYREG
   #define CMTIMERF            DUMMYREG
   #define CMNVT               DUMMYREG

   /*---------------------------------------------------------------------------*/
   /* Nexus Controller */

   #define NOWNT               HW_REGISTER_RW(DEBUG_MASTER_BASE + 0x4)
   #define NIOREQ              HW_REGISTER_RW(DEBUG_MASTER_BASE + 0x0)

   /*---------------------------------------------------------------------------*/
   /* Reset Controller */

//   #define RSTCS               HW_REGISTER_RW(RESET_CONTROLLER_BASE + 0x0)
//   #define RSTWD               HW_REGISTER_RW(RESET_CONTROLLER_BASE + 0x4)
//   #define RSTID               HW_REGISTER_RW(RESET_CONTROLLER_BASE + 0x8)
//   #define RSTFD               HW_REGISTER_RW(RESET_CONTROLLER_BASE + 0xc)
//
//   #define RSC0ADDR            HW_REGISTER_RW(RESET_CONTROLLER_BASE + 0x10)

   /*---------------------------------------------------------------------------*/
   /* Scaler hardware registers */

   #define SCALER_BASE_ADDRESS               SCALER_BASE
   #define SCALER_INPUT_CONTROL              HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x00 )
   #define SCALER_IRQ_STATUS                 HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x04 )
   #define SCALER_ID                         HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x08 )
   #define SCALER_ALT_CONTROL                HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x0C )
   #define SCALER_PROFILE                    HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x10 )
   #define SCALER_DITHER                     HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x14 )
   //#define SCALER_DISPEOLN                   HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x18 )

   #define SCALER_DISP_LIST_0                HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x20 )
   #define SCALER_DISP_LIST_1                HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x24 )
   #define SCALER_DISP_LIST_2                HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x28 )
   #define SCALER_DISP_LIST_STATUS           HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x2C )

   #define SCALER_DISPCTL_0                  HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x40 )
   #define SCALER_DISPBKGND_0                HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x44 )
   #define SCALER_DISPSTAT_0                 HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x48 )

   #define SCALER_DISPCTL_1                  HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x50 )
   #define SCALER_DISPBKGND_1                HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x54 )
   #define SCALER_DISPSTAT_1                 HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x58 )
   #define SCALER_DISPBASE_1                 HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x5C )

   #define SCALER_DISPCTL_2                  HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x60 )
   #define SCALER_DISPBKGND_2                HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x64 )
   #define SCALER_DISPSTAT_2                 HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x68 )
   #define SCALER_DISPBASE_2                 HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x6C )

   #define SCALER_GAM_ADDRESS                HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0x78 )
   #define SCALER_GAM_DATA                   HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0xE0 )

   //#define SCALER_DISPSLAVE0                 HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0xC0 )
   //#define SCALER_DISPSLAVE1                 HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0xC8 )
   //#define SCALER_DISPSLAVE2                 HW_REGISTER_RW( SCALER_BASE_ADDRESS + 0xD0 )

   //the start location of the scalers context memory
   #define SCALER_CONTEXT_MEMORY_START       (SCALER_BASE_ADDRESS + 0x2000)

   #define SCALER_CONTEXT_MEM_SIZE           ( 1024 * 16 ) //16k

   //the size of the line buffer memory
   #define SCALER_LINE_BUFFER_MEM_SIZE       (94 * 1024)

   //The size of the COB buffer (the output fifo) in pixels
   #define SCALER_COB_FIFO_SIZE              (0x4000) //16Kpix == 48kBytes

   /*---------------------------------------------------------------------------*/
   /* PWM */
   #define PWMCTL                            HW_REGISTER_RW( PWM_BASE_ADDRESS + 0x00 )
   #define PWMSTA                            HW_REGISTER_RW( PWM_BASE_ADDRESS + 0x04 )
   #define PWMDMAC                           HW_REGISTER_RW( PWM_BASE_ADDRESS + 0x08 )
   #define PWMRNG1                           HW_REGISTER_RW( PWM_BASE_ADDRESS + 0x10 )
   #define PWMDAT1                           HW_REGISTER_RW( PWM_BASE_ADDRESS + 0x14 )
   #define PWMFIF1                           HW_REGISTER_RW( PWM_BASE_ADDRESS + 0x18 )
   #define PWMRNG2                           HW_REGISTER_RW( PWM_BASE_ADDRESS + 0x20 )
   #define PWMDAT2                           HW_REGISTER_RW( PWM_BASE_ADDRESS + 0x24 )
   #define PWMRNG3                           HW_REGISTER_RW( PWM_BASE_ADDRESS + 0x30 )
   #define PWMDAT3                           HW_REGISTER_RW( PWM_BASE_ADDRESS + 0x34 )
   #define PWMRNG4                           HW_REGISTER_RW( PWM_BASE_ADDRESS + 0x40 )
   #define PWMDAT4                           HW_REGISTER_RW( PWM_BASE_ADDRESS + 0x44 )

   #define PWMRNG(n)                         HW_REGISTER_RW( PWM_BASE_ADDRESS + 0x10*n )     // n=1,2,3,4
   #define PWMDAT(n)                         HW_REGISTER_RW( PWM_BASE_ADDRESS + 0x10*n + 4 )

   #define PWMCTL_PWEN1                      0
   #define PWMCTL_MODE1                      1
   #define PWMCTL_RPTL1                      2
   #define PWMCTL_SBIT1                      3
   #define PWMCTL_POLA1                      4
   #define PWMCTL_USEF1                      5
   #define PWMCTL_CLRF1                      6
   #define PWMCTL_MSEN1                      7
   #define PWMCTL_PWEN2                      8
   #define PWMCTL_MODE2                      9
   #define PWMCTL_RPTL2                      10
   #define PWMCTL_SBIT2                      11
   #define PWMCTL_POLA2                      12
   #define PWMCTL_USEF2                      13
   #define PWMCTL_MSEN2                      15
   #define PWMCTL_PWEN3                      16
   #define PWMCTL_MODE3                      17
   #define PWMCTL_RPTL3                      18
   #define PWMCTL_SBIT3                      19
   #define PWMCTL_POLA3                      20
   #define PWMCTL_USEF3                      21
   #define PWMCTL_MSEN3                      23
   #define PWMCTL_PWEN4                      24
   #define PWMCTL_MODE4                      25
   #define PWMCTL_RPTL4                      26
   #define PWMCTL_SBIT4                      27
   #define PWMCTL_POLA4                      28
   #define PWMCTL_USEF4                      29
   #define PWMCTL_MSEN4                      31
   #define PWMCTL_PWEN(n)                    (((n-1)<<3)+0)      // n=1,2,3,4
   #define PWMCTL_MODE(n)                    (((n-1)<<3)+1)
   #define PWMCTL_RPTL(n)                    (((n-1)<<3)+2)
   #define PWMCTL_SBIT(n)                    (((n-1)<<3)+3)
   #define PWMCTL_POLA(n)                    (((n-1)<<3)+4)
   #define PWMCTL_USEF(n)                    (((n-1)<<3)+5)
   #define PWMCTL_MSEN(n)                    (((n-1)<<3)+7)

   #define PWMSTA_FULL1                      0
   #define PWMSTA_EMPT1                      1
   #define PWMSTA_WERR1                      2
   #define PWMSTA_RERR1                      3
   #define PWMSTA_GAPO1                      4
   #define PWMSTA_GAPO2                      5
   #define PWMSTA_GAPO3                      6
   #define PWMSTA_GAPO4                      7
   #define PWMSTA_BERR                       8
   #define PWMSTA_STA1                       9
   #define PWMSTA_STA2                       10
   #define PWMSTA_STA3                       11
   #define PWMSTA_STA4                       12

   #define PWMDMAC_DREQ_LEN                  8
   #define PWMDMAC_DREQ                      0
   #define PWMDMAC_PANIC_LEN                 8
   #define PWMDMAC_PANIC                     8
   #define PWMDMAC_ENAB                      31

   /*---------------------------------------------------------------------------*/
   /* Transposer */

   #define TRANSPOSER_DST_PTR                HW_REGISTER_RW( TRANSPOSER_BASE_ADDRESS + 0x00 )
   #define TRANSPOSER_DST_PITCH              HW_REGISTER_RW( TRANSPOSER_BASE_ADDRESS + 0x04 )
   #define TRANSPOSER_DIMENSIONS             HW_REGISTER_RW( TRANSPOSER_BASE_ADDRESS + 0x08 )
   #define TRANSPOSER_CONTROL                HW_REGISTER_RW( TRANSPOSER_BASE_ADDRESS + 0x0C )
   #define TRANSPOSER_PROGRESS               HW_REGISTER_RO( TRANSPOSER_BASE_ADDRESS + 0x10 )

   /*---------------------------------------------------------------------------*/
   /* Video Codec */

   #define VCSIGNAL0           HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x4408b4)
   #define VCINTMASK0          HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x4408b8)
   #define VCSIGNAL1           HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x4408bc)
   #define VCINTMASK1          HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x4408c0)
#ifndef VCODEC_VERSION
   // Set default to old A0 version
   #define VCODEC_VERSION 821
#endif

#if (VCODEC_VERSION>=800)

   #define VCE_BASE 0x7f100000
   #define VCE_DATA_MEM_OFFSET 0
   #define VCE_DATA_MEM_SIZE 0x2000
   #define VCE_PROGRAM_MEM_OFFSET 0x10000
   #define VCE_PROGRAM_MEM_SIZE 0x4000
   #define VCE_REGISTERS_OFFSET 0x20000
   #define VCE_REGISTERS_COUNT 63
   #define VCE_STATUS_OFFSET 0x40000
   #define VCE_STATUS_BUSYBITS_MASK 0xffff
   #define VCE_STATUS_REASON_POS 16
   #define VCE_STATUS_REASON_MASK 0x1f
   #define VCE_BUSY_BKPT 0x00
   #define VCE_BUSY_USER 0x01 // up to 0x07 inclusive
   #define VCE_BUSY_DMAIN 0x08
   #define VCE_BUSY_DMAOUT 0x09
   #define VCE_BUSY_MEMSYNC 0x0a
   #define VCE_BUSY_SLEEP 0x0b
   #define VCE_REASON_STOPPED 0x10
   #define VCE_REASON_RUNNING 0x11
   #define VCE_REASON_RESET 0x12
   #define VCE_REASON_SINGLE 0x13
   #define VCE_STATUS_RUNNING_POS 24
   #define VCE_STATUS_NANOFLAG_POS 25
   #define VCE_STATUS_INTERRUPT_POS 31
   #define VCE_VERSION_OFFSET 0x40004
   #define VCE_PC_PF0_OFFSET 0x40008
   #define VCE_PC_IF0_OFFSET 0x4000c
   #define VCE_PC_RD0_OFFSET 0x40010
   #define VCE_PC_EX0_OFFSET 0x40014
   #define VCE_CONTROL_OFFSET 0x40020
   #define VCE_CONTROL_CLEAR_RUN 0
   #define VCE_CONTROL_SET_RUN 1
   #define VCE_CONTROL_SINGLE_STEP 3
   #define VCE_BAD_ADDR_OFFSET 0x40030
   #define VCE_SEMA_CLEAR_OFFSET 0x40024
   #define VCE_SEMA_SET_OFFSET 0x40028
   #define VCE_SEMA_COUNT 8
   #define VCE_SIM_DEBUG_OPTIONS_OFFSET 0x40100

   #define VCE_DATA_MEM_BASE      HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x100000)
   #define VCE_PROGRAM_MEM_BASE   HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x110000)
   #define VCE_REGISTERS_BASE     HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x120000)
   #define VCE_STATUS             HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x140000)
   #define VCE_VERSION            HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x140004)
   #define VCE_PC_PF0             HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x140008)
   #define VCE_PC_IF0             HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x14000C)
   #define VCE_PC_RD0             HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x140010)
   #define VCE_PC_EX0             HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x140014)
   #define VCE_CONTROL            HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x140020)
   #define VCE_SEMA_CLEAR         HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x140024)
   #define VCE_SEMA_SET           HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x140028)
   #define VCE_BAD_ADDR           HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x140030)
   #define VCE_SIM_DEBUG_OPTIONS  HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x140100)

#else
   #define PP_PC               HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x110000)
   #define PP_CNTL             HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x110004)
   #define PP_ACC              HW_REGISTER_RW(VIDEOCODEC_BASE_ADDRESS + 0x110008)
#endif

   /*---------------------------------------------------------------------------*/
   /* DSI */

   // Define some macros for compatability with old VCIII code which does not
   // know about more than one dsi peripheral. Just direct it to DSI0...
   #define DSI_CTRL               HW_REGISTER_RW( DSI_BASE + 0x00 )
   #define DSI_CMD_PKTC           HW_REGISTER_RW( DSI_BASE + 0x04 )
   #define DSI_CMD_PKTH           HW_REGISTER_RW( DSI_BASE + 0x08 )
   #define DSI_RX1_PKTH           HW_REGISTER_RW( DSI_BASE + 0x0C )
   #define DSI_RX2_PKTH           HW_REGISTER_RW( DSI_BASE + 0x10 )
   #define DSI_CMD_DATA_FIFO      HW_REGISTER_RW( DSI_BASE + 0x14 )
   #define DSI_DISP0_CTRL         HW_REGISTER_RW( DSI_BASE + 0x18 )
   #define DSI_DISP1_CTRL         HW_REGISTER_RW( DSI_BASE + 0x1C )
   #define DSI_PIX_FIFO           HW_REGISTER_RW( DSI_BASE + 0x20 )
   #define   DSI_INT_STATUS       HW_REGISTER_RW( DSI_BASE + 0x24  )
   #define   DSI_INT_ENABLE       HW_REGISTER_RW( DSI_BASE + 0x28  )
   #define   DSI_STAT             HW_REGISTER_RW( DSI_BASE + 0x2C  )
   #define   DSI_HSTX_TO_CNT      HW_REGISTER_RW( DSI_BASE + 0x30  )
   #define   DSI_LPRX_TO_CNT      HW_REGISTER_RW( DSI_BASE + 0x34  )
   #define   DSI_TA_TO_CNT        HW_REGISTER_RW( DSI_BASE + 0x38  )
   #define   DSI_PR_TO_CNT        HW_REGISTER_RW( DSI_BASE + 0x3C  )
   #define   DSI_PHY_CONTROL      HW_REGISTER_RW( DSI_BASE + 0x40  )
   // HSCLKZERO, HSCLKPRE, HSCLKPREP
   #define   DSI_HS_CLT0          HW_REGISTER_RW( DSI_BASE + 0x44  )
   // HSCLKTRAIL, HSCLKPOST
   #define   DSI_HS_CLT1          HW_REGISTER_RW( DSI_BASE + 0x48  )
   // HSWAKEUP
   #define   DSI_HS_CLT2          HW_REGISTER_RW( DSI_BASE + 0x4C  )
   // HS EXIT, ZERO, PRE
   #define   DSI_HS_DLT3          HW_REGISTER_RW( DSI_BASE + 0x50  )
   // HS TRAIL LPX
   #define   DSI_HS_DLT4          HW_REGISTER_RW( DSI_BASE + 0x54  )
   // HS INIT
   #define   DSI_HS_DLT5          HW_REGISTER_RW( DSI_BASE + 0x58  )
   // lp GET SURE GO LPX
   #define   DSI_LP_DLT6          HW_REGISTER_RW( DSI_BASE + 0x5C  )
   // LP WAKEUP
   #define   DSI_LP_DLT7          HW_REGISTER_RW( DSI_BASE + 0x60  )
   #define   DSI_AFEC0            HW_REGISTER_RW( DSI_BASE + 0x64  )
   #define   DSI_AFEC1            HW_REGISTER_RW( DSI_BASE + 0x68  )
   #define   DSI_TST_SEL          HW_REGISTER_RW( DSI_BASE + 0x6C  )
   #define   DSI_TST_MON          HW_REGISTER_RW( DSI_BASE + 0x70  )

   /*---------------------------------------------------------------------------*/
   /* CDP */

   #define   CDPC                 HW_REGISTER_RW( CDP_BASE + 0x00  )
   #define   CDP_PHYC             HW_REGISTER_RW( CDP_BASE + 0x04  )
   #define   CDP_PHYTSTDAT        HW_REGISTER_RW( CDP_BASE + 0x08  )
   #define   CDP_DEBUG0           HW_REGISTER_RW( CDP_BASE + 0x0C  )
   #define   CDP_DEBUG1           HW_REGISTER_RW( CDP_BASE + 0x10  )

   /*---------------------------------------------------------------------------*/
   /* BootROM Peripheral */

   #define BOOTROM_ROM_START                 ( BOOTROM_BASE_ADDRESS + 0x0 ) //Start of the ROM
   #define BOOTROM_ROM_LENGTH                ( 1024 * 32 ) // Length of the Boot ROM

   #define BOOTROM_RAM_START                 ( BOOTROM_BASE_ADDRESS + 0x8000 ) //Start of the RAM
   #define BOOTROM_RAM_LENGTH                ( 1024 * 2 ) // Length of the Boot RAM

   #define BOOTROM_BRCTL                     HW_REGISTER_RW( BOOTROM_BASE_ADDRESS + 0xC000 )

   /*---------------------------------------------------------------------------*/
   /* Unicam Peripheral */
   #define UNICAM_REG( x, d )  HW_REGISTER_RW( (((x) == 0) ? CAM0_BASE : CAM1_BASE) + d )

#if defined(__BCM2708A0__)
   #define UNICAM_CTRL( x )    UNICAM_REG( x, 0x000 )
   #define UNICAM_STA( x )     UNICAM_REG( x, 0x004 )
   #define UNICAM_ANA( x )     UNICAM_REG( x, 0x008 )
   #define UNICAM_PRI( x )     UNICAM_REG( x, 0x00c )
   #define UNICAM_CLK( x )     UNICAM_REG( x, 0x010 )
   #define UNICAM_DAT0( x )    UNICAM_REG( x, 0x014 )
   #define UNICAM_DAT1( x )    UNICAM_REG( x, 0x018 )
   #define UNICAM_DAT2( x )    UNICAM_REG( x, 0x01c )
   #define UNICAM_DAT3( x )    UNICAM_REG( x, 0x020 )
   #define UNICAM_CMP0( x )    UNICAM_REG( x, 0x024 )
   #define UNICAM_CMP1( x )    UNICAM_REG( x, 0x028 )
   #define UNICAM_CAP0( x )    UNICAM_REG( x, 0x02c )
   #define UNICAM_CAP1( x )    UNICAM_REG( x, 0x030 )
   #define UNICAM_DBG0( x )    UNICAM_REG( x, 0x0f0 )
   #define UNICAM_DBG1( x )    UNICAM_REG( x, 0x0f4 )
   #define UNICAM_DBG2( x )    UNICAM_REG( x, 0x0f8 )
   #define UNICAM_ICTL( x )    UNICAM_REG( x, 0x100 )
   #define UNICAM_ISTA( x )    UNICAM_REG( x, 0x104 )
   #define UNICAM_IDI( x )     UNICAM_REG( x, 0x108 )
   #define UNICAM_IPIPE( x )   UNICAM_REG( x, 0x10c )
   #define UNICAM_IBSA( x )    UNICAM_REG( x, 0x110 )
   #define UNICAM_IBEA( x )    UNICAM_REG( x, 0x114 )
   #define UNICAM_IBLS( x )    UNICAM_REG( x, 0x118 )
   #define UNICAM_IBWP( x )    UNICAM_REG( x, 0x11c )
   #define UNICAM_IHWIN( x )   UNICAM_REG( x, 0x120 )
   #define UNICAM_IHSTA( x )   UNICAM_REG( x, 0x124 )
   #define UNICAM_IVWIN( x )   UNICAM_REG( x, 0x128 )
   #define UNICAM_IVSTA( x )   UNICAM_REG( x, 0x12c )
   #define UNICAM_DCS( x )     UNICAM_REG( x, 0x200 )
   #define UNICAM_DBSA( x )    UNICAM_REG( x, 0x204 )
   #define UNICAM_DBEA( x )    UNICAM_REG( x, 0x208 )
   #define UNICAM_DBWP( x )    UNICAM_REG( x, 0x20c )
#else
   #define UNICAM_CTRL( x )    UNICAM_REG( x, 0x000 )
   #define UNICAM_STA( x )     UNICAM_REG( x, 0x004 )
   #define UNICAM_ANA( x )     UNICAM_REG( x, 0x008 )
   #define UNICAM_PRI( x )     UNICAM_REG( x, 0x00c )
   #define UNICAM_CLK( x )     UNICAM_REG( x, 0x010 )
   #define UNICAM_CLT( x )     UNICAM_REG( x, 0x014 )
   #define UNICAM_DAT0( x )    UNICAM_REG( x, 0x018 )
   #define UNICAM_DAT1( x )    UNICAM_REG( x, 0x01c )
   #define UNICAM_DAT2( x )    UNICAM_REG( x, 0x020 )
   #define UNICAM_DAT3( x )    UNICAM_REG( x, 0x024 )
   #define UNICAM_DLT( x )     UNICAM_REG( x, 0x028 )
   #define UNICAM_CMP0( x )    UNICAM_REG( x, 0x02c )
   #define UNICAM_CMP1( x )    UNICAM_REG( x, 0x030 )
   #define UNICAM_CAP0( x )    UNICAM_REG( x, 0x034 )
   #define UNICAM_CAP1( x )    UNICAM_REG( x, 0x038 )
   #define UNICAM_ICTL( x )    UNICAM_REG( x, 0x100 )
   #define UNICAM_ISTA( x )    UNICAM_REG( x, 0x104 )
   #define UNICAM_IDI0( x )    UNICAM_REG( x, 0x108 )
   #define UNICAM_IPIPE( x )   UNICAM_REG( x, 0x10c )
   #define UNICAM_IBSA0( x )   UNICAM_REG( x, 0x110 )
   #define UNICAM_IBEA0( x )   UNICAM_REG( x, 0x114 )
   #define UNICAM_IBLS( x )    UNICAM_REG( x, 0x118 )
   #define UNICAM_IBWP( x )    UNICAM_REG( x, 0x11c )
   #define UNICAM_IHWIN( x )   UNICAM_REG( x, 0x120 )
   #define UNICAM_IHSTA( x )   UNICAM_REG( x, 0x124 )
   #define UNICAM_IVWIN( x )   UNICAM_REG( x, 0x128 )
   #define UNICAM_IVSTA( x )   UNICAM_REG( x, 0x12c )
   #define UNICAM_ICC( x )     UNICAM_REG( x, 0x130 )
   #define UNICAM_ICS( x )     UNICAM_REG( x, 0x134 )
   #define UNICAM_IDC( x )     UNICAM_REG( x, 0x138 )
   #define UNICAM_IDPO( x )    UNICAM_REG( x, 0x13c )
   #define UNICAM_IDCA( x )    UNICAM_REG( x, 0x140 )
   #define UNICAM_IDCD( x )    UNICAM_REG( x, 0x144 )
   #define UNICAM_IDS( x )     UNICAM_REG( x, 0x148 )
   #define UNICAM_DCS( x )     UNICAM_REG( x, 0x200 )
   #define UNICAM_DBSA0( x )   UNICAM_REG( x, 0x204 )
   #define UNICAM_DBEA0( x )   UNICAM_REG( x, 0x208 )
   #define UNICAM_DBWP( x )    UNICAM_REG( x, 0x20c )
   #define UNICAM_DBCTL( x )   UNICAM_REG( x, 0x300 )
   #define UNICAM_IBSA1( x )   UNICAM_REG( x, 0x304 )
   #define UNICAM_IBEA1( x )   UNICAM_REG( x, 0x308 )
   #define UNICAM_IDI1( x )    UNICAM_REG( x, 0x30c )
   #define UNICAM_DBSA1( x )   UNICAM_REG( x, 0x310 )
   #define UNICAM_DBEA1( x )   UNICAM_REG( x, 0x314 )
   #define UNICAM_MISC( x )    UNICAM_REG( x, 0x400 )
#endif

   /*---------------------------------------------------------------------------*/
   /* CCP2TX Peripheral - now from ccp2tx[_a0].h */
   #define CCP2TC                                     CCP2TX_TC
   #define CCP2TS                                     CCP2TX_TS
#ifndef __BCM2708A0__
   #define CCP2TAC                                    CCP2TX_TAC
#endif
   #define CCP2TPC                                    CCP2TX_TPC
   #define CCP2TSC                                    CCP2TX_TSC
   #define CCP2TIC                                    CCP2TX_TIC
   #define CCP2TTC                                    CCP2TX_TTC
   #define CCP2TBA                                    CCP2TX_TBA
   #define CCP2TDL                                    CCP2TX_TDL
   #define CCP2TD                                     CCP2TX_TD
#ifndef __BCM2708A0__
   #define CCP2TSPARE                                 CCP2TX_TSPARE
#endif

   /*---------------------------------------------------------------------------*/
   /* VEC Peripheral */

   #define WSE_RESET                                  VEC_WSE_RESET
   #define WSE_CONTROL                                VEC_WSE_CONTROL
   #define WSE_WSS_DATA                               VEC_WSE_WSS_DATA
   #define WSE_VPS_DATA_1                             VEC_WSE_VPS_DATA_1
   #define WSE_VPS_CONTROL                            VEC_WSE_VPS_CONTROL

   #define CGMSAE_RESET                               VEC_CGMSAE_RESET      
   #define CGMSAE_TOP_CONTROL                         VEC_CGMSAE_TOP_CONTROL
   #define CGMSAE_BOT_CONTROL                         VEC_CGMSAE_BOT_CONTROL
   #define CGMSAE_TOP_FORMAT                          VEC_CGMSAE_TOP_FORMAT 
   #define CGMSAE_BOT_FORMAT                          VEC_CGMSAE_BOT_FORMAT 
   #define CGMSAE_TOP_DATA                            VEC_CGMSAE_TOP_DATA   
   #define CGMSAE_BOT_DATA                            VEC_CGMSAE_BOT_DATA   
   #define CGMSAE_REVID                               VEC_CGMSAE_REVID      

   /*---------------------------------------------------------------------------*/
   /* System Timer */

   // The old register names are used all over the place, so we probably need to
   // keep these for now.
   // These also appear in systimer.h; ideally, any modules which access these
   // registers directly should #include that header file.
#if 1 // TODO: remove these one day.
   #define STCS_0                ST_CS
   #define STC_0                 ST_CLO
   #define STCLO_0               ST_CLO
   #define STCHI_0               ST_CHI
   #define STC0_0                ST_C0
   #define STC1_0                ST_C1
   #define STC2_0                ST_C2
   #define STC3_0                ST_C3

   #define STCS                  ST_CS
   #define STC                   ST_CLO
   #define STCLO                 ST_CLO
   #define STCHI                 ST_CHI
   #define STC0                  ST_C0
   #define STC1                  ST_C1
   #define STC2                  ST_C2
   #define STC3                  ST_C3
#endif
   /*---------------------------------------------------------------------------*/
   /* Crypto/ID module */
   #define IDCLVWMCU           HW_REGISTER_RW(0x10002000)
   #define IDCMDIDU            HW_REGISTER_RW(0x10002004)
   #define IDCKEYHU            HW_REGISTER_RW(0x10002008)
   #define IDCKEYLU            HW_REGISTER_RW(0x1000200C)
   #define IDCCMD              HW_REGISTER_RW(0x10002010)
   #define IDCCFG              HW_REGISTER_RW(0x10002014)
   #define IDCKSEL             HW_REGISTER_RW(0x10002018)
   #define IDCLVWMC            HW_REGISTER_RW(0x10002020)
   #define IDCMDID             HW_REGISTER_RW(0x10002024)

   /*---------------------------------------------------------------------------*/
   /* Single 16550 UART */
   #define URBR                HW_REGISTER_RO(UART_BASE + 0x00)
   #define UTHR                HW_REGISTER_RW(UART_BASE + 0x00)
   #define UIER                HW_REGISTER_RW(UART_BASE + 0x04)
   #define UIIR                HW_REGISTER_RO(UART_BASE + 0x08)
   #define UFCR                HW_REGISTER_RW(UART_BASE + 0x08)
   #define ULCR                HW_REGISTER_RW(UART_BASE + 0x0C)
   #define UMCR                HW_REGISTER_RW(UART_BASE + 0x10)
   #define ULSR                HW_REGISTER_RW(UART_BASE + 0x14)
   #define UMSR                HW_REGISTER_RW(UART_BASE + 0x18)
   #define USCR                HW_REGISTER_RW(UART_BASE + 0x1C)
   #define UDLL                HW_REGISTER_RW(UART_BASE + 0x00)
   #define UDLM                HW_REGISTER_RW(UART_BASE + 0x04)
   #define UEN                 HW_REGISTER_RW(UART_BASE + 0x20)

   #define VIDEOCORE_NUM_UART_PORTS    1

   /*---------------------------------------------------------------------------*/
   /* ADC */
   #define ADCCS               HW_REGISTER_RW(ADC_BASE_ADDRESS + 0x00)
   #define ADCR0               HW_REGISTER_RW(ADC_BASE_ADDRESS + 0x04)
   #define ADCR1               HW_REGISTER_RW(ADC_BASE_ADDRESS + 0x08)

   /*---------------------------------------------------------------------------*/
   /* General Purpose I/O */

   //Max num of pins in the chip
   #define GPIO_MAX_PINS      54

   /*---------------------------------------------------------------------------*/
   /* JPEG block */
   #define JCTRL               HW_REGISTER_RW(JPEG_BASE +    0)
   #define JICST               HW_REGISTER_RW(JPEG_BASE +  0x4)
   #define JMCTRL              HW_REGISTER_RW(JPEG_BASE +  0x8)
   #define JDCCTRL             HW_REGISTER_RW(JPEG_BASE + 0x0C)
   #define JCBA                HW_REGISTER_RW(JPEG_BASE + 0x10)
   #define JNCB                HW_REGISTER_RW(JPEG_BASE + 0x14)
   #define JSDA                HW_REGISTER_RW(JPEG_BASE + 0x18)
   #define JNSB                HW_REGISTER_RW(JPEG_BASE + 0x1C)
   #define JSBO                HW_REGISTER_RW(JPEG_BASE + 0x20)
   #define JMOP                HW_REGISTER_RW(JPEG_BASE + 0x24)
   #define JHADDR              HW_REGISTER_RW(JPEG_BASE + 0x28)
   #define JHWDATA             HW_REGISTER_RW(JPEG_BASE + 0x2C)
   #define JMADDR              HW_REGISTER_RW(JPEG_BASE + 0x30)
   #define JMWDATA             HW_REGISTER_RW(JPEG_BASE + 0x34)
   #define JOADDR              HW_REGISTER_RW(JPEG_BASE + 0x38)
   #define JOWDATA             HW_REGISTER_RW(JPEG_BASE + 0x3C)
   #define JQADDR              HW_REGISTER_RW(JPEG_BASE + 0x40)
   #define JQWDATA             HW_REGISTER_RW(JPEG_BASE + 0x44)
   #define JQCTRL              HW_REGISTER_RW(JPEG_BASE + 0x48)
   #define JC0BA               HW_REGISTER_RW(JPEG_BASE + 0x4C)
   #define JC1BA               HW_REGISTER_RW(JPEG_BASE + 0x50)
   #define JC2BA               HW_REGISTER_RW(JPEG_BASE + 0x54)
   #define JC0S                HW_REGISTER_RW(JPEG_BASE + 0x58)
   #define JC1S                HW_REGISTER_RW(JPEG_BASE + 0x5C)
   #define JC2S                HW_REGISTER_RW(JPEG_BASE + 0x60)
   #define JC0W                HW_REGISTER_RW(JPEG_BASE + 0x64)
   #define JC1W                HW_REGISTER_RW(JPEG_BASE + 0x68)
   #define JC2W                HW_REGISTER_RW(JPEG_BASE + 0x6C)

   #define JCTRL_START         (1 << 7)
   #define JCTRL_DCTEN         (1 << 4)
   #define JCTRL_RESET         (1 << 3)
   #define JCTRL_FLUSH         (1 << 2)
   #define JCTRL_STUFF         (1 << 1)
   #define JCTRL_MODE          (1 << 0)

   #define JHADDR_TABLEF       (1 << 31)

   #define JICST_ERR           (1 << 19)
   #define JICST_MARKER        (1 << 18)
   #define JICST_SDONE         (1 << 17)
   #define JICST_CDONE         (1 << 16)
   #define JICST_INTE          (1 <<  3)
   #define JICST_INTM          (1 <<  2)
   #define JICST_INTSD         (1 <<  1)
   #define JICST_INTCD         (1 <<  0)

   #define JDCCTRL_DISDC       (1 << 20)
   #define JDCCTRL_SETDC(n)    (1 << ((n) + 16))
   #define JDCCTRL_DCCOMP_MASK 0xFFFF

   #define JMCTRL_DC_TAB(n)    (1 << (2*(n)))
   #define JMCTRL_AC_TAB(n)    (1 << (2*(n)+1))
   #define JMCTRL_NUMCMP       (1 << 8)
   #define JMCTRL_CMP(n)       (1 << (4*(n) + 16))
   #define JMCTRL_420_MODE     (0 << 14)
   #define JMCTRL_422_MODE     (1 << 14)
   #define JMCTRL_444_MODE     (2 << 14)

   #define JMCTRL_UNUSED_BITS  ((1 << 13) | (1 << 12) | (1 << 11))

   #define AC_HUFFTABLE_OFFSET(t) ((t) * 0x100)
   #define DC_HUFFTABLE_OFFSET(t) ((t) * 0x10 + 0x200)

   #define AC_OSETTABLE_OFFSET(t) ((t) * 0x10)
   #define DC_OSETTABLE_OFFSET(t) ((t) * 0x10 + 0x20)

   #define AC_MAXCTABLE_OFFSET(t) ((t) * 0x10)
   #define DC_MAXCTABLE_OFFSET(t) ((t) * 0x10 + 0x20)

   /*---------------------------------------------------------------------------*/
   /* External Memory Interface */
   #define SDCS               HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x00)
   #define SDSA               HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x04)
   #define SDSB               HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x08)
   #define SDSC               HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x0C)
   #define SDEM               HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x10)
   #define SDPT               HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x14)
   #define SDIDL              HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x18)
   #define SDRTC              HW_REGISTER_RO( SDRAM_BASE_ADDRESS + 0x1C)
   #define SDWTC              HW_REGISTER_RO( SDRAM_BASE_ADDRESS + 0x20)
   #define SDRDC              HW_REGISTER_RO( SDRAM_BASE_ADDRESS + 0x24)
   #define SDWDC              HW_REGISTER_RO( SDRAM_BASE_ADDRESS + 0x28)
   #define SDRAC              HW_REGISTER_RO( SDRAM_BASE_ADDRESS + 0x2C)
   #define SDCYC              HW_REGISTER_RO( SDRAM_BASE_ADDRESS + 0x30)
   #define SDACC              HW_REGISTER_RO( SDRAM_BASE_ADDRESS + 0x34)
   #define SDDAT              HW_REGISTER_RO( SDRAM_BASE_ADDRESS + 0x38)
   #define SDSECSRT0          HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x3C)
   #define SDSECEND0          HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x40)
   #define SDSECSRT1          HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x44)
   #define SDSECEND1          HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x48)
   #define SDSECSRT2          HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x4C)
   #define SDSECEND2          HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x50)
   #define SDSECSRT3          HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x54)
   #define SDSECEND3          HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x58)
   #define SDDELC0            HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x5C)
   #define SDDELS0            HW_REGISTER_RO( SDRAM_BASE_ADDRESS + 0x60)
   #define SDDELC1            HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x64)
   #define SDDELS1            HW_REGISTER_RO( SDRAM_BASE_ADDRESS + 0x68)
   #define SDDELC2            HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x6C)
   #define SDDELS2            HW_REGISTER_RO( SDRAM_BASE_ADDRESS + 0x70)
   #define SDDELC3            HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x74)
   #define SDDELS3            HW_REGISTER_RO( SDRAM_BASE_ADDRESS + 0x78)
   #define SDTMC              HW_REGISTER_RW( SDRAM_BASE_ADDRESS + 0x7C)


   /*---------------------------------------------------------------------------*/
   /* Secondary Memory Interface */

   #define SMICS               HW_REGISTER_RW(SMI_BASE + 0x00)
   #define SMIL                HW_REGISTER_RW(SMI_BASE + 0x04)
   #define SMIA                HW_REGISTER_RW(SMI_BASE + 0x08)
   #define SMID                HW_REGISTER_RW(SMI_BASE + 0x0C)
   #define SMIDSR0             HW_REGISTER_RW(SMI_BASE + 0x10)
   #define SMIDSW0             HW_REGISTER_RW(SMI_BASE + 0x14)
   #define SMIDSR1             HW_REGISTER_RW(SMI_BASE + 0x18)
   #define SMIDSW1             HW_REGISTER_RW(SMI_BASE + 0x1C)
   #define SMIDSR2             HW_REGISTER_RW(SMI_BASE + 0x20)
   #define SMIDSW2             HW_REGISTER_RW(SMI_BASE + 0x24)
   #define SMIDSR3             HW_REGISTER_RW(SMI_BASE + 0x28)
   #define SMIDSW3             HW_REGISTER_RW(SMI_BASE + 0x2C)
   #define SMIDC               HW_REGISTER_RW(SMI_BASE + 0x30)
   #define SMIDCS              HW_REGISTER_RW(SMI_BASE + 0x34)
   #define SMIDA               HW_REGISTER_RW(SMI_BASE + 0x38)
   #define SMIDD               HW_REGISTER_RW(SMI_BASE + 0x3C)
   #define SMIFD               HW_REGISTER_RW(SMI_BASE + 0x40)

   #define SMI_FIFO_ADDRESS(device,addr)   (((((device))&0x3)<<8)|((addr)&0xff))

   // SMI control register bits
   #define SMICS_ENABLE    0
   #define SMICS_DONE      1
   #define SMICS_ACTIVE    2
   #define SMICS_START     3
   #define SMICS_CLEARFIFO 4
   #define SMICS_WRITE     5
   #define SMICS_PAD       6
   #define SMICS_TEEN      8
   #define SMICS_INTD      9
   #define SMICS_INTT      10
   #define SMICS_INTR      11
   #define SMICS_PVMODE    12
   #define SMICS_SETERR    13
   #define SMICS_PXLDAT    14
   #define SMICS_EDREQ     15
   #define SMICS_AFERR     25
   #define SMICS_TXW       26
   #define SMICS_RXR       27
   #define SMICS_TXD       28
   #define SMICS_RXD       29
   #define SMICS_TXE       30
   #define SMICS_RXF       31

   // SMI address and direct address register bits.
   #define SMIA_DEVICE     8
   #define SMIDA_DEVICE    8

   // SMI DSR* and DSW* common fields
   #define SMIDS_STROBE    0
   #define SMIDS_DREQ      7
   #define SMIDS_PACE      8
   #define SMIDS_PACEALL   15
   #define SMIDS_HOLD      16
   #define SMIDS_SETUP     24
   #define SMIDS_WIDTH     30
   // SMI DSR* register specific
   #define SMIDS_FSETUP    22
   #define SMIDS_MODE68    23
   // SMI DSW* register specific
   #define SMIDS_SWAP      22
   #define SMIDS_FORMAT    23

   // SMI direct control/status register bits.
   #define SMIDCS_ENABLE   0
   #define SMIDCS_START    1
   #define SMIDCS_DONE     2
   #define SMIDCS_WRITE    3

   // SMI dma control threshold register bits.
   #define SMIDC_REQW      0
   #define SMIDC_REQR      6
   #define SMIDC_PANICW    12
   #define SMIDC_PANICR    18
   #define SMIDC_DMAP      24
   #define SMIDC_DMAEN     28

   // SMI FIFO debug
   #define SMIFD_FCNT      0
   #define SMIFD_FLVL      8

   /*---------------------------------------------------------------------------*/
   /* AC'97/I2S Controller */
   #define ACISCS              HW_REGISTER_RW( ACIS_BASE_ADDRESS + 0x00 )
   #define ACISFIFO            HW_REGISTER_RW( ACIS_BASE_ADDRESS + 0x04 )
   #define ACISCA              HW_REGISTER_RW( ACIS_BASE_ADDRESS + 0x08 )
   #define ACISCD              HW_REGISTER_RW( ACIS_BASE_ADDRESS + 0x0C )
   #define ACISMODE            HW_REGISTER_RW( ACIS_BASE_ADDRESS + 0x10 )
   #define ACISASR             HW_REGISTER_RW( ACIS_BASE_ADDRESS + 0x14 )

   #define ACPLAYRATE          0x2c
   #define ACRECORDRATE        0x32

   /*---------------------------------------------------------------------------*/
   /* SLIMbus interface */
   #define SLIM_NUM_DCC        10 /* there are 10 data channel controllers */
   #define SLIM_DCC_BASE(n)    (SLIM_BASE + 0x200 + (n) * 0x20)
   #define SLIM_DCC_PA0(n)     HW_REGISTER_RW(SLIM_DCC_BASE(n) + 0x00)
   #define SLIM_DCC_PA1(n)     HW_REGISTER_RW(SLIM_DCC_BASE(n) + 0x04)
   #define SLIM_DCC_CON(n)     HW_REGISTER_RW(SLIM_DCC_BASE(n) + 0x08)
   #define SLIM_DCC_STAT(n)    HW_REGISTER_RW(SLIM_DCC_BASE(n) + 0x0c)

#if defined(__BCM2708A0__)
   // backward compatibility for drivers that use B0 register definitions
   #define SLIM_MC_IN_CON      SLIM_SMC_IN_CON
   #define SLIM_MC_OUT_CON     SLIM_SMC_OUT_CON
   #define SLIM_MC_IN_STAT     SLIM_SMC_IN_STAT
   #define SLIM_MC_OUT_STAT    SLIM_SMC_OUT_STAT
   #define SLIM_DMA_MC_TX      SLIM_DMA_SMC_TX
   #define SLIM_DMA_MC_RX      SLIM_DMA_SMC_RX
#endif

   /*---------------------------------------------------------------------------*/
   /* USB Peripheral */
   #define USB_DIEPINT_off(n)  HW_REGISTER_RW(&USB_DIEPINT+(n*0x20))
   #define USB_HCINT_off(n)    HW_REGISTER_RW(&USB_HCINT+(n*0x20))

   #define USB_DFIFOn(n)       HW_REGISTER_RW(USB_BASE+0x1000+(n*0x1000))
   #define USB_DIEPCTLn(n)     HW_REGISTER_RW(USB_BASE+0x0900+(n*0x20))
   #define USB_DIEPTSIZn(n)    HW_REGISTER_RW(USB_BASE+0x0910+(n*0x20))
   #define USB_DIEPDMAn(n)     HW_REGISTER_RW(USB_BASE+0x0914+(n*0x20))
   #define USB_DTXFSTSn(n)     HW_REGISTER_RW(USB_BASE+0x0918+(n*0x20))

   #define USB_MDIO_CSR        HW_REGISTER_RW( USB_BASE + 0x80 )
   #define USB_MDIO_GEN        HW_REGISTER_RW( USB_BASE + 0x84 )
   #define USB_VBUS_DRV        HW_REGISTER_RW( USB_BASE + 0x88 )
   #define    USB_VBUS_DRV_SESSEND     (1<<0)
   #define    USB_VBUS_DRV_VBUSVALID   (1<<1)
   #define    USB_VBUS_DRV_BVALID      (1<<2)
   #define    USB_VBUS_DRV_AVALID      (1<<3)
   #define    USB_VBUS_DRV_DRVVBUS     (1<<4)
   #define    USB_VBUS_DRV_CHRGVBUS    (1<<5)
   #define    USB_VBUS_DRV_DISCHRGVBUS (1<<6)

   /*---------------------------------------------------------------------------*/
   /* PCM Controller */
   #define PCMCS               PCM_CS_A
   //#define PCMFIFO             PCM_FIFO_A erroneously defined as _RO in bcm2708_chip/pcm.h
   #define PCMFIFO             HW_REGISTER_RW( PCM_BASE + 0x04 )
   #define PCMMODE             PCM_MODE_A
   #define PCMRXC              PCM_RXC_A
   #define PCMTXC              PCM_TXC_A
   #define PCMDREQ             HW_REGISTER_RW( PCM_BASE + 0x14 )
   #define PCMINTEN            HW_REGISTER_RW( PCM_BASE + 0x18 )
   #define PCMINTSTC           HW_REGISTER_RW( PCM_BASE + 0x1c )


   // bit fields for PCMCS
   #define PCMCS_EN              (1 << 0)
   #define PCMCS_RXON            (1 << 1)
   #define PCMCS_TXON            (1 << 2)
   #define PCMCS_TXCLR           (1 << 3)
   #define PCMCS_RXCLR           (1 << 4)
   #define PCMCS_TXTHR_LSB       5
   #define PCMCS_TXTHR_EMPTY     (0 << PCMCS_TXTHR_LSB)
   #define PCMCS_TXTHR_1_QUARTER (1 << PCMCS_TXTHR_LSB)
   #define PCMCS_TXTHR_3_QUARTER (2 << PCMCS_TXTHR_LSB)
   #define PCMCS_TXTHR_FULL      (3 << PCMCS_TXTHR_LSB)
   #define PCMCS_RXTHR_LSB       7
   #define PCMCS_RXTHR_EMPTY     (0 << PCMCS_RXTHR_LSB)
   #define PCMCS_RXTHR_1_QUARTER (1 << PCMCS_RXTHR_LSB)
   #define PCMCS_RXTHR_3_QUARTER (2 << PCMCS_RXTHR_LSB)
   #define PCMCS_RXTHR_FULL      (3 << PCMCS_RXTHR_LSB)

   #define PCMCS_DMAEN           (1 << 9)
   #define PCMCS_INTT            (1 << 10)
   #define PCMCS_INTR            (1 << 11)
   #define PCMCS_INTE            (1 << 12)
   #define PCMCS_TXSYNC          (1 << 13)
   #define PCMCS_RXSYNC          (1 << 14)
   #define PCMCS_TXERR           (1 << 15)
   #define PCMCS_RXERR           (1 << 16)
   #define PCMCS_TXW             (1 << 17)
   #define PCMCS_RXR             (1 << 18)
   #define PCMCS_TXD             (1 << 19)
   #define PCMCS_RXD             (1 << 20)
   #define PCMCS_TXE             (1 << 21)
   #define PCMCS_RXF             (1 << 22)
   #define PCMCS_RXSEX           (1 << 23)
   #define PCMCS_SYNC            (1 << 24)

   // bit fields for PCMMODE
   #define PCMMODE_FSI           (1 << 20)
   #define PCMMODE_FSM           (1 << 21)
   #define PCMMODE_CLKI          (1 << 22)
   #define PCMMODE_CLKM          (1 << 23)
   #define PCMMODE_FLEN          10
   #define PCMMODE_FSLEN         0
   #define PCMMODE_FTXP          (1 << 24)
   #define PCMMODE_FRXP          (1 << 25)
   #define PCMMODE_PDMRX         (1 << 26)
   #define PCMMODE_PDMRXN        (1 << 27)

   // macros for PCMTXC and PCMRXC
   #define PCM_CH1POS_LSB        20
   #define PCM_CH1WID_LSB        16
   #define PCM_CH2POS_LSB        4
   #define PCM_CH2WID_LSB        0
   // parameters --> bitmasks
   #define PCM_CH1WEX            (1 << 31)
   #define PCM_CH2WEX            (1 << 15)
   #define PCM_WIDTH2(x)         ( (((x - 8) & 0x0f) << PCM_CH2WID_LSB) + PCM_CH2WEX * (((x-8)&0x10)>>4) )
   #define PCM_POS2(x)           (((x) & 0x3ff) << PCM_CH2POS_LSB)
   #define PCM_WIDTH1(x)         ( (((x - 8) & 0x0f) << PCM_CH1WID_LSB) + PCM_CH1WEX * (((x-8)&0x10)>>4) )
   #define PCM_POS1(x)           (((x) & 0x3ff) << PCM_CH1POS_LSB)
   #define PCM_CH2EN             (1 << 14)
   #define PCM_CH1EN             (1 << 30)
   // bitmasks --> parameters
   #define PCM_WID1(x)           ( (((x >> PCM_CH1WID_LSB) & 0x0f) + 8) + ((x & PCM_CH1WEX) ? 16 : 0) )
   #define PCM_WID2(x)           ( (((x >> PCM_CH2WID_LSB) & 0x0f) + 8) + ((x & PCM_CH2WEX) ? 16 : 0) )

   // bit fields in the PCMDREQ register
   #define PCMDREQ_TXPANICTHR_LSB 24
   #define PCMDREQ_RXPANICTHR_LSB 16
   #define PCMDREQ_TXDREQTHR_LSB  8
   #define PCMDREQ_RXDREQTHR_LSB  0

   #define PCM_FIFO_DEPTH        64    // words


   /*---------------------------------------------------------------------------*/
   /* I2C Master */
   #define I2CC_0              HW_REGISTER_RW(I2C_BASE_0 + 0x00)
   #define I2CS_0              HW_REGISTER_RW(I2C_BASE_0 + 0x04)
   #define I2CDLEN_0           HW_REGISTER_RW(I2C_BASE_0 + 0x08)
   #define I2CA_0              HW_REGISTER_RW(I2C_BASE_0 + 0x0C)
   #define I2CFIFO_0           HW_REGISTER_RW(I2C_BASE_0 + 0x10)
   #define I2CDIV_0            HW_REGISTER_RW(I2C_BASE_0 + 0x14)
   #define I2CDEL_0            HW_REGISTER_RW(I2C_BASE_0 + 0x18)
   #define I2CCLKT_0           HW_REGISTER_RW(I2C_BASE_0 + 0x1C)

   #define I2CC_1              HW_REGISTER_RW(I2C_BASE_1 + 0x00)
   #define I2CS_1              HW_REGISTER_RW(I2C_BASE_1 + 0x04)
   #define I2CDLEN_1           HW_REGISTER_RW(I2C_BASE_1 + 0x08)
   #define I2CA_1              HW_REGISTER_RW(I2C_BASE_1 + 0x0C)
   #define I2CFIFO_1           HW_REGISTER_RW(I2C_BASE_1 + 0x10)
   #define I2CDIV_1            HW_REGISTER_RW(I2C_BASE_1 + 0x14)
   #define I2CDEL_1            HW_REGISTER_RW(I2C_BASE_1 + 0x18)
   #define I2CCLKT_1           HW_REGISTER_RW(I2C_BASE_1 + 0x1C)

   #define I2CC_2              HW_REGISTER_RW(I2C_BASE_2 + 0x00)
   #define I2CS_2              HW_REGISTER_RW(I2C_BASE_2 + 0x04)
   #define I2CDLEN_2           HW_REGISTER_RW(I2C_BASE_2 + 0x08)
   #define I2CA_2              HW_REGISTER_RW(I2C_BASE_2 + 0x0C)
   #define I2CFIFO_2           HW_REGISTER_RW(I2C_BASE_2 + 0x10)
   #define I2CDIV_2            HW_REGISTER_RW(I2C_BASE_2 + 0x14)
   #define I2CDEL_2            HW_REGISTER_RW(I2C_BASE_2 + 0x18)
   #define I2CCLKT_2           HW_REGISTER_RW(I2C_BASE_2 + 0x1C)

   #define I2CC_x( x )         HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x00 )
   #define I2CS_x( x )         HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x04 )
   #define I2CDLEN_x( x )      HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x08 )
   #define I2CA_x( x )         HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x0C )
   #define I2CFIFO_x( x )      HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x10 )
   #define I2CDIV_x( x )       HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x14 )
   #define I2CDEL_x( x )       HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x18 )
   #define I2CCLKT_x( x )      HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x1C )

   // define some bitfields within these I2C registers...
   #define I2CC_EN            (1 << 15)
   #define I2CC_INTR          (1 << 10)
   #define I2CC_INTT          (1 << 9)
   #define I2CC_INTD          (1 << 8)
   #define I2CC_START         (1 << 7)
   #define I2CC_CLEAR         (3 << 4)
   #define I2CC_READ          (1 << 0)

   #define I2CS_CLKT          (1 << 9)
   #define I2CS_ERR           (1 << 8)
   #define I2CS_RXF           (1 << 7)
   #define I2CS_TXE           (1 << 6)
   #define I2CS_RXD           (1 << 5)
   #define I2CS_TXD           (1 << 4)
   #define I2CS_RXR           (1 << 3)
   #define I2CS_TXW           (1 << 2)
   #define I2CS_DONE          (1 << 1)
   #define I2CS_TA            (1 << 0)

   #define I2CDEL_FEDL        (16)
   #define I2CDEL_REDL        (0)

   //Note! Remove this eventually.
   //Just define the old VCII I2C peripheral
   #define I2CC               I2CC_0
   #define I2CS               I2CS_0
   #define I2CDLEN            I2CDLEN_0
   #define I2CA               I2CA_0
   #define I2CFIFO            I2CFIFO_0
   #define I2CDIV             I2CDIV_0
   #define I2CDEL             I2CDEL_0
   #define I2CCLKT            I2CCLKT_0

   /*---------------------------------------------------------------------------*/
   /* Performance Monitor */
   #define PRMCS               HW_REGISTER_RW(PERFMON_BASE_ADDRESS + 0x00)
   #define PRMCV               HW_REGISTER_RW(PERFMON_BASE_ADDRESS + 0x04)
   #define PRMSCC              HW_REGISTER_RW(PERFMON_BASE_ADDRESS + 0x08)

   /*---------------------------------------------------------------------------*/
   /* OTP */

   //#define OTP_CONFIG_REG                       OTP_WRAP_CONFIG_REG
   //OTP bit definitions
   // NB the addressing has changed between VCIII and VCIV - we now address by row (each row contains 32 bits)
   #define OTP_JTAG_DEBUG_KEY_ROW                8
   #define OTP_JTAG_DEBUG_KEY_SIZE_IN_ROWS       4
   #define OTP_VPU_CACHE_KEY_ROW                 (OTP_JTAG_DEBUG_KEY_ROW+OTP_JTAG_DEBUG_KEY_SIZE_IN_ROWS)
   #define OTP_VPU_CACHE_KEY_SIZE_IN_ROWS        4

   #define OTP_CONTROL_ROW                       (OTP_VPU_CACHE_KEY_ROW+OTP_VPU_CACHE_KEY_SIZE_IN_ROWS)
   #define OTP_CONTROL_SIZE_IN_ROWS              1
  
   #define OTP_BOOT_ROM_ROW                      (OTP_CONTROL_ROW+OTP_CONTROL_SIZE_IN_ROWS)
   #define OTP_BOOT_ROM_SIZE_IN_ROWS             1

   #define OTP_BOOT_ROM_ROW_REDUNDANT            (OTP_BOOT_ROM_ROW+OTP_BOOT_ROM_SIZE_IN_ROWS)

#ifdef __BCM2708A0__
   #define OTP_BOOT_SIGNING_KEY_ROW              (OTP_BOOT_ROM_ROW_REDUNDANT+OTP_BOOT_ROM_SIZE_IN_ROWS)
   #define OTP_BOOT_SIGNING_KEY_SIZE_IN_ROWS     4

   #define OTP_BOOT_SIGNING_PARITY_ROW           (OTP_BOOT_SIGNING_KEY_ROW+OTP_BOOT_SIGNING_KEY_SIZE_IN_ROWS)
   #define OTP_BOOT_SIGNING_PARITY_SIZE_IN_ROWS  1

   #define OTP_CODE_SIGNING_KEY_ROW              (OTP_BOOT_SIGNING_PARITY_ROW+OTP_BOOT_SIGNING_PARITY_SIZE_IN_ROWS)
   #define OTP_CODE_SIGNING_KEY_SIZE_IN_ROWS     4

   #define OTP_CODE_SIGNING_PARITY_ROW           (OTP_CODE_SIGNING_KEY_ROW+OTP_CODE_SIGNING_KEY_SIZE_IN_ROWS)
   #define OTP_CODE_SIGNING_PARITY_SIZE_IN_ROWS  1

   #define OTP_HDCP_AES_KEY_ROW                  (OTP_CODE_SIGNING_PARITY_ROW+OTP_CODE_SIGNING_PARITY_SIZE_IN_ROWS)
   #define OTP_HDCP_AES_KEY_SIZE_IN_ROWS         4

   #define OTP_HDCP_AES_PARITY_ROW               (OTP_HDCP_AES_KEY_ROW+OTP_HDCP_AES_KEY_SIZE_IN_ROWS)
   #define OTP_HDCP_AES_PARITY_SIZE_IN_ROWS      1

   #define OTP_PUBLIC_KEY_ROW                    (OTP_HDCP_AES_PARITY_ROW+OTP_HDCP_AES_PARITY_SIZE_IN_ROWS)
   #define OTP_PUBLIC_KEY_SIZE_IN_ROWS           4

   #define OTP_PUBLIC_PARITY_ROW                 (OTP_PUBLIC_KEY_ROW+OTP_PUBLIC_KEY_SIZE_IN_ROWS)
   #define OTP_PUBLIC_PARITY_SIZE_IN_ROWS        1

   #define OTP_PRIVATE_KEY_ROW                   (OTP_PUBLIC_PARITY_ROW+OTP_PUBLIC_PARITY_SIZE_IN_ROWS)
   #define OTP_PRIVATE_KEY_SIZE_IN_ROWS          4

   #define OTP_PRIVATE_PARITY_ROW                (OTP_PRIVATE_KEY_ROW+OTP_PRIVATE_KEY_SIZE_IN_ROWS)
   #define OTP_PRIVATE_PARITY_SIZE_IN_ROWS       1

   #define OTP_CODE_SIGNING_FLAG_ROW             (OTP_PRIVATE_PARITY_ROW+OTP_PRIVATE_PARITY_SIZE_IN_ROWS)
   #define OTP_CODE_SIGNING_FLAG_SIZE_IN_ROWS    1

   // Suspend/resume secure RAM key: ensure that these values match the ones
   // used in vcsuspend_asm_vc4.s
   #define OTP_SUSPEND_SECURE_RAM_KEY             (OTP_CODE_SIGNING_FLAG_ROW+OTP_CODE_SIGNING_FLAG_SIZE_IN_ROWS)               // 64
   #define OTP_SUSPEND_SECURE_RAM_KEY_SIZE_IN_ROWS    2

   // to allow A0 to continue to build without too many #ifdefs define the redundant rows to be the same as the original
   #define OTP_BOOT_SIGNING_KEY_ROW_REDUNDANT     OTP_BOOT_SIGNING_KEY_ROW
   #define OTP_CODE_SIGNING_KEY_ROW_REDUNDANT     OTP_CODE_SIGNING_KEY_ROW
   #define OTP_HDCP_AES_KEY_ROW_REDUNDANT         OTP_HDCP_AES_KEY_ROW
   #define OTP_PUBLIC_KEY_ROW_REDUNDANT           OTP_PUBLIC_KEY_ROW
   #define OTP_PRIVATE_KEY_ROW_REDUNDANT          OTP_PRIVATE_KEY_ROW
#else
   // BCM2708B0 has less reliable OTP so we need a redundant row for the boot signing key (and the corresponding parity)
   #define OTP_BOOT_SIGNING_KEY_ROW                   (OTP_BOOT_ROM_ROW_REDUNDANT+OTP_BOOT_ROM_SIZE_IN_ROWS)                 // 19
   #define OTP_BOOT_SIGNING_KEY_SIZE_IN_ROWS          4

   #define OTP_BOOT_SIGNING_KEY_ROW_REDUNDANT         (OTP_BOOT_SIGNING_KEY_ROW+OTP_BOOT_SIGNING_KEY_SIZE_IN_ROWS)           // 23

   #define OTP_BOOT_SIGNING_PARITY_ROW                (OTP_BOOT_SIGNING_KEY_ROW_REDUNDANT+OTP_BOOT_SIGNING_KEY_SIZE_IN_ROWS) // 27
   #define OTP_BOOT_SIGNING_PARITY_SIZE_IN_ROWS       1

   #define OTP_CODE_SIGNING_KEY_ROW                   (OTP_BOOT_SIGNING_PARITY_ROW+OTP_BOOT_SIGNING_PARITY_SIZE_IN_ROWS)     // 28
   #define OTP_CODE_SIGNING_KEY_SIZE_IN_ROWS          4

   #define OTP_CODE_SIGNING_KEY_ROW_REDUNDANT         (OTP_CODE_SIGNING_KEY_ROW+OTP_CODE_SIGNING_KEY_SIZE_IN_ROWS)           // 32

   #define OTP_CODE_SIGNING_PARITY_ROW                (OTP_CODE_SIGNING_KEY_ROW_REDUNDANT+OTP_CODE_SIGNING_KEY_SIZE_IN_ROWS) // 36
   #define OTP_CODE_SIGNING_PARITY_SIZE_IN_ROWS       1

   #define OTP_HDCP_AES_KEY_ROW                       (OTP_CODE_SIGNING_PARITY_ROW+OTP_CODE_SIGNING_PARITY_SIZE_IN_ROWS)     // 37
   #define OTP_HDCP_AES_KEY_SIZE_IN_ROWS              4

   #define OTP_HDCP_AES_KEY_ROW_REDUNDANT             (OTP_HDCP_AES_KEY_ROW+OTP_HDCP_AES_KEY_SIZE_IN_ROWS)                   // 41

   #define OTP_HDCP_AES_PARITY_ROW                    (OTP_HDCP_AES_KEY_ROW_REDUNDANT+OTP_HDCP_AES_KEY_SIZE_IN_ROWS)         // 45
   #define OTP_HDCP_AES_PARITY_SIZE_IN_ROWS           1

   #define OTP_PUBLIC_KEY_ROW                         (OTP_HDCP_AES_PARITY_ROW+OTP_HDCP_AES_PARITY_SIZE_IN_ROWS)             // 46
   #define OTP_PUBLIC_KEY_SIZE_IN_ROWS                4

   #define OTP_PUBLIC_KEY_ROW_REDUNDANT               (OTP_PUBLIC_KEY_ROW+OTP_PUBLIC_KEY_SIZE_IN_ROWS)                       // 50

   #define OTP_PUBLIC_PARITY_ROW                      (OTP_PUBLIC_KEY_ROW_REDUNDANT+OTP_PUBLIC_KEY_SIZE_IN_ROWS)             // 54
   #define OTP_PUBLIC_PARITY_SIZE_IN_ROWS             1

   #define OTP_PRIVATE_KEY_ROW                        (OTP_PUBLIC_PARITY_ROW+OTP_PUBLIC_PARITY_SIZE_IN_ROWS)                 // 55
   #define OTP_PRIVATE_KEY_SIZE_IN_ROWS               4

   #define OTP_PRIVATE_KEY_ROW_REDUNDANT              (OTP_PRIVATE_KEY_ROW+OTP_PRIVATE_KEY_SIZE_IN_ROWS)                     // 59

   #define OTP_PRIVATE_PARITY_ROW                     (OTP_PRIVATE_KEY_ROW_REDUNDANT+OTP_PRIVATE_KEY_SIZE_IN_ROWS)           // 63
   #define OTP_PRIVATE_PARITY_SIZE_IN_ROWS            1

   #define OTP_CODE_SIGNING_FLAG_ROW                  (OTP_PRIVATE_PARITY_ROW+OTP_PRIVATE_PARITY_SIZE_IN_ROWS)               // 64
   #define OTP_CODE_SIGNING_FLAG_SIZE_IN_ROWS         1

   // Suspend/resume secure RAM key: ensure that these values match the ones
   // used in vcsuspend_asm_vc4.s
   #define OTP_SUSPEND_SECURE_RAM_KEY                 (OTP_CODE_SIGNING_FLAG_ROW+OTP_CODE_SIGNING_FLAG_SIZE_IN_ROWS)         // 65
   #define OTP_SUSPEND_SECURE_RAM_KEY_SIZE_IN_ROWS    2

   // strictly this is C0 only
   #define OTP_BOOT_EXTRAS_ROW                        (OTP_SUSPEND_SECURE_RAM_KEY+OTP_SUSPEND_SECURE_RAM_KEY_SIZE_IN_ROWS)   // 67
   #define OTP_BOOT_EXTRAS_ROW_SIZE_IN_ROWS           1
// locations fixed by hardware
   #define OTP_JTAG_DEBUG_KEY_ROW_REDUNDANT           68
   #define OTP_VPU_CACHE_KEY_ROW_REDUNDANT            72
   #define OTP_JTAG_VPU_PARITY_REDUNDANT              76

   #if( OTP_BOOT_EXTRAS_ROW+OTP_BOOT_EXTRAS_ROW_SIZE_IN_ROWS > OTP_JTAG_DEBUG_KEY_ROW_REDUNDANT )
      #error "User OTP space has overwritten CPU bits" OTP_SUSPEND_SECURE_RAM_KEY OTP_SUSPEND_SECURE_RAM_KEY_SIZE_IN_ROWS OTP_JTAG_DEBUG_KEY_ROW_REDUNDANT
   #endif

#endif


   #define OTP_MIN_ROW                           OTP_JTAG_DEBUG_KEY_ROW
   #define OTP_MAX_ROW                           OTP_SUSPEND_SECURE_RAM_KEY

   // bit locations in the CONTROL OTP register
   #define OTP_JTAG_DEBUG_KEY_PARITY_START_BIT   0
   #define OTP_VPU_CACHE_KEY_PARITY_START_BIT    8
   #define OTP_JTAG_DISABLE_BIT                  16
   #define OTP_JTAG_DISABLE_REDUNDANT_BIT        17
   #define OTP_MACROVISION_START_BIT             18
   #define OTP_MACROVISION_REDUNDANT_START_BIT   20
   #define OTP_DECRYPTION_ENABLE_FOR_DEBUG       22
   #define OTP_ARM_DISABLE_BIT                   24
   #define OTP_ARM_DISABLE_REDUNDANT_BIT         25

   #define OTP_JTAG_PARITY_MASK                  0xFF
   #define OTP_VPU_CACHE_PARITY_MASK             0xFF

   #define OTP_BYTES_PER_ROW                     4

// make sure locations used by the hardware are correct
#if( OTP_CONTROL_ROW != 16)
   #error "The OTP control row has moved - it must be 16!"
#endif
#if( OTP_BOOT_ROM_ROW != 17 )
   #error "The OTP bootrom row has moved - it must be 17!"
#endif
#if( OTP_BOOT_ROM_ROW_REDUNDANT != 18 )
   #error "The OTP bootrom copy row has moved - it must be 18!"
#endif

   /*---------------------------------------------------------------------------*/
   /* Threading unit */

   /* Registers for the threading unit */
   #define TH0_BASE                  0x18011000
   #define TH0_ADDR_MASK             0x0000003F
   #define TH1_BASE                  0x1A008000
   #define TH1_ADDR_MASK             0x0000003F

   #define TH0CS                     HW_REGISTER_RW(TH0_BASE + 0x00)
   #define TH0CFG                    HW_REGISTER_RW(TH0_BASE + 0x04)
   #define TH0STPC                   HW_REGISTER_RW(TH0_BASE + 0x08)
   #define TH0ITPC                   HW_REGISTER_RW(TH0_BASE + 0x0C)
   #define TH0T0PC                   HW_REGISTER_RW(TH0_BASE + 0x10)
   #define TH0T0UD                   HW_REGISTER_RW(TH0_BASE + 0x14)
   #define TH0T1PC                   HW_REGISTER_RW(TH0_BASE + 0x18)
   #define TH0T1UD                   HW_REGISTER_RW(TH0_BASE + 0x1C)
   #define TH0T2PC                   HW_REGISTER_RW(TH0_BASE + 0x20)
   #define TH0T2UD                   HW_REGISTER_RW(TH0_BASE + 0x24)
   #define TH0T3PC                   HW_REGISTER_RW(TH0_BASE + 0x28)
   #define TH0T3UD                   HW_REGISTER_RW(TH0_BASE + 0x2C)

   #define TH1CS                     HW_REGISTER_RW(TH1_BASE + 0x00)
   #define TH1CFG                    HW_REGISTER_RW(TH1_BASE + 0x04)
   #define TH1STPC                   HW_REGISTER_RW(TH1_BASE + 0x08)
   #define TH1ITPC                   HW_REGISTER_RW(TH1_BASE + 0x0C)
   #define TH1T0PC                   HW_REGISTER_RW(TH1_BASE + 0x10)
   #define TH1T0UD                   HW_REGISTER_RW(TH1_BASE + 0x14)
   #define TH1T1PC                   HW_REGISTER_RW(TH1_BASE + 0x18)
   #define TH1T1UD                   HW_REGISTER_RW(TH1_BASE + 0x1C)
   #define TH1T2PC                   HW_REGISTER_RW(TH1_BASE + 0x20)
   #define TH1T2UD                   HW_REGISTER_RW(TH1_BASE + 0x24)
   #define TH1T3PC                   HW_REGISTER_RW(TH1_BASE + 0x28)
   #define TH1T3UD                   HW_REGISTER_RW(TH1_BASE + 0x2C)

   /* Windows Open GL plugin - emulator only */
   #define WOGLPTR            HW_REGISTER_RW(0x1C00FFFC)

   /* Hardware 3D unit */

   #define GR_VCACHE_BASE            0x1a00a000
   #define GR_VCACHE_ADDR_MASK       0x00001fff
   #define GR_VCACHE_SIZE            0x00002000        // in bytes

   #define GR_UNIFORM_BASE           0x1a00c000
   #define GR_UNIFORM_ADDR_MASK      0x00000fff
   #define GR_UNIFORM_SIZE           0x00001000        // in bytes

   /* Registers for Vertex Cache Manager */
   #define GR_VCM_BASE               0x1A005C00
   #define GR_VCM_ADDR_MASK          0x0000003f
   #define GR_VCM_CI_BASE            0x1A005C80
   #define GR_VCM_CI_ADDR_MASK       0x0000007f

   #define GRMCS                     HW_REGISTER_RW(GR_VCM_BASE + 0x00)
   #define GRMCFG                    HW_REGISTER_RW(GR_VCM_BASE + 0x04)
   #define GRMSVI                    HW_REGISTER_RW(GR_VCM_BASE + 0x08)
   #define GRMSADR                   HW_REGISTER_RW(GR_VCM_BASE + 0x0C)
   #define GRMSCT                    HW_REGISTER_RW(GR_VCM_BASE + 0x10)
   #define GRMOADR                   HW_REGISTER_RW(GR_VCM_BASE + 0x14)
   #define GRMOCT                    HW_REGISTER_RW(GR_VCM_BASE + 0x18)
   #define GRMMCT                    HW_REGISTER_RW(GR_VCM_BASE + 0x1C)
   #define GRMSSI0                   HW_REGISTER_RW(GR_VCM_BASE + 0x20)
   #define GRMSSI1                   HW_REGISTER_RW(GR_VCM_BASE + 0x24)
   #define GRMCCT                    HW_REGISTER_RW(GR_VCM_BASE + 0x28)

   #define GRMCIL0                   HW_REGISTER_RW(GR_VCM_CI_BASE + 0x00)
   #define GRMCIL1                   HW_REGISTER_RW(GR_VCM_CI_BASE + 0x20)
   #define GRMCIH0                   HW_REGISTER_RW(GR_VCM_CI_BASE + 0x40)
   #define GRMCIH1                   HW_REGISTER_RW(GR_VCM_CI_BASE + 0x60)

   #define GR_VCD_BASE               0x1A005A00
   #define GR_VCD_ADDR_MASK          0x0000007f

   #define GRDCS                     HW_REGISTER_RW(GR_VCD_BASE + 0x00)
   #define GRDCFG                    HW_REGISTER_RW(GR_VCD_BASE + 0x04)
   #define GRDACFG0                  HW_REGISTER_RW(GR_VCD_BASE + 0x20)
   #define GRDACFG1                  HW_REGISTER_RW(GR_VCD_BASE + 0x24)
   #define GRDACFG2                  HW_REGISTER_RW(GR_VCD_BASE + 0x28)
   #define GRDACFG3                  HW_REGISTER_RW(GR_VCD_BASE + 0x2C)
   #define GRDACFG4                  HW_REGISTER_RW(GR_VCD_BASE + 0x30)
   #define GRDACFG5                  HW_REGISTER_RW(GR_VCD_BASE + 0x34)
   #define GRDACFG6                  HW_REGISTER_RW(GR_VCD_BASE + 0x38)
   #define GRDACFG7                  HW_REGISTER_RW(GR_VCD_BASE + 0x3C)
   #define GRDAADR0                  HW_REGISTER_RW(GR_VCD_BASE + 0x40)
   #define GRDAADR1                  HW_REGISTER_RW(GR_VCD_BASE + 0x44)
   #define GRDAADR2                  HW_REGISTER_RW(GR_VCD_BASE + 0x48)
   #define GRDAADR3                  HW_REGISTER_RW(GR_VCD_BASE + 0x4C)
   #define GRDAADR4                  HW_REGISTER_RW(GR_VCD_BASE + 0x50)
   #define GRDAADR5                  HW_REGISTER_RW(GR_VCD_BASE + 0x54)
   #define GRDAADR6                  HW_REGISTER_RW(GR_VCD_BASE + 0x58)
   #define GRDAADR7                  HW_REGISTER_RW(GR_VCD_BASE + 0x5C)

   /* Registers for Primitive Setup Engine */
   #define GR_PSE_BASE              0x1A005800
   #define GR_PSE_ADDR_MASK         0x0000007f

   #define GRSCS                    HW_REGISTER_RW(GR_PSE_BASE + 0x00)
   #define GRSCFG                   HW_REGISTER_RW(GR_PSE_BASE + 0x04)
   #define GRSVADR                  HW_REGISTER_RW(GR_PSE_BASE + 0x08)
   #define GRSVFMT                  HW_REGISTER_RW(GR_PSE_BASE + 0x0C)
   #define GRSSP                    HW_REGISTER_RW(GR_PSE_BASE + 0x10)
   #define GRSPADR                  HW_REGISTER_RW(GR_PSE_BASE + 0x14)
   #define GRSPCT                   HW_REGISTER_RW(GR_PSE_BASE + 0x18)
   #define GRSAADR                  HW_REGISTER_RW(GR_PSE_BASE + 0x1C)
   #define GRSACT                   HW_REGISTER_RW(GR_PSE_BASE + 0x20)
   #define GRSDOF                   HW_REGISTER_RW(GR_PSE_BASE + 0x24)
   #define GRSDOU                   HW_REGISTER_RW(GR_PSE_BASE + 0x28)
   #define GRSDMIN                  HW_REGISTER_RW(GR_PSE_BASE + 0x2C)
   #define GRSDMAX                  HW_REGISTER_RW(GR_PSE_BASE + 0x30)
   #define GRSPSZ                   HW_REGISTER_RW(GR_PSE_BASE + 0x34)
   #define GRSLW                    HW_REGISTER_RW(GR_PSE_BASE + 0x38)
   #define GRSFSF                   HW_REGISTER_RW(GR_PSE_BASE + 0x3C)
   #define GRSDZS                   HW_REGISTER_RW(GR_PSE_BASE + 0x40)
   #define GRSHPX                   HW_REGISTER_RW(GR_PSE_BASE + 0x44)

   /* Debug Registers for Primitive Setup Engine */
   #define GR_PSE_DEBUG_BASE        0x1A005900
   #define GR_PSE_DEBUG_ADDR_MASK   0x00000003

   #define GRS_DBGE                 HW_REGISTER_RW(GR_PSE_DEBUG_BASE + 0x00)

   /* Registers for Pixel Pipeline */
   #define GR_PPL_BASE              0x1A005600
   #define GR_PPL_ADDR_MASK         0x0000007F

   #define GRPCS                    HW_REGISTER_RW(GR_PPL_BASE + 0x00)
   #define GRPCFG                   HW_REGISTER_RW(GR_PPL_BASE + 0x04)
   #define GRPCLXY                  HW_REGISTER_RW(GR_PPL_BASE + 0x08)
   #define GRPCLSZ                  HW_REGISTER_RW(GR_PPL_BASE + 0x0C)
   #define GRPVORG                  HW_REGISTER_RW(GR_PPL_BASE + 0x10)
   // gap of 8 bytes
   // gap of 16 bytes
   // gap of 16 bytes
   #define GRPZBCG                  HW_REGISTER_RW(GR_PPL_BASE + 0x40)
   #define GRPSFCG                  HW_REGISTER_RW(GR_PPL_BASE + 0x44)
   #define GRPSBCG                  HW_REGISTER_RW(GR_PPL_BASE + 0x48)
   #define GRPSCC                   HW_REGISTER_RW(GR_PPL_BASE + 0x4C)
   #define GRPBCFG                  HW_REGISTER_RW(GR_PPL_BASE + 0x50)
   #define GRPBCC                   HW_REGISTER_RW(GR_PPL_BASE + 0x54)
   #define GRPCDSM                  HW_REGISTER_RW(GR_PPL_BASE + 0x58) // xxx dc4
   #define GRPCZSM                  HW_REGISTER_RW(GR_PPL_BASE + 0x58)

   #define GRPCBS                   HW_REGISTER_RW(GR_PPL_BASE + 0x5C)
   #define GRPABS                   HW_REGISTER_RW(GR_PPL_BASE + 0x60)
   #define GRPFCOL                  HW_REGISTER_RW(GR_PPL_BASE + 0x64)

   /* Debug Registers for Pixel Pipeline */
   #define GR_PPL_DEBUG_BASE        0x1A005740
   #define GR_PPL_DEBUG_ADDR_MASK   0x0000001F

   #define GRP_FDBGO                HW_REGISTER_RW(GR_PPL_DEBUG_BASE + 0x00)
   #define GRP_FDBGB                HW_REGISTER_RW(GR_PPL_DEBUG_BASE + 0x04)
   #define GRP_FDBGR                HW_REGISTER_RW(GR_PPL_DEBUG_BASE + 0x08)
   #define GRP_FDBGS                HW_REGISTER_RW(GR_PPL_DEBUG_BASE + 0x0C)
   #define GRP_SDBG0                HW_REGISTER_RW(GR_PPL_DEBUG_BASE + 0x10)


   /* Registers for the Frame Buffer Cache */
   #define GR_FBC_BASE              0x1A005400
   #define GR_FBC_ADDR_MASK         0x0000007F

   #define GRFCS                    HW_REGISTER_RW(GR_FBC_BASE + 0x00)
   #define GRFCFG                   HW_REGISTER_RW(GR_FBC_BASE + 0x04)
   #define GRFTLOC                  HW_REGISTER_RW(GR_FBC_BASE + 0x08)
   #define GRFDIMS                  HW_REGISTER_RW(GR_FBC_BASE + 0x0C)
   #define GRFCCFG                  HW_REGISTER_RW(GR_FBC_BASE + 0x10)
   #define GRFCBA                   HW_REGISTER_RW(GR_FBC_BASE + 0x14)
   #define GRFZCFG                  HW_REGISTER_RW(GR_FBC_BASE + 0x1C)
   #define GRFZBA                   HW_REGISTER_RW(GR_FBC_BASE + 0x20)
   #define GRFZCV                   HW_REGISTER_RW(GR_FBC_BASE + 0x24)
   #define GRFSCV                   HW_REGISTER_RW(GR_FBC_BASE + 0x28)
   #define GRFCMSK                  HW_REGISTER_RW(GR_FBC_BASE + 0x2C)
   #define GRFECFG                  HW_REGISTER_RW(GR_FBC_BASE + 0x30)
   #define GRFEBA                   HW_REGISTER_RW(GR_FBC_BASE + 0x34)
   #define GRFCCV0                  HW_REGISTER_RW(GR_FBC_BASE + 0x40)
   #define GRFCCV1                  HW_REGISTER_RW(GR_FBC_BASE + 0x44)
   #define GRFCCV2                  HW_REGISTER_RW(GR_FBC_BASE + 0x48)
   #define GRFCCV3                  HW_REGISTER_RW(GR_FBC_BASE + 0x4C)
   #define GRFCCV4                  HW_REGISTER_RW(GR_FBC_BASE + 0x50)
   #define GRFCCV5                  HW_REGISTER_RW(GR_FBC_BASE + 0x54)
   #define GRFCCV6                  HW_REGISTER_RW(GR_FBC_BASE + 0x58)
   #define GRFCCV7                  HW_REGISTER_RW(GR_FBC_BASE + 0x5C)

   #define GR_FBC_DEBUG_BASE        0x1A005500
   #define GR_FBC_DEBUG_ADDR_MASK   0x7F

   #define GRFCSTAT                HW_REGISTER_RW(GR_FBC_DEBUG_BASE + 0x00)

   /* VPM access via VRF configuration */
   #define GR_VPM_VRFCFG_BASE       0x1A005D00
   #define GR_VPM_VRFCFG_ADDR_MASK  0x00000003
   #define GRVVSTRD                 HW_REGISTER_RW(GR_VPM_VRFCFG_BASE + 0x00)

   /* Registers for the Texture Unit */

   /* Mipmap pointer memoies (TU0 & TU1) */
   #define GRTMPM0_BASE             0x1A005E00
   #define GRTMPM1_BASE             0x1A005F00

   #define GRTMPM0                 HW_REGISTER_RW(GRTMPM0_BASE + 0x00)
   #define GRTMPM1                 HW_REGISTER_RW(GRTMPM1_BASE + 0x00)

   #define GRTMPM_MASK              0xFFFFFF00

   #define GR_TU_BASE0              0x1A005200
   #define GR_TU_BASE1              0x1A005220
   #define GR_TU_BASE2              0x1A005240
   #define GR_TU_BASE3              0x1A005260

   #define GR_TU_BASE4              0x1A005280
   #define GR_TU_BASE5              0x1A0052A0
   #define GR_TU_BASE6              0x1A0052C0
   #define GR_TU_BASE7              0x1A0052E0

   #define GR_TU_DBG_BASE           0x1A005300

   #define GR_TU_ADDR_MASK          0x000000FF
   #define GR_TU_UNIT_MASK          0xFFFFFF1F

   /* Control/Status registers for TU0 & TU1 */
   #define GRTCS0                   HW_REGISTER_RW(GR_TU_BASE0 + 0x00)
   #define GRTCS1                   HW_REGISTER_RW(GR_TU_BASE4 + 0x00)

   /* Common palette for all contexts per texutre unit */
   #define GRTPTBA0                 HW_REGISTER_RW(GR_TU_BASE1 + 0x00)
   #define GRTPTBA1                 HW_REGISTER_RW(GR_TU_BASE5 + 0x00)

   /* 4 sets of context registers for physical texture unit 0 */
   #define GRTCFG0                  HW_REGISTER_RW(GR_TU_BASE0 + 0x04)
   #define GRTDIM0                  HW_REGISTER_RW(GR_TU_BASE0 + 0x08)
   #define GRTBCOL0                 HW_REGISTER_RW(GR_TU_BASE0 + 0x0C)
   #define GRTLBIAS0                HW_REGISTER_RW(GR_TU_BASE0 + 0x1C)

   #define GRTCFG1                  HW_REGISTER_RW(GR_TU_BASE1 + 0x04)
   #define GRTDIM1                  HW_REGISTER_RW(GR_TU_BASE1 + 0x08)
   #define GRTBCOL1                 HW_REGISTER_RW(GR_TU_BASE1 + 0x0C)
   #define GRTLBIAS1                HW_REGISTER_RW(GR_TU_BASE1 + 0x1C)

   #define GRTCFG2                  HW_REGISTER_RW(GR_TU_BASE2 + 0x04)
   #define GRTDIM2                  HW_REGISTER_RW(GR_TU_BASE2 + 0x08)
   #define GRTBCOL2                 HW_REGISTER_RW(GR_TU_BASE2 + 0x0C)
   #define GRTLBIAS2                HW_REGISTER_RW(GR_TU_BASE2 + 0x1C)

   #define GRTCFG3                  HW_REGISTER_RW(GR_TU_BASE3 + 0x04)
   #define GRTDIM3                  HW_REGISTER_RW(GR_TU_BASE3 + 0x08)
   #define GRTBCOL3                 HW_REGISTER_RW(GR_TU_BASE3 + 0x0C)
   #define GRTLBIAS3                HW_REGISTER_RW(GR_TU_BASE3 + 0x1C)

   /* 4 sets of context registers for physical texture unit 1 */
   #define GRTCFG4                  HW_REGISTER_RW(GR_TU_BASE4 + 0x04)
   #define GRTDIM4                  HW_REGISTER_RW(GR_TU_BASE4 + 0x08)
   #define GRTBCOL4                 HW_REGISTER_RW(GR_TU_BASE4 + 0x0C)
   #define GRTLBIAS4                HW_REGISTER_RW(GR_TU_BASE4 + 0x1C)

   #define GRTCFG5                  HW_REGISTER_RW(GR_TU_BASE5 + 0x04)
   #define GRTDIM5                  HW_REGISTER_RW(GR_TU_BASE5 + 0x08)
   #define GRTBCOL5                 HW_REGISTER_RW(GR_TU_BASE5 + 0x0C)
   #define GRTLBIAS5                HW_REGISTER_RW(GR_TU_BASE5 + 0x1C)

   #define GRTCFG6                  HW_REGISTER_RW(GR_TU_BASE6 + 0x04)
   #define GRTDIM6                  HW_REGISTER_RW(GR_TU_BASE6 + 0x08)
   #define GRTBCOL6                 HW_REGISTER_RW(GR_TU_BASE6 + 0x0C)
   #define GRTLBIAS6                HW_REGISTER_RW(GR_TU_BASE6 + 0x1C)

   #define GRTCFG7                  HW_REGISTER_RW(GR_TU_BASE7 + 0x04)
   #define GRTDIM7                  HW_REGISTER_RW(GR_TU_BASE7 + 0x08)
   #define GRTBCOL7                 HW_REGISTER_RW(GR_TU_BASE7 + 0x0C)
   #define GRTLBIAS7                HW_REGISTER_RW(GR_TU_BASE7 + 0x1C)

   /* TU debug registers */
   #define GRTDBG0                  HW_REGISTER_RW(GR_TU_DBG_BASE + 0x00)

   /* Extra registers per TU for child image support */
   #define GRTCOFF0                 HW_REGISTER_RW(GR_TU_DBG_BASE + 0x04)
   #define GRTCDIM0                 HW_REGISTER_RW(GR_TU_DBG_BASE + 0x08)
   #define GRTCOFF1                 HW_REGISTER_RW(GR_TU_DBG_BASE + 0x84)
   #define GRTCDIM1                 HW_REGISTER_RW(GR_TU_DBG_BASE + 0x88)


   /* System Registers */
   #define GR_SYSTEM_BASE           0x1A005000

   #define GROCS                    HW_REGISTER_RW(GR_SYSTEM_BASE)
   #define GROCFG                   HW_REGISTER_RW(GR_SYSTEM_BASE + 4)
   #define GROIDC                   HW_REGISTER_RW(GR_SYSTEM_BASE + 8)

   /* System debug register */
   #define GR_SYSTEM_DEBUG_BASE     0x1A005100

   #define GRODBGA                  HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x000)

   /* Performance Counters Regs */
   #define GROPCTRC                 HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x070)
   #define GROPCTRE                 HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x074)

   #define GROPCTR0                 HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x080)
   #define GROPCTRS0                HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x084)
   #define GROPCTR1                 HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x088)
   #define GROPCTRS1                HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x08C)
   #define GROPCTR2                 HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x090)
   #define GROPCTRS2                HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x094)
   #define GROPCTR3                 HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x098)
   #define GROPCTRS3                HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x09C)
   #define GROPCTR4                 HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0A0)
   #define GROPCTRS4                HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0A4)
   #define GROPCTR5                 HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0A8)
   #define GROPCTRS5                HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0AC)
   #define GROPCTR6                 HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0B0)
   #define GROPCTRS6                HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0B4)
   #define GROPCTR7                 HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0B8)
   #define GROPCTRS7                HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0BC)
   #define GROPCTR8                 HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0C0)
   #define GROPCTRS8                HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0C4)
   #define GROPCTR9                 HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0C8)
   #define GROPCTRS9                HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0CC)
   #define GROPCTR10                HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0D0)
   #define GROPCTRS10               HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0D4)
   #define GROPCTR11                HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0D8)
   #define GROPCTRS11               HW_REGISTER_RW(GR_SYSTEM_DEBUG_BASE + 0x0DC)

   /* Performance Counters Defs */

   #define GROPCTR_FOVCULLEDPRIMS         0x01
   #define GROPCTR_FOVCLIPPEDPRIMS        0x02
   #define GROPCTR_REVCULLEDPRIMS         0x03
   #define GROPCTR_NOFEPIXELPRIMS         0x04
   #define GROPCTR_FEVALIDPRIMS           0x05
   #define GROPCTR_FEZCULLEDQUADS         0x06
   #define GROPCTR_FEVALIDQUADS           0x07
   #define GROPCTR_FEINVALIDPIXELS        0x08
   #define GROPCTR_FEPEZRDY               0x09
   #define GROPCTR_FEPEZIDLE              0x0A
   #define GROPCTR_FESTALLPREFETCH        0x0B
   #define GROPCTR_FESPMRDY               0x0C
   #define GROPCTR_FESPMSTALL             0x0D
   #define GROPCTR_TU0_SAME_SET_STALL     0x0E
   #define GROPCTR_TU0_SAME_BANK_STALL    0x0F
   #define GROPCTR_TU0_AXI_REQ_FIFO_FULL  0x10
   #define GROPCTR_TU0_CACHE_ACCESSES     0x11
   #define GROPCTR_TU0_CACHE_STALLS       0x12
   #define GROPCTR_TU0_CACHE_REQ_STALLS   0x13
   #define GROPCTR_TU0_CACHE_MISSES       0x14
   #define GROPCTR_TU0_CACHE_RCV_WAITS    0x15
   #define GROPCTR_TU1_SAME_SET_STALL     0x16
   #define GROPCTR_TU1_SAME_BANK_STALL    0x17
   #define GROPCTR_TU1_AXI_REQ_FIFO_FULL  0x18
   #define GROPCTR_TU1_CACHE_ACCESSES     0x19
   #define GROPCTR_TU1_CACHE_STALLS       0x1A
   #define GROPCTR_TU1_CACHE_REQ_STALLS   0x1B
   #define GROPCTR_TU1_CACHE_MISSES       0x1C
   #define GROPCTR_TU1_CACHE_RCV_WAITS    0x1D
   #define GROPCTR_PBE_FE_STALLS          0x1E
   #define GROPCTR_PBE_DEPTH_TEST_FAIL    0x1F
   #define GROPCTR_PBE_STCL_TEST_FAIL     0x20
   #define GROPCTR_PBE_DPTH_STCL_PASS     0x21
   #define GROPCTR_FBC_CZ_CLRFLG_FETCHES  0x22
   #define GROPCTR_FBC_CZ_LINE_FLUSHES    0x23
   #define GROPCTR_FBC_CZ_PBE_REQS        0x24
   #define GROPCTR_FBC_CZ_PBE_STALLS      0x25
   #define GROPCTR_FBC_CZ_PBE_MISSES      0x26
   #define GROPCTR_FBC_CZ_PBE_HITS        0x27
   #define GROPCTR_FBC_CZ_FETCH_STALLS    0x28
   #define GROPCTR_FBC_CZ_FE_QUAD_REQS    0x29
   #define GROPCTR_FBC_CZ_FE_LINE_REQS    0x2A
   #define GROPCTR_FBC_CZ_FE_UNUSED       0x2B
   #define GROPCTR_FBC_CZ_FE_MISSES       0x2C
   #define GROPCTR_FBC_CZ_FE_HITS         0x2D
   #define GROPCTR_FBC_CZ_FE_DISCARDED    0x2E
   #define GROPCTR_FBC_CZ_UM_STALLS       0x2F
   #define GROPCTR_FBC_CZ_FETCHES         0x30
   #define GROPCTR_FBC_CZ_EVICTIONS       0x31
   #define GROPCTR_FBC_EZ_CLRFLG_FETCHES  0x32
   #define GROPCTR_FBC_EZ_LINE_FLUSHES    0x33
   #define GROPCTR_FBC_EZ_PBE_REQS        0x34
   #define GROPCTR_FBC_EZ_PBE_STALLS      0x35
   #define GROPCTR_FBC_EZ_PBE_MISSES      0x36
   #define GROPCTR_FBC_EZ_PBE_HITS        0x37
   #define GROPCTR_FBC_EZ_FETCH_STALLS    0x38
   #define GROPCTR_FBC_EZ_FE_REQS         0x39
   #define GROPCTR_FBC_EZ_FE_MISSES       0x3A
   #define GROPCTR_FBC_EZ_FE_HITS         0x3B
   #define GROPCTR_FBC_EZ_FE_FETCHES      0x3C
   #define GROPCTR_FBC_EZ_UM_STALLS       0x3D
   #define GROPCTR_FBC_EZ_FETCHES         0x3E
   #define GROPCTR_FBC_EZ_EVICTIONS       0x3F

   //VRF defines
   #define VRF_SIZE (4096+64+64)

#endif /* _HARDWARE_VC4_H */

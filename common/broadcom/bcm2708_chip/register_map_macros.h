// common macros and definitions that dont come from the register definition files
// this also contains some aliasing  to provide names that arnt the same in the
// auto generated naming scheme.
// hopefully these will gradually be resolved and the problem will go away.

#ifndef _INCLUDE_BCM2760_CHIP_REGISTER_MAP_H


   #define HW_REGISTER_RW(addr) (*(volatile unsigned long *)(addr))
   #define HW_REGISTER_RO(addr) (*(const volatile unsigned long *)(addr))
   #define HW_POINTER_TO_ADDRESS(pointer) ((uint32_t)(void *)&(pointer))


   #define ADDRESS_EXTERNAL(p) ((int)(p) & (3<<26))

   // Note: these macro evaluate argument twice - beware of side effects
   #define ALIAS_NORMAL(x)               ((void*)(((unsigned)(x)&~0xf0000000)|0x00000000)) // normal cached data (uses main 128K L2 cache)
   #define ALIAS_STREAMING(x)            ((void*)(((unsigned)(x)&~0xf0000000)|0x10000000)) // streaming data (used second 32K L2 cache) *not implemented*
   #define ALIAS_COHERENT(x)             ((void*)(((unsigned)(x)&~0xf0000000)|0x20000000)) // cache coherent but non-allocating
   #define ALIAS_DIRECT(x)               ((void*)(((unsigned)(x)&~0xf0000000)|0x30000000)) // uncached
   #define ALIAS_ANY_NONALLOCATING(x)    ((void*)(((unsigned)(x)&~0x00000000)|0x20000000)) // change NORMAL to COHERENT, and keep DIRECT DIRECT

   #define IS_ALIAS_NORMAL(x)    ((((unsigned)(x)>>28)&0xf)==0)
   #define IS_ALIAS_STREAMING(x) ((((unsigned)(x)>>28)&0xf)==1)
   #define IS_ALIAS_COHERENT(x)  ((((unsigned)(x)>>28)&0xf)==2)
   #define IS_ALIAS_DIRECT(x)    ((((unsigned)(x)>>28)&0xf)==3)
   #define IS_ALIAS_NONALLOCATING(x) ((((unsigned)(x)>>28)&0xe)==2)

   //default interrupt vector table base address
   #define INTERRUPT_VECTOR_BASE 0

   //common interrupts
   #define INTERRUPT_EXCEPTION_OFFSET  0
   #define INTERRUPT_EXCEPTION_NUM     32

   #define INTERRUPT_SOFTINT_OFFSET    32
   #define INTERRUPT_SOFTINT_NUM       32

   #define INTERRUPT_HARDINT_OFFSET    64
   #define INTERRUPT_HARDINT_NUM       64

   #define MAX_DMA_NUM 8
   #define MAX_DMA_SUB 1
   #define MAX_GPIO_NUM 2
   #define MAX_TIMER_NUM 4
   #define MAX_EXCEPTION_NUM 8

   #define BOOTROM_BASE_ADDRESS           0x10000000

    //VRF defines
   #define VRF_SIZE (4096+64+64)

   // no of GPIO pins
   #define VIDEOCORE_NUM_GPIO_PINS        70


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
   #define INTERRUPT_DMA9                 (INTERRUPT_HW_OFFSET + 25 )
   #define INTERRUPT_DMA10                (INTERRUPT_HW_OFFSET + 26 )
   #define INTERRUPT_DMA11                (INTERRUPT_HW_OFFSET + 27 )
   #define INTERRUPT_DMA12                (INTERRUPT_HW_OFFSET + 28 )
   #define INTERRUPT_DMA13                (INTERRUPT_HW_OFFSET + 29 )
   #define INTERRUPT_DMA14                (INTERRUPT_HW_OFFSET + 30 )
   #define INTERRUPT_DMA15                (INTERRUPT_HW_OFFSET + 31 )
   #define INTERRUPT_HOSTPORT             (INTERRUPT_HW_OFFSET + 32 )
   #define INTERRUPT_VIDEOSCALER          (INTERRUPT_HW_OFFSET + 33 )
   #define INTERRUPT_CRYPTO               (INTERRUPT_HW_OFFSET + 34 )
   #define INTERRUPT_SPARE1               (INTERRUPT_HW_OFFSET + 35 )
   #define INTERRUPT_DSI0                 (INTERRUPT_HW_OFFSET + 36 )
//   #define INTERRUPT_DSI1                 (INTERRUPT_HW_OFFSET + 37 )
   #define INTERRUPT_CCP2                 (INTERRUPT_HW_OFFSET + 38 )
   #define INTERRUPT_CSI2                 (INTERRUPT_HW_OFFSET + 39 )
   #define INTERRUPT_HDMI0                (INTERRUPT_HW_OFFSET + 40 )
   #define INTERRUPT_HDMI1                (INTERRUPT_HW_OFFSET + 41 )
//   #define INTERRUPT_HDMI2                (INTERRUPT_HW_OFFSET + 42 )
   #define INTERRUPT_PIXELVALVE1          (INTERRUPT_HW_OFFSET + 42 )
   #define INTERRUPT_PARALLELCAMERA       (INTERRUPT_HW_OFFSET + 43 )
   #define INTERRUPT_PIXELVALVE0          (INTERRUPT_HW_OFFSET + 44 )
   #define INTERRUPT_PLL                  (INTERRUPT_HW_OFFSET + 45 )
   #define INTERRUPT_CDP                  (INTERRUPT_HW_OFFSET + 46 )
   #define INTERRUPT_SMI                  (INTERRUPT_HW_OFFSET + 47 )
   #define INTERRUPT_GPIO0                (INTERRUPT_HW_OFFSET + 48 )
   #define INTERRUPT_GPIO1                (INTERRUPT_HW_OFFSET + 49 )
   #define INTERRUPT_GPIO2                (INTERRUPT_HW_OFFSET + 50 )
   #define INTERRUPT_GPION                (INTERRUPT_HW_OFFSET + 51 )
   #define INTERRUPT_SLIMBUS              (INTERRUPT_HW_OFFSET + 52 )
   #define INTERRUPT_I2C                  (INTERRUPT_HW_OFFSET + 53 )
   #define INTERRUPT_SPI                  (INTERRUPT_HW_OFFSET + 54 )
   #define INTERRUPT_I2SPCM               (INTERRUPT_HW_OFFSET + 55 )
   #define INTERRUPT_SDIO                 (INTERRUPT_HW_OFFSET + 56 )
   #define INTERRUPT_UART                 (INTERRUPT_HW_OFFSET + 57 )
   #define INTERRUPT_ADC                  (INTERRUPT_HW_OFFSET + 58 )
   #define INTERRUPT_VEC                  (INTERRUPT_HW_OFFSET + 59 )
   #define INTERRUPT_SPARE2               (INTERRUPT_HW_OFFSET + 60 )
   #define INTERRUPT_SPARE3               (INTERRUPT_HW_OFFSET + 61 )
   #define INTERRUPT_SPARE4               (INTERRUPT_HW_OFFSET + 62 )
   #define INTERRUPT_SPARE5               (INTERRUPT_HW_OFFSET + 63 )
   #define INTERRUPT_DUMMY                (INTERRUPT_HW_OFFSET + 63 )

   // aliases
   #define INTERRUPT_HOSTINTERFACE INTERRUPT_HOSTPORT
   #define INTERRUPT_SDCARDHOST    INTERRUPT_SDIO

   // temporary dummy register definitions to avoid compile errors
   #define DUMMYREG HW_REGISTER_RW( 0x7C ) //software exception vector 15




   /*---------------------------------------------------------------------------*/
   /*---------------------------------------------------------------------------*/
   // ALIAS TABLE
   // this is currently here to avoid problems with the new reg definition names
   // not matching the old.  This needs to be sorted and removed
   // TODO  FIXME
   /*---------------------------------------------------------------------------*/
   /*---------------------------------------------------------------------------*/

   /* Cache controller */
   #define IC0CS              L1_IC0_CONTROL
   #define IC0START           L1_I_FLUSH_S
   #define IC0END             L1_I_FLUSH_E
   #define DC0CS              L1_D_CONTROL
   #define DC0START           L1_D_FLUSH_S
   #define DC0END             L1_D_FLUSH_E
   #define L2CS               L2_CONT_OFF
   #define L2START            L2_FLUSH_STA
   #define L2END              L2_FLUSH_END




   /*---------------------------------------------------------------------------*/
   /* Priority arbiter */

   #define AM_VP_L2_PRI                                             HW_REGISTER_RW( 0x1800d000 )
   #define AM_VP_UC_PRI                                             HW_REGISTER_RW( 0x1800d004 )
   #define AM_VP_PERPRI                                             HW_REGISTER_RW( 0x1800d008 )
   #define AM_HO_MEMPRI                                             HW_REGISTER_RW( 0x1800d00c )
   #define AM_HO_PERPRI                                             HW_REGISTER_RW( 0x1800d010 )
   #define AM_DB_MEMPRI                                             HW_REGISTER_RW( 0x1800d014 )
   #define AM_DB_PERPRI                                             HW_REGISTER_RW( 0x1800d018 )
   #define AM_HVSM_PRI                                              HW_REGISTER_RW( 0x1800d01c )

   /*---------------------------------------------------------------------------*/
   /* Nexus Controller */

   #define NOWNT               HW_REGISTER_RW(DEBUG_MASTER_BASE + 0x4)
   #define NIOREQ              HW_REGISTER_RW(DEBUG_MASTER_BASE + 0x0)

   /*---------------------------------------------------------------------------*/
   /* Reset Controller */

   #define RSTCS               HW_REGISTER_RW(RS_BASE + 0x0)
   #define RSTWD               HW_REGISTER_RW(RS_BASE + 0x4)
   #define RSTID               HW_REGISTER_RW(RS_BASE + 0x8)
   #define RSTFD               HW_REGISTER_RW(RS_BASE + 0xc)
   #define RSC0ADDR            HW_REGISTER_RW(RS_BASE + 0x10)





   //the start location of the scalers context memory
   #define SCALER_CONTEXT_MEM_SIZE           ( 1024 * 16 ) //16k

   //the size of the line buffer memory
   #define SCALER_LINE_BUFFER_MEM_SIZE       ( 1024 * 64 ) //64k

   //The maximum size of the COB buffer (the output fifo)
   #define SCALER_COB_FIFO_SIZE              (0x4000) //8Kbytes

   /*---------------------------------------------------------------------------*/
   /* Transposer */

   #define TRANSPOSER_DST_PTR                TXP_DST_PTR
   #define TRANSPOSER_DST_PITCH              TXP_DST_PITCH
   #define TRANSPOSER_DIMENSIONS             TXP_DIM
   #define TRANSPOSER_CONTROL                TXP_CTRL
   #define TRANSPOSER_PROGRESS               TXP_PROGRESS

   /*---------------------------------------------------------------------------*/
   /* Pixel Valve */

   #define PIXELVALVE_C_0                    PIXELVALVE0_C
   #define PIXELVALVE_VC_0                   PIXELVALVE0_VC
   #define PIXELVALVE_HSYNC_0                PIXELVALVE0_HSYNC
   #define PIXELVALVE_VSYNC_0                PIXELVALVE0_VSYNC
   #define PIXELVALVE_VSIZE_0                PIXELVALVE0_VSIZE
   #define PIXELVALVE_INTEN_0                PIXELVALVE0_INTEN
   #define PIXELVALVE_INTSTAT_0              PIXELVALVE0_INTSTAT
   #define PIXELVALVE_STAT_0                 PIXELVALVE0_STAT

   #define PIXELVALVE_C_1                    PIXELVALVE1_C
   #define PIXELVALVE_VC_1                   PIXELVALVE1_VC
   #define PIXELVALVE_HSYNC_1                PIXELVALVE1_HSYNC
   #define PIXELVALVE_VSYNC_1                PIXELVALVE1_VSYNC
   #define PIXELVALVE_VSIZE_1                PIXELVALVE1_VSIZE
   #define PIXELVALVE_INTEN_1                PIXELVALVE1_INTEN
   #define PIXELVALVE_INTSTAT_1              PIXELVALVE1_INTSTAT
   #define PIXELVALVE_STAT_1                 PIXELVALVE1_STAT

   #define PIXELVALVE_C_x( x )               HW_REGISTER_RW( ((0 == x) ? PIXELVALVE0_BASE : PIXELVALVE1_BASE) + 0x00 )
   #define PIXELVALVE_VC_x( x )              HW_REGISTER_RW( ((0 == x) ? PIXELVALVE0_BASE : PIXELVALVE1_BASE) + 0x04 )
   #define PIXELVALVE_HSYNC_x( x )           HW_REGISTER_RW( ((0 == x) ? PIXELVALVE0_BASE : PIXELVALVE1_BASE) + 0x08 )
   #define PIXELVALVE_VSYNC_x( x )           HW_REGISTER_RW( ((0 == x) ? PIXELVALVE0_BASE : PIXELVALVE1_BASE) + 0x0C )
   #define PIXELVALVE_VSIZE_x( x )           HW_REGISTER_RW( ((0 == x) ? PIXELVALVE0_BASE : PIXELVALVE1_BASE) + 0x10 )
   #define PIXELVALVE_INTEN_x( x )           HW_REGISTER_RW( ((0 == x) ? PIXELVALVE0_BASE : PIXELVALVE1_BASE) + 0x14 )
   #define PIXELVALVE_INTSTAT_x( x )         HW_REGISTER_RW( ((0 == x) ? PIXELVALVE0_BASE : PIXELVALVE1_BASE) + 0x18 )
   #define PIXELVALVE_STAT_x( x )            HW_REGISTER_RW( ((0 == x) ? PIXELVALVE0_BASE : PIXELVALVE1_BASE) + 0x1C )

   /*---------------------------------------------------------------------------*/
   /* DSI */

   #define   DSI_CTRL             HW_REGISTER_RW( DSI_BASE + 0x00  )
   #define   DSI_CMD_PKTC         HW_REGISTER_RW( DSI_BASE + 0x04  )
   #define   DSI_CMD_PKTH         HW_REGISTER_RW( DSI_BASE + 0x08  )
   #define   DSI_RX1_PKTH         HW_REGISTER_RW( DSI_BASE + 0x0C  )
   #define   DSI_RX2_PKTH         HW_REGISTER_RW( DSI_BASE + 0x10  )
   #define   DSI_CMD_DATA_FIFO    HW_REGISTER_RW( DSI_BASE + 0x14  )
   #define   DSI_DISP0_CTRL       HW_REGISTER_RW( DSI_BASE + 0x18  )
   #define   DSI_DISP1_CTRL       HW_REGISTER_RW( DSI_BASE + 0x1C  )
   #define   DSI_PIX_FIFO         HW_REGISTER_RW( DSI_BASE + 0x20  )
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
   /* Multicore sync block */

   //number of hardware semaphores
   #define MULTICORE_SYNC_NUM_SEMAPHORES              (32)

   //registers...
   #define MULTICORE_SYNC_SEMA_MASK( num )            HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + ((num) * 0x4) )

   #define MULTICORE_SYNC_SEMA_MASK_0                 HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x00 )
   #define MULTICORE_SYNC_SEMA_MASK_1                 HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x04 )
   #define MULTICORE_SYNC_SEMA_MASK_2                 HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x08 )
   #define MULTICORE_SYNC_SEMA_MASK_3                 HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x0C )
   #define MULTICORE_SYNC_SEMA_MASK_4                 HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x10 )
   #define MULTICORE_SYNC_SEMA_MASK_5                 HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x14 )
   #define MULTICORE_SYNC_SEMA_MASK_6                 HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x18 )
   #define MULTICORE_SYNC_SEMA_MASK_7                 HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x1C )
   #define MULTICORE_SYNC_SEMA_MASK_8                 HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x20 )
   #define MULTICORE_SYNC_SEMA_MASK_9                 HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x24 )
   #define MULTICORE_SYNC_SEMA_MASK_10                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x28 )
   #define MULTICORE_SYNC_SEMA_MASK_11                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x2C )
   #define MULTICORE_SYNC_SEMA_MASK_12                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x30 )
   #define MULTICORE_SYNC_SEMA_MASK_13                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x34 )
   #define MULTICORE_SYNC_SEMA_MASK_14                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x38 )
   #define MULTICORE_SYNC_SEMA_MASK_15                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x3C )
   #define MULTICORE_SYNC_SEMA_MASK_16                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x40 )
   #define MULTICORE_SYNC_SEMA_MASK_17                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x44 )
   #define MULTICORE_SYNC_SEMA_MASK_18                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x48 )
   #define MULTICORE_SYNC_SEMA_MASK_19                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x4C )
   #define MULTICORE_SYNC_SEMA_MASK_20                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x50 )
   #define MULTICORE_SYNC_SEMA_MASK_21                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x54 )
   #define MULTICORE_SYNC_SEMA_MASK_22                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x58 )
   #define MULTICORE_SYNC_SEMA_MASK_23                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x5C )
   #define MULTICORE_SYNC_SEMA_MASK_24                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x60 )
   #define MULTICORE_SYNC_SEMA_MASK_25                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x64 )
   #define MULTICORE_SYNC_SEMA_MASK_26                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x68 )
   #define MULTICORE_SYNC_SEMA_MASK_27                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x6C )
   #define MULTICORE_SYNC_SEMA_MASK_28                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x70 )
   #define MULTICORE_SYNC_SEMA_MASK_29                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x74 )
   #define MULTICORE_SYNC_SEMA_MASK_30                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x78 )
   #define MULTICORE_SYNC_SEMA_MASK_31                HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x7C )

   #define MULTICORE_SYNC_SEMA_STATUS                 HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x80 )
   #define MULTICORE_SYNC_IREQ_0                      HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x84 )
   #define MULTICORE_SYNC_IREQ_1                      HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x88 )

   #define MULTICORE_SYNC_ICSET_0                     HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x90 )
   #define MULTICORE_SYNC_ICSET_1                     HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x94 )
   #define MULTICORE_SYNC_ICCLR_0                     HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x98 )
   #define MULTICORE_SYNC_ICCLR_1                     HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0x9C )

   #define MULTICORE_SYNC_MBOX_0                      HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0xA0 )
   #define MULTICORE_SYNC_MBOX_1                      HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0xA4 )
   #define MULTICORE_SYNC_MBOX_2                      HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0xA8 )
   #define MULTICORE_SYNC_MBOX_3                      HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0xAC )
   #define MULTICORE_SYNC_MBOX_4                      HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0xB0 )
   #define MULTICORE_SYNC_MBOX_5                      HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0xB4 )
   #define MULTICORE_SYNC_MBOX_6                      HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0xB8 )
   #define MULTICORE_SYNC_MBOX_7                      HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0xBC )

   #define MULTICORE_SYNC_MBOX_MASK( num )            HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + ((num) * 0x4) )

   //The special intercore system semaphore, used for fine grained counting
   #define MULTICORE_SYNC_VPU_SEMA_0                  HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0xC0 )
   #define MULTICORE_SYNC_VPU_SEMA_1                  HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0xC4 )

   #define MULTICORE_SYNC_VPU_SEMA_x( x )             ( ((x) == 0) ? MULTICORE_SYNC_VPU_SEMA_0 : MULTICORE_SYNC_VPU_SEMA_1 )

   #define MULTICORE_SYNC_VPU_SEMA_STATUS             HW_REGISTER_RW( MULTICORE_SYNC_BASE_ADDRESS + 0xC8 )

      //read a semaphore block until its '0'
   //when the semaphore is free, allow it to increment a shared counter


   /*---------------------------------------------------------------------------*/
   /* CCP2 Peripheral */

   #define CCP2RC                                     HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x00 )
   #define CCP2RS                                     HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x04 )
   #define CCP2RDR1                                   HW_REGISTER_RO( CCP2_BASE_ADDRESS + 0x80 )
   #define CCP2RDR2                                   HW_REGISTER_RO( CCP2_BASE_ADDRESS + 0x84 )
   #define CCP2RDR3                                   HW_REGISTER_RO( CCP2_BASE_ADDRESS + 0x88 )

   #define CCP2RC0                                    HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x100 )
   #define CCP2RPC0                                   HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x104 )
   #define CCP2RS0                                    HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x108 )
   #define CCP2RSA0                                   HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x10C )
   #define CCP2REA0                                   HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x110 )
   #define CCP2RWP0                                   HW_REGISTER_RO( CCP2_BASE_ADDRESS + 0x114 )
   #define CCP2RBC0                                   HW_REGISTER_RO( CCP2_BASE_ADDRESS + 0x118 )
   #define CCP2RLS0                                   HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x11C )
   #define CCP2RDSA0                                  HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x120 )
   #define CCP2RDEA0                                  HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x124 )
   #define CCP2RDS0                                   HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x128 )

   #define CCP2RC1                                    HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x200 )
   #define CCP2RPC1                                   HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x204 )
   #define CCP2RS1                                    HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x208 )
   #define CCP2RSA1                                   HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x20C )
   #define CCP2REA1                                   HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x210 )
   #define CCP2RWP1                                   HW_REGISTER_RO( CCP2_BASE_ADDRESS + 0x214 )
   #define CCP2RBC1                                   HW_REGISTER_RO( CCP2_BASE_ADDRESS + 0x218 )
   #define CCP2RLS1                                   HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x21C )
   #define CCP2RDSA1                                  HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x220 )
   #define CCP2RDEA1                                  HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x224 )
   #define CCP2RDS1                                   HW_REGISTER_RW( CCP2_BASE_ADDRESS + 0x228 )


   /*---------------------------------------------------------------------------*/
   /* CSI2 Peripheral */

   #define CSI2_RC                                    HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x00 )
   #define CSI2_RS                                    HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x04 )
   #define CSI2_RDLS                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x08 )
   #define CSI2_RGSP                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x0C )
   #define CSI2_TREN                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x10 )
   #define CSI2_THSSTO                                HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x14 )
   #define CSI2_THSSET                                HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x18 )
   #define CSI2_THSCKTO                               HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x1C )

   #define CSI2LPRX0                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x20 )
   #define CSI2LPRX1                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x24 )
   #define CSI2LPRX2                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x28 )
   #define CSI2LPRX3                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x2C )
   #define CSI2LPRXC                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x30 )
   #define CSI2TRIG                                   HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x40 )

   #define CSI2DBGDPHY                                HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x80 )
   #define CSI2DBGMISC                                HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x84 )

   #define CSI2_SRST                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x90 )
   #define CSI2RDR3                                   HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x94 )

   #define CSI2_RC0                                   HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x100 )
   #define CSI2_RPC0                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x104 )
   #define CSI2_RS0                                   HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x108 )
   #define CSI2_RSA0                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x10C )
   #define CSI2_REA0                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x110 )
   #define CSI2_RWP0                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x114 )
   #define CSI2_RBC0                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x118 )
   #define CSI2_RLS0                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x11C )
   #define CSI2_RDSA0                                 HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x120 )
   #define CSI2_RDEA0                                 HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x124 )
   #define CSI2_RDS0                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x128 )
   #define CSI2_DTOV0                                 HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x12C )

   #define CSI2_RC1                                   HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x200 )
   #define CSI2_RPC1                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x204 )
   #define CSI2_RS1                                   HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x208 )
   #define CSI2_RSA1                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x20C )
   #define CSI2_REA1                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x210 )
   #define CSI2_RWP1                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x214 )
   #define CSI2_RBC1                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x218 )
   #define CSI2_RLS1                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x21C )
   #define CSI2_RDSA1                                 HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x220 )
   #define CSI2_RDEA1                                 HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x224 )
   #define CSI2_RDS1                                  HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x228 )
   #define CSI2_DTOV1                                 HW_REGISTER_RW( CSI2_BASE_ADDRESS + 0x22C )

   #define CSI2_RC_x( x )                             ( ((x) == 0) ? CSI2_RC0 : CSI2_RC1 )
   #define CSI2_RPC_x( x )                            ( ((x) == 0) ? CSI2_RPC0 : CSI2_RPC1 )
   #define CSI2_RS_x( x )                             ( ((x) == 0) ? CSI2_RS0 : CSI2_RS1 )
   #define CSI2_RSA_x( x )                            ( ((x) == 0) ? CSI2_RSA0 : CSI2_RSA1 )
   #define CSI2_REA_x( x )                            ( ((x) == 0) ? CSI2_REA0 : CSI2_REA1 )
   #define CSI2_RWP_x( x )                            ( ((x) == 0) ? CSI2_RWP0 : CSI2_RWP1 )
   #define CSI2_RBC_x( x )                            ( ((x) == 0) ? CSI2_RBC0 : CSI2_RBC1 )
   #define CSI2_RLS_x( x )                            ( ((x) == 0) ? CSI2_RLS0 : CSI2_RLS1 )
   #define CSI2_RDSA_x( x )                           ( ((x) == 0) ? CSI2_RDSA0 : CSI2_RDSA1 )
   #define CSI2_RDEA_x( x )                           ( ((x) == 0) ? CSI2_RDEA0 : CSI2_RDEA1 )
   #define CSI2_RDS_x( x )                            ( ((x) == 0) ? CSI2_RDS0 : CSI2_RDS1 )
   #define CSI2_DTOV_x( x )                           ( ((x) == 0) ? CSI2_DTOV0 : CSI2_DTOV1 )

   /*---------------------------------------------------------------------------*/
   /* DPI Peripheral */

   #define DPI_DPIC                                   HW_REGISTER_RW( DPI_BASE_ADDRESS + 0x00 )

   /*---------------------------------------------------------------------------*/
   /* VEC Peripheral */
   #define WSE_RESET                       VEC_WSE_RESET
   #define WSE_CONTROL                     VEC_WSE_CONTROL
   #define WSE_WSS_DATA                    VEC_WSE_WSS_DATA
   #define WSE_VPS_DATA_1                  VEC_WSE_VPS_DATA_1
   #define WSE_VPS_CONTROL                 VEC_WSE_VPS_CONTROL
   #define CGMSAE_RESET                    VEC_CGMSAE_RESET
   #define CGMSAE_TOP_CONTROL              VEC_CGMSAE_TOP_CONTROL
   #define CGMSAE_BOT_CONTROL              VEC_CGMSAE_BOT_CONTROL
   #define CGMSAE_TOP_FORMAT               VEC_CGMSAE_TOP_FORMAT
   #define CGMSAE_BOT_FORMAT               VEC_CGMSAE_BOT_FORMAT
   #define CGMSAE_TOP_DATA                 VEC_CGMSAE_TOP_DATA
   #define CGMSAE_BOT_DATA                 VEC_CGMSAE_BOT_DATA
   #define CGMSAE_REVID                    VEC_CGMSAE_REVID
   #define VIDEO_ENC_RevID                 VEC_ENC_RevID
   #define VIDEO_ENC_PrimaryControl        VEC_ENC_PrimaryControl


   /*---------------------------------------------------------------------------*/
   /* HDMI Peripheral */

   #define HDMI_RBUS_REGS

   #define HDMI_CORE_REV                              HW_REGISTER_RW( HDMI_BASE_ADDRESS + 0 )
   #define HDMI_SW_RESET_CNTRL                        HW_REGISTER_RW( HDMI_BASE_ADDRESS + 4 )
   #define HDMI_HOTPLUG_INT                           HW_REGISTER_RW( HDMI_BASE_ADDRESS + 8 )
   #define HDMI_HOTPLUG                               HW_REGISTER_RW( HDMI_BASE_ADDRESS + 12 )
   #define HDMI_BKSV0                                 HW_REGISTER_RW( HDMI_BASE_ADDRESS + 16 )
   #define HDMI_BKSV1                                 HW_REGISTER_RW( HDMI_BASE_ADDRESS + 20 )
   #define HDMI_AN0                                   HW_REGISTER_RW( HDMI_BASE_ADDRESS + 24 )
   #define HDMI_AN1                                   HW_REGISTER_RW( HDMI_BASE_ADDRESS + 28 )
   #define HDMI_AN_INFLUENCE_1                        HW_REGISTER_RW( HDMI_BASE_ADDRESS + 32 )
   #define HDMI_AN_INFLUENCE_2                        HW_REGISTER_RW( HDMI_BASE_ADDRESS + 36 )
   #define HDMI_TST_AN0                               HW_REGISTER_RW( HDMI_BASE_ADDRESS + 40 )
   #define HDMI_TST_AN1                               HW_REGISTER_RW( HDMI_BASE_ADDRESS + 44 )
   #define HDMI_KSV_FIFO_0                            HW_REGISTER_RW( HDMI_BASE_ADDRESS + 48 )
   #define HDMI_KSV_FIFO_1                            HW_REGISTER_RW( HDMI_BASE_ADDRESS + 52 )
   #define HDMI_V                                     HW_REGISTER_RW( HDMI_BASE_ADDRESS + 56 )
   #define HDMI_HDCP_KEY_1                            HW_REGISTER_RW( HDMI_BASE_ADDRESS + 60 )
   #define HDMI_HDCP_KEY_2                            HW_REGISTER_RW( HDMI_BASE_ADDRESS + 64 )
   #define HDMI_HDCP_CTL                              HW_REGISTER_RW( HDMI_BASE_ADDRESS + 68 )
   #define HDMI_CP_STATUS                             HW_REGISTER_RW( HDMI_BASE_ADDRESS + 72 )
   #define HDMI_CP_INTEGRITY                          HW_REGISTER_RW( HDMI_BASE_ADDRESS + 76 )
   #define HDMI_CP_INTEGRITY_CFG                      HW_REGISTER_RW( HDMI_BASE_ADDRESS + 80 )
   #define HDMI_CP_CONFIG                             HW_REGISTER_RW( HDMI_BASE_ADDRESS + 84 )
   #define HDMI_CP_TST                                HW_REGISTER_RW( HDMI_BASE_ADDRESS + 88 )
   #define HDMI_FIFO_CTL                              HW_REGISTER_RW( HDMI_BASE_ADDRESS + 92 )
   #define HDMI_READ_POINTERS                         HW_REGISTER_RW( HDMI_BASE_ADDRESS + 96 )
   #define HDMI_ENCODER_CTL                           HW_REGISTER_RW( HDMI_BASE_ADDRESS + 112 )
   #define HDMI_PERT_CONFIG                           HW_REGISTER_RW( HDMI_BASE_ADDRESS + 116 )
   #define HDMI_PERT_LFSR_PRELOAD                     HW_REGISTER_RW( HDMI_BASE_ADDRESS + 120 )
   #define HDMI_PERT_LFSR_FEEDBACK_MASK               HW_REGISTER_RW( HDMI_BASE_ADDRESS + 124 )
   #define HDMI_PERT_INSERT_ERR                       HW_REGISTER_RW( HDMI_BASE_ADDRESS + 128 )
   #define HDMI_PERT_INSERT_ERR_SEPARATION            HW_REGISTER_RW( HDMI_BASE_ADDRESS + 132 )
   #define HDMI_PERT_TEST_LENGTH                      HW_REGISTER_RW( HDMI_BASE_ADDRESS + 136 )
   #define HDMI_PERT_DATA                             HW_REGISTER_RW( HDMI_BASE_ADDRESS + 140 )
   #define HDMI_MAI_CHANNEL_MAP                       HW_REGISTER_RW( HDMI_BASE_ADDRESS + 144 )
   #define HDMI_MAI_CONFIG                            HW_REGISTER_RW( HDMI_BASE_ADDRESS + 148 )
   #define HDMI_MAI_FORMAT                            HW_REGISTER_RW( HDMI_BASE_ADDRESS + 152 )
   #define HDMI_AUDIO_PACKET_CONFIG                   HW_REGISTER_RW( HDMI_BASE_ADDRESS + 156 )
   #define HDMI_RAM_PACKET_CONFIG                     HW_REGISTER_RW( HDMI_BASE_ADDRESS + 160 )
   #define HDMI_RAM_PACKET_STATUS                     HW_REGISTER_RW( HDMI_BASE_ADDRESS + 164 )
   #define HDMI_CRP_CFG                               HW_REGISTER_RW( HDMI_BASE_ADDRESS + 168 )
   #define HDMI_CTS_0                                 HW_REGISTER_RW( HDMI_BASE_ADDRESS + 172 )
   #define HDMI_CTS_1                                 HW_REGISTER_RW( HDMI_BASE_ADDRESS + 176 )
   #define HDMI_CTS_PERIOD_0                          HW_REGISTER_RW( HDMI_BASE_ADDRESS + 180 )
   #define HDMI_CTS_PERIOD_1                          HW_REGISTER_RW( HDMI_BASE_ADDRESS + 184 )
   #define HDMI_BCH_CONFIGURATION                     HW_REGISTER_RW( HDMI_BASE_ADDRESS + 188 )
   #define HDMI_SCHEDULER_CONTROL                     HW_REGISTER_RW( HDMI_BASE_ADDRESS + 192 )
   #define HDMI_HORZA                                 HW_REGISTER_RW( HDMI_BASE_ADDRESS + 196 )
   #define HDMI_HORZB                                 HW_REGISTER_RW( HDMI_BASE_ADDRESS + 200 )
   #define HDMI_VERTA0                                HW_REGISTER_RW( HDMI_BASE_ADDRESS + 204 )
   #define HDMI_VERTB0                                HW_REGISTER_RW( HDMI_BASE_ADDRESS + 208 )
   #define HDMI_VERTA1                                HW_REGISTER_RW( HDMI_BASE_ADDRESS + 212 )
   #define HDMI_VERTB1                                HW_REGISTER_RW( HDMI_BASE_ADDRESS + 216 )
   #define HDMI_TEST                                  HW_REGISTER_RW( HDMI_BASE_ADDRESS + 220 )
   #define HDMI_MBIST_TM                              HW_REGISTER_RW( HDMI_BASE_ADDRESS + 224 )
   #define HDMI_MISC_CONTROL                          HW_REGISTER_RW( HDMI_BASE_ADDRESS + 228 )
   #define HDMI_CEC_CNTRL_1                           HW_REGISTER_RW( HDMI_BASE_ADDRESS + 232 )
   #define HDMI_CEC_CNTRL_2                           HW_REGISTER_RW( HDMI_BASE_ADDRESS + 236 )
   #define HDMI_CEC_CNTRL_3                           HW_REGISTER_RW( HDMI_BASE_ADDRESS + 240 )
   #define HDMI_CEC_CNTRL_4                           HW_REGISTER_RW( HDMI_BASE_ADDRESS + 244 )
   #define HDMI_CEC_CNTRL_5                           HW_REGISTER_RW( HDMI_BASE_ADDRESS + 248 )
   #define HDMI_CEC_TX_DATA_1                         HW_REGISTER_RW( HDMI_BASE_ADDRESS + 252 )
   #define HDMI_CEC_TX_DATA_2                         HW_REGISTER_RW( HDMI_BASE_ADDRESS + 256 )
   #define HDMI_CEC_TX_DATA_3                         HW_REGISTER_RW( HDMI_BASE_ADDRESS + 260 )
   #define HDMI_CEC_TX_DATA_4                         HW_REGISTER_RW( HDMI_BASE_ADDRESS + 264 )
   #define HDMI_CEC_RX_DATA_1                         HW_REGISTER_RW( HDMI_BASE_ADDRESS + 268 )
   #define HDMI_CEC_RX_DATA_2                         HW_REGISTER_RW( HDMI_BASE_ADDRESS + 272 )
   #define HDMI_CEC_RX_DATA_3                         HW_REGISTER_RW( HDMI_BASE_ADDRESS + 276 )
   #define HDMI_CEC_RX_DATA_4                         HW_REGISTER_RW( HDMI_BASE_ADDRESS + 280 )
   #define HDMI_PACKET_FIFO_CTL                       HW_REGISTER_RW( HDMI_BASE_ADDRESS + 284 )
   #define HDMI_PACKET_FIFO_CFG                       HW_REGISTER_RW( HDMI_BASE_ADDRESS + 288 )
   #define HDMI_PACKET_FIFO_STATUS                    HW_REGISTER_RW( HDMI_BASE_ADDRESS + 292 )
   #define HDMI_DVO_TIMING_ADJUST_A                   HW_REGISTER_RW( HDMI_BASE_ADDRESS + 296 )
   #define HDMI_DVO_TIMING_ADJUST_B                   HW_REGISTER_RW( HDMI_BASE_ADDRESS + 300 )
   #define HDMI_DVO_TIMING_ADJUST_C                   HW_REGISTER_RW( HDMI_BASE_ADDRESS + 304 )
   #define HDMI_DVO_TIMING_ADJUST_D                   HW_REGISTER_RW( HDMI_BASE_ADDRESS + 308 )
   #define HDMI_DETECTED_HORZA                        HW_REGISTER_RW( HDMI_BASE_ADDRESS + 312 )
   #define HDMI_DETECTED_HORZB                        HW_REGISTER_RW( HDMI_BASE_ADDRESS + 316 )
   #define HDMI_DETECTED_VERTA0                       HW_REGISTER_RW( HDMI_BASE_ADDRESS + 320 )
   #define HDMI_DETECTED_VERTB0                       HW_REGISTER_RW( HDMI_BASE_ADDRESS + 324 )
   #define HDMI_DETECTED_VERTA1                       HW_REGISTER_RW( HDMI_BASE_ADDRESS + 328 )
   #define HDMI_DETECTED_VERTB1                       HW_REGISTER_RW( HDMI_BASE_ADDRESS + 332 )
   #define HDMI_HDMI_13_AUDIO_CFG_1                   HW_REGISTER_RW( HDMI_BASE_ADDRESS + 336 )
   #define HDMI_HDMI_13_AUDIO_STATUS_1                HW_REGISTER_RW( HDMI_BASE_ADDRESS + 340 )
   #define HDMI_HDMI_HBR_AUDIO_PACKET_HEADER          HW_REGISTER_RW( HDMI_BASE_ADDRESS + 344 )


   #define HDMI_TX_PHY_BASE                           (HDMI_BASE_ADDRESS + 0x2c0)

   #define HDMI_TX_PHY_HDMI_TX_PHY_RESET_CTL          HW_REGISTER_RW( HDMI_TX_PHY_BASE + 0  )
   #define HDMI_TX_PHY_HDMI_TX_PHY_CTL_0              HW_REGISTER_RW( HDMI_TX_PHY_BASE + 4  )
   #define HDMI_TX_PHY_HDMI_TX_PHY_CTL_1              HW_REGISTER_RW( HDMI_TX_PHY_BASE + 8  )
   #define HDMI_TX_PHY_HDMI_TX_PHY_CTL_2              HW_REGISTER_RW( HDMI_TX_PHY_BASE + 12 )
   #define HDMI_TX_PHY_HDMI_TX_PHY_PLL_CFG            HW_REGISTER_RW( HDMI_TX_PHY_BASE + 16 )
   #define HDMI_TX_PHY_HDMI65_TX_PHY_TMDS_CFG         HW_REGISTER_RW( HDMI_TX_PHY_BASE + 20 )
   #define HDMI_TX_PHY_HDMI_TX_PHY_STATUS             HW_REGISTER_RW( HDMI_TX_PHY_BASE + 24 )
   #define HDMI_TX_PHY_SPREAD_SPECTRUM                HW_REGISTER_RW( HDMI_TX_PHY_BASE + 28 )
   #define HDMI_TX_PHY_HDMI_TX_PHY_SPARE              HW_REGISTER_RW( HDMI_TX_PHY_BASE + 32 )

   #define HDMI_ASYNC_RM_BASE                         (HDMI_BASE_ADDRESS + 0x300)

   #define HDMI_ASYNC_RM_CONTROL                      HW_REGISTER_RW( HDMI_ASYNC_RM_BASE + 0  )
   #define HDMI_ASYNC_RM_RATE_RATIO                   HW_REGISTER_RW( HDMI_ASYNC_RM_BASE + 4  )
   #define HDMI_ASYNC_RM_SAMPLE_INC                   HW_REGISTER_RW( HDMI_ASYNC_RM_BASE + 8  )
   #define HDMI_ASYNC_RM_PHASE_INC                    HW_REGISTER_RW( HDMI_ASYNC_RM_BASE + 12 )
   #define HDMI_ASYNC_RM_INTEGRATOR                   HW_REGISTER_RW( HDMI_ASYNC_RM_BASE + 16 )
   #define HDMI_ASYNC_RM_OFFSET                       HW_REGISTER_RW( HDMI_ASYNC_RM_BASE + 20 )
   #define HDMI_ASYNC_RM_FORMAT                       HW_REGISTER_RW( HDMI_ASYNC_RM_BASE + 24 )

   #define HDMI_INTR2_BASE                            (HDMI_BASE_ADDRESS + 0x340)

   #define  HDMI_CPU_STATUS                           HW_REGISTER_RW( HDMI_INTR2_BASE + 0x00 )
   #define HDMI_CPU_SET                               HW_REGISTER_RW( HDMI_INTR2_BASE + 0x04 )
   #define HDMI_CPU_CLEAR                             HW_REGISTER_RW( HDMI_INTR2_BASE + 0x08 )
   #define  HDMI_CPU_MASK_STATUS                      HW_REGISTER_RW( HDMI_INTR2_BASE + 0x0c )
   #define HDMI_CPU_MASK_SET                          HW_REGISTER_RW( HDMI_INTR2_BASE + 0x10 )
   #define HDMI_CPU_MASK_CLEAR                        HW_REGISTER_RW( HDMI_INTR2_BASE + 0x14 )
   #define  HDMI_PCI_STATUS                           HW_REGISTER_RW( HDMI_INTR2_BASE + 0x18 )
   #define HDMI_PCI_SET                               HW_REGISTER_RW( HDMI_INTR2_BASE + 0x1c )
   #define HDMI_PCI_CLEAR                             HW_REGISTER_RW( HDMI_INTR2_BASE + 0x20 )
   #define  HDMI_PCI_MASK_STATUS                      HW_REGISTER_RW( HDMI_INTR2_BASE + 0x24 )
   #define HDMI_PCI_MASK_SET                          HW_REGISTER_RW( HDMI_INTR2_BASE + 0x28 )
   #define  HDMI_PCI_MASK_CLEAR                       HW_REGISTER_RW( HDMI_INTR2_BASE + 0x2c )

   #define HDMI_RAM_BASE                              (HDMI_BASE_ADDRESS + 0x400)

   #define HDMI_RAM_GCP_0                             HW_REGISTER_RW( HDMI_RAM_BASE + 0   )
   #define HDMI_RAM_GCP_1                             HW_REGISTER_RW( HDMI_RAM_BASE + 4   )
   #define HDMI_RAM_GCP_2                             HW_REGISTER_RW( HDMI_RAM_BASE + 8   )
   #define HDMI_RAM_GCP_3                             HW_REGISTER_RW( HDMI_RAM_BASE + 12  )
   #define HDMI_RAM_GCP_4                             HW_REGISTER_RW( HDMI_RAM_BASE + 16  )
   #define HDMI_RAM_GCP_5                             HW_REGISTER_RW( HDMI_RAM_BASE + 20  )
   #define HDMI_RAM_GCP_6                             HW_REGISTER_RW( HDMI_RAM_BASE + 24  )
   #define HDMI_RAM_GCP_7                             HW_REGISTER_RW( HDMI_RAM_BASE + 28  )
   #define HDMI_RAM_GCP_8                             HW_REGISTER_RW( HDMI_RAM_BASE + 32  )
   #define HDMI_RAM_PACKET_1_0                        HW_REGISTER_RW( HDMI_RAM_BASE + 36  )
   #define HDMI_RAM_PACKET_1_1                        HW_REGISTER_RW( HDMI_RAM_BASE + 40  )
   #define HDMI_RAM_PACKET_1_2                        HW_REGISTER_RW( HDMI_RAM_BASE + 44  )
   #define HDMI_RAM_PACKET_1_3                        HW_REGISTER_RW( HDMI_RAM_BASE + 48  )
   #define HDMI_RAM_PACKET_1_4                        HW_REGISTER_RW( HDMI_RAM_BASE + 52  )
   #define HDMI_RAM_PACKET_1_5                        HW_REGISTER_RW( HDMI_RAM_BASE + 56  )
   #define HDMI_RAM_PACKET_1_6                        HW_REGISTER_RW( HDMI_RAM_BASE + 60  )
   #define HDMI_RAM_PACKET_1_7                        HW_REGISTER_RW( HDMI_RAM_BASE + 64  )
   #define HDMI_RAM_PACKET_1_8                        HW_REGISTER_RW( HDMI_RAM_BASE + 68  )
   #define HDMI_RAM_PACKET_2_0                        HW_REGISTER_RW( HDMI_RAM_BASE + 72  )
   #define HDMI_RAM_PACKET_2_1                        HW_REGISTER_RW( HDMI_RAM_BASE + 76  )
   #define HDMI_RAM_PACKET_2_2                        HW_REGISTER_RW( HDMI_RAM_BASE + 80  )
   #define HDMI_RAM_PACKET_2_3                        HW_REGISTER_RW( HDMI_RAM_BASE + 84  )
   #define HDMI_RAM_PACKET_2_4                        HW_REGISTER_RW( HDMI_RAM_BASE + 88  )
   #define HDMI_RAM_PACKET_2_5                        HW_REGISTER_RW( HDMI_RAM_BASE + 92  )
   #define HDMI_RAM_PACKET_2_6                        HW_REGISTER_RW( HDMI_RAM_BASE + 96  )
   #define HDMI_RAM_PACKET_2_7                        HW_REGISTER_RW( HDMI_RAM_BASE + 100 )
   #define HDMI_RAM_PACKET_2_8                        HW_REGISTER_RW( HDMI_RAM_BASE + 104 )
   #define HDMI_RAM_PACKET_3_0                        HW_REGISTER_RW( HDMI_RAM_BASE + 108 )
   #define HDMI_RAM_PACKET_3_1                        HW_REGISTER_RW( HDMI_RAM_BASE + 112 )
   #define HDMI_RAM_PACKET_3_2                        HW_REGISTER_RW( HDMI_RAM_BASE + 116 )
   #define HDMI_RAM_PACKET_3_3                        HW_REGISTER_RW( HDMI_RAM_BASE + 120 )
   #define HDMI_RAM_PACKET_3_4                        HW_REGISTER_RW( HDMI_RAM_BASE + 124 )
   #define HDMI_RAM_PACKET_3_5                        HW_REGISTER_RW( HDMI_RAM_BASE + 128 )
   #define HDMI_RAM_PACKET_3_6                        HW_REGISTER_RW( HDMI_RAM_BASE + 132 )
   #define HDMI_RAM_PACKET_3_7                        HW_REGISTER_RW( HDMI_RAM_BASE + 136 )
   #define HDMI_RAM_PACKET_3_8                        HW_REGISTER_RW( HDMI_RAM_BASE + 140 )
   #define HDMI_RAM_PACKET_4_0                        HW_REGISTER_RW( HDMI_RAM_BASE + 144 )
   #define HDMI_RAM_PACKET_4_1                        HW_REGISTER_RW( HDMI_RAM_BASE + 148 )
   #define HDMI_RAM_PACKET_4_2                        HW_REGISTER_RW( HDMI_RAM_BASE + 152 )
   #define HDMI_RAM_PACKET_4_3                        HW_REGISTER_RW( HDMI_RAM_BASE + 156 )
   #define HDMI_RAM_PACKET_4_4                        HW_REGISTER_RW( HDMI_RAM_BASE + 160 )
   #define HDMI_RAM_PACKET_4_5                        HW_REGISTER_RW( HDMI_RAM_BASE + 164 )
   #define HDMI_RAM_PACKET_4_6                        HW_REGISTER_RW( HDMI_RAM_BASE + 168 )
   #define HDMI_RAM_PACKET_4_7                        HW_REGISTER_RW( HDMI_RAM_BASE + 172 )
   #define HDMI_RAM_PACKET_4_8                        HW_REGISTER_RW( HDMI_RAM_BASE + 176 )
   #define HDMI_RAM_PACKET_5_0                        HW_REGISTER_RW( HDMI_RAM_BASE + 180 )
   #define HDMI_RAM_PACKET_5_1                        HW_REGISTER_RW( HDMI_RAM_BASE + 184 )
   #define HDMI_RAM_PACKET_5_2                        HW_REGISTER_RW( HDMI_RAM_BASE + 188 )
   #define HDMI_RAM_PACKET_5_3                        HW_REGISTER_RW( HDMI_RAM_BASE + 192 )
   #define HDMI_RAM_PACKET_5_4                        HW_REGISTER_RW( HDMI_RAM_BASE + 196 )
   #define HDMI_RAM_PACKET_5_5                        HW_REGISTER_RW( HDMI_RAM_BASE + 200 )
   #define HDMI_RAM_PACKET_5_6                        HW_REGISTER_RW( HDMI_RAM_BASE + 204 )
   #define HDMI_RAM_PACKET_5_7                        HW_REGISTER_RW( HDMI_RAM_BASE + 208 )
   #define HDMI_RAM_PACKET_5_8                        HW_REGISTER_RW( HDMI_RAM_BASE + 212 )
   #define HDMI_RAM_PACKET_6_0                        HW_REGISTER_RW( HDMI_RAM_BASE + 216 )
   #define HDMI_RAM_PACKET_6_1                        HW_REGISTER_RW( HDMI_RAM_BASE + 220 )
   #define HDMI_RAM_PACKET_6_2                        HW_REGISTER_RW( HDMI_RAM_BASE + 224 )
   #define HDMI_RAM_PACKET_6_3                        HW_REGISTER_RW( HDMI_RAM_BASE + 228 )
   #define HDMI_RAM_PACKET_6_4                        HW_REGISTER_RW( HDMI_RAM_BASE + 232 )
   #define HDMI_RAM_PACKET_6_5                        HW_REGISTER_RW( HDMI_RAM_BASE + 236 )
   #define HDMI_RAM_PACKET_6_6                        HW_REGISTER_RW( HDMI_RAM_BASE + 240 )
   #define HDMI_RAM_PACKET_6_7                        HW_REGISTER_RW( HDMI_RAM_BASE + 244 )
   #define HDMI_RAM_PACKET_6_8                        HW_REGISTER_RW( HDMI_RAM_BASE + 248 )
   #define HDMI_RAM_PACKET_7_0                        HW_REGISTER_RW( HDMI_RAM_BASE + 252 )
   #define HDMI_RAM_PACKET_7_1                        HW_REGISTER_RW( HDMI_RAM_BASE + 256 )
   #define HDMI_RAM_PACKET_7_2                        HW_REGISTER_RW( HDMI_RAM_BASE + 260 )
   #define HDMI_RAM_PACKET_7_3                        HW_REGISTER_RW( HDMI_RAM_BASE + 264 )
   #define HDMI_RAM_PACKET_7_4                        HW_REGISTER_RW( HDMI_RAM_BASE + 268 )
   #define HDMI_RAM_PACKET_7_5                        HW_REGISTER_RW( HDMI_RAM_BASE + 272 )
   #define HDMI_RAM_PACKET_7_6                        HW_REGISTER_RW( HDMI_RAM_BASE + 276 )
   #define HDMI_RAM_PACKET_7_7                        HW_REGISTER_RW( HDMI_RAM_BASE + 280 )
   #define HDMI_RAM_PACKET_7_8                        HW_REGISTER_RW( HDMI_RAM_BASE + 284 )
   #define HDMI_RAM_PACKET_8_0                        HW_REGISTER_RW( HDMI_RAM_BASE + 288 )
   #define HDMI_RAM_PACKET_8_1                        HW_REGISTER_RW( HDMI_RAM_BASE + 292 )
   #define HDMI_RAM_PACKET_8_2                        HW_REGISTER_RW( HDMI_RAM_BASE + 296 )
   #define HDMI_RAM_PACKET_8_3                        HW_REGISTER_RW( HDMI_RAM_BASE + 300 )
   #define HDMI_RAM_PACKET_8_4                        HW_REGISTER_RW( HDMI_RAM_BASE + 304 )
   #define HDMI_RAM_PACKET_8_5                        HW_REGISTER_RW( HDMI_RAM_BASE + 308 )
   #define HDMI_RAM_PACKET_8_6                        HW_REGISTER_RW( HDMI_RAM_BASE + 312 )
   #define HDMI_RAM_PACKET_8_7                        HW_REGISTER_RW( HDMI_RAM_BASE + 316 )
   #define HDMI_RAM_PACKET_8_8                        HW_REGISTER_RW( HDMI_RAM_BASE + 320 )
   #define HDMI_RAM_PACKET_9_0                        HW_REGISTER_RW( HDMI_RAM_BASE + 324 )
   #define HDMI_RAM_PACKET_9_1                        HW_REGISTER_RW( HDMI_RAM_BASE + 328 )
   #define HDMI_RAM_PACKET_9_2                        HW_REGISTER_RW( HDMI_RAM_BASE + 332 )
   #define HDMI_RAM_PACKET_9_3                        HW_REGISTER_RW( HDMI_RAM_BASE + 336 )
   #define HDMI_RAM_PACKET_9_4                        HW_REGISTER_RW( HDMI_RAM_BASE + 340 )
   #define HDMI_RAM_PACKET_9_5                        HW_REGISTER_RW( HDMI_RAM_BASE + 344 )
   #define HDMI_RAM_PACKET_9_6                        HW_REGISTER_RW( HDMI_RAM_BASE + 348 )
   #define HDMI_RAM_PACKET_9_7                        HW_REGISTER_RW( HDMI_RAM_BASE + 352 )
   #define HDMI_RAM_PACKET_9_8                        HW_REGISTER_RW( HDMI_RAM_BASE + 356 )
   #define HDMI_RAM_PACKET_10_0                       HW_REGISTER_RW( HDMI_RAM_BASE + 360 )
   #define HDMI_RAM_PACKET_10_1                       HW_REGISTER_RW( HDMI_RAM_BASE + 364 )
   #define HDMI_RAM_PACKET_10_2                       HW_REGISTER_RW( HDMI_RAM_BASE + 368 )
   #define HDMI_RAM_PACKET_10_3                       HW_REGISTER_RW( HDMI_RAM_BASE + 372 )
   #define HDMI_RAM_PACKET_10_4                       HW_REGISTER_RW( HDMI_RAM_BASE + 376 )
   #define HDMI_RAM_PACKET_10_5                       HW_REGISTER_RW( HDMI_RAM_BASE + 380 )
   #define HDMI_RAM_PACKET_10_6                       HW_REGISTER_RW( HDMI_RAM_BASE + 384 )
   #define HDMI_RAM_PACKET_10_7                       HW_REGISTER_RW( HDMI_RAM_BASE + 388 )
   #define HDMI_RAM_PACKET_10_8                       HW_REGISTER_RW( HDMI_RAM_BASE + 392 )
   #define HDMI_RAM_PACKET_11_0                       HW_REGISTER_RW( HDMI_RAM_BASE + 396 )
   #define HDMI_RAM_PACKET_11_1                       HW_REGISTER_RW( HDMI_RAM_BASE + 400 )
   #define HDMI_RAM_PACKET_11_2                       HW_REGISTER_RW( HDMI_RAM_BASE + 404 )
   #define HDMI_RAM_PACKET_11_3                       HW_REGISTER_RW( HDMI_RAM_BASE + 408 )
   #define HDMI_RAM_PACKET_11_4                       HW_REGISTER_RW( HDMI_RAM_BASE + 412 )
   #define HDMI_RAM_PACKET_11_5                       HW_REGISTER_RW( HDMI_RAM_BASE + 416 )
   #define HDMI_RAM_PACKET_11_6                       HW_REGISTER_RW( HDMI_RAM_BASE + 420 )
   #define HDMI_RAM_PACKET_11_7                       HW_REGISTER_RW( HDMI_RAM_BASE + 424 )
   #define HDMI_RAM_PACKET_11_8                       HW_REGISTER_RW( HDMI_RAM_BASE + 428 )
   #define HDMI_RAM_PACKET_12_0                       HW_REGISTER_RW( HDMI_RAM_BASE + 432 )
   #define HDMI_RAM_PACKET_12_1                       HW_REGISTER_RW( HDMI_RAM_BASE + 436 )
   #define HDMI_RAM_PACKET_12_2                       HW_REGISTER_RW( HDMI_RAM_BASE + 440 )
   #define HDMI_RAM_PACKET_12_3                       HW_REGISTER_RW( HDMI_RAM_BASE + 444 )
   #define HDMI_RAM_PACKET_12_4                       HW_REGISTER_RW( HDMI_RAM_BASE + 448 )
   #define HDMI_RAM_PACKET_12_5                       HW_REGISTER_RW( HDMI_RAM_BASE + 452 )
   #define HDMI_RAM_PACKET_12_6                       HW_REGISTER_RW( HDMI_RAM_BASE + 456 )
   #define HDMI_RAM_PACKET_12_7                       HW_REGISTER_RW( HDMI_RAM_BASE + 460 )
   #define HDMI_RAM_PACKET_12_8                       HW_REGISTER_RW( HDMI_RAM_BASE + 464 )
   #define HDMI_RAM_PACKET_13_0                       HW_REGISTER_RW( HDMI_RAM_BASE + 468 )
   #define HDMI_RAM_PACKET_13_1                       HW_REGISTER_RW( HDMI_RAM_BASE + 472 )
   #define HDMI_RAM_PACKET_13_2                       HW_REGISTER_RW( HDMI_RAM_BASE + 476 )
   #define HDMI_RAM_PACKET_13_3                       HW_REGISTER_RW( HDMI_RAM_BASE + 480 )
   #define HDMI_RAM_PACKET_13_4                       HW_REGISTER_RW( HDMI_RAM_BASE + 484 )
   #define HDMI_RAM_PACKET_13_5                       HW_REGISTER_RW( HDMI_RAM_BASE + 488 )
   #define HDMI_RAM_PACKET_13_6                       HW_REGISTER_RW( HDMI_RAM_BASE + 492 )
   #define HDMI_RAM_PACKET_13_7                       HW_REGISTER_RW( HDMI_RAM_BASE + 496 )
   #define HDMI_RAM_PACKET_13_8                       HW_REGISTER_RW( HDMI_RAM_BASE + 500 )
   #define HDMI_RAM_PACKET_RSVD_0                     HW_REGISTER_RW( HDMI_RAM_BASE + 504 )
   #define HDMI_RAM_PACKET_RSVD_1                     HW_REGISTER_RW( HDMI_RAM_BASE + 508 )




   /*---------------------------------------------------------------------------*/
   /* DMA Controller */

   /* DMA Source Definitions */

   #define CAM_DMA           (0*DMA_TI_PERMAP)
   #define DISP_DMA          (0*DMA_TI_PERMAP)
   #define MS_DMA            (0*DMA_TI_PERMAP)
   #define BIT_STREAM_DMA    (0*DMA_TI_PERMAP)
   #define ACIS_DMA          (0*DMA_TI_PERMAP)
   #define SDRAM_CTRL_DMA    (0*DMA_TI_PERMAP)

   #define DSI_DMA           ( 1*DMA_TI_PERMAP)
   #define PCM_TX_DMA        ( 2*DMA_TI_PERMAP)
   #define PCM_RX_DMA        ( 3*DMA_TI_PERMAP)
   #define SMI_DMA           ( 4*DMA_TI_PERMAP)
   #define PWM_DMA           ( 5*DMA_TI_PERMAP)
   #define SPI_TX_DMA        ( 6*DMA_TI_PERMAP)
   #define SPI_RX_DMA        ( 7*DMA_TI_PERMAP)
   #define SLIM_DTX_DMA      ( 8*DMA_TI_PERMAP)
   #define SLIM_DRX_DMA      ( 9*DMA_TI_PERMAP)
   #define SLIM_CTX_DMA      (10*DMA_TI_PERMAP)
   #define SLIM_CRX_DMA      (11*DMA_TI_PERMAP)
   #define UNUSED_DMA_12     (12*DMA_TI_PERMAP)
   #define SD_HOST_DMA       (13*DMA_TI_PERMAP)
   #define UNUSED_DMA_14     (14*DMA_TI_PERMAP)
   #define ADC_DMA           (15*DMA_TI_PERMAP)
   #define DSI2_DMA          (16*DMA_TI_PERMAP)
   #define HDMI_DMA          (17*DMA_TI_PERMAP)
   // 18 unused
   #define CRYPTO_IP_DMA     (19*DMA_TI_PERMAP)
   #define CRYPTO_OP_DMA     (20*DMA_TI_PERMAP)
   #define SCALER_0_DMA      (21*DMA_TI_PERMAP)
   #define SCALER_1_DMA      (22*DMA_TI_PERMAP)
   #define SCALER_2_DMA      (23*DMA_TI_PERMAP)
   #define SMI_SCALER_0_DMA  (24*DMA_TI_PERMAP)
   #define SMI_SCALER_1_DMA  (25*DMA_TI_PERMAP)
   #define SMI_SCALER_2_DMA  (26*DMA_TI_PERMAP)
   // 27-31 unused


   /*---------------------------------------------------------------------------*/

   #define DMA_CH_BASE(n)              (DMA0_BASE+(n)*0x100)
   #define DMA_REG(n,offset)           HW_REGISTER_RW(DMA_CH_BASE(n)+(offset))
   #define DMA_INTERRUPT(n)            (INTERRUPT_DMA0+(n))


   /* DMA Register Addresses */
   #define DMA_CS(n)                   DMA_REG(n,0x00)
   #define DMA_CB_ADDR(n)              DMA_REG(n,0x04)
   #define DMA_CB_TI(n)                DMA_REG(n,0x08)
   #define DMA_CB_SA(n)                DMA_REG(n,0x0C)
   #define DMA_CB_DA(n)                DMA_REG(n,0x10)
   #define DMA_CB_TL(n)                DMA_REG(n,0x14)
   #define DMA_CB_2DSTR(n)             DMA_REG(n,0x18)
   #define DMA_CB_NEXT(n)              DMA_REG(n,0x1C)
   #define DMA_DEBUG(n)                DMA_REG(n,0x20)


/* B0 DEFINITIONS */
   /* DMA_CS - Control and status reg bit fields */
   #define DMA_CS_ACTIVE                (1<<0)
   #define DMA_CS_END                   (1<<1)
   #define DMA_CS_INT                   (1<<2)
   #define DMA_CS_DREQ                  (1<<3)
   #define DMA_CS_PAUSED                (1<<4)
   #define DMA_CS_DREQ_PAUSED           (1<<5)
   #define DMA_CS_WAITING_FOR_LAST_WRITE (1<<6)
   #define DMA_CS_ERROR                 (1<<8)
   #define DMA_CS_PRIORITY              (1<<16)
   #define DMA_CS_PANIC_PRIORITY        (1<<20)
   #define DMA_CS_WAIT_FOR_LAST_WRITE   (1<<28)
   #define DMA_CS_DIS_DBS_PAUSE         (1<<29)
   #define DMA_CS_ABORT                 (1<<30)
   #define DMA_CS_RESET                 (1<<31)

   /* DMA_CB_TI - Control Block,  Transfer Information fields */
   #define DMA_TI_INT                  (1<<0)
   #define DMA_TI_TDMODE               (1<<1)
   #define DMA_TI_WAIT_RESP            (1<<3)
   #define DMA_TI_D_INC                (1<<4)
   #define DMA_TI_D_WIDTH              (1<<5)
   #define DMA_TI_D_128                (1<<5)
   #define DMA_TI_D_32                 0
   #define DMA_TI_D_DREQ               (1<<6)
   #define DMA_TI_D_IGNORE             (1<<7)
   #define DMA_TI_S_INC                (1<<8)
   #define DMA_TI_S_WIDTH              (1<<9)
   #define DMA_TI_S_128                (1<<9)
   #define DMA_TI_S_32                 0
   #define DMA_TI_S_DREQ               (1<<10)
   #define DMA_TI_S_IGNORE             (1<<11)
   #define DMA_TI_BURST_N(n)           (((n) & 0x0f) <<12)
   #define DMA_TI_PERMAP               (1<<16)
   #define DMA_TI_PER_MAP(n)           (((n) & 0x1f) <<16)
   #define DMA_TI_WAITS(n)             (((n) & 0x1f) <<21)
   #define DMA_TI_NO_WIDE_BURSTS       (1<<26)

   /* DMA_DEBUG - Debug Register bit fields */
   #define DMA_DEBUG_VERSION           (1<<25)
   #define DMA_DEBUG_STATE             (1<<16)
   #define DMA_DEBUG_ID                (1<<8)
   #define DMA_DEBUG_OUTSTANDING_WRITES (1<<4)
   #define DMA_DEBUG_READ_ERR          (1<<2)
   #define DMA_DEBUG_FIFO_ERR          (1<<1)
   #define DMA_DEBUG_READ_LAST_ERR     (0)

   /* dma15 address is used as a highlevel enable for each DMA for clock gate purposes */
   #define DMA_ENABLE                  HW_REGISTER_RW(DMA_CH_BASE(15))







   /*---------------------------------------------------------------------------*/
   /* Startup uses  these so point them to core 0 int controller */
   #define IC_VADDR    IC0_VADDR
   #define IC_MASK0    IC0_MASK0
   
   /*---------------------------------------------------------------------------*/
   /* Interrupt Controller for core 0 */
   
   
   #define IC_0               IC0_C
   #define IS_0               IC0_S
   #define ISRC0_0            IC0_SRC0
   #define ISRC1_0            IC0_SRC1
   #define IMASK0_0           IC0_MASK0
   #define IMASK1_0           IC0_MASK1
   #define IMASK2_0           IC0_MASK2
   #define IMASK3_0           IC0_MASK3
   #define IMASK4_0           IC0_MASK4
   #define IMASK5_0           IC0_MASK5
   #define IMASK6_0           IC0_MASK6
   #define IMASK7_0           IC0_MASK7
   #define IMASKx_0( x )      IC0_MASKx( x )
   #define IVADDR_0           IC0_VADDR
   #define IWAKEUP_0          IC0_WAKEUP
   #define IPROFILE_0         IC0_PROFILE
   #define IFORCE0_0          IC0_FORCE0
   #define IFORCE1_0          IC0_FORCE1


   //we are assigning the VCII regs to core 0 for temp software compatability
   #define IC                  IC_0
   #define IS                  IS_0
   #define ISRC                ISRC0_0
   #define IMASK0              IMASK0_0
   #define IMASK1              IMASK1_0
   #define IMASK2              IMASK2_0
   #define IMASK3              IMASK3_0

   /*---------------------------------------------------------------------------*/
   /* System Timer */

   //the first core's timer block
   #define STCS_0                ST_CS
   #define STC_0                 ST_CLO
   #define STCLO_0               ST_CLO
   #define STCHI_0               ST_CHI
   #define STC0_0                ST_C0
   #define STC1_0                ST_C1
   #define STC2_0                ST_C2
   #define STC3_0                ST_C3


   //Sys timer select macro's
   #define STCS_x( x )           ( ((x)==0) ? STCS_0 : STCS_1 )
   #define STC_x( x )            ( ((x)==0) ? STC_0 : STC_1 )
   #define STCLO_x( x )          ( ((x)==0) ? STCLO_0 : STCLO_1 )
   #define STCHI_x( x )          ( ((x)==0) ? STCHI_0 : STCHI_1 )
   #define STC0_x( x )           ( ((x)==0) ? STC0_0 : STC0_1 )
   #define STC1_x( x )           ( ((x)==0) ? STC1_0 : STC1_1 )
   #define STC2_x( x )           ( ((x)==0) ? STC2_0 : STC2_1 )
   #define STC3_x( x )           ( ((x)==0) ? STC3_0 : STC3_1 )

   //Temp - define the old register names to look like timer_0 on core 0
   #define STCS                  STCS_0
   #define STC                   STC_0
   #define STCLO                 STCLO_0
   #define STCHI                 STCHI_0
   #define STC0                  STC0_0
   #define STC1                  STC1_0
   #define STC2                  STC2_0
   #define STC3                  STC3_0

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
   #define GPFSEL0             HW_REGISTER_RW(GP_BASE + 0x00)
   #define GPFSEL1             HW_REGISTER_RW(GP_BASE + 0x04)
   #define GPFSEL2             HW_REGISTER_RW(GP_BASE + 0x08)
   #define GPFSEL3             HW_REGISTER_RW(GP_BASE + 0x0c)
   #define GPFSEL4             HW_REGISTER_RW(GP_BASE + 0x10)
   #define GPFSEL5             HW_REGISTER_RW(GP_BASE + 0x14)
   #define GPFSEL6             HW_REGISTER_RW(GP_BASE + 0x18)

   #define GPSET0              HW_REGISTER_RW(GP_BASE + 0x1C)
   #define GPSET1              HW_REGISTER_RW(GP_BASE + 0x20)
   #define GPSET2              HW_REGISTER_RW(GP_BASE + 0x24)

   #define GPCLR0              HW_REGISTER_RW(GP_BASE + 0x28)
   #define GPCLR1              HW_REGISTER_RW(GP_BASE + 0x2C)
   #define GPCLR2              HW_REGISTER_RW(GP_BASE + 0x30)

   #define GPLEV0              HW_REGISTER_RW(GP_BASE + 0x34)
   #define GPLEV1              HW_REGISTER_RW(GP_BASE + 0x38)
   #define GPLEV2              HW_REGISTER_RW(GP_BASE + 0x3C)

   #define GPEDS0              HW_REGISTER_RW(GP_BASE + 0x40)
   #define GPEDS1              HW_REGISTER_RW(GP_BASE + 0x44)
   #define GPEDS2              HW_REGISTER_RW(GP_BASE + 0x48)

   #define GPREN0              HW_REGISTER_RW(GP_BASE + 0x4C)
   #define GPREN1              HW_REGISTER_RW(GP_BASE + 0x50)
   #define GPREN2              HW_REGISTER_RW(GP_BASE + 0x54)

   #define GPFEN0              HW_REGISTER_RW(GP_BASE + 0x58)
   #define GPFEN1              HW_REGISTER_RW(GP_BASE + 0x5C)
   #define GPFEN2              HW_REGISTER_RW(GP_BASE + 0x60)

   #define GPHEN0              HW_REGISTER_RW(GP_BASE + 0x64)
   #define GPHEN1              HW_REGISTER_RW(GP_BASE + 0x68)
   #define GPHEN2              HW_REGISTER_RW(GP_BASE + 0x6C)

   #define GPLEN0              HW_REGISTER_RW(GP_BASE + 0x70)
   #define GPLEN1              HW_REGISTER_RW(GP_BASE + 0x74)
   #define GPLEN2              HW_REGISTER_RW(GP_BASE + 0x78)

   #define GPAREN0             HW_REGISTER_RW(GP_BASE + 0x7C)
   #define GPAREN1             HW_REGISTER_RW(GP_BASE + 0x80)
   #define GPAREN2             HW_REGISTER_RW(GP_BASE + 0x84)

   #define GPAFEN0             HW_REGISTER_RW(GP_BASE + 0x88)
   #define GPAFEN1             HW_REGISTER_RW(GP_BASE + 0x8C)
   #define GPAFEN2             HW_REGISTER_RW(GP_BASE + 0x90)

   #define GPPUD               HW_REGISTER_RW(GP_BASE + 0x94)
   #define GPPUDCLK0           HW_REGISTER_RW(GP_BASE + 0x98)
   #define GPPUDCLK1           HW_REGISTER_RW(GP_BASE + 0x9C)
   #define GPPUDCLK2           HW_REGISTER_RW(GP_BASE + 0xA0)

   /*---------------------------------------------------------------------------*/
   /* JPEG block */
   #define JCTRL               HW_REGISTER_RW(JP_BASE +    0)
   #define JICST               HW_REGISTER_RW(JP_BASE +  0x4)
   #define JMCTRL              HW_REGISTER_RW(JP_BASE +  0x8)
   #define JDCCTRL             HW_REGISTER_RW(JP_BASE + 0x0C)
   #define JCBA                HW_REGISTER_RW(JP_BASE + 0x10)
   #define JNCB                HW_REGISTER_RW(JP_BASE + 0x14)
   #define JSDA                HW_REGISTER_RW(JP_BASE + 0x18)
   #define JNSB                HW_REGISTER_RW(JP_BASE + 0x1C)
   #define JSBO                HW_REGISTER_RW(JP_BASE + 0x20)
   #define JMOP                HW_REGISTER_RW(JP_BASE + 0x24)
   #define JHADDR              HW_REGISTER_RW(JP_BASE + 0x28)
   #define JHWDATA             HW_REGISTER_RW(JP_BASE + 0x2C)
   #define JMADDR              HW_REGISTER_RW(JP_BASE + 0x30)
   #define JMWDATA             HW_REGISTER_RW(JP_BASE + 0x34)
   #define JOADDR              HW_REGISTER_RW(JP_BASE + 0x38)
   #define JOWDATA             HW_REGISTER_RW(JP_BASE + 0x3C)

   #define JCTRL_START         (1 << 7)
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

   #define JMCTRL_UNUSED_BITS  ((1 << 15) | (1 << 14) | (1 << 13) | (1 << 12) | (1 << 11))

   #define AC_HUFFTABLE_OFFSET(t) ((t) * 0x100)
   #define DC_HUFFTABLE_OFFSET(t) ((t) * 0x10 + 0x200)

   #define AC_OSETTABLE_OFFSET(t) ((t) * 0x10)
   #define DC_OSETTABLE_OFFSET(t) ((t) * 0x10 + 0x20)

   #define AC_MAXCTABLE_OFFSET(t) ((t) * 0x10)
   #define DC_MAXCTABLE_OFFSET(t) ((t) * 0x10 + 0x20)

   /*---------------------------------------------------------------------------*/
   /* External Memory Interface */
   #define SDCS                   SD_CS
   #define SDSA                   SD_SA
   #define SDSB                   SD_SB
   #define SDSC                   SD_SC
   #define SDEM                   SD_EM
   #define SDPT                   SD_PT
   #define SDIDL                  SD_IDL
   #define SDRTC                  SD_RTC
   #define SDWTC                  SD_WTC
   #define SDRDC                  SD_RDC
   #define SDWDC                  SD_WDC
   #define SDRAC                  SD_RAC
   #define SDCYC                  SD_CYC
   #define SDACC                  SD_ACC
   #define SDDAT                  SD_DAT
   #define SDSECSRT0              SD_SECSRT0
   #define SDSECEND0              SD_SECEND0
   #define SDSECSRT1              SD_SECSRT1
   #define SDSECEND1              SD_SECEND1
   #define SDSECSRT2              SD_SECSRT2
   #define SDSECEND2              SD_SECEND2
   #define SDSECSRT3              SD_SECSRT3
   #define SDSECEND3              SD_SECEND3
   #define SDDELC0                SD_DELC0
   #define SDDELS0                SD_DELS0
   #define SDDELC1                SD_DELC1
   #define SDDELS1                SD_DELS1
   #define SDDELC2                SD_DELC2
   #define SDDELS2                SD_DELS2
   #define SDDELC3                SD_DELC3
   #define SDDELS3                SD_DELS3
   #define SDTMC                  SD_TMC


   /*---------------------------------------------------------------------------*/
   /* SPI Master */
   #define SPICS               HW_REGISTER_RW(SPI_BASE_ADDRESS + 0x00)
   #define SPIFIFO             HW_REGISTER_RW(SPI_BASE_ADDRESS + 0x04)
   #define SPICLK              HW_REGISTER_RW(SPI_BASE_ADDRESS + 0x08)
   #define SPIDLEN             HW_REGISTER_RW(SPI_BASE_ADDRESS + 0x0C)

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

   #define SMI_FIFO_ADDRESS(device,addr)   (((((device))&0x3)<<8)|((addr)&0xff))

   /*---------------------------------------------------------------------------*/
   /* TE Controller */

   #define TE0C                HW_REGISTER_RW(TECTL_BASE_ADDRESS + 0x00)
   #define TE1C                HW_REGISTER_RW(TECTL_BASE_ADDRESS + 0x04)
   #define TE0_VSWIDTH         HW_REGISTER_RW(TECTL_BASE_ADDRESS + 0x08)
   #define TE1_VSWIDTH         HW_REGISTER_RW(TECTL_BASE_ADDRESS + 0x0C)

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
   /* SD card host */
   #define SDCMD               HW_REGISTER_RW(SDCARD_BASE + 0x00)
   #define SDARG               HW_REGISTER_RW(SDCARD_BASE + 0x04)
   #define SDTOUT              HW_REGISTER_RW(SDCARD_BASE + 0x08)
   #define SDCDIV              HW_REGISTER_RW(SDCARD_BASE + 0x0C)
   #define SDRSP0              HW_REGISTER_RW(SDCARD_BASE + 0x10)
   #define SDRSP1              HW_REGISTER_RW(SDCARD_BASE + 0x14)
   #define SDRSP2              HW_REGISTER_RW(SDCARD_BASE + 0x18)
   #define SDRSP3              HW_REGISTER_RW(SDCARD_BASE + 0x1C)
   #define SDHSTS              HW_REGISTER_RW(SDCARD_BASE + 0x20)
   #define SDVDD               HW_REGISTER_RW(SDCARD_BASE + 0x30)
   #define SDEDM               HW_REGISTER_RW(SDCARD_BASE + 0x34)
   #define SDHCFG              HW_REGISTER_RW(SDCARD_BASE + 0x38)
   #define SDHBCT              HW_REGISTER_RW(SDCARD_BASE + 0x3C)
   #define SDDATA              HW_REGISTER_RW(SDCARD_BASE + 0x40)
   #define SDHBLC              HW_REGISTER_RW(SDCARD_BASE + 0x50)

   /*---------------------------------------------------------------------------*/
   /* SLIMbus interface */
   #ifdef __BCM2707_A0__
   #define SLIM_CON            HW_REGISTER_RW(SLIMBUS_BASE + 0x000)
   #define SLIM_STAT           HW_REGISTER_RW(SLIMBUS_BASE + 0x004)
   #define SLIM_FS             HW_REGISTER_RW(SLIMBUS_BASE + 0x008)
   #define SLIM_DMA_SMC_RX     HW_REGISTER_RW(SLIMBUS_BASE + 0x010)
   #define SLIM_DMA_SMC_TX     HW_REGISTER_RW(SLIMBUS_BASE + 0x014)
   #define SLIM_DMA_DC_RX      HW_REGISTER_RW(SLIMBUS_BASE + 0x018)
   #define SLIM_DMA_DC_TX      HW_REGISTER_RW(SLIMBUS_BASE + 0x01C)
   #define SLIM_SMC_IN_CON     HW_REGISTER_RW(SLIMBUS_BASE + 0x100)
   #define SLIM_SMC_IN_STAT    HW_REGISTER_RW(SLIMBUS_BASE + 0x104)
   #define SLIM_SMC_OUT_CON    HW_REGISTER_RW(SLIMBUS_BASE + 0x120)
   #define SLIM_SMC_OUT_STAT   HW_REGISTER_RW(SLIMBUS_BASE + 0x124)
   #define SLIM_DCC0_PA0       HW_REGISTER_RW(SLIMBUS_BASE + 0x200)
   #define SLIM_DCC0_PA1       HW_REGISTER_RW(SLIMBUS_BASE + 0x204)
   #define SLIM_DCC0_CON       HW_REGISTER_RW(SLIMBUS_BASE + 0x208)
   #define SLIM_DCC0_STAT      HW_REGISTER_RW(SLIMBUS_BASE + 0x20c)
   #define SLIM_DCC1_PA0       HW_REGISTER_RW(SLIMBUS_BASE + 0x220)
   #define SLIM_DCC1_PA1       HW_REGISTER_RW(SLIMBUS_BASE + 0x224)
   #define SLIM_DCC1_CON       HW_REGISTER_RW(SLIMBUS_BASE + 0x228)
   #define SLIM_DCC1_STAT      HW_REGISTER_RW(SLIMBUS_BASE + 0x22c)
   #define SLIM_DCC2_PA0       HW_REGISTER_RW(SLIMBUS_BASE + 0x240)
   #define SLIM_DCC2_PA1       HW_REGISTER_RW(SLIMBUS_BASE + 0x244)
   #define SLIM_DCC2_CON       HW_REGISTER_RW(SLIMBUS_BASE + 0x248)
   #define SLIM_DCC2_STAT      HW_REGISTER_RW(SLIMBUS_BASE + 0x24c)
   #define SLIM_DCC3_PA0       HW_REGISTER_RW(SLIMBUS_BASE + 0x260)
   #define SLIM_DCC3_PA1       HW_REGISTER_RW(SLIMBUS_BASE + 0x264)
   #define SLIM_DCC3_CON       HW_REGISTER_RW(SLIMBUS_BASE + 0x268)
   #define SLIM_DCC3_STAT      HW_REGISTER_RW(SLIMBUS_BASE + 0x26c)
   #define SLIM_DCC4_PA0       HW_REGISTER_RW(SLIMBUS_BASE + 0x280)
   #define SLIM_DCC4_PA1       HW_REGISTER_RW(SLIMBUS_BASE + 0x284)
   #define SLIM_DCC4_CON       HW_REGISTER_RW(SLIMBUS_BASE + 0x288)
   #define SLIM_DCC4_STAT      HW_REGISTER_RW(SLIMBUS_BASE + 0x28c)
   #define SLIM_DCC5_PA0       HW_REGISTER_RW(SLIMBUS_BASE + 0x2a0)
   #define SLIM_DCC5_PA1       HW_REGISTER_RW(SLIMBUS_BASE + 0x2a4)
   #define SLIM_DCC5_CON       HW_REGISTER_RW(SLIMBUS_BASE + 0x2a8)
   #define SLIM_DCC5_STAT      HW_REGISTER_RW(SLIMBUS_BASE + 0x2ac)
   #define SLIM_DCC6_PA0       HW_REGISTER_RW(SLIMBUS_BASE + 0x2c0)
   #define SLIM_DCC6_PA1       HW_REGISTER_RW(SLIMBUS_BASE + 0x2c4)
   #define SLIM_DCC6_CON       HW_REGISTER_RW(SLIMBUS_BASE + 0x2c8)
   #define SLIM_DCC6_STAT      HW_REGISTER_RW(SLIMBUS_BASE + 0x2cc)
   #define SLIM_DCC7_PA0       HW_REGISTER_RW(SLIMBUS_BASE + 0x2e0)
   #define SLIM_DCC7_PA1       HW_REGISTER_RW(SLIMBUS_BASE + 0x2e4)
   #define SLIM_DCC7_CON       HW_REGISTER_RW(SLIMBUS_BASE + 0x2e8)
   #define SLIM_DCC7_STAT      HW_REGISTER_RW(SLIMBUS_BASE + 0x2ec)
   #define SLIM_DCC8_PA0       HW_REGISTER_RW(SLIMBUS_BASE + 0x300)
   #define SLIM_DCC8_PA1       HW_REGISTER_RW(SLIMBUS_BASE + 0x304)
   #define SLIM_DCC8_CON       HW_REGISTER_RW(SLIMBUS_BASE + 0x308)
   #define SLIM_DCC8_STAT      HW_REGISTER_RW(SLIMBUS_BASE + 0x30c)
   #define SLIM_DCC9_PA0       HW_REGISTER_RW(SLIMBUS_BASE + 0x320)
   #define SLIM_DCC9_PA1       HW_REGISTER_RW(SLIMBUS_BASE + 0x324)
   #define SLIM_DCC9_CON       HW_REGISTER_RW(SLIMBUS_BASE + 0x328)
   #define SLIM_DCC9_STAT      HW_REGISTER_RW(SLIMBUS_BASE + 0x32c)
   #endif
   /*---------------------------------------------------------------------------*/
   /* PCM Controller */
   #define PCMCS               HW_REGISTER_RW( PCM_BASE_ADDRESS + 0x00 )
   #define PCMFIFO             HW_REGISTER_RW( PCM_BASE_ADDRESS + 0x04 )
   #define PCMMODE             HW_REGISTER_RW( PCM_BASE_ADDRESS + 0x08 )
   #define PCMRXC              HW_REGISTER_RW( PCM_BASE_ADDRESS + 0x0C )
   #define PCMTXC              HW_REGISTER_RW( PCM_BASE_ADDRESS + 0x10 )
   #define PCMDREQ             HW_REGISTER_RW( PCM_BASE_ADDRESS + 0x14 )

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

   #define I2CC_3              HW_REGISTER_RW(I2C_BASE_3 + 0x00)
   #define I2CS_3              HW_REGISTER_RW(I2C_BASE_3 + 0x04)
   #define I2CDLEN_3           HW_REGISTER_RW(I2C_BASE_3 + 0x08)
   #define I2CA_3              HW_REGISTER_RW(I2C_BASE_3 + 0x0C)
   #define I2CFIFO_3           HW_REGISTER_RW(I2C_BASE_3 + 0x10)
   #define I2CDIV_3            HW_REGISTER_RW(I2C_BASE_3 + 0x14)
   #define I2CDEL_3            HW_REGISTER_RW(I2C_BASE_3 + 0x18)
   #define I2CCLKT_3           HW_REGISTER_RW(I2C_BASE_3 + 0x1C)

   #define I2CC_x( x )         HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x00 )
   #define I2CS_x( x )         HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x04 )
   #define I2CDLEN_x( x )      HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x08 )
   #define I2CA_x( x )         HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x0C )
   #define I2CFIFO_x( x )      HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x10 )
   #define I2CDIV_x( x )       HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x14 )
   #define I2CDEL_x( x )       HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x18 )
   #define I2CCLKT_x( x )      HW_REGISTER_RW( ((0 == x) ? I2C_BASE_0 : (I2C_BASE_1 + (0x1000 * (x-1)))) + 0x1C )

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

   //---------------------------------------------------------------------------
   // RNG interrupt no longer uses Crypto local interrupt register
   // It is connected to interrupt bit 52 : ISRC1_0 bit 52-32=20 (0x0100000)
   // Mask has 6 IRQ/reg => floor(52/8)=6  Bits = 16:19

   #define CRYPTO_ISR                        ISRC1_0
   #define CRYPTO_ISR_RNG_INT                0x0100000
// FIXME dunno what this means   #define RNG_INT_MASK                      IMASK6_0
   // for RNG_INT_MASK_ENABLE/DISABLE see rnghw.h


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
   #define WOGLPTR            HW_REGISTER_RW(0x1820FFFC)

   /* Hardware 3D unit */

   #define GR_VCACHE_BASE            0x1a00a000
   #define GR_UNIFORM_BASE           0x1a00c000

   #define GR_VCACHE_ADDR_MASK       0x00001fff
   #define GR_VCACHE_SIZE            0x00002000        // in bytes

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



   //---------------------------------------------------------------
   // illegal addresses   - no hardware anymore
   // shouldnt even be in the code  FIXME  TODO
   //---------------------------------------------------------------
   #define I1CACHE_BASE                   HW_REGISTER_RW(0xffffffff)
   #define D1CACHE_BASE                   HW_REGISTER_RW(0xffffffff)
   #define INT_CTL_BASE_ADDR1             HW_REGISTER_RW(0xffffffff)
   #define INT_CTL_BASE_ADDR1             HW_REGISTER_RW(0xffffffff)
   #define RUN_ARBITER_CTRL_BASE_ADDRESS  HW_REGISTER_RW(0xffffffff)
   #define SYSTEM_TIMER_BASE1             HW_REGISTER_RW(0xffffffff)
   #define VPU1_THREAD_CTRL_BASE_ADDRESS  HW_REGISTER_RW(0xffffffff)
   #define VPU1_UNIFORM_MEM_BASE_ADDRESS  HW_REGISTER_RW(0xffffffff)
   #define IC1CS                          HW_REGISTER_RW(0xffffffff)
   #define IC1START                       HW_REGISTER_RW(0xffffffff)
   #define IC1END                         HW_REGISTER_RW(0xffffffff)
   #define DC1CS                          HW_REGISTER_RW(0xffffffff)
   #define DC1START                       HW_REGISTER_RW(0xffffffff)
   #define DC1END                         HW_REGISTER_RW(0xffffffff)
   /* Interrupt Controller for core 1 */
   #define IC_1                HW_REGISTER_RW(0xffffffff)
   #define IS_1                HW_REGISTER_RW(0xffffffff)
   #define ISRC0_1             HW_REGISTER_RW(0xffffffff)
   #define ISRC1_1             HW_REGISTER_RW(0xffffffff)
   #define IMASK0_1            HW_REGISTER_RW(0xffffffff)
   #define IMASK1_1            HW_REGISTER_RW(0xffffffff)
   #define IMASK2_1            HW_REGISTER_RW(0xffffffff)
   #define IMASK3_1            HW_REGISTER_RW(0xffffffff)
   #define IMASK4_1            HW_REGISTER_RW(0xffffffff)
   #define IMASK5_1            HW_REGISTER_RW(0xffffffff)
   #define IMASK6_1            HW_REGISTER_RW(0xffffffff)
   #define IMASK7_1            HW_REGISTER_RW(0xffffffff)
   #define IMASKx_1( x )       HW_REGISTER_RW(0xffffffff)
   #define IVADDR_1            HW_REGISTER_RW(0xffffffff)
   #define IWAKEUP_1           HW_REGISTER_RW(0xffffffff)
   #define IPROFILE_1          HW_REGISTER_RW(0xffffffff)
   #define IFORCE0_1           HW_REGISTER_RW(0xffffffff)
   #define IFORCE1_1           HW_REGISTER_RW(0xffffffff)
   //The 2nd core's timer block
   #define STCS_1                HW_REGISTER_RW(0xffffffff)
   #define STC_1                 HW_REGISTER_RO(0xffffffff)
   #define STCLO_1               HW_REGISTER_RO(0xffffffff)
   #define STCHI_1               HW_REGISTER_RO(0xffffffff)
   #define STC0_1                HW_REGISTER_RW(0xffffffff)
   #define STC1_1                HW_REGISTER_RW(0xffffffff)
   #define STC2_1                HW_REGISTER_RW(0xffffffff)
   #define STC3_1                HW_REGISTER_RW(0xffffffff)

#endif

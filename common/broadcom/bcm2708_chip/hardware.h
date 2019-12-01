
// macro definitions plus aliases to maintain some old reg names
// Commented out to reveal fake defines
//#include "register_map_macros.h"

// Necessary macros to make C code understand registers
#if defined(_ATHENA_)
   #define HW_REGISTER_RW(addr) ((addr))
#else   
   #define HW_REGISTER_RW(addr) (*(volatile unsigned long *)(addr))
#endif   
   #define HW_REGISTER_RO(addr) (*(const volatile unsigned long *)(addr))
   #define HW_POINTER_TO_ADDRESS(pointer) ((uint32_t)(void *)&(pointer))

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
   #define INTERRUPT_CCP2TX               (INTERRUPT_HW_OFFSET + 34 )
   #define INTERRUPT_SDC                  (INTERRUPT_HW_OFFSET + 35 )
   #define INTERRUPT_DSI0                 (INTERRUPT_HW_OFFSET + 36 )
   #define INTERRUPT_SPARE2               (INTERRUPT_HW_OFFSET + 37 )
   #define INTERRUPT_CAM0                 (INTERRUPT_HW_OFFSET + 38 )
   #define INTERRUPT_CAM1                 (INTERRUPT_HW_OFFSET + 39 )
   #define INTERRUPT_HDMI0                (INTERRUPT_HW_OFFSET + 40 )
   #define INTERRUPT_HDMI1                (INTERRUPT_HW_OFFSET + 41 )
   #define INTERRUPT_PIXELVALVE1          (INTERRUPT_HW_OFFSET + 42 )
   #define INTERRUPT_SPARE3               (INTERRUPT_HW_OFFSET + 43 )
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
   #define INTERRUPT_SPARE4               (INTERRUPT_HW_OFFSET + 62 )
   #define INTERRUPT_SPARE5               (INTERRUPT_HW_OFFSET + 63 )
   #define INTERRUPT_DUMMY                (INTERRUPT_HW_OFFSET + 63 )
   #define ISRC0_0                        IC0_SRC0
   #define ISRC0_1                        IC1_SRC0
   #define ISRC1_0                        IC0_SRC1
   #define ISRC1_1                        IC1_SRC1



/*---------------------------------------------------------------------------*/

// auto generated regestermap
#include "register_map.h"

 /*---------------------------------------------------------------------------*/
   /* DMA Source Definitions */

   // port 0 is wired as permanently on inside the DMA
   #define CAM_DMA           (0<<DMA0_TI_PERMAP_LSB)
   #define DISP_DMA          (0<<DMA0_TI_PERMAP_LSB)
   #define MS_DMA            (0<<DMA0_TI_PERMAP_LSB)
   #define BIT_STREAM_DMA    (0<<DMA0_TI_PERMAP_LSB)
   #define ACIS_DMA          (0<<DMA0_TI_PERMAP_LSB)
   #define SDRAM_CTRL_DMA    (0<<DMA0_TI_PERMAP_LSB)

   #define DSI_DMA           ( 1<<DMA0_TI_PERMAP_LSB)
   #define PCM_TX_DMA        ( 2<<DMA0_TI_PERMAP_LSB)
   #define PCM_RX_DMA        ( 3<<DMA0_TI_PERMAP_LSB)
   #define SMI_DMA           ( 4<<DMA0_TI_PERMAP_LSB)
   #define PWM_DMA           ( 5<<DMA0_TI_PERMAP_LSB)
   #define SPI_TX_DMA        ( 6<<DMA0_TI_PERMAP_LSB)
   #define SPI_RX_DMA        ( 7<<DMA0_TI_PERMAP_LSB)
   #define SLIM_DTX_DMA      ( 8<<DMA0_TI_PERMAP_LSB)
   #define SLIM_DRX_DMA      ( 9<<DMA0_TI_PERMAP_LSB)
   #define SLIM_CTX_DMA      (10<<DMA0_TI_PERMAP_LSB)
   #define SLIM_CRX_DMA      (11<<DMA0_TI_PERMAP_LSB)
   #define UNUSED_DMA_12     (12<<DMA0_TI_PERMAP_LSB)
   #define SD_HOST_DMA       (13<<DMA0_TI_PERMAP_LSB)
   #define UNUSED_DMA_14     (14<<DMA0_TI_PERMAP_LSB)
   #define DSI1_DMA          (15<<DMA0_TI_PERMAP_LSB)

   #define UNUSED_DMA_16     (16<<DMA0_TI_PERMAP_LSB)
   #define HDMI_DMA          (17<<DMA0_TI_PERMAP_LSB)
   #define UNUSED_DMA_18     (18<<DMA0_TI_PERMAP_LSB)

   #define CRYPTO_IP_DMA     (19<<DMA0_TI_PERMAP_LSB)
   #define CRYPTO_OP_DMA     (20<<DMA0_TI_PERMAP_LSB)
   #define SCALER_0_DMA      (21<<DMA0_TI_PERMAP_LSB)
   #define SCALER_1_DMA      (22<<DMA0_TI_PERMAP_LSB)
   #define SCALER_2_DMA      (23<<DMA0_TI_PERMAP_LSB)
   #define SMI_SCALER_0_DMA  (24<<DMA0_TI_PERMAP_LSB)
   #define SMI_SCALER_1_DMA  (25<<DMA0_TI_PERMAP_LSB)
   #define SMI_SCALER_2_DMA  (26<<DMA0_TI_PERMAP_LSB)

   // 27-32 unused
   #define DMA_TI_S_128                (1<<9)
   #define DMA_TI_S_INC                (1<<8)
   #define DMA_TI_D_32                 0
   #define DMA_TI_D_DREQ               (1<<6)
   #define DMA_TI_PER_MAP(n)           (((n) & 0x1f) <<16)

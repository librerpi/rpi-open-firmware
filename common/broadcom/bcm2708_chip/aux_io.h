//
// Auxiliary I/O header file
//

#define AUX_IO_BASE               0x7E215000
#define AUX_IRQ                   (AUX_IO_BASE+0x000)
#define AUX_ENABLES               (AUX_IO_BASE+0x004)
#define    AUX_ENABLE_MINIUART    0x01
#define    AUX_ENABLE_SPI0        0x02
#define    AUX_ENABLE_SPI1        0x04


//
// Micro UART
//
// Baud rate = sysclk/(8*(BAUD_REG+1))

#define AUX_MU_IO_REG             (AUX_IO_BASE+0x040) // Write=TX read=RX
#define AUX_MU_BDLS_REG           (AUX_IO_BASE+0x040) // Baudrate LS
#define AUX_MU_BDMS_REG           (AUX_IO_BASE+0x044) // Baudrate MS.

#define AUX_MU_IER_REG            (AUX_IO_BASE+0x044) // IRQ enbl. reg.
#define    AUX_MU_IER_RXIRQEN     0x01  //
#define    AUX_MU_IER_TXIRQEN     0x02  //
// Line interrupts are not supported

#define AUX_MU_IIR_REG            (AUX_IO_BASE+0x048) // IRQ status reg
#define    AUX_MU_IIR_NOIRQS      0x01  // No irq pending
#define    AUX_MU_IIR_IRQ         0x06  // 10 = rec irq, 01 = tx irq
// Timeout is not supported

#define AUX_MU_FCR_REG            (AUX_IO_BASE+0x048) // FIFO control reg
#define    AUX_MU_FCR_RXCLR       0x02  // Flush receive FF
#define    AUX_MU_FCR_TXCLR       0x04  // Flush transmit fifo


#define AUX_MU_LCR_REG            (AUX_IO_BASE+0x04C) // Line control reg.
#define    AUX_MU_LCR_7BITS       0x02 // 7 bits mode
#define    AUX_MU_LCR_8BITS       0x03 // 8 bits mode
#define    AUX_MU_LCR_BREAK       0x40 // send break
#define    AUX_MU_LCR_DLAB        0x80 // DLAB access
// 5 & 6 bits are not supported
// 2 stop bits are not supported
// Parity bits are not supported

#define AUX_MU_MCR_REG            (AUX_IO_BASE+0x050) // Modem control reg.
#define    AUX_MU_MCR_RTS         0x02 // Set RTS high
// DTR is not supported
// Out1/2 are not supported
// Loopback is not supported

#define AUX_MU_LSR_REG            (AUX_IO_BASE+0x054) // Line status reg.
#define    AUX_MU_LSR_DR          0x01 // Receive Data ready
#define    AUX_MU_LSR_OE          0x02 // Receiver overrun error
#define    AUX_MU_LSR_THRE        0x20 // Transmitter holding register
#define    AUX_MU_LSR_TEMT        0x40 // Transmitter empty
// Parity bits (and thus errors) are not supported
// Framing errors are not detected
// Break detect is not (yet) supported

#define AUX_MU_MSR_REG            (AUX_IO_BASE+0x058) // Modem status reg.
#define    AUX_MU_MSR_CTS         0x10
// Delta CTS not supported
// DCE,DCD not supportred

#define AUX_MU_SCRATCH            (AUX_IO_BASE+0x05C) // Scratch reg.

// None 16550 features
#define AUX_MU_CNTL_REG           (AUX_IO_BASE+0x060) // AUX control reg.
#define    AUX_MU_CNTL_REC_ENBL   0x01 // receiver enable
#define    AUX_MU_CNTL_TRN_ENBL   0x02 // transmitter enable
#define    AUX_MU_CNTL_AUTO_RTR   0x04 // RTR set by RX FF level
#define    AUX_MU_CNTL_AUTO_CTS   0x08 // CTS auto stops transmitter
#define    AUX_MU_CNTL_FLOW3      0x00 // Stop on RX FF 3 entries left
#define    AUX_MU_CNTL_FLOW2      0x10 // Stop on RX FF 2 entries left
#define    AUX_MU_CNTL_FLOW1      0x20 // Stop on RX FF 1 entries left
#define    AUX_MU_CNTL_FLOW4      0x30 // Stop on RX FF 4 entries left
#define    AUX_MU_CNTL_AURTRINV   0x40 // Invert AUTO RTR polarity
#define    AUX_MU_CNTL_AUCTSINV   0x80 // Invert AUTO CTS polarity


#define AUX_MU_STAT_REG           (AUX_IO_BASE+0x064) // AUX status reg.
#define    AUX_MU_STAT_RX_DATA    0x00000001 // RX FF has value
#define    AUX_MU_STAT_TX_SPACE   0x00000002 // TX FF has space (not full)
#define    AUX_MU_STAT_RX_IDLE    0x00000004 // Receiver is idle
#define    AUX_MU_STAT_TX_IDLE    0x00000008 // Transmitter is idle
#define    AUX_MU_STAT_RX_OFLW    0x00000010 // Receiver FF overflow error
#define    AUX_MU_STAT_TX_FULL    0x00000020 // Transmit FF full
#define    AUX_MU_STAT_RTR        0x00000040 // Status of the RTR line
#define    AUX_MU_STAT_CTS        0x00000080 // Status of the CTS line (fully synced)
#define    AUX_MU_STAT_TXEMPTY    0x00000100 // TX FF is empty
#define    AUX_MU_STAT_TXDONE     0x00000200  // TX FF is empty and TX is idle
#define    AUX_MU_STAT_RXFILL     0x00FF0000 // RX FF fill level
#define    AUX_MU_STAT_TXFILL     0xFF000000 // TX FF fill level

#define AUX_MU_BAUD_REG           (AUX_IO_BASE+0x068) // Baudrate reg (16 bits)
                                             // Baud rate = sysclk/(8*(BAUD_REG+1))

//
// SPI 0 (SPI1 in the device!)
//
#define AUX_SPI0_CNTL0_REG        (AUX_IO_BASE+0x080) // control reg 0
#define   AUX_SPI_CNTL0_BITS      0x0000003F // Number of bits to send/receive
#define   AUX_SPI_CNTL0_OUTMS     0x00000040 // Shift MS bit out first)
#define   AUX_SPI_CNTL0_INVCLK    0x00000080 // Invert SPI_CLK
#define   AUX_SPI_CNTL0_OUTRISE   0x00000100 // data out leaves on rising clock edge
#define   AUX_SPI_CNTL0_OUTFALL   0x00000000 // data out leaves on falling clock edge
#define   AUX_SPI_CNTL0_FFCLR     0x00000200 // Reset fifos (Set and clear bit)
#define   AUX_SPI_CNTL0_INRISE    0x00000400 // data in on rising clock edge
#define   AUX_SPI_CNTL0_INFALL    0x00000000 // data in on falling clock edge
#define   AUX_SPI_CNTL0_SERENBL   0x00000800 // Serial enable (does not disable FFs)
#define   AUX_SPI_CNTL0_HOLD0     0x00000000 // Dout hold 0 sys clock cycles
#define   AUX_SPI_CNTL0_HOLD4     0x00001000 // Dout hold 4 sys clock cycle
#define   AUX_SPI_CNTL0_HOLD7     0x00002000 // Dout hold 7 sys clock cycles
#define   AUX_SPI_CNTL0_HOLD10    0x00003000 // Dout hold 10 sys clock cycles
#define   AUX_SPI_CNTL0_VARWID    0x00004000 // Variable width mode (din[15-12]=bits)
#define   AUX_SPI_CNTL0_CSFROMFF  0x00008000 // CS pattern comesfrom MS 3 TX FIFO bits
#define   AUX_SPI_CNTL0_POSTIN    0x00010000 // Load last bit after cycles finished
#define   AUX_SPI_CNTL0_CS_HIGH   0x000E0000 // All CS are high
#define   AUX_SPI_CNTL0_CS0_N     0x000C0000 // CS 0 low
#define   AUX_SPI_CNTL0_CS1_N     0x000A0000 // CS 1 low
#define   AUX_SPI_CNTL0_CS2_N     0x00060000 // CS 2 low
#define   AUX_SPI_CNTL0_CSA_N     0x00000000 // ALL CS low (test only)
#define   AUX_SPI_CNTL0_SPEED     0xFFF00000 // SPI clock = sysclock/(2xspeed)
#define   AUX_SPI_CNTL0_SPEEDSHFT 20         // Speed shift left value

#define AUX_SPI0_CNTL1_REG        (AUX_IO_BASE+0x084) // control reg 1
#define   AUX_SPI_CNTL1_HOLDIN    0x00000001 // Do not clear DIN register at start
#define   AUX_SPI_CNTL1_INMS      0x00000002 // Shift data in MS first MS--->LS
//#define   AUX_SPI_CNTL1_CS_NOW    0x00000004 // Assert CS pattern now
#define   AUX_SPI_CNTL1_EMPTYIRQ  0x00000040 // IRQ on TX Fifo empty
#define   AUX_SPI_CNTL1_DONEIRQ   0x00000080 // IRQ on IDLE AND TxFifo empty
#define   AUX_SPI_CNTL1_CSPLUS1   0x00000100 // CS HI plus 1 bit
#define   AUX_SPI_CNTL1_CSPLUS2   0x00000200 // CS HI plus 2 bit
#define   AUX_SPI_CNTL1_CSPLUS3   0x00000300 // CS HI plus 3 bit
#define   AUX_SPI_CNTL1_CSPLUS4   0x00000400 // CS HI plus 4 bit
#define   AUX_SPI_CNTL1_CSPLUS5   0x00000500 // CS HI plus 5 bit
#define   AUX_SPI_CNTL1_CSPLUS6   0x00000600 // CS HI plus 6 bit
#define   AUX_SPI_CNTL1_CSPLUS7   0x00000700 // CS HI plus 7 bit

#define AUX_SPI0_STAT_REG         (AUX_IO_BASE+0x088) // Status reg.
#define    AUX_SPI_STAT_BITCNT    0x0000003F // Bits remaining to be shifted out
#define    AUX_SPI_STAT_BUSY      0x00000040 // FSM is busy
#define    AUX_SPI_STAT_RXEMPTY   0x00000080 // RX FF is empty
#define    AUX_SPI_STAT_RXFULL    0x00000100 // RX FF is full
#define    AUX_SPI_STAT_TXEMPTY   0x00000200 // TX FF is empyt
#define    AUX_SPI_STAT_TXFULL    0x00000400 // TX FF is full
#define    AUX_SPI_STAT_RXFILL    0x000F0000 // RX FF fill level
#define    AUX_SPI_STAT_TXFILL    0x0F000000 // TX FF fill level
#define AUX_SPI0_PEEK_REG         (AUX_IO_BASE+0x08C) // Read but do not take from FF
#define AUX_SPI0_IO_REG           (AUX_IO_BASE+0x0A0) // Write = TX, read=RX
#define AUX_SPI0_TXHOLD_REG       (AUX_IO_BASE+0x0B0) // Write = TX keep cs, read=RX



//
// SPI 1 (SPI2 in the device!)
//
#define AUX_SPI1_CNTL0_REG        (AUX_IO_BASE+0x0C0)
#define AUX_SPI1_CNTL1_REG        (AUX_IO_BASE+0x0C4)
#define AUX_SPI1_STAT_REG         (AUX_IO_BASE+0x0C8)
#define AUX_SPI1_PEEK_REG         (AUX_IO_BASE+0x0CC)
#define AUX_SPI1_IO_REG           (AUX_IO_BASE+0x0E0)
#define AUX_SPI1_TXHOLD_REG       (AUX_IO_BASE+0x0F0) // Write = TX keep cs, read=RX

//
//  Some usefull GPIO macros
//
#define CLR_GPIO(g) *(volatile uint32_t *)(GP_BASE+(((g)/10)<<2))&= ~(7<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(volatile uint32_t *)(GP_BASE+(((g)/10)<<2))|= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

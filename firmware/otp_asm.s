# 1 "otp_asm.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "otp_asm.S"


A2W_BASE = 0x7e102000
A2W_XOSC0 = 0x090

OTP_BASE = 0x7e20f000
OTP_CONFIG_REG = 0x04
OTP_CTRL_LO_REG = 0x08
OTP_CTRL_HI_REG = 0x0c
OTP_STATUS_REG = 0x10
OTP_DATA_REG = 0x18
OTP_ADDR_REG = 0x1c




  OTP_READY_BIT = 0


.text
.global otp_read_internal
otp_read_internal:
  stm r6-r7,lr,(--sp)

  mov r6, r0
  bl otp_open
  mov r0, r6
  bl otp_read_reg
  mov r7, r0
  bl otp_close
  mov r0, r7

  ldm r6-r7,pc,(sp++)

otp_open:
  stm lr, (--sp)
  mov r2, 0x03
  mov r1, OTP_BASE
  mov r3, A2W_BASE + A2W_XOSC0
  st r2, (r1 + OTP_CONFIG_REG)

  mov r0, 2
1:
  ld r2, (r3)
  addcmpbge r0, -1, 0, 1b

  mov r2, 0
  st r2, (r1 + OTP_CTRL_HI_REG)
  st r2, (r1 + OTP_CTRL_LO_REG)

  mov r0, 2
1:
  ld r2, (r3)
  addcmpbge r0, -1, 0, 1b

  mov r2, 0x2
  st r2, (r1 + OTP_CONFIG_REG)
  ldm pc, (sp++)

otp_read_reg:
  stm lr, (--sp)
  mov r1, OTP_BASE
  mov r3, A2W_BASE + A2W_XOSC0
  st r0, (r1 + OTP_ADDR_REG)

  mov r0, 2
1:
  ld r2, (r3)
  addcmpbge r0, -1, 0, 1b

  mov r2, 0
  ld r0, (r1 + OTP_ADDR_REG)
  st r2, (r1 + OTP_CTRL_HI_REG)
  st r2, (r1 + OTP_CTRL_LO_REG)

  mov r0, 2
1:
  ld r2, (r3)
  addcmpbge r0, -1, 0, 1b

  mov r2, 1
  ld r0, (r1 + OTP_CTRL_LO_REG)
  st r2, (r1 + OTP_CTRL_LO_REG)
  ld r0, (r1 + OTP_CTRL_LO_REG)
1:
  mov r0, 2
2:
  ld r2, (r3)
  addcmpbge r0, -1, 0, 2b

  ld r0, (r1 + OTP_STATUS_REG)
  btest r0, OTP_READY_BIT
  beq 1b
  ld r0, (r1 + OTP_DATA_REG)
  ldm pc, (sp++)

otp_close:
  stm lr, (--sp)
  mov r2, 0
  mov r1, OTP_BASE
  mov r3, A2W_BASE + A2W_XOSC0
  st r2, (r1 + OTP_CTRL_HI_REG)
  st r2, (r1 + OTP_CTRL_LO_REG)

  mov r0, 2
1:
  ld r2, (r3)
  addcmpbge r0, -1, 0, 1b

  mov r2, 0
  st r2, (r1 + OTP_CONFIG_REG)
  ldm pc, (sp++)

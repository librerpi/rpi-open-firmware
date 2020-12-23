adapter driver sysfsgpio
sysfsgpio_jtag_nums 11 25 10 9
sysfsgpio_trst_num 7

set _CHIPNAME rpi1

transport select jtag

set _DAP_TAPID 0x07b7617F

jtag newtap $_CHIPNAME arm -irlen 5 -expected-id $_DAP_TAPID

set _TARGETNAME $_CHIPNAME.arm1176

target create $_TARGETNAME arm11 -chain-position $_CHIPNAME.arm

rpi1.arm1176 configure -event gdb-attach { halt }


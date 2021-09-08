#source [find interface/sysfsgpio-raspberrypi.cfg]

adapter driver sysfsgpio
sysfsgpio_jtag_nums 11 25 10 9
sysfsgpio_trst_num 7

set _CHIPNAME rpi3

transport select jtag

set _DAP_TAPID 0x4ba00477

jtag newtap $_CHIPNAME tap -irlen 4 -ircapture 0x1 -irmask 0xf -expected-id $_DAP_TAPID -enable
dap create $_CHIPNAME.dap -chain-position $_CHIPNAME.tap

set _TARGETNAME $_CHIPNAME.a53
set _CTINAME $_CHIPNAME.cti

set DBGBASE {0x80010000 0x80012000 0x80014000 0x80016000}
set CTIBASE {0x80018000 0x80019000 0x8001a000 0x8001b000}
set _cores 4

for { set _core 0 } { $_core < $_cores } { incr _core } {
	cti create $_CTINAME.$_core -dap $_CHIPNAME.dap -ap-num 0 -ctibase [lindex $CTIBASE $_core]

	target create $_TARGETNAME.$_core aarch64 -dap $_CHIPNAME.dap -coreid $_core -dbgbase [lindex $DBGBASE $_core] -cti $_CTINAME.$_core

	$_TARGETNAME.$_core configure -event reset-assert-post "aarch64 dbginit"
	$_TARGETNAME.$_core configure -event gdb-attach { halt }
}

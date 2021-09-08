#source [find interface/sysfsgpio-raspberrypi.cfg]

adapter driver sysfsgpio
sysfsgpio_jtag_nums 11 25 10 9
sysfsgpio_trst_num 7

set _CHIPNAME rpi2

transport select jtag

set _DAP_TAPID 0x4ba00477

jtag newtap $_CHIPNAME tap -irlen 4 -ircapture 0x1 -irmask 0xf -expected-id $_DAP_TAPID -enable
dap create $_CHIPNAME.dap -chain-position $_CHIPNAME.tap

set _TARGETNAME $_CHIPNAME.a7

set DBGBASE {0x80010000 0x80012000 0x80014000 0x80016000}
set _cores 4

for { set _core 0 } { $_core < $_cores } { incr _core } {

	target create $_TARGETNAME.$_core cortex_a -dap $_CHIPNAME.dap -coreid $_core -dbgbase [lindex $DBGBASE $_core]

	$_TARGETNAME.$_core configure -event reset-assert-post "cortex_a dbginit"
	$_TARGETNAME.$_core configure -event gdb-attach { halt }
}

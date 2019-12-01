This dir contains all the register map files for the design
The bulk of it is created with the create_regs script  (run create_regs)  which
parses the *_regs files in the hdl dirs and creates the individual files

The top level file is chip/hdl/bcm2708_regs.tcl which defines the contents
and off sets of the chip level memory map.

Create regs then searches the dir tree for each object mentioned in bcm2708_regs.tcl
and locates its   "object_regs.tcl"  file

It then generates all the .h .inc ... files.

These are then all lumped together in a  register_map.h  register_map.inc etc file



The general procedure to modify this is



check out all of chip/verification/code/vcinclude
run create_regs  in this dir
revert all unchanged files in  vcinclude
check the modified ones and checkl them in if they are ok.


Note:
If create regs cant find a _regs.tcl file it will crash
If it finds two files with the same name it will crash

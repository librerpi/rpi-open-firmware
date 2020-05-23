#pragma once

#define MEM_CODE_END 0x20000

#define MEM_STACK_SIZE 0x4000
// stack0 end
#define MEM_STACK_END (MEM_CODE_END + MEM_STACK_SIZE)

#define MEM_HEAP_START MEM_CODE_END + (MEM_STACK_SIZE*4)
#define MEM_HEAP_SIZE 0x100000
#define MEM_HEAP_END (MEM_HEAP_START + MEM_HEAP_SIZE)

#define MEM_USABLE_START MEM_HEAP_END

/*
 * 0-50kb                      .text .data .bss
 * 128kb    MEM_CODE_END
 * 0x20000 (128kb) to 23fff    stack0
 * 0x24000 to 0x27fff          stack1
 * 0x28000 to 0x2bfff          stack2
 * 0x2c000 to 0x2ffff          stack3
 * 0x30000 to 0x130000         heap
 * 32mb  (KERNEL_LOAD_ADDRESS) start of kernel
 * 250mb (DTB_LOAD_ADDRESS)    start of dtb file
 */

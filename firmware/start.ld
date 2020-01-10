/*
 * based heavily on https://github.com/ptesarik/vc4boot
 */

OUTPUT_FORMAT("elf32-vc4", "elf32-vc4", "elf32-vc4")
OUTPUT_ARCH(vc4)
ENTRY(_start)
GROUP(-lgloss -lc -lteststub -lgcc)

MEMORY {
  /* starts 236mb into the C alias, direct uncached
   * 256mb long
   */
  ram (rwx) : ORIGIN = 0xc0000000 + 2M, LENGTH = 0x10000000
}
SECTIONS {
  .text : ALIGN(32) {
    PROVIDE(_start = .);
    . = ALIGN(4);
    *(.text .stub .text.* .gnu.linkonce.t.*)
    *(.gnu.warning)
    KEEP (*(.init))
    KEEP (*(.fini))
    /* Constructors and destructors */
    KEEP (*crtbegin*.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend*.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    KEEP (*crtbegin*.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend*.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    KEEP (*(.jcr))
    _etext = .;
  } >ram =0
  .rodata : {
    . = ALIGN(4);
    _frodata = .;
    *(.rodata .rodata.* .gnu.linkonce.r.*)
    *(.rodata1)
    _erodata = .;
  } >ram
  .data : {
    . = ALIGN(4);
    _fdata = .;
    *(.data .data.* .gnu.linkonce.d.*)
    *(.data1)
    SORT(CONSTRUCTORS)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
    _edata = .;
  } >ram
  .bss : {
    . = ALIGN(4);
    _fbss = .;
    *(.dynsbss)
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
    *(.dynbss)
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    _end = .;
    PROVIDE (end = .);
  } >ram
  /* First location in stack is highest address in RAM */
  PROVIDE(_fstack = ORIGIN(ram) + LENGTH(ram) - 4);
}

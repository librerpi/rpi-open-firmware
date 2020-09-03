.text
.global arm7_dcache_off
arm7_dcache_off:
  // save clobbered registers
  push {r4, r5, r7, r9, r10, r11, lr}
  // start of flush logic
  mrc p15, 1, r0, c0, c0, 1  // CLIDR -> r0
  ands r3, r0, #0x07000000
  mov r3, r3, lsr #23        // Cache level value (naturally aligned)
  beq Finished
  mov r10, #0
Loop1:
  add r2, r10, r10, lsr #1    // work out 3 x cache level
  mov r1, r0, lsr r2          // bottom 3 bits are the cache type for this level
  and r1, r1, #7              // get those 3 bits alone
  cmp r1, #2
  blt Skip                    // no cache or only i-cache at this level
  mcr p15, 2, r10, c0, c0, 0  // r10 -> CSSELR
  isb                         // isb to sync the change to CCSIDR
  mrc p15, 1, r1, c0, c0, 0   // CCSIDR -> r1
  and r2, r1, #7              // extract the line length field
  add r2, r2, #4              // add 4 for the line length offset (log2 16 bytes)
  mov r4, #0x3ff
  ands r4, r4, r1, lsr #3     // r4 is the max number on the way size (right aligned)
  clz r5, r4                  // r5 is the bit position of the way size increment
  mov r9, r4                  // r9 working copy of the max way size (right aligned)
Loop2:
  mov r7, #0x00007FFF
  ands r7, r7, r1, lsr #13    // r7 is the max number of the index size (right aligned)
Loop3:
  orr r11, r10, r9, lsl r5    // factor in the way number and cache number into r11
  orr r11, r11, r7, lsl r2    // factor in the index number
  mcr p15, 0, r11, c7, c10, 2 // DCCSW, clean by set/way
  subs r7, r7, #1             // decrement the index
  bge Loop3
  subs r9, r9, #1             // decrement the way number
  bge Loop2
Skip:
  add r10, r10, #2            // increment the cache number
  cmp r3, r10
  dsb                         // ensure completion of previus cache instruction
  bgt Loop1
Finished:
  mrc p15, 0, r0, c1, c0, 0 // SCTLR -> r0
  bic r0, r0, #(1<<2)       // clear bit 2 dcache
  mcr p15, 0, r0, c1, c0, 0 // r0 -> SCTLR
  dsb
  pop {r4, r5, r7, r9, r10, r11, pc}

.global arm_mmu_off
arm_mmu_off:
  mrc p15, 0, r0, c1, c0, 0
  bic r0, r0, #(1<<1)       // clear bit 1 mmu
  mcr p15, 0, r0, c1, c0, 0 // r0 -> SCTLR
  dsb
  isb
  bx lr

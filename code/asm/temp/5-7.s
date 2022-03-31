  .text
  .global _start

_start:
  lui a4, 0x80000
  li t0, 0x6c6c6568
  sw t0, 120(a4)
  li t0, 0x6f772c6f
  sw t0, 124(a4)
  li t0, 0x21646c72
  sw t0, 128(a4)
  li t0, 0x00
  sw t0, 132(a4)

  li t1, 0
  lbu a5, 120(a4)
  beqz a5, stop

loop:
  addi t1, t1, 1
  addi a4, a4, 1
  lbu a5, 120(a4)
  bnez a5, loop

stop:
  j stop

  .end

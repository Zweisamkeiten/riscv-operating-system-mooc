#ifndef __PLATFORM_H__
#define __PLATFORM_H__

  .global MAXNUM_CPU
  .equ    MAXNUM_CPU, 8
  .global UART0
  .equ    UART0, 0x10000000

#endif
  
  .equ PHR, 0 # Receive Holding Register (read mode)
  .equ THR, 0 # Transmit Holding Register (write mode)
  .equ DLL, 0 # LSB of Divisor Latch (write mode)
  .equ IER, 1 # Interrupt Enable Register (write mode)
  .equ DLM, 1 # MSB of Divisor Latch (write mode)
  .equ FCR, 2 # FIFO Control Register (write mode)
  .equ ISR, 2 # Interrupt Status REgister (read mode)
  .equ LCR, 3 # Line Control Register (write mode)
  .equ MCR, 4 # Modem Control Register
  .equ LSR, 5 # Line Status Register
  .equ MSR, 6 # Modem Status Register
  .equ SPR, 7 # ScratchPad Register

  .equ LSR_RX_READY, 1
  .equ LSR_TX_IDLE,  0b100000

.macro UART_REG, reg
  la  sp, UART0
  addi sp, sp, \reg
.endm

.macro uart_read_reg, reg
  UART_REG \reg
  lbu t0, 0(sp)
.endm

.macro uart_write_reg, reg, v
  UART_REG \reg
  sb  \v, 0(sp)
.endm

	.globl	uart_init
	.type	uart_init, @function
uart_init:
  la sp, stack_end

  li  t0, 0x00
  uart_write_reg IER, t0

  uart_read_reg LCR
  li  t1, 1
  slli t1, t1, 7
  or  t1, t0, t1
  uart_write_reg LCR, t1
  li  t0, 0x03
  uart_write_reg DLL, t0
  li  t0, 0x00
  uart_write_reg DLM, t0

  li  t0, 0
  li  t1, 3
  slli t1, t1, 0
  or  t1, t0, t1
  uart_write_reg LCR, t1

  ret

	.globl	uart_putc
	.type	uart_putc, @function
uart_putc:
  la sp, stack_end
  # prologue
  addi sp, sp, -4
  sw  s0, 0(sp)
  mv  s0, a0

loop1:
  uart_read_reg LSR
  li  t1, LSR_TX_IDLE
  and t0, t0, t1
  beqz t0, loop1

  uart_write_reg THR, s0

  # epilogue
  lw  s0, 0(sp)
  addi sp, sp, 4

  ret

	.globl	uart_puts
	.type	uart_puts, @function
uart_puts:
  la sp, stack_end
  # prologue
  addi sp, sp, -12
  sw  s0, 0(sp)
  sw  s1, 4(sp)
  sw  ra, 8(sp)
  mv  s1, a0

loop2:
  lb  t0, 0(s1)
  mv  a0, t0
  addi s1, s1, 1
  call uart_putc
  bnez t0, loop2

  # epilogue
  lw  s0, 0(sp)
  lw  s1, 4(sp)
  lw  ra, 8(sp)
  addi sp, sp, 12

  ret

stack_start:
  .rept 12
  .word 0
  .endr
stack_end:

  .end

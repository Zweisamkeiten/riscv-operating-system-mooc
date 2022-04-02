  .global start_kernel
  .type start_kernel, @function
start_kernel:
  call uart_init

  la a0, .LC0
  call uart_puts

loop:
  j loop
  ret

.LC0:
  .string	"Hello, RVOS!\n"


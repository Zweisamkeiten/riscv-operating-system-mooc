#include "os.h"

#define DELAY 1000

void user_task0(void* param)
{
	uart_puts("Task 0: Created!\n");
	while (1) {
		uart_puts("Task 0: Running...\n");
		task_delay(DELAY);
		task_yield();
	}
}

void user_task1(void* param)
{
	uart_puts("Task 1: Created!\n");
	while (1) {
		uart_puts("Task 1: Running...\n");
		task_delay(DELAY);
		task_yield();
	}
}

void user_task2(void* param)
{
	uart_puts("Task 2: Created!\n");
	while (1) {
		uart_puts("Task 2: Running...\n");
		task_delay(DELAY);
		uart_puts("Task 2: Exiting...\n");
		task_exit();
		/* task_yield(); */
	}
}

void user_task3(void* param)
{
	uart_puts("Task 3: Created!\n");
	while (1) {
		uart_puts("Task 3: Running...\n");
		task_delay(DELAY);
		/* task_yield(); */
		uart_puts("Task 3: Exiting...\n");
		task_exit();
	}
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void)
{
	/* task_create(user_task0, NULL, 0); */
	/* task_create(user_task1, NULL, 1); */
	/* task_create(user_task2, NULL, 2); */
	/* task_create(user_task3, NULL, 3); */
	task_create(user_task0, NULL, 1);
	task_create(user_task1, NULL, 1);
	task_create(user_task2, NULL, 1);
	task_create(user_task3, NULL, 2);
}


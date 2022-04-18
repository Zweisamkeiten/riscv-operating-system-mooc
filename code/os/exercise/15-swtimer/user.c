#include "os.h"

#define DELAY 5

//#define USE_LOCK
struct userdata {
	int counter;
	char *str;
};

struct userdata person1 = {0, "Jack"};
struct userdata person2 = {0, "M"};
struct userdata person3 = {0, "N"};

void timer_func(void *arg)
{
	if (NULL == arg)
		return;

	struct userdata *param = (struct userdata *)arg;

	param->counter++;
	printf("=====> TIMEOUT: %s: %d\n", param->str, param->counter);
}

void user_task0(void)
{
	uart_puts("Task 0: Created!\n");

	timer *t1 = timer_create(timer_func, &person1, 2);
	if (NULL == t1) {
		printf("timer_create() failed!\n");
	}
	timer *t2 = timer_create(timer_func, &person2, 1);
	if (NULL == t2) {
		printf("timer_create() failed!\n");
	}
	timer *t3 = timer_create(timer_func, &person3, 3);
	if (NULL == t3) {
		printf("timer_create() failed!\n");
	}
	timer *t4 = timer_create(timer_func, &person1, 3);
	if (NULL == t4) {
		printf("timer_create() failed!\n");
	}
	timer *t5 = timer_create(timer_func, &person2, 2);
	if (NULL == t5) {
		printf("timer_create() failed!\n");
	}
	timer *t6 = timer_create(timer_func, &person3, 4);
	if (NULL == t6) {
		printf("timer_create() failed!\n");
	}
	while (1) {
#ifdef USE_LOCK
		spin_lock();
#endif
		uart_puts("Task 0: Running ... \n");
		task_delay(DELAY);
		/* for (int i = 0; i < 5; i++) { */
		/* 	uart_puts("Task 0: Running... \n"); */
		/* 	task_delay(DELAY); */
		/* } */
		/* uart_puts("Task 0: End ... \n"); */
#ifdef USE_LOCK
		spin_unlock();
#endif
	}
}

void user_task1(void)
{
	uart_puts("Task 1: Created!\n");
	while (1) {
		/* uart_puts("Task 1: Begin ... \n"); */
		/* for (int i = 0; i < 5; i++) { */
			uart_puts("Task 1: Running... \n");
			task_delay(DELAY+1);
		/* } */
		/* uart_puts("Task 1: End ... \n"); */
	}
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void)
{
	task_create(user_task0);
	task_create(user_task1);
}


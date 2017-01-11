/*
 *
 * test/test_pthreadpool.c
 *
 * Copyright (C) 2016      ming_qian<xxjsqm@126.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mxq.h>
#include <mxq_pthreadpool.h>

void *test_handle(void *args)
{
	int l = (int)args;

	sleep(1);
	printf("%d\n", l);

	return NULL;
}

static int test()
{
	int i;
	PthreadPool pool;

	pool = mxq_pthreadpool_init(8);
	if (!pool)
		return -1;

	for (i = 0; i < 100; i++) {
		mxq_pthreadpool_add_task(pool, test_handle, (void *)i);
	}

	while (mxq_task_count(pool))
		sleep(3);
	mxq_pthreadpool_destroy(pool);
	pool = NULL;

	return 0;
}

int main(int argc, char *argv[])
{
	test();

	return 0;
}

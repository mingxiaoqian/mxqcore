/*
 * Realtek Semiconductor Corp.
 *
 * librtscamkit/test/test_queue.c
 *
 * Copyright (C) 2014      Ming Qian<ming_qian@realsil.com.cn>
 */
#include <stdio.h>
#include <stdlib.h>
#include <mxq.h>
#include <mxq_queue.h>

static int test()
{
	Queue q = mxq_queue_init();
	int i;

	for (i = 0; i < 100; i++)
		mxq_queue_put(q, (Item)i);

	while (!mxq_queue_empty(q))
		printf("%d\n", (int)mxq_queue_get(q));

	mxq_queue_destroy(q);
	q = NULL;

	return 0;
}

struct test_point {
	int x;
	int y;
};

static int test_pointor()
{
	struct test_point points[10];
	Queue q = mxq_queue_init();
	int i;

	for (i = 0; i < MXQ_ARRAY_SIZE(points); i++) {
		struct test_point *point = points + i;
		point->x = (i + 1);
		point->y = point->x * point->x;
		mxq_queue_put(q, (Item)point);
	}

	while (!mxq_queue_empty(q)) {
		struct test_point *point = mxq_queue_get(q);
		printf("(%d, %d)\n", point->x, point->y);
	}

	mxq_queue_destroy(q);
	q = NULL;

	return 0;
}

int main(int argc, char *argv[])
{
	test();
	test_pointor();

	return 0;
}


/*
 *
 * include/mxq_queue.h
 *
 * Copyright (C) 2016      ming_qian<xxjsqm@126.com>
 */

#ifndef _INCLUDE_MXQ_QUEUE_H
#define _INCLUDE_MXQ_QUEUE_H

typedef void *Item;

typedef struct mxq_queue *Queue;

typedef void (*cleanup_item)(Item item);

Queue mxq_queue_init();
void mxq_queue_destroy(Queue q);
int mxq_queue_empty(Queue q);
int mxq_queue_push_back(Queue q, Item item);
Item mxq_queue_pop(Queue q);
void mxq_queue_clear(Queue q, cleanup_item cleanup);

int mxq_queue_lock(Queue q);
int mxq_queue_unlock(Queue q);
int mxq_queue_count(Queue q);
Item mxq_queue_enumerate(Queue q, int index);

#define list_queue(q, func, arg...) \
	do {\
		int i;\
		mxq_queue_lock(q);\
		for (i = 0; i < mxq_queue_count(q); i++) {\
			Item item = mxq_queue_enumerate(q, i);\
			func(item, ##arg);\
		}\
		mxq_queue_unlock(q);\
	} while (0)

#endif


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

Queue mxq_queue_init();
void mxq_queue_destroy(Queue q);
int mxq_queue_empty(Queue q);
int mxq_queue_put(Queue q, Item item);
Item mxq_queue_get(Queue q);
int mxq_queue_count(Queue q);

#endif


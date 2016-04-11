/*
 *
 * src/mxq_queue.c
 *
 * Copyright (C) 2016      ming_qian<xxjsqm@126.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include "list.h"
#include "mxq.h"
#include "mxq_queue.h"

struct mxq_queue_node {
	Item item;
	struct list_head list;
};

struct mxq_queue {
	struct list_head queue;
	pthread_mutex_t lock;
};

Queue mxq_queue_init()
{
	Queue q;

	q = calloc(1, sizeof(struct mxq_queue));
	if (q) {
		pthread_mutex_init(&q->lock, NULL);
		INIT_LIST_HEAD(&q->queue);
	}

	return q;
}

int mxq_queue_empty(Queue q)
{
	assert(q);

	return list_empty(&q->queue);
}

struct mxq_queue_node *__new_node(Item item)
{
	struct mxq_queue_node *node;

	node = calloc(1, sizeof(struct mxq_queue_node));
	if (node)
		node->item = item;

	return node;
}

static int __mxq_queue_push_back(Queue q, Item item)
{
	struct mxq_queue_node *node;

	node = __new_node(item);
	if (!node) {
		return -1;
	}
	list_add_tail(&node->list, &q->queue);

	return 0;
}

static Item __mxq_queue_pop(Queue q)
{
	struct mxq_queue_node *head;
	Item item;

	if (mxq_queue_empty(q))
		return NULL;

	head = list_first_entry(&q->queue, struct mxq_queue_node, list);
	list_del_init(&head->list);

	item = head->item;
	MXQ_SAFE_DELETE(head);

	return item;
}

int mxq_queue_push_back(Queue q, Item item)
{
	int ret;

	assert(q);

	pthread_mutex_lock(&q->lock);
	ret = __mxq_queue_push_back(q, item);
	pthread_mutex_unlock(&q->lock);

	return ret;
}

Item mxq_queue_pop(Queue q)
{
	Item item;

	assert(q);

	pthread_mutex_lock(&q->lock);
	item = __mxq_queue_pop(q);
	pthread_mutex_unlock(&q->lock);

	return item;
}

static void __mxq_queue_clear(Queue q, cleanup_item cleanup)
{
	while (!mxq_queue_empty(q)) {
		Item item = __mxq_queue_pop(q);
		if (cleanup)
			cleanup(item);
	}
}

void mxq_queue_clear(Queue q, cleanup_item cleanup)
{
	assert(q);

	pthread_mutex_lock(&q->lock);
	__mxq_queue_clear(q, cleanup);
	pthread_mutex_unlock(&q->lock);
}

void mxq_queue_destroy(Queue q)
{
	if (!q)
		return;

	mxq_queue_clear(q, NULL);

	pthread_mutex_destroy(&q->lock);
	MXQ_SAFE_DELETE(q);
}

int mxq_queue_lock(Queue q)
{
	assert(q);

	return pthread_mutex_lock(&q->lock);
}

int mxq_queue_unlock(Queue q)
{
	assert(q);

	return pthread_mutex_unlock(&q->lock);
}

static int __mxq_queue_count(Queue q)
{
	int count = 0;
	struct mxq_queue_node *node;

	list_for_each_entry(node, &q->queue, list)
		count++;

	return count;
}

int mxq_queue_count(Queue q)
{
	assert(q);

	return __mxq_queue_count(q);
}

static Item __mxq_queue_enumerate(Queue q, int index)
{
	int i = 0;
	struct mxq_queue_node *node;

	if (index < 0)
		return NULL;

	list_for_each_entry(node, &q->queue, list) {
		if (i++ == index)
			return node->item;
	}

	return NULL;
}

Item mxq_queue_enumerate(Queue q, int index)
{
	assert(q);

	return __mxq_queue_enumerate(q, index);
}



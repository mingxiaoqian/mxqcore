/*
 *
 * src/mxq_pthreadpool.c
 *
 * Copyright (C) 2016      ming_qian<xxjsqm@126.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include "mxq.h"
#include "mxq_queue.h"
#include "mxq_pthreadpool.h"

#ifndef MXQ_PTHREADPOOL_SIZE_DFT
#define MXQ_PTHREADPOOL_SIZE_DFT		8
#endif

#ifndef MXQ_PTHREADPOOL_SIZE_MAX
#define MXQ_PTHREADPOOL_SIZE_MAX		64
#endif

struct mxq_pthreadpool {
	pthread_mutex_t lock;
	pthread_cond_t cond;

	Queue tasks;

	Queue workers;

	int shutdown;
};

struct mxq_task {
	task_handle handle;
	void *args;
};

struct mxq_task *__new_task(task_handle handle, void *args)
{
	struct mxq_task *task;

	task = calloc(1, sizeof(*task));
	if (task) {
		task->handle = handle;
		task->args = args;
	}

	return task;
}

void __delete_task(struct mxq_task *task)
{
	MXQ_SAFE_DELETE(task);
}

void *thread_run(void *arg)
{
	PthreadPool pool = arg;
	struct mxq_task *task;

	if (!pool)
		return NULL;

	while (1) {
		pthread_mutex_lock(&pool->lock);

		if (mxq_queue_empty(pool->tasks) && !pool->shutdown)
			pthread_cond_wait(&pool->cond, &pool->lock);

		if (pool->shutdown) {
			pthread_mutex_unlock(&pool->lock);
			pthread_exit(NULL);
		}
		task = mxq_queue_pop(pool->tasks);
		pthread_mutex_unlock(&pool->lock);

		if (task) {
			task->handle(task->args);
			MXQ_SAFE_DELETE(task);
		}
	}

	return NULL;
}

PthreadPool mxq_pthreadpool_init(int number)
{
	struct mxq_pthreadpool *pool;
	int i;

	if (number <= 0)
		number = MXQ_PTHREADPOOL_SIZE_DFT;
	if (number > MXQ_PTHREADPOOL_SIZE_MAX)
		number = MXQ_PTHREADPOOL_SIZE_MAX;

	pool = calloc(1, sizeof(*pool));
	if (!pool)
		return NULL;

	pthread_mutex_init(&pool->lock, NULL);
	pthread_cond_init(&pool->cond, NULL);
	pool->tasks = mxq_queue_init();
	if (!pool->tasks)
		goto error;
	pool->workers = mxq_queue_init();
	if (!pool->workers)
		goto error;

	for (i = 0; i < number; i++) {
		pthread_t th;
		if (!pthread_create(&th, NULL, thread_run, (void *)pool))
			mxq_queue_push_back(pool->workers, th);
	}

	if (mxq_queue_empty(pool->workers))
		goto error;

	pool->shutdown = 0;

	return pool;
error:
	if (pool->tasks) {
		mxq_queue_destroy(pool->tasks);
		pool->tasks = NULL;
	}
	if (pool->workers) {
		mxq_queue_destroy(pool->workers);
		pool->workers = NULL;
	}
	pthread_cond_destroy(&pool->cond);
	pthread_mutex_destroy(&pool->lock);
	MXQ_SAFE_DELETE(pool);

	return NULL;
}

void mxq_pthreadpool_destroy(PthreadPool pool)
{
	if (!pool)
		return;

	if (pool->shutdown)
		return;

	pool->shutdown = 1;

	pthread_cond_broadcast(&pool->cond);

	list_queue(pool->workers, pthread_join, NULL);
	mxq_queue_clear(pool->workers, NULL);
	mxq_queue_destroy(pool->workers);
	pool->workers = NULL;

	mxq_queue_clear(pool->tasks, (cleanup_item) __delete_task);
	mxq_queue_destroy(pool->tasks);
	pool->tasks = NULL;

	pthread_cond_destroy(&pool->cond);
	pthread_mutex_destroy(&pool->lock);

	MXQ_SAFE_DELETE(pool);
}

int mxq_pthreadpool_add_task(PthreadPool pool, task_handle handle, void *args)
{
	struct mxq_task *task;

	assert(pool);
	assert(handle);

	task = __new_task(handle, args);
	if (!task)
		return -1;

	pthread_mutex_lock(&pool->lock);
	mxq_queue_push_back(pool->tasks, task);
	pthread_mutex_unlock(&pool->lock);

	pthread_cond_signal(&pool->cond);

	return 0;
}

int mxq_thread_count(PthreadPool pool)
{
	assert(pool);

	return mxq_queue_count(pool->workers);
}

int mxq_task_count(PthreadPool pool)
{
	assert(pool);

	return mxq_queue_count(pool->tasks);
}

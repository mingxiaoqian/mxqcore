/*
 *
 * include/mxq_pthreadpool.h
 *
 * Copyright (C) 2016      ming_qian<xxjsqm@126.com>
 */

#ifndef _INCLUDE_MXQ_PTHREADPOOL_H
#define _INCLUDE_MXQ_PTHREADPOOL_H

typedef struct mxq_pthreadpool *PthreadPool;
typedef void *(*task_handle) (void *);

PthreadPool mxq_pthreadpool_init(int number);
void mxq_pthreadpool_destroy(PthreadPool pool);
int mxq_pthreadpool_add_task(PthreadPool pool, task_handle handle, void *args);

int mxq_thread_count(PthreadPool pool);
int mxq_task_count(PthreadPool pool);

#endif

/*
 *
 * include/mxq.h
 *
 * Copyright (C) 2016      ming_qian<xxjsqm@126.com>
 */

#ifndef _INCLUDE_MXQ_H
#define _INCLUDE_MXQ_H

#ifndef MXQ_SAFE_DELETE
#define MXQ_SAFE_DELETE(p)	\
	do {\
		if (p)\
			free(p);\
		p = NULL;\
	} while (0)
#endif

#ifndef MXQ_ARRAY_SIZE
#define MXQ_ARRAY_SIZE(array)	(sizeof(array) / sizeof((array)[0]))
#endif

#endif

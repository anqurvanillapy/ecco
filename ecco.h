#pragma once

#include <cstdlib>
#include <cassert>

/**
 *  Utility.
 */

#define freen(p)    do { free(p); p = nullptr; } while (0)

/**
 *  Static croutine.
 */

#define CO_BEGIN    static int __co_state = 0; switch (__co_state) { case 0:;
#define CO_END_WITH(v)  } return (v)
#define CO_END          } return

#define CO_YIELD_WITH(v)        \
	do {                        \
		__co_state = __LINE__;  \
		return (v);             \
	case __LINE__:;             \
	} while (0)
#define CO_YIELD                \
	do {                        \
		__co_state = __LINE__;  \
		return;                 \
	case __LINE__:;             \
	} while (0)

/**
 *  Coroutine with context.
 */

using co_ctx_t = void*;

#define CCO_CTX_PARAM       void** __co_ctx_param

#define CCO_CTX_DECL_BEGIN      struct __co_ctx_t { int state
#define CCO_CTX_DECL_END(ctx)   } *ctx = (__co_ctx_t*)*__co_ctx_param

#define CCO_BEGIN_WITH(ctx)                             \
	if (!ctx) {                                         \
		*__co_ctx_param = malloc(sizeof(*ctx));         \
		ctx = (__co_ctx_t*)*__co_ctx_param;      \
		assert(ctx);                                    \
		ctx->state = 0;                                 \
	}                                                   \
	switch (ctx->state) {                               \
	case 0:;
#define CCO_END_WITH(v)     \
	}                       \
	freen(*__co_ctx_param); \
	return (v)
#define CCO_END             \
	}                       \
	freen(*__co_ctx_param); \
	return

#define CCO_YIELD_WITH(v)                                           \
	do {                                                            \
		((__co_ctx_t*)*__co_ctx_param)->state = __LINE__;    \
		return (v);                                                 \
		case __LINE__:;                                             \
	} while (0)
#define CCO_YIELD                                                   \
	do {                                                            \
		((__co_ctx_t*)*__co_ctx_param)->state = __LINE__;    \
		return;                                                     \
		case __LINE__:;                                             \
	} while (0)

#define CCO_RETURN_WITH(v)  \
	do { freen(*__co_ctx_param); return (v); } while (0)
#define CCO_RETURN  \
	do { freen(*__co_ctx_param); return; } while (0)

#define CCO_CTX_FREE(ctx)   freen(ctx)

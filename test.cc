#include <iostream>
#include "ecco.h"

namespace {

int
static_generator(int n)
{
CO_BEGIN;
	for (static int i = 0; i < n; ++i) {
		CO_YIELD_WITH(i);
	}
CO_END_WITH(-1);
}

void
static_quack()
{
CO_BEGIN;
	for (static int i = 0; i < 3; ++i) {
		std::cout << "quack!" << std::endl;
		CO_YIELD;
	}
	throw std::runtime_error("i just can statically quack for 3 times");
CO_END;
}

int
generator(CCO_CTX_PARAM, int n)
{
CCO_CTX_DECL_BEGIN;
	int i;
CCO_CTX_DECL_END(foo);

CCO_BEGIN_WITH(foo);
	for (foo->i = 0; foo->i < n; foo->i++) {
		CCO_YIELD_WITH(foo->i);
	}
CCO_END_WITH(-1);
}

void
quack(CCO_CTX_PARAM)
{
CCO_CTX_DECL_BEGIN;
	int i;
CCO_CTX_DECL_END(foo);

CCO_BEGIN_WITH(foo);
	for (foo->i = 0; foo->i < 3; foo->i++) {
		std::cout << "quack!" << std::endl;
		CCO_YIELD;
	}
	throw std::runtime_error("i just can quack for 3 times");
CCO_END;
}

} /* namespace */

void
test_static_co()
{
	int n;
	std::cout << "[";
	while (true) {
		n = static_generator(10);
		if (n == -1) {
			break;
		}
		std::cout << n << ",";
	}
	std::cout << "\b]" << std::endl;

	try {
		while (true) static_quack();
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void
test_reentrant_co()
{
	co_ctx_t ctx = nullptr;

	int n = 0;
	std::cout << "[";
	while (true) {
		n = generator(&ctx, 10);
		if (n == -1) {
			break;
		}
		std::cout << n << ",";
	}
	std::cout << "\b]" << std::endl;

	try {
		while (true) quack(&ctx);
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	CCO_CTX_FREE(ctx);
}

int
main()
{
	test_static_co();
	test_reentrant_co();
}

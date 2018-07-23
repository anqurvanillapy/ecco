# ecco

A C/C++ coroutine library inspired by Duff's device and Simon Tatham.  The *e*
in *ecco* means *event*.

## Usage

1. Coroutine with static states.

```cpp
// foo.cc
#include <iostream>
#include "ecco.h"

int
generator(int n)
{
CO_BEGIN;
    // Use `static' for global persistent states.
    for (static int i = 0; i < n; ++i) {
        CO_YIELD_WITH(i);
    }
CO_END_WITH(-1);
}

int
main()
{
    int n;
    while (true) {
        n = generator(4);
        if (n == -1) {
            break;
        }
        std::cout << n << std::endl;
    }
    return 0;
}
```

```bash
$ c++ foo.cc; ./a.out
0
1
2
3
```

2. Coroutine with context.

```cpp
// bar.cc
#include <iostream>
#include "ecco.h"

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

int
main()
{
    co_ctx_t ctx = nullptr;
    try {
        while (true) quack(&ctx);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    CCO_CTX_FREE(ctx);
}
```

```bash
$ c++ bar.cc; ./a.out
quack!
quack!
quack!
i just can quack for 3 times
```

## License

MIT

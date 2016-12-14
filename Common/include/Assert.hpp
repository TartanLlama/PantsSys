#pragma once

#ifdef NDEBUG
#define ASSERT(expr) do { (void)sizeof(expr);} while (0)
#else
#define ASSERT(expr) assert(expr)
#endif

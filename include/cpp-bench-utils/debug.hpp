#pragma once

namespace cbu {
#if !defined(NDEBUG) || defined(_DEBUG) || defined(DEBUG)
#define IS_DEBUG 1
#else
#define IS_DEBUG 0
#endif

#if IS_DEBUG
    constexpr bool is_debug = true;
#else
    constexpr bool is_debug = false;
#endif

#undef IS_DEBUG
}

#pragma once

#include "bench.hpp"
#include "debug.hpp"
#include "matrix.hpp"
#include "validate.hpp"
#include "vector.hpp"

#ifdef __CUDACC__
#include "cuda-utils.hpp"
#endif

#ifdef __SYCL_COMPILER_VERSION
#include "sycl-utils.hpp"
#endif

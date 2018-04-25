#pragma once

#include "Core\Profiling\Profiler.h"
#include "Core\Profiling\Accumulator.h"
#include "Core\Profiling\ScopedTimer.h"

#if defined(_ENABLE_PROFILING)
#define PROFILE_UNSCOPED(name_index) \
static Accumulator myAccumulator(name_index); \
ScopedTimer myTimer(&myAccumulator);

#else
#define PROFILE_UNSCOPED(name_index) __noop
#endif

#define matrix_StressUnitTest 0
#define collisionDetection_foo 1
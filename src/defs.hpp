
#pragma once

#define DISABLE_COPY(x) \
    x(const x&) = delete; \
    x& operator =(const x&) = delete;

#define DISABLE_MOVE(x) \
    x(x&&) = delete; \
    x& operator =(x&&) = delete;

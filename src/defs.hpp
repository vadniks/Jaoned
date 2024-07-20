/*
 * Jaoned - an OpenGL & QT based drawing board
 * Copyright (C) 2024 Vadim Nikolaev (https://github.com/vadniks).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

static_assert(
    sizeof(char) == 1 &
    sizeof(short) == 2 &
    sizeof(int) == 4 &
    sizeof(float) == 4 &
    sizeof(long) == 8 &
    sizeof(double) == 8 &
    sizeof(void*) == 8
);

#define DISABLE_COPY(x) \
    x(const x&) = delete; \
    x& operator =(const x&) = delete;

#define DISABLE_MOVE(x) \
    x(x&&) = delete; \
    x& operator =(x&&) = delete;

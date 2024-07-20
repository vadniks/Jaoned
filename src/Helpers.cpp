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

#include "Helpers.hpp"
#include <QColor>

int Helpers::packQColor(const QColor& color) {
    return
        ((color.red() & 0xff) << 0) |
        ((color.green() & 0xff) << 8) |
        ((color.blue() & 0xff) << 16) |
        ((color.alpha() & 0xff) << 24);
}

QColor Helpers::unpackQColor(int color) {
    return {
        static_cast<int>((color >> 0) & 0xff),
        static_cast<int>((color >> 8) & 0xff),
        static_cast<int>((color >> 16) & 0xff),
        static_cast<int>((color >> 24) & 0xff)
    };
}

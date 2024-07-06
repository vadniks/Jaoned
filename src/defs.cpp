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

#include "defs.hpp"

uint qColorToUint(const QColor& qColor) {
    return
        ((qColor.red() & 0xff) << 0) |
        ((qColor.green() & 0xff) << 8) |
        ((qColor.blue() & 0xff) << 16) |
        ((qColor.alpha() & 0xff) << 24);
}

QColor uintToQColor(uint color) {
    return {
        static_cast<int>((color >> 0) & 0xff),
        static_cast<int>((color >> 8) & 0xff),
        static_cast<int>((color >> 16) & 0xff),
        static_cast<int>((color >> 24) & 0xff)
    };
}

QList<uchar> packVec2(const glm::vec2& vec) {
    QList<uchar> bytes(4 + 4);
    memcpy(&(bytes.data()[0]), &(vec.x), 4);
    memcpy(&(bytes.data()[4]), &(vec.y), 4);
    return bytes;
}

glm::vec2 unpackVec2(const QList<uchar>& bytes) {
    glm::vec2 vec;
    memcpy(&(vec.x), &(bytes.data()[0]), 4);
    memcpy(&(vec.y), &(bytes.data()[4]), 4);
    return vec;
}

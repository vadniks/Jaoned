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

#include "dto.hpp"

Point::Point(int x, int y) : mX(x), mY(y) {}

int Point::x() const {
    return mX;
}

int Point::y() const {
    return mY;
}

QList<char> Point::pack() {
    QList<char> bytes(4 + 4);
    memcpy(bytes.data() + 0, &mX, 4);
    memcpy(bytes.data() + 4, &mY, 4);
    return bytes;
}

Point Point::unpack(const QList<char>& bytes) {
    int x, y;
    memcpy(&x, bytes.data() + 0, 4);
    memcpy(&y, bytes.data() + 4, 4);
    return {x, y};
}

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

#include "Board.hpp"

Board::Board(int id, const QColor& color, const QString& title) :
    mId(id), mColor(color), mSize(static_cast<int>(title.size())), mTitle(title)
{
    assert(title.size() > 0 && title.size() <= MAX_TITLE_SIZE);
}

int Board::id() const {
    return mId;
}

int Board::size() const {
    return mSize;
}

QString Board::title() const {
    return mTitle;
}

QColor Board::color() const {
    return mColor;
}

QVector<uchar> Board::pack() const {
    QVector<uchar> bytes(4 + 4 + 4 + mSize);

    const uint color = qColorToUint(mColor);

    memcpy(&(bytes.data()[0]), &mId, 4);
    memcpy(&(bytes.data()[4]), &color, 4);
    memcpy(&(bytes.data()[8]), &mSize, 4);
    memcpy(&(bytes.data()[12]), mTitle.data(), mSize);

    return bytes;
}

Board Board::unpack(const QVector<uchar>& bytes) {
    int id = 0, color = 0, size = 0;

    memcpy(&id, &(bytes.data()[0]), 4);
    memcpy(&color, &(bytes.data()[4]), 4);
    memcpy(&size, &(bytes.data()[8]), 4);

    QString title(size, '\0');
    memcpy(title.data(), &(bytes.data()[8]), size);

    return {id, uintToQColor(static_cast<uint>(color)), title};
}

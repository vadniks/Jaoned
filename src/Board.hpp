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

#include "drawns.hpp"
#include <QString>
#include <QColor>

class Board final {
private:
    int mId;
    QColor mColor;
    int mSize;
    QString mTitle;
public:
    static inline const int MAX_TITLE_SIZE = 16;
    static inline const int MAX_BOARD_SIZE = 4 + 4 + 4 + MAX_TITLE_SIZE; // 28
public:
    Board(int id, const QColor& color, const QString& title);

    int id() const;
    QColor color() const;
    int size() const;
    QString title() const;

    QVector<uchar> pack() const;
    static Board unpack(const QVector<uchar>& bytes);
};

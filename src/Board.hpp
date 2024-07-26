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

#include <QString>
#include <QColor>

class Board final {
private:
    int mId;
    QColor mColor;
//    int mSize;
    QString mTitle;
public:
    static const int INVALID_ID = -1;
    static const int MAX_TITLE_SIZE = 16;
public:
    Board(int id, const QColor& color, const QString& title);

    int id() const;
    QColor color() const;
    QString title() const;

    QList<char> pack() const;
    static Board unpack(const QList<char>& bytes);
};

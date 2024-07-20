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

#include <QList>

class Point final {
private:
    int mX, mY;
public:
    Point(int x, int y);

    int x() const;
    int y() const;

    QList<char> pack() const;
    static Point unpack(const QList<char>& bytes);
};

class PointsSet final {
private:
    bool mErase;
    int mWidth;
    int mColor;
    // int mSize;
    QList<Point> mPoints;
public:
    PointsSet(bool erase, int width, int color, const QList<Point>& points);

    bool erase() const;
    int width() const;
    int color() const;
    const QList<Point>& points() const;

    QList<char> pack() const;
    static PointsSet unpack(const QList<char>& bytes);
};

class Line final {
private:
    Point mStart;
    Point mEnd;
    int mWidth;
    int mColor;
public:
    Line(Point start, Point end, int width, int color);

    Point start() const;
    Point end() const;
    int width() const;
    int color() const;

    QList<char> pack() const;
    static Line unpack(const QList<char>& bytes);
};

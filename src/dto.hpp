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

// TODO: append Dto to all class names

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

class Text final {
private:
    Point mPos;
    int mFontSize;
    int mColor;
    // int mSize;
    QList<char> mText;
public:
    Text(Point pos, int fontSize, int color, const QList<char>& text);

    Point pos() const;
    int fontSize() const;
    int color() const;
    QList<char> text() const;

    QList<char> pack() const;
    static Text unpack(const QList<char>& bytes);
};

class Image final {
private:
    Point mPos;
    int mWidth;
    int mHeight;
    // int mSize;
    QList<char> mTexture;
public:
    Image(Point pos, int width, int height, const QList<char>& texture);

    Point pos() const;
    int width() const;
    int height() const;
    const QList<char>& texture() const;

    QList<char> pack() const;
    static Image unpack(const QList<char>& bytes);
};

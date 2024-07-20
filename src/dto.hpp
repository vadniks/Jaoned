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

class PointDto final {
private:
    int mX, mY;
public:
    PointDto(int x, int y);

    int x() const;
    int y() const;

    QList<char> pack() const;
    static PointDto unpack(const QList<char>& bytes);
};

class PointsSetDto final {
private:
    bool mErase;
    int mWidth;
    int mColor;
    // int mSize;
    QList<PointDto> mPoints;
public:
    PointsSetDto(bool erase, int width, int color, const QList<PointDto>& points);

    bool erase() const;
    int width() const;
    int color() const;
    const QList<PointDto>& points() const;

    QList<char> pack() const;
    static PointsSetDto unpack(const QList<char>& bytes);
};

class LineDto final {
private:
    PointDto mStart;
    PointDto mEnd;
    int mWidth;
    int mColor;
public:
    LineDto(PointDto start, PointDto end, int width, int color);

    PointDto start() const;
    PointDto end() const;
    int width() const;
    int color() const;

    QList<char> pack() const;
    static LineDto unpack(const QList<char>& bytes);
};

class TextDto final {
private:
    PointDto mPos;
    int mFontSize;
    int mColor;
    // int mSize;
    QList<char> mText;
public:
    TextDto(PointDto pos, int fontSize, int color, const QList<char>& text);

    PointDto pos() const;
    int fontSize() const;
    int color() const;
    QList<char> text() const;

    QList<char> pack() const;
    static TextDto unpack(const QList<char>& bytes);
};

class ImageDto final {
private:
    PointDto mPos;
    int mWidth;
    int mHeight;
    // int mSize;
    QList<char> mTexture;
public:
    ImageDto(PointDto pos, int width, int height, const QList<char>& texture);

    PointDto pos() const;
    int width() const;
    int height() const;
    const QList<char>& texture() const;

    QList<char> pack() const;
    static ImageDto unpack(const QList<char>& bytes);
};

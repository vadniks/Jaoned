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

QList<char> Point::pack() const {
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

PointsSet::PointsSet(bool erase, int width, int color, const QList<Point>& points)
    : mErase(erase), mWidth(width), mColor(color), mPoints(points)
{}

bool PointsSet::erase() const {
    return mErase;
}

int PointsSet::width() const {
    return mWidth;
}

int PointsSet::color() const {
    return mColor;
}

const QList<Point>& PointsSet::points() const {
    return mPoints;
}

QList<char> PointsSet::pack() const {
    const auto size = static_cast<int>(mPoints.size());

    QList<char> bytes(1 + 4 + 4 + 4 + size * 8);

    memcpy(bytes.data() + 0, &mErase, 1);
    memcpy(bytes.data() + 1, &mWidth, 4);
    memcpy(bytes.data() + 5, &mColor, 4);
    memcpy(bytes.data() + 9, &size, 4);

    for (int i = 0; i < mPoints.size(); i++)
        memcpy(bytes.data() + 13 + i * 8, mPoints[i].pack().data(), 8);

    return bytes;
}

PointsSet PointsSet::unpack(const QList<char>& bytes) {
    bool erase;
    int width, color, size;

    memcpy(&erase, bytes.data() + 0, 1);
    memcpy(&width, bytes.data() + 1, 4);
    memcpy(&color, bytes.data() + 5, 4);
    memcpy(&size, bytes.data() + 9, 4);

    QList<Point> points;
    for (int i = 0; i < size; i++)
        points.append(Point::unpack(bytes.mid(13 + i * 8, 8)));

    return {erase, width, color, points};
}

Line::Line(Point start, Point end, int width, int color) : mStart(start), mEnd(end), mWidth(width), mColor(color) {}

Point Line::start() const {
    return mStart;
}

Point Line::end() const {
    return mEnd;
}

int Line::width() const {
    return mWidth;
}

int Line::color() const {
    return mColor;
}

QList<char> Line::pack() const {
    QList<char> bytes(8 + 8 + 4 + 4);
    memcpy(bytes.data() + 0, mStart.pack().data(), 8);
    memcpy(bytes.data() + 8, mEnd.pack().data(), 8);
    memcpy(bytes.data() + 16, &mWidth, 4);
    memcpy(bytes.data() + 20, &mColor, 4);
    return bytes;
}

Line Line::unpack(const QList<char>& bytes) {
    const auto start = Point::unpack(bytes.mid(0, 8));
    const auto end = Point::unpack(bytes.mid(8, 8));

    int width, color;
    memcpy(&width, bytes.data() + 16, 4);
    memcpy(&color, bytes.data() + 20, 4);

    return {start, end, width, color};
}

Text::Text(Point pos, int fontSize, int color, const QList<char>& text)
    : mPos(pos), mFontSize(fontSize), mColor(color), mText(text)
{}

Point Text::pos() const {
    return mPos;
}

int Text::fontSize() const {
    return mFontSize;
}

int Text::color() const {
    return mColor;
}

QList<char> Text::text() const {
    return mText;
}

QList<char> Text::pack() const {
    const auto size = static_cast<int>(mText.size());

    QList<char> bytes(8 + 4 + 4 + 4 + size);
    memcpy(bytes.data() + 0, mPos.pack().data(), 8);
    memcpy(bytes.data() + 8, &mFontSize, 4);
    memcpy(bytes.data() + 12, &mColor, 4);
    memcpy(bytes.data() + 16, &size, 4);
    memcpy(bytes.data() + 20, mText.data(), size);
    return bytes;
}

Text Text::unpack(const QList<char>& bytes) {
    const auto pos = Point::unpack(bytes.mid(0, 8));
    int fontSize, color, size;

    memcpy(&fontSize, bytes.data() + 8, 4);
    memcpy(&color, bytes.data() + 12, 4);
    memcpy(&size, bytes.data() + 16, 4);

    QList<char> text(size);
    memcpy(text.data(), bytes.data() + 20, size);

    return {pos, fontSize, color, text};
}

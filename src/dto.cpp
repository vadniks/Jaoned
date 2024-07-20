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

PointDto::PointDto(int x, int y) : mX(x), mY(y) {}

int PointDto::x() const {
    return mX;
}

int PointDto::y() const {
    return mY;
}

QList<char> PointDto::pack() const {
    QList<char> bytes(4 + 4);
    memcpy(bytes.data() + 0, &mX, 4);
    memcpy(bytes.data() + 4, &mY, 4);
    return bytes;
}

PointDto PointDto::unpack(const QList<char>& bytes) {
    int x, y;
    memcpy(&x, bytes.data() + 0, 4);
    memcpy(&y, bytes.data() + 4, 4);
    return {x, y};
}

PointsSetDto::PointsSetDto(bool erase, int width, int color, const QList<PointDto>& points)
    : mErase(erase), mWidth(width), mColor(color), mPoints(points)
{}

bool PointsSetDto::erase() const {
    return mErase;
}

int PointsSetDto::width() const {
    return mWidth;
}

int PointsSetDto::color() const {
    return mColor;
}

const QList<PointDto>& PointsSetDto::points() const {
    return mPoints;
}

QList<char> PointsSetDto::pack() const {
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

PointsSetDto PointsSetDto::unpack(const QList<char>& bytes) {
    bool erase;
    int width, color, size;

    memcpy(&erase, bytes.data() + 0, 1);
    memcpy(&width, bytes.data() + 1, 4);
    memcpy(&color, bytes.data() + 5, 4);
    memcpy(&size, bytes.data() + 9, 4);

    QList<PointDto> points;
    for (int i = 0; i < size; i++)
        points.append(PointDto::unpack(bytes.mid(13 + i * 8, 8)));

    return {erase, width, color, points};
}

LineDto::LineDto(PointDto start, PointDto end, int width, int color) : mStart(start), mEnd(end), mWidth(width), mColor(color) {}

PointDto LineDto::start() const {
    return mStart;
}

PointDto LineDto::end() const {
    return mEnd;
}

int LineDto::width() const {
    return mWidth;
}

int LineDto::color() const {
    return mColor;
}

QList<char> LineDto::pack() const {
    QList<char> bytes(8 + 8 + 4 + 4);
    memcpy(bytes.data() + 0, mStart.pack().data(), 8);
    memcpy(bytes.data() + 8, mEnd.pack().data(), 8);
    memcpy(bytes.data() + 16, &mWidth, 4);
    memcpy(bytes.data() + 20, &mColor, 4);
    return bytes;
}

LineDto LineDto::unpack(const QList<char>& bytes) {
    const auto start = PointDto::unpack(bytes.mid(0, 8));
    const auto end = PointDto::unpack(bytes.mid(8, 8));

    int width, color;
    memcpy(&width, bytes.data() + 16, 4);
    memcpy(&color, bytes.data() + 20, 4);

    return {start, end, width, color};
}

TextDto::TextDto(PointDto pos, int fontSize, int color, const QList<char>& text)
    : mPos(pos), mFontSize(fontSize), mColor(color), mText(text)
{}

PointDto TextDto::pos() const {
    return mPos;
}

int TextDto::fontSize() const {
    return mFontSize;
}

int TextDto::color() const {
    return mColor;
}

QList<char> TextDto::text() const {
    return mText;
}

QList<char> TextDto::pack() const {
    const auto size = static_cast<int>(mText.size());

    QList<char> bytes(8 + 4 + 4 + 4 + size);
    memcpy(bytes.data() + 0, mPos.pack().data(), 8);
    memcpy(bytes.data() + 8, &mFontSize, 4);
    memcpy(bytes.data() + 12, &mColor, 4);
    memcpy(bytes.data() + 16, &size, 4);
    memcpy(bytes.data() + 20, mText.data(), size);
    return bytes;
}

TextDto TextDto::unpack(const QList<char>& bytes) {
    const auto pos = PointDto::unpack(bytes.mid(0, 8));
    int fontSize, color, size;

    memcpy(&fontSize, bytes.data() + 8, 4);
    memcpy(&color, bytes.data() + 12, 4);
    memcpy(&size, bytes.data() + 16, 4);

    QList<char> text(size);
    memcpy(text.data(), bytes.data() + 20, size);

    return {pos, fontSize, color, text};
}

ImageDto::ImageDto(PointDto pos, int width, int height, const QList<char>& texture)
    : mPos(pos), mWidth(width), mHeight(height), mTexture(texture)
{}

PointDto ImageDto::pos() const {
    return mPos;
}

int ImageDto::width() const {
    return mWidth;
}

int ImageDto::height() const {
    return mHeight;
}

const QList<char>& ImageDto::texture() const {
    return mTexture;
}

QList<char> ImageDto::pack() const {
    const auto size = static_cast<int>(mTexture.size());

    QList<char> bytes(8 + 4 + 4 + 4 + size);
    memcpy(bytes.data() + 0, mPos.pack().data(), 8);
    memcpy(bytes.data() + 8, &mWidth, 4);
    memcpy(bytes.data() + 12, &mHeight, 4);
    memcpy(bytes.data() + 16, &size, 4);
    memcpy(bytes.data() + 20, mTexture.data(), size);
    return bytes;
}

ImageDto ImageDto::unpack(const QList<char>& bytes) {
    const auto pos = PointDto::unpack(bytes.mid(0, 8));
    int width, height, size;

    memcpy(&width, bytes.data() + 8, 4);
    memcpy(&height, bytes.data() + 12, 4);
    memcpy(&size, bytes.data() + 16, 4);

    QList<char> texture(size);
    memcpy(texture.data(), bytes.data() + 20, size);

    return {pos, width, height, texture};
}

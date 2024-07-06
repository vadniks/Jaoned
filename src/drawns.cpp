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

#include "drawns.hpp"

// TODO: replace QVector with QList

QList<uchar> DrawnPointsSet::pack() {
    QList<uchar> bytes(4 + 4 + 8 * points.size());

    memcpy(&(bytes.data()[0]), &width, 4);

    const uint xColor = qColorToUint(color);
    memcpy(&(bytes.data()[4]), &xColor, 4);

    for (int i = 0; i < points.size(); i++)
        memcpy(&(bytes.data()[8 + i * 2]), packVec2(points[i]).data(), 8);

    return bytes;
}

DrawnPointsSet* DrawnPointsSet::unpack(const QList<uchar>& bytes) {
    int width;
    memcpy(&width, &(bytes.data()[0]), 4);

    uint color;
    memcpy(&color, &(bytes.data()[4]), 4);

    QList<glm::vec2> points;
    for (int i = 8; i < bytes.size() - 8; i += 8)
        points.append(unpackVec2(bytes.mid(i, i + 8)));

    auto pointsSet = new DrawnPointsSet(width, uintToQColor(color));
    pointsSet->points = points;
    return pointsSet;
}

QList<uchar> DrawnLine::pack() {
    QList<uchar> bytes(8 + 8 + 4 + 4);

    memcpy(&(bytes.data()[0]), packVec2(start).data(), 8);
    memcpy(&(bytes.data()[8]), packVec2(end).data(), 8);
    memcpy(&(bytes.data()[16]), &width, 4);

    const uint xColor = qColorToUint(color);
    memcpy(&(bytes.data()[16]), &xColor, 4);

    return bytes;
}

DrawnLine* DrawnLine::unpack(const QList<uchar>& bytes) {
    const glm::vec2 start = unpackVec2(bytes.mid(0, 8));
    glm::vec2 end = unpackVec2(bytes.mid(8, 16));

    int width;
    memcpy(&width, &(bytes.data()[16]), 4);

    uint color;
    memcpy(&color, &(bytes.data()[20]), 4);

    return new DrawnLine(start, end, width, uintToQColor(color));
}

QList<uchar> DrawnText::pack() {
    QList<uchar> bytes(8 + 4 + 4 + text.size());

    memcpy(&(bytes.data()[0]), packVec2(pos).data(), 8);
    memcpy(&(bytes.data()[8]), &size, 4);

    const auto xColor = qColorToUint(color);
    memcpy(&(bytes.data()[12]), &xColor, 4);

    memcpy(&(bytes.data()[16]), text.data(), text.size());

    return bytes;
}

DrawnText* DrawnText::unpack(const QList<uchar>& bytes) {
    const glm::vec2 pos = unpackVec2(bytes.mid(0, 8));

    int size;
    memcpy(&size, &(bytes.data()[8]), 4);

    uint color;
    memcpy(&color, &(bytes.data()[12]), 4);

    QList<uchar> text = bytes.mid(16);
    QByteArray xText(text.size(), 0);
    memcpy(xText.data(), text.data(), text.size());

    return new DrawnText(pos, size, uintToQColor(color), QString(xText));
}

QList<uchar> DrawnImage::pack() {
//    QList<uchar> bytes(8 + 8 + );
}

DrawnImage* DrawnImage::unpack(const QList<uchar>& bytes) {
    return nullptr;
}

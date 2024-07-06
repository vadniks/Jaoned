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

#include "defs.hpp"
#include "Texture.hpp"
#include <QColor>
#include <glm/glm.hpp>

struct DrawnElement { // abstract
protected:
    DrawnElement() {}
public:
    virtual ~DrawnElement() = default;

    DISABLE_COPY(DrawnElement)
    DISABLE_MOVE(DrawnElement)
};

struct DrawnPointsSet final : public DrawnElement {
    int width;
    QColor color;
    QList<glm::vec2> points;

    DrawnPointsSet(int width, const QColor& color) : width(width), color(color), points() {}
    ~DrawnPointsSet() override = default;

    QList<uchar> pack();
    static DrawnPointsSet* unpack(const QList<uchar>& bytes);

    DISABLE_COPY(DrawnPointsSet)
    DISABLE_MOVE(DrawnPointsSet)
};

struct DrawnLine final : public DrawnElement {
    glm::vec2 start;
    glm::vec2 end;
    int width;
    QColor color;

    DrawnLine(const glm::vec2& start, const glm::vec2 end, int width, const QColor& color) : start(start), end(end), width(width), color(color) {}
    ~DrawnLine() override = default;

    QList<uchar> pack();
    static DrawnLine* unpack(const QList<uchar>& bytes);

    DISABLE_COPY(DrawnLine)
    DISABLE_MOVE(DrawnLine)
};

struct DrawnText final : public DrawnElement {
    glm::vec2 pos;
    int size;
    QColor color;
    QString text;

    DrawnText(const glm::vec2& pos, int size, const QColor& color, const QString& text) : pos(pos), size(size), color(color), text(text) {}
    ~DrawnText() override = default;

    QList<uchar> pack();
    static DrawnText* unpack(const QList<uchar>& bytes);

    DISABLE_COPY(DrawnText)
    DISABLE_MOVE(DrawnText)
};

struct DrawnImage final : public DrawnElement {
    glm::vec2 pos;
    glm::vec2 size;
    Texture* texture;

    DrawnImage(const glm::vec2& pos, const glm::vec2& size, Texture* texture) : pos(pos), size(size), texture(texture) {}

    ~DrawnImage() override {
        delete texture;
    }

    QList<uchar> pack();
    static DrawnImage* unpack(const QList<uchar>& bytes);

    DISABLE_COPY(DrawnImage)
    DISABLE_MOVE(DrawnImage)
};

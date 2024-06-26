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
#include <QOpenGLFunctions_3_3_Core>

class Texture final {
private:
    QOpenGLFunctions_3_3_Core& mGl;
    unsigned mId;
    int mWidth, mHeight;
public:
    Texture(QOpenGLFunctions_3_3_Core& gl, int width, int height, const uchar* data, int format = GL_RGBA);
    ~Texture();

    DISABLE_COPY(Texture)
    DISABLE_MOVE(Texture)

    void bind();
    QSize size();
};

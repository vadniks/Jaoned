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

#include "Texture.hpp"
#include <QSize>

Texture::Texture(QOpenGLFunctions_3_3_Core& gl, int width, int height, const uchar* data, int format) :
    mGl(gl),
    mId(0),
    mWidth(width),
    mHeight(height),
    mData(width * height, 0)
{
    assert(format == GL_RED || format == GL_RGB || format == GL_RGBA);

    mGl.glGenTextures(1, &mId);
    mGl.glBindTexture(GL_TEXTURE_2D, mId);

    if (format == GL_RED)
        mGl.glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    mGl.glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );

    mGl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    mGl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    mGl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    mGl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    mGl.glBindTexture(GL_TEXTURE_2D, 0);

    memcpy(mData.data(), data, width * height);
}

Texture::~Texture() {
    mGl.glDeleteTextures(1, &mId);
}

void Texture::bind() {
    mGl.glBindTexture(GL_TEXTURE_2D, mId);
}

QSize Texture::size() {
    return {mWidth, mHeight};
}

QList<uchar> Texture::data() {
    return mData;
}

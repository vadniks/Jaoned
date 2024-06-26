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
#include "CompoundShader.hpp"
#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <freetype2/ft2build.h>
#include <freetype/freetype.h>

class Renderer final {
private:
    QOpenGLFunctions_3_3_Core& mGl;
    CompoundShader* mShapeShader, * mSpriteShader;
    unsigned mVbo, mEbo, mVao;
    glm::mat4 mProjection;
    FT_Library mFtLib;
    FT_Face mFtFace;
public:
    explicit Renderer(QOpenGLFunctions_3_3_Core& gl);
    ~Renderer();

    DISABLE_COPY(Renderer)
    DISABLE_MOVE(Renderer)

    void setProjection(const glm::mat4& projection);

    void drawPoint(const glm::vec2& position, float pointSize, const glm::vec4& color);
    void drawPoints(int count, const QVector<float>& vertices, float pointSize, const glm::vec4& color, int drawMode);
    void drawLine(const glm::vec2& positionStart, const glm::vec2& positionEnd, float lineWidth, const glm::vec4& color);
    void drawHollowCircle(const glm::vec2& positionCenter, int radius, const glm::vec4& color);
    void drawTexture(Texture& texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, bool isMono = false);
    void drawText(const QString& text, int size, const glm::vec2& position, const glm::vec4& color);
    QSize textMetrics(const QString& text, int size);
};

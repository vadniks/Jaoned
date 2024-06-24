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
#include <glm/glm.hpp>

class CompoundShader final {
private:
    QOpenGLFunctions_3_3_Core& mGl;
    unsigned mProgramId;
public:
    CompoundShader(QOpenGLFunctions_3_3_Core& gl, const QString& vertexCode, const QString& fragmentCode);
    ~CompoundShader();

    DISABLE_COPY(CompoundShader)
    DISABLE_MOVE(CompoundShader)

    void use();
    void setValue(const QString& name, bool value);
    void setValue(const QString& name, float value);
    void setValue(const QString& name, int value);
    void setValue(const QString& name, const glm::vec2& value);
    void setValue(const QString& name, const glm::vec3& value);
    void setValue(const QString& name, const glm::vec4& value);
    void setValue(const QString& name, const glm::mat3& value);
    void setValue(const QString& name, const glm::mat4& value);
};

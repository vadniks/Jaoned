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

#include "CompoundShader.hpp"
#include <glm/gtc/type_ptr.hpp>

CompoundShader::CompoundShader(QOpenGLFunctions_3_3_Core& gl, const QString& vertexCode, const QString& fragmentCode) : mGl(gl), mProgramId(0) {
    int success;
    unsigned vertex = mGl.glCreateShader(GL_VERTEX_SHADER);
    mGl.glShaderSource(vertex, 1, (const char*[1]) {vertexCode.toStdString().c_str()}, nullptr);
    mGl.glCompileShader(vertex);
    mGl.glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    assert(success == GL_TRUE);

    unsigned fragment = mGl.glCreateShader(GL_FRAGMENT_SHADER);
    mGl.glShaderSource(fragment, 1, (const char*[1]) {fragmentCode.toStdString().c_str()}, nullptr);
    mGl.glCompileShader(fragment);
    mGl.glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    assert(success == GL_TRUE);

    mProgramId = mGl.glCreateProgram();
    mGl.glAttachShader(mProgramId, vertex);
    mGl.glAttachShader(mProgramId, fragment);
    mGl.glLinkProgram(mProgramId);
    mGl.glGetProgramiv(mProgramId, GL_LINK_STATUS, &success);
    assert(success == GL_TRUE);

    mGl.glDeleteShader(vertex);
    mGl.glDeleteShader(fragment);
}

CompoundShader::~CompoundShader() {
    mGl.glDeleteProgram(mProgramId);
}

void CompoundShader::use() {
    mGl.glUseProgram(mProgramId);
}

void CompoundShader::setValue(const QString& name, bool value) {
    mGl.glUniform1i(mGl.glGetUniformLocation(mProgramId, name.toStdString().c_str()), (int) value);
}

void CompoundShader::setValue(const QString& name, float value) {
    mGl.glUniform1f(mGl.glGetUniformLocation(mProgramId, name.toStdString().c_str()), value);
}

void CompoundShader::setValue(const QString& name, int value) {
    mGl.glUniform1i(mGl.glGetUniformLocation(mProgramId, name.toStdString().c_str()), value);
}

void CompoundShader::setValue(const QString& name, const glm::vec2& value) {
    mGl.glUniform2f(mGl.glGetUniformLocation(mProgramId, name.toStdString().c_str()), value.x, value.y);
}

void CompoundShader::setValue(const QString& name, const glm::vec3& value) {
    mGl.glUniform3f(mGl.glGetUniformLocation(mProgramId, name.toStdString().c_str()), value.x, value.y, value.z);
}

void CompoundShader::setValue(const QString& name, const glm::vec4& value) {
    mGl.glUniform4f(mGl.glGetUniformLocation(mProgramId, name.toStdString().c_str()), value.x, value.y, value.z, value.w);
}

void CompoundShader::setValue(const QString& name, const glm::mat3& value) {
    mGl.glUniformMatrix3fv(mGl.glGetUniformLocation(mProgramId, name.toStdString().c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void CompoundShader::setValue(const QString& name, const glm::mat4& value) {
    mGl.glUniformMatrix4fv(mGl.glGetUniformLocation(mProgramId, name.toStdString().c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

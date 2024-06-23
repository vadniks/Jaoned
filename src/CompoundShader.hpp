
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


#pragma once

#include "defs.hpp"
#include "Texture.hpp"
#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>

class Renderer {
private:
    QOpenGLFunctions_3_3_Core& mGl;
public:
    explicit Renderer(QOpenGLFunctions_3_3_Core& gl);
    ~Renderer();

    DISABLE_COPY(Renderer)
    DISABLE_MOVE(Renderer)

    void drawPoint(const glm::vec2& position, float pointSize, const glm::vec4& color);
    void drawLine(const glm::vec2& positionStart, const glm::vec2& positionEnd, float lineWidth, const glm::vec4& color);
    void drawRectangle(const glm::vec2& position, const glm::vec2& size, float lineWidth, const glm::vec4& color, bool filled);
    void drawCircle(const glm::vec2& positionCenter, int radius, float pointSize, const glm::vec4& color, bool filled);
    void drawTexture(Texture& texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
};

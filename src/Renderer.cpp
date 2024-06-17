
#include "Renderer.hpp"

Renderer::Renderer(QOpenGLFunctions_3_3_Core& gl) : mGl(gl) {

}

Renderer::~Renderer() {

}

void Renderer::drawPoint(const glm::vec2& position, float pointSize, const glm::vec4& color) {

}

void Renderer::drawLine(const glm::vec2& positionStart, const glm::vec2& positionEnd, float lineWidth, const glm::vec4& color) {

}

void Renderer::drawRectangle(const glm::vec2& position, const glm::vec2& size, float lineWidth, const glm::vec4& color, bool filled) {

}

void Renderer::drawCircle(const glm::vec2& positionCenter, int radius, float pointSize, const glm::vec4& color, bool filled) {

}

void Renderer::drawTexture(Texture& texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {

}

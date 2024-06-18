
#include "BoardWidget.hpp"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

BoardWidget::BoardWidget() : mRenderer(nullptr) {

}

BoardWidget::~BoardWidget() {
    delete mRenderer;
}

QSize BoardWidget::minimumSizeHint() const {
    return {16 * 75, 9 * 75};
}

QSize BoardWidget::sizeHint() const {
    return minimumSizeHint();
}

void BoardWidget::initializeGL() {
    QOpenGLFunctions_3_3_Core::initializeOpenGLFunctions();
    mRenderer = new Renderer(*this);
    glViewport(0, 0, 100, 100);
}

void BoardWidget::paintGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    const auto size = QOpenGLWidget::size();
    const glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(size.width()), static_cast<float>(size.height()), 0.0f, -1.0f, 1.0f);
    mRenderer->setProjection(projection);

    mRenderer->drawRectangle(glm::vec2(10, 10), glm::vec2(100, 100), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), false);
}

void BoardWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

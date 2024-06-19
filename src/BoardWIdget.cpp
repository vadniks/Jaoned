
#include "BoardWidget.hpp"
#include <QFontMetrics>
#include <QPainter>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

BoardWidget::BoardWidget() : mProjection(1.0f), mRenderer(nullptr) {

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

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

    glViewport(0, 0, 100, 100);
}

void BoardWidget::paintGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mRenderer->drawRectangle(glm::vec2(100, 100), glm::vec2(100, 800), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), false);

    mRenderer->drawText("Hello gWorld!", 32, glm::vec2(100, 100), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void BoardWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    mProjection = glm::ortho(0.0f, static_cast<float>(w), static_cast<float>(h), 0.0f, -1.0f, 1.0f);
    mRenderer->setProjection(mProjection);
}


#include "GLWidget.hpp"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

GLWidget::GLWidget() : mContext(new QOpenGLContext(this)) {
    mContext->create();
}

GLWidget::~GLWidget() {
    delete mContext;
}

QSize GLWidget::minimumSizeHint() const {
    return {16 * 75, 9 * 75};
}

QSize GLWidget::sizeHint() const {
    return minimumSizeHint();
}

void GLWidget::initializeGL() {
    QOpenGLFunctions_3_3_Core::initializeOpenGLFunctions();
    glViewport(0, 0, 100, 100);
}

void GLWidget::paintGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    const auto size = QOpenGLWidget::size();
    const glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(size.width()), static_cast<float>(size.height()), 0.0f, -1.0f, 1.0f);


}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

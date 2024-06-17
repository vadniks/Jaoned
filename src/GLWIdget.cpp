
#include "GLWidget.hpp"

GLWidget::GLWidget() {

}

GLWidget::~GLWidget() {

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
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

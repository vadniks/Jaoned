
#include "BoardWidget.hpp"
#include <QKeyEvent>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

BoardWidget::BoardWidget() : mProjection(1.0f), mRenderer(nullptr), mOffsetX(0), mOffsetY(0), mMousePressed(false), mMouseDrawnPoints() {
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

BoardWidget::~BoardWidget() {
    delete mRenderer;
}

QSize BoardWidget::minimumSizeHint() const {
    return {16 * 75, 9 * 75};
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

    for (const auto& i : mMouseDrawnPoints)
        mRenderer->drawPoint(glm::vec2(static_cast<float>(i.x), static_cast<float>(i.y)), 5, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void BoardWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    updateProjection(w, h);
}

void BoardWidget::keyPressEvent(QKeyEvent* event) {
    const auto step = 10;
    switch (event->key()) {
        case Qt::Key::Key_W:
            mOffsetY -= step;
            break;
        case Qt::Key::Key_A:
            mOffsetX -= step;
            break;
        case Qt::Key::Key_S:
            mOffsetY += step;
            break;
        case Qt::Key::Key_D:
            mOffsetX += step;
            break;
    }

    updateProjection(size().width(), size().height());
    update();
}

void BoardWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!mMousePressed) return;

    const auto pos = event->pos();
    mMouseDrawnPoints.push_back(glm::ivec2(pos.x() + mOffsetX, pos.y() + mOffsetY));

    update();
}

void BoardWidget::mousePressEvent(QMouseEvent* event) {
    mMousePressed = true;
}

void BoardWidget::mouseReleaseEvent(QMouseEvent* event) {
    mMousePressed = false;
}

void BoardWidget::updateProjection(int width, int height) {
    mProjection = glm::ortho(
        0.0f + static_cast<float>(mOffsetX),
        static_cast<float>(width + mOffsetX),
        static_cast<float>(height + mOffsetY),
        0.0f + static_cast<float>(mOffsetY),
        -1.0f,
        1.0f
    );
    mRenderer->setProjection(mProjection);
}


#include "BoardWidget.hpp"
#include <QKeyEvent>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

BoardWidget::BoardWidget() :
    mProjection(1.0f),
    mRenderer(nullptr),
    mOffsetX(0),
    mOffsetY(0),
    mMouseDrawnPoints(),
    mCurrentMouseDrawnPoints(nullptr)
{
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

BoardWidget::~BoardWidget() {
    for (auto i : mMouseDrawnPoints)
        delete i;

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

    if (mCurrentMouseDrawnPoints != nullptr) {
        for (const auto& i: *mCurrentMouseDrawnPoints)
            mRenderer->drawPoint(glm::vec2(static_cast<float>(i.x), static_cast<float>(i.y)), 1, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    for (auto pointsSet : mMouseDrawnPoints) {
        int j = 0;
        for (const auto& i : *pointsSet) {
            if (j < pointsSet->size() - 1)
                mRenderer->drawLine(glm::vec2(static_cast<float>(i.x), static_cast<float>(i.y)), glm::vec2(static_cast<float>(pointsSet->operator[](j + 1).x), static_cast<float>(pointsSet->operator[](j + 1).y)), 5.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            j++;
        }
    }
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
    if (mCurrentMouseDrawnPoints == nullptr) return;

    const auto pos = event->pos();
    mCurrentMouseDrawnPoints->push_back(glm::ivec2(pos.x() + mOffsetX, pos.y() + mOffsetY));

    update();
}

void BoardWidget::mousePressEvent(QMouseEvent* event) {
    mCurrentMouseDrawnPoints = new QVector<glm::ivec2>();
}

void BoardWidget::mouseReleaseEvent(QMouseEvent* event) {
    mMouseDrawnPoints.push_back(mCurrentMouseDrawnPoints);
    mCurrentMouseDrawnPoints = nullptr;
    update();
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

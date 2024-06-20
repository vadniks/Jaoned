
#include "BoardWidget.hpp"
#include <QKeyEvent>
#include <QPainter>

BoardWidget::BoardWidget() :
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
}

QSize BoardWidget::minimumSizeHint() const {
    return {16 * 75, 9 * 75};
}

void BoardWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setPen(QPen(QColor(255, 255, 255)));
    painter.setBrush(QBrush(QColor(255, 255, 255)));

    if (mCurrentMouseDrawnPoints != nullptr) {
        for (const auto& i: *mCurrentMouseDrawnPoints)
            painter.drawPoint(i.x, i.y);
    }

    for (auto pointsSet : mMouseDrawnPoints) {
        int j = 0;
        for (const auto& i : *pointsSet) {
            if (j < pointsSet->size() - 1)
                painter.drawLine(i.x, i.y, pointsSet->operator[](j + 1).x, pointsSet->operator[](j + 1).y);
//                mRenderer->drawLine(glm::vec2(static_cast<float>(i.x), static_cast<float>(i.y)), glm::vec2(static_cast<float>(pointsSet->operator[](j + 1).x), static_cast<float>(pointsSet->operator[](j + 1).y)), 5.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            j++;
        }
    }
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

    update();
}

void BoardWidget::mouseMoveEvent(QMouseEvent* event) {
    if (mCurrentMouseDrawnPoints == nullptr) return;

    const auto pos = event->pos();
    mCurrentMouseDrawnPoints->push_back(Coordinate(pos.x() + mOffsetX, pos.y() + mOffsetY));

    update();
}

void BoardWidget::mousePressEvent(QMouseEvent* event) {
    mCurrentMouseDrawnPoints = new QVector<Coordinate>();
}

void BoardWidget::mouseReleaseEvent(QMouseEvent* event) {
    mMouseDrawnPoints.push_back(mCurrentMouseDrawnPoints);
    mCurrentMouseDrawnPoints = nullptr;

    update();
}

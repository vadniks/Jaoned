
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
    painter.setRenderHints(QPainter::RenderHint::Antialiasing | QPainter::RenderHint::TextAntialiasing);

    painter.setPen(QPen(QColor(0, 0, 0), 1));
    painter.setBrush(QBrush(QColor(0, 0, 0)));
    painter.drawRect(0, 0, size().width(), size().height());

    painter.setPen(QPen(QColor(255, 255, 255), 5));
    painter.setBrush(QBrush(QColor(0, 0, 0)));

    if (mCurrentMouseDrawnPoints != nullptr) {
        for (const auto& i: *mCurrentMouseDrawnPoints)
            painter.drawPoint(i.x + mOffsetX, i.y + mOffsetY);
    }

    for (auto pointsSet : mMouseDrawnPoints) {
        int j = 0;
        for (const auto& i : *pointsSet) {
            if (j < pointsSet->size() - 1)
                painter.drawLine(i.x + mOffsetX, i.y + mOffsetY, pointsSet->operator[](j + 1).x + mOffsetX, pointsSet->operator[](j + 1).y + mOffsetY);
            j++;
        }
    }

    QWidget::paintEvent(event);
}

void BoardWidget::keyPressEvent(QKeyEvent* event) {
    const auto step = 10;

    switch (event->key()) {
        case Qt::Key::Key_W:
            mOffsetY += step;
            break;
        case Qt::Key::Key_A:
            mOffsetX += step;
            break;
        case Qt::Key::Key_S:
            mOffsetY -= step;
            break;
        case Qt::Key::Key_D:
            mOffsetX -= step;
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

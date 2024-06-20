
#include "BoardWidget.hpp"
#include <QKeyEvent>
#include <QPainter>

BoardWidget::Coordinate::Coordinate(int x, int y) : x(x), y(y) {}

BoardWidget::LineCoordinates::LineCoordinates(Coordinate start, Coordinate end) : start(start), end(end) {}

BoardWidget::BoardWidget() :
    mMode(Mode::DRAW),
    mColor(static_cast<int>(0xffffffff)),
    mWidth(5),
    mOffsetX(0),
    mOffsetY(0),
    mMouseDrawnPoints(),
    mCurrentMouseDrawnPoints(nullptr),
    mLines(),
    mCurrentLine(nullptr)
{
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

BoardWidget::~BoardWidget() {
    for (auto i : mMouseDrawnPoints)
        delete i;

    for (auto i : mLines)
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

    QColor color(
        (mColor >> 0) & 0xff,
        (mColor >> 8) & 0xff,
        (mColor >> 16) & 0xff,
        (mColor >> 24) & 0xff
    );
    painter.setPen(QPen(color, mWidth));
    painter.setBrush(QBrush(color));

    paintDrawn(painter);
    paintLines(painter);

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
    switch (mMode) {
        case Mode::DRAW:
            if (mCurrentMouseDrawnPoints == nullptr) break;

            const auto pos = event->pos();
            mCurrentMouseDrawnPoints->push_back(Coordinate(pos.x() + mOffsetX, pos.y() + mOffsetY));
            break;
    }

    update();
}

void BoardWidget::mousePressEvent(QMouseEvent* event) {
    switch (mMode) {
        case Mode::DRAW:
            mCurrentMouseDrawnPoints = new QVector<Coordinate>();
            break;
    }
}

void BoardWidget::mouseReleaseEvent(QMouseEvent* event) {
    switch (mMode) {
        case Mode::DRAW:
            mMouseDrawnPoints.push_back(mCurrentMouseDrawnPoints);
            mCurrentMouseDrawnPoints = nullptr;
            break;
    }

    update();
}

void BoardWidget::paintDrawn(QPainter& painter) {
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
}

void BoardWidget::paintLines(QPainter& painter) {
    if (mCurrentLine != nullptr)
        painter.drawLine(mCurrentLine->start.x, mCurrentLine->start.y, mCurrentLine->end.x, mCurrentLine->end.y);
}

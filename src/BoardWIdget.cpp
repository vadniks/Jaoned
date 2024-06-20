
#include "BoardWidget.hpp"
#include <QKeyEvent>
#include <QPainter>

BoardWidget::Coordinate::Coordinate(int x, int y) : x(x), y(y) {}

BoardWidget::LineCoordinates::LineCoordinates(Coordinate start, Coordinate end) : start(start), end(end) {}

BoardWidget::BoardWidget() :
    mMode(Mode::DRAW),
    mTheme(true),
    mColor(static_cast<int>(0xffffffff)),
    mPointWidth(5),
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
    painter.setBrush(QBrush(mTheme ? QColor(0, 0, 0) : QColor(255, 255, 255)));
    painter.drawRect(0, 0, size().width(), size().height());

    QColor color(
        (mColor >> 0) & 0xff,
        (mColor >> 8) & 0xff,
        (mColor >> 16) & 0xff,
        (mColor >> 24) & 0xff
    );
    painter.setPen(QPen(color, mPointWidth));
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
            mCurrentMouseDrawnPoints->push_back(Coordinate(event->pos().x() + mOffsetX, event->pos().y() + mOffsetY));
            break;
        case Mode::LINE:
            if (mCurrentLine == nullptr) break;
            mCurrentLine->end.x = event->pos().x();
            mCurrentLine->end.y = event->pos().y();
            break;
    }

    update();
}

void BoardWidget::mousePressEvent(QMouseEvent* event) {
    switch (mMode) {
        case Mode::DRAW:
            mCurrentMouseDrawnPoints = new QVector<Coordinate>();
            break;
        case Mode::LINE:
            Coordinate coordinate(event->pos().x() + mOffsetX, event->pos().y() + mOffsetY);
            mCurrentLine = new LineCoordinates(coordinate, coordinate);
            break;
    }
}

void BoardWidget::mouseReleaseEvent(QMouseEvent* event) {
    switch (mMode) {
        case Mode::DRAW:
            mMouseDrawnPoints.push_back(mCurrentMouseDrawnPoints);
            mCurrentMouseDrawnPoints = nullptr;
            break;
        case Mode::LINE:
            mLines.push_back(mCurrentLine);
            mCurrentLine = nullptr;
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

    for (const auto& i : mLines)
        painter.drawLine(i->start.x, i->start.y, i->end.x, i->end.y);
}

void BoardWidget::setMode(Mode mode) {
    mMode = mode;
}

void BoardWidget::setTheme(bool theme) {
    mTheme = theme;
}

void BoardWidget::setColor(int color) {
    mColor = color;
}

void BoardWidget::setPointWidth(int width) {
    mPointWidth = width;
}

Mode BoardWidget::mode() const {
    return mMode;
}

bool BoardWidget::theme() const {
    return mTheme;
}

int BoardWidget::color() const {
    return mColor;
}

int BoardWidget::pointWidth() const {
    return mPointWidth;
}

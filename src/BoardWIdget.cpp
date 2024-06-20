
#include "BoardWidget.hpp"
#include <QKeyEvent>
#include <QPainter>
#include <glm/ext/matrix_clip_space.hpp>

BoardWidget::Coordinate::Coordinate(int x, int y) : x(x), y(y) {}

BoardWidget::LineCoordinates::LineCoordinates(Coordinate start, Coordinate end) : start(start), end(end) {}

BoardWidget::BoardWidget() :
    mMode(Mode::DRAW),
    mTheme(true),
    mColor(static_cast<int>(0xffffffff)),
    mPointWidth(5),
    mProjection(1.0f),
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
            Coordinate coordinate(event->pos().x(), event->pos().y());
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

void BoardWidget::updateProjection(int width, int height) {
    mProjection = glm::ortho(
        0.0f + static_cast<float>(mOffsetX),
        static_cast<float>(width + mOffsetX),
        static_cast<float>(height + mOffsetY),
        0.0f + static_cast<float>(mOffsetY),
        -1.0f,
        1.0f
    );
}

void BoardWidget::paintDrawn(QPainter& painter) {
    if (mCurrentMouseDrawnPoints != nullptr) {
        for (const auto& i: *mCurrentMouseDrawnPoints) {
            auto pos = glm::vec4(static_cast<float>(i.x), static_cast<float>(i.y), 0.0f, 1.0f);
            pos = mProjection * pos;
            pos.x = (pos.x + 1) * (static_cast<float>(size().width()) / 2.0f) + 0.0f;
            pos.y = (pos.y + 1) * (static_cast<float>(size().width()) / 2.0f) + 0.0f;

            painter.drawPoint(static_cast<int>(pos.x), static_cast<int>(pos.y));
        }
    }

    for (auto pointsSet : mMouseDrawnPoints) {
        int j = 0;
        for (const auto& i : *pointsSet) {
            if (j < pointsSet->size() - 1) {
                auto startPos = glm::vec4(static_cast<float>(i.x), static_cast<float>(i.y), 0.0f, 1.0f);
                startPos = mProjection * startPos;
                startPos.x = (startPos.x + 1) * (static_cast<float>(size().width()) / 2.0f) + 0.0f;
                startPos.y = (startPos.y + 1) * (static_cast<float>(size().width()) / 2.0f) + 0.0f;

                auto endPos = glm::vec4(static_cast<float>(pointsSet->operator[](j + 1).x), static_cast<float>(pointsSet->operator[](j + 1).y), 0.0f, 1.0f);
                endPos = mProjection * startPos;
                endPos.x = (endPos.x + 1) * (static_cast<float>(size().width()) / 2.0f) + 0.0f;
                endPos.y = (endPos.y + 1) * (static_cast<float>(size().width()) / 2.0f) + 0.0f;

                painter.drawLine(static_cast<int>(startPos.x), static_cast<int>(startPos.y), static_cast<int>(endPos.x), static_cast<int>(endPos.y));
            }
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

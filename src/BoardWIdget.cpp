
#include "BoardWidget.hpp"
#include <QKeyEvent>
#include <glm/ext/matrix_clip_space.hpp>

BoardWidget::LineCoordinates::LineCoordinates(const glm::vec2& start, const glm::vec2& end) : start(start), end(end) {}

BoardWidget::BoardWidget() :
    mMode(Mode::DRAW),
    mTheme(true),
    mColor(static_cast<int>(0xffffffff)),
    mPointWidth(5),
    mProjection(1.0f),
    mRenderer(nullptr),
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

    delete mRenderer;
}

QSize BoardWidget::minimumSizeHint() const {
    return {16 * 75, 9 * 75};
}

void BoardWidget::initializeGL() {
    QOpenGLFunctions_3_3_Core::initializeOpenGLFunctions();
    mRenderer = new Renderer(*this);
    updateProjection();

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

    glViewport(0, 0, 100, 100);
}

void BoardWidget::paintGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    paintDrawn();
    paintLines();
}

void BoardWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    updateProjection();
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

    updateProjection();
    update();
}

void BoardWidget::mouseMoveEvent(QMouseEvent* event) {
    const int x = event->pos().x();
    const int y = event->pos().y();

    switch (mMode) {
        case Mode::DRAW:
            if (mCurrentMouseDrawnPoints == nullptr) break;
            mCurrentMouseDrawnPoints->push_back(glm::vec2(static_cast<float>(x + mOffsetX), static_cast<float>(y + mOffsetY)));
            break;
        case Mode::LINE:
            if (mCurrentLine == nullptr) break;
            mCurrentLine->end.x = static_cast<float>(x + mOffsetX);
            mCurrentLine->end.y = static_cast<float>(y + mOffsetY);
            break;
    }

    update();
}

void BoardWidget::mousePressEvent(QMouseEvent* event) {
    switch (mMode) {
        case Mode::DRAW:
            mCurrentMouseDrawnPoints = new QVector<glm::vec2>();
            break;
        case Mode::LINE:
            glm::vec2 start(static_cast<float>(event->pos().x() + mOffsetX), static_cast<float>(event->pos().y() + mOffsetY));
            mCurrentLine = new LineCoordinates(start, start);
            break;
    }
}

void BoardWidget::mouseReleaseEvent(QMouseEvent*) {
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

void BoardWidget::updateProjection() {
    const auto xSize = size();

    mProjection = glm::ortho(
        0.0f + static_cast<float>(mOffsetX),
        static_cast<float>(xSize.width() + mOffsetX),
        static_cast<float>(xSize.height() + mOffsetY),
        0.0f + static_cast<float>(mOffsetY),
        -1.0f,
        1.0f
    );

    mRenderer->setProjection(mProjection);
}

void BoardWidget::paintDrawn() {
    const glm::vec4 color = makeGlColor();

    if (mCurrentMouseDrawnPoints != nullptr) {
        for (const auto& i: *mCurrentMouseDrawnPoints) {
            const auto pos = glm::vec2(static_cast<float>(i.x), static_cast<float>(i.y));
            mRenderer->drawPoint(pos, static_cast<float>(mPointWidth), color);
        }
    }

    for (auto pointsSet : mMouseDrawnPoints) {
        int j = 0;
        for (const auto& i : *pointsSet) {
            if (j < pointsSet->size() - 1) {
                const auto startPos = glm::vec2(static_cast<float>(i.x), static_cast<float>(i.y));
                const auto endPos = glm::vec2(static_cast<float>(pointsSet->operator[](j + 1).x), static_cast<float>(pointsSet->operator[](j + 1).y));
                mRenderer->drawLine(startPos, endPos, static_cast<float>(mPointWidth), color);
            }
            j++;
        }
    }
}

void BoardWidget::paintLines() {
    const glm::vec4 color = makeGlColor();

    if (mCurrentLine != nullptr) {
        const auto startPos = glm::vec2(static_cast<float>(mCurrentLine->start.x), static_cast<float>(mCurrentLine->start.y));
        const auto endPos = glm::vec2(static_cast<float>(mCurrentLine->end.x), static_cast<float>(mCurrentLine->end.y));
        mRenderer->drawLine(startPos, endPos, static_cast<float>(mPointWidth), color);
    }

    for (const auto& i : mLines) {
        const auto startPos = glm::vec2(static_cast<float>(i->start.x), static_cast<float>(i->start.y));
        const auto endPos = glm::vec2(static_cast<float>(i->end.x), static_cast<float>(i->end.y));
        mRenderer->drawLine(startPos, endPos, static_cast<float>(mPointWidth), color);
    }
}

glm::vec4 BoardWidget::makeGlColor() const {
    return {
        static_cast<float>((mColor >> 0) & 0xff) / 255.0f,
        static_cast<float>((mColor >> 8) & 0xff) / 255.0f,
        static_cast<float>((mColor >> 16) & 0xff) / 255.0f,
        static_cast<float>((mColor >> 24) & 0xff) / 255.0f
    };
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
    assert(width > 0 && width <= 100);
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

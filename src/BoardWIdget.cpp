
#include "BoardWidget.hpp"
#include <QKeyEvent>
#include <glm/ext/matrix_clip_space.hpp>

class DrawnElement {
public:
    QColor color;
    int size;

    explicit DrawnElement(QColor color, int size) : color(color), size(size) {}
};

class DrawnPoint final : public DrawnElement {
public:
    glm::vec2 pos;

    DrawnPoint(const glm::vec2& pos, const QColor& color, int size) : DrawnElement(color, size), pos(pos) {}
};

class DrawnLine final : public DrawnElement {
public:
    glm::vec2 start;
    glm::vec2 end;

    DrawnLine(const glm::vec2& start, const glm::vec2 end, const QColor& color, int size) : DrawnElement(color, size), start(start), end(end) {}
};

BoardWidget::BoardWidget() :
    mMode(Mode::DRAW),
    mTheme(Theme::Dark),
    mColor(0xff, 0xff, 0xff),
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

    glViewport(0, 0, 100, 100);
}

void BoardWidget::paintGL() {
    if (mTheme == Theme::Dark)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    else
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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
            mCurrentMouseDrawnPoints->push_back(DrawnPoint(glm::vec2(static_cast<float>(x + mOffsetX), static_cast<float>(y + mOffsetY)), mColor, mPointWidth));
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
    const int x = event->pos().x();
    const int y = event->pos().y();

    switch (mMode) {
        case Mode::DRAW:
            mCurrentMouseDrawnPoints = new QVector<DrawnPoint>();
            mCurrentMouseDrawnPoints->push_back(DrawnPoint(glm::vec2(static_cast<float>(x + mOffsetX), static_cast<float>(y + mOffsetY)), mColor, mPointWidth));
            break;
        case Mode::LINE:
            glm::vec2 start(static_cast<float>(x + mOffsetX), static_cast<float>(y + mOffsetY));
            mCurrentLine = new DrawnLine(start, start, mColor, mPointWidth);
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
    for (auto pointsSet : mMouseDrawnPoints) {
        int j = 0;
        for (const auto& i : *pointsSet) {
            if (j < pointsSet->size() - 1) {
                const auto startPos = glm::vec2(static_cast<float>(i.pos.x), static_cast<float>(i.pos.y));
                const auto endPos = glm::vec2(static_cast<float>(pointsSet->operator[](j + 1).pos.x), static_cast<float>(pointsSet->operator[](j + 1).pos.y));
                const auto color = makeGlColor(i.color);
                const auto width = static_cast<float>(i.size);

                mRenderer->drawLine(startPos, endPos, width, color);
                mRenderer->drawFilledCircle(startPos, static_cast<int>(width / 2.0f), color);
            }
            j++;
        }
    }

    if (mCurrentMouseDrawnPoints != nullptr) {
        for (const auto& i: *mCurrentMouseDrawnPoints) {
            const auto pos = glm::vec2(static_cast<float>(i.pos.x), static_cast<float>(i.pos.y));
            mRenderer->drawPoint(pos, static_cast<float>(i.size), makeGlColor(i.color));
        }
    }
}

void BoardWidget::paintLines() {
    if (mCurrentLine != nullptr) {
        const auto startPos = glm::vec2(static_cast<float>(mCurrentLine->start.x), static_cast<float>(mCurrentLine->start.y));
        const auto endPos = glm::vec2(static_cast<float>(mCurrentLine->end.x), static_cast<float>(mCurrentLine->end.y));
        mRenderer->drawLine(startPos, endPos, static_cast<float>(mCurrentLine->size), makeGlColor(mCurrentLine->color));
    }

    for (const auto& i : mLines) {
        const auto startPos = glm::vec2(static_cast<float>(i->start.x), static_cast<float>(i->start.y));
        const auto endPos = glm::vec2(static_cast<float>(i->end.x), static_cast<float>(i->end.y));
        mRenderer->drawLine(startPos, endPos, static_cast<float>(i->size), makeGlColor(i->color));
    }
}

glm::vec4 BoardWidget::makeGlColor(const QColor& color) const {
    return {
        static_cast<float>(color.red()) / 255.0f,
        static_cast<float>(color.green()) / 255.0f,
        static_cast<float>(color.blue()) / 255.0f,
        static_cast<float>(color.alpha()) / 255.0f
    };
}

void BoardWidget::setMode(Mode mode) {
    mMode = mode;
}

void BoardWidget::setTheme(Theme theme) {
    mTheme = theme;
    update();
}

void BoardWidget::setColor(const QColor& color) {
    mColor = color;
}

void BoardWidget::setPointWidth(int width) {
    assert(width > 0 && width <= MAX_POINT_WIDTH);
    mPointWidth = width;
}

Mode BoardWidget::mode() const {
    return mMode;
}

Theme BoardWidget::theme() const {
    return mTheme;
}

QColor BoardWidget::color() const {
    return mColor;
}

int BoardWidget::pointWidth() const {
    return mPointWidth;
}

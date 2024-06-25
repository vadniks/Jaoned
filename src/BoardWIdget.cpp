/*
 * Jaoned - an OpenGL & QT based drawing board
 * Copyright (C) 2024 Vadim Nikolaev (https://github.com/vadniks).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "BoardWidget.hpp"
#include <QKeyEvent>
#include <glm/ext/matrix_clip_space.hpp>

class DrawnElement {
public:
    QColor color;

    explicit DrawnElement(const QColor& color) : color(color) {}
};

class DrawnPoint final : public DrawnElement {
public:
    glm::vec2 pos;
    int width;

    DrawnPoint(const glm::vec2& pos, const QColor& color, int width) : DrawnElement(color), pos(pos), width(width) {}
};

class DrawnLine final : public DrawnElement {
public:
    glm::vec2 start;
    glm::vec2 end;
    int width;

    // TODO: change order of arguments
    DrawnLine(const glm::vec2& start, const glm::vec2 end, const QColor& color, int width) : DrawnElement(color), start(start), end(end), width(width) {}
};

class DrawnText final : public DrawnElement {
public:
    QString text;
    glm::vec2 pos;
    int size;

    DrawnText(const QString& text, const glm::vec2& pos, int size, const QColor& color) : DrawnElement(color), text(text), pos(pos), size(size) {}
};

class DrawnImage final : DrawnElement {
public:
    glm::vec2 pos;
    glm::vec2 size;
    Texture* texture;

    DrawnImage(const glm::vec2& pos, const glm::vec2& size, Texture* texture) : DrawnElement(QColor(0, 0, 0, 0)), pos(pos), size(size), texture(texture) {}

    ~DrawnImage() {
        delete texture;
    }
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
    mCurrentLine(nullptr),
    mTexts(),
    mCurrentText(nullptr)
{
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

BoardWidget::~BoardWidget() {
    for (auto i : mMouseDrawnPoints)
        delete i;

    for (auto i : mLines)
        delete i;

    for (auto i : mTexts)
        delete i;

    for (auto i : mImages)
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

    paintImages();
    paintTexts();
    paintLines();
    paintDrawn();
}

void BoardWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    updateProjection();
}

void BoardWidget::keyPressEvent(QKeyEvent* event) {
    const auto step = 10;

    switch (event->key()) {
        case Qt::Key::Key_Up:
            mOffsetY -= step;
            break;
        case Qt::Key::Key_Left:
            mOffsetX -= step;
            break;
        case Qt::Key::Key_Down:
            mOffsetY += step;
            break;
        case Qt::Key::Key_Right:
            mOffsetX += step;
            break;
    }

    if (mCurrentText != nullptr)
        mCurrentText->text = mCurrentText->text + event->text();

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
        case Mode::TEXT:
            if (mCurrentText == nullptr) break;
            mCurrentText->pos.x = static_cast<float>(x + mOffsetX);
            mCurrentText->pos.y = static_cast<float>(y + mOffsetY);
            break;
        case Mode::IMAGE:
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
            {
                glm::vec2 start(static_cast<float>(x + mOffsetX), static_cast<float>(y + mOffsetY));
                mCurrentLine = new DrawnLine(start, start, mColor, mPointWidth);
            }
            break;
        case Mode::TEXT:
            {
                glm::vec2 pos(static_cast<float>(x + mOffsetX), static_cast<float>(y + mOffsetY));
                mCurrentText = new DrawnText("", pos, mPointWidth, mColor);
            }
            break;
        case Mode::IMAGE:
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
        case Mode::TEXT:
            if (!mCurrentText->text.isEmpty()) {
                mTexts.push_back(mCurrentText);
                mCurrentText = nullptr;
            } else {
                delete mCurrentText;
                mCurrentText = nullptr;
            }
            break;
        case Mode::IMAGE:
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

static glm::vec4 makeGlColor(const QColor& color) {
    return {
        static_cast<float>(color.red()) / 255.0f,
        static_cast<float>(color.green()) / 255.0f,
        static_cast<float>(color.blue()) / 255.0f,
        static_cast<float>(color.alpha()) / 255.0f
    };
}

void BoardWidget::paintDrawn() {
    for (auto pointsSet : mMouseDrawnPoints) {
        int j = 0;
        for (const auto& i : *pointsSet) {
            if (j < pointsSet->size() - 1) {
                const auto startPos = glm::vec2(static_cast<float>(i.pos.x), static_cast<float>(i.pos.y));
                const auto endPos = glm::vec2(static_cast<float>(pointsSet->operator[](j + 1).pos.x), static_cast<float>(pointsSet->operator[](j + 1).pos.y));
                const auto color = makeGlColor(i.color);
                const auto width = static_cast<float>(i.width);

                mRenderer->drawLine(startPos, endPos, width, color);
                mRenderer->drawPoint(startPos, width * 0.7f, color);
                mRenderer->drawHollowCircle(startPos, static_cast<int>(width / 2.0f), color);
            }
            j++;
        }
    }

    if (mCurrentMouseDrawnPoints != nullptr) {
        for (const auto& i: *mCurrentMouseDrawnPoints) {
            const auto pos = glm::vec2(static_cast<float>(i.pos.x), static_cast<float>(i.pos.y));
            mRenderer->drawPoint(pos, static_cast<float>(i.width) * 0.7f, makeGlColor(i.color));
            mRenderer->drawHollowCircle(pos, static_cast<int>(static_cast<float>(i.width) / 2.0f), makeGlColor(i.color));
        }
    }
}

void BoardWidget::paintLines() {
    for (auto i : mLines) {
        const auto startPos = glm::vec2(static_cast<float>(i->start.x), static_cast<float>(i->start.y));
        const auto endPos = glm::vec2(static_cast<float>(i->end.x), static_cast<float>(i->end.y));
        mRenderer->drawLine(startPos, endPos, static_cast<float>(i->width), makeGlColor(i->color));
    }

    if (mCurrentLine != nullptr) {
        const auto startPos = glm::vec2(static_cast<float>(mCurrentLine->start.x), static_cast<float>(mCurrentLine->start.y));
        const auto endPos = glm::vec2(static_cast<float>(mCurrentLine->end.x), static_cast<float>(mCurrentLine->end.y));
        mRenderer->drawLine(startPos, endPos, static_cast<float>(mCurrentLine->width), makeGlColor(mCurrentLine->color));
    }
}

static void blending(bool enable) {
    if (enable) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
    } else {
        glBlendFunc(GL_SRC_COLOR, GL_ZERO);
        glDisable(GL_BLEND);
    }
}

void BoardWidget::paintTexts() {
    blending(true);

    for (auto i : mTexts)
        mRenderer->drawText(i->text, i->size, i->pos, makeGlColor(i->color));

    if (mCurrentText != nullptr) {
        mRenderer->drawLine(mCurrentText->pos, mCurrentText->pos + glm::vec2(0.0f, static_cast<float>(mCurrentText->size)), 1.0f, makeGlColor(mCurrentText->color));
        mRenderer->drawText(mCurrentText->text, mCurrentText->size, mCurrentText->pos, makeGlColor(mCurrentText->color));
    }

    blending(false);
}

void BoardWidget::paintImages() {
    blending(true);

    for (auto i : mImages)
        mRenderer->drawTexture(*(i->texture), i->pos, i->size, 0.0f, makeGlColor(mColor));

    blending(false);
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "bugprone-integer-division"

void BoardWidget::addImage(const glm::vec2& size, const uchar* data) {
    const QSize frameSize = this->size();
    auto* texture = new Texture(*this, static_cast<int>(size.x), static_cast<int>(size.y), data);

    mImages.push_back(new DrawnImage(
        glm::vec2(static_cast<float>(frameSize.width() / 2 - static_cast<int>(size.x) / 2 + mOffsetX), static_cast<float>(frameSize.height() / 2 - static_cast<int>(size.y) / 2 + mOffsetY)),
        size,
        texture
    ));
}

#pragma clang diagnostic pop

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

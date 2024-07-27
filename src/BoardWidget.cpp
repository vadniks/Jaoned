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
#include "Helpers.hpp"
#include <QKeyEvent>
#include <glm/ext/matrix_clip_space.hpp>

struct DrawnElement { // abstract
protected:
    DrawnElement() {}
public:
    virtual ~DrawnElement() = default;

    DISABLE_COPY(DrawnElement)
    DISABLE_MOVE(DrawnElement)
};

struct DrawnPointsSet final : public DrawnElement {
    bool erase;
    int width;
    QColor color;
    QList<glm::vec2> points;

    DrawnPointsSet(bool erase, int width, const QColor& color) : erase(erase), width(width), color(color), points() {}
    ~DrawnPointsSet() override = default;

    DISABLE_COPY(DrawnPointsSet)
    DISABLE_MOVE(DrawnPointsSet)
};

struct DrawnLine final : public DrawnElement {
    glm::vec2 start;
    glm::vec2 end;
    int width;
    QColor color;

    DrawnLine(const glm::vec2& start, const glm::vec2 end, int width, const QColor& color) : start(start), end(end), width(width), color(color) {}
    ~DrawnLine() override = default;

    DISABLE_COPY(DrawnLine)
    DISABLE_MOVE(DrawnLine)
};

struct DrawnText final : public DrawnElement {
    QString text;
    glm::vec2 pos;
    int size;
    QColor color;

    DrawnText(const QString& text, const glm::vec2& pos, int size, const QColor& color) : text(text), pos(pos), size(size), color(color) {}
    ~DrawnText() override = default;

    DISABLE_COPY(DrawnText)
    DISABLE_MOVE(DrawnText)
};

struct DrawnImage final : public DrawnElement {
    glm::vec2 pos;
    glm::vec2 size;
    Texture* texture;

    DrawnImage(const glm::vec2& pos, const glm::vec2& size, Texture* texture) : pos(pos), size(size), texture(texture) {}

    ~DrawnImage() override {
        delete texture;
    }

    DISABLE_COPY(DrawnImage)
    DISABLE_MOVE(DrawnImage)
};

BoardWidget::BoardWidget(const std::function<void ()>& parentWidgetModeUpdater) :
    mMode(Mode::DRAW),
    mTheme(Theme::Dark),
    mColor(0xff, 0xff, 0xff),
    mPointWidth(5),
    mProjection(1.0f),
    mRenderer(nullptr),
    mOffsetX(0),
    mOffsetY(0),
    mElements(),
    mCurrentPointsSet(nullptr),
    mCurrentLine(nullptr),
    mCurrentText(nullptr),
    mCurrentImage(nullptr),
    mDrawCurrentImage(false),
    mParentWidgetModeUpdater(parentWidgetModeUpdater)
{
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

BoardWidget::~BoardWidget() {
    for (auto i : mElements)
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

    for (auto element : mElements) {
        if (dynamic_cast<DrawnPointsSet*>(element) != nullptr)
            paintPointsSet(dynamic_cast<DrawnPointsSet*>(element));
        else if (dynamic_cast<DrawnLine*>(element) != nullptr)
            paintLine(dynamic_cast<DrawnLine*>(element));
        else if (dynamic_cast<DrawnText*>(element) != nullptr)
            paintText(dynamic_cast<DrawnText*>(element));
        else if (dynamic_cast<DrawnImage*>(element) != nullptr)
            paintImage(dynamic_cast<DrawnImage*>(element));
    }

    switch (mMode) {
        case Mode::ERASE:
            [[gnu::fallthrough]];
        case Mode::DRAW:
            paintPointsSet(nullptr);
            break;
        case Mode::LINE:
            paintLine(nullptr);
            break;
        case Mode::TEXT:
            paintText(nullptr);
            break;
        case Mode::IMAGE:
            paintImage(nullptr);
            break;
    }
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

    if (mCurrentText != nullptr) {
        if (event->key() != Qt::Key::Key_Backspace)
            mCurrentText->text = mCurrentText->text + event->text();
        else
            mCurrentText->text = mCurrentText->text.mid(0, mCurrentText->text.size() - 1);
    }

    updateProjection();
    update();
}

void BoardWidget::mouseMoveEvent(QMouseEvent* event) {
    const int x = event->pos().x();
    const int y = event->pos().y();

    switch (mMode) {
        case Mode::ERASE:
            [[gnu::fallthrough]];
        case Mode::DRAW:
            mCurrentPointsSet->points.push_back(glm::vec2(static_cast<float>(x + mOffsetX), static_cast<float>(y + mOffsetY)));
            break;
        case Mode::LINE:
            mCurrentLine->end.x = static_cast<float>(x + mOffsetX);
            mCurrentLine->end.y = static_cast<float>(y + mOffsetY);
            break;
        case Mode::TEXT:
            mCurrentText->pos.x = static_cast<float>(x + mOffsetX);
            mCurrentText->pos.y = static_cast<float>(y + mOffsetY);
            break;
        case Mode::IMAGE:
            mCurrentImage->pos.x = static_cast<float>(x + mOffsetX);
            mCurrentImage->pos.y = static_cast<float>(y + mOffsetY);
            break;
    }

    update();
}

void BoardWidget::mousePressEvent(QMouseEvent* event) {
    const int x = event->pos().x();
    const int y = event->pos().y();

    switch (mMode) {
        case Mode::ERASE:
            [[gnu::fallthrough]];
        case Mode::DRAW:
            mCurrentPointsSet = new DrawnPointsSet(mMode == Mode::ERASE, mPointWidth, mColor);
            mCurrentPointsSet->points.push_back(glm::vec2(static_cast<float>(x + mOffsetX), static_cast<float>(y + mOffsetY)));
            break;
        case Mode::LINE:
            {
                glm::vec2 start(static_cast<float>(x + mOffsetX), static_cast<float>(y + mOffsetY));
                mCurrentLine = new DrawnLine(start, start, mPointWidth, mColor);
            }
            break;
        case Mode::TEXT:
            {
                glm::vec2 pos(static_cast<float>(x + mOffsetX), static_cast<float>(y + mOffsetY));
                mCurrentText = new DrawnText("", pos, mPointWidth, mColor);
            }
            break;
        case Mode::IMAGE:
            {
                glm::vec2 pos(static_cast<float>(x + mOffsetX), static_cast<float>(y + mOffsetY));
                mCurrentImage->pos = pos;
                mDrawCurrentImage = true;
            }
            break;
    }
}

void BoardWidget::mouseReleaseEvent(QMouseEvent*) {
    switch (mMode) {
        case Mode::ERASE:
            [[gnu::fallthrough]];
        case Mode::DRAW: // TODO: move each case to a separate function
            {
                mElements.push(mCurrentPointsSet);

                QList<PointDto> points;
                for (const auto& point : mCurrentPointsSet->points)
                    points.append(PointDto(static_cast<int>(point.x), static_cast<int>(point.y)));

                emit pointsSetAdded(PointsSetDto(
                    mCurrentPointsSet->erase,
                    mCurrentPointsSet->width,
                    Helpers::packQColor(mCurrentPointsSet->color),
                    points
                ));

                mCurrentPointsSet = nullptr;
            }
            break;
        case Mode::LINE:
            mElements.push(mCurrentLine);

            emit lineAdded(LineDto(
                PointDto(static_cast<int>(mCurrentLine->start.x), static_cast<int>(mCurrentLine->start.y)),
                PointDto(static_cast<int>(mCurrentLine->end.x), static_cast<int>(mCurrentLine->end.y)),
                mCurrentLine->width,
                Helpers::packQColor(mCurrentLine->color)
            ));

            mCurrentLine = nullptr;
            break;
        case Mode::TEXT:
            if (!mCurrentText->text.isEmpty()) {
                mElements.push(mCurrentText);

                QList<char> text;
                for (auto byte : mCurrentText->text.toUtf8())
                    text.append(byte);

                emit textAdded(TextDto(
                    PointDto(static_cast<int>(mCurrentText->pos.x), static_cast<int>(mCurrentText->pos.y)),
                    mCurrentText->size,
                    Helpers::packQColor(mCurrentText->color),
                    text
                ));

                mCurrentText = nullptr;
            } else {
                delete mCurrentText;
                mCurrentText = nullptr;
            }
            break;
        case Mode::IMAGE:
            mDrawCurrentImage = false;

            mElements.push(mCurrentImage);

            QList<char> texture;
            for (auto byte : mCurrentImage->texture->data())
                texture.append(static_cast<char>(byte));

            emit imageAdded(ImageDto(
                PointDto(static_cast<int>(mCurrentImage->pos.x), static_cast<int>(mCurrentImage->pos.y)),
                static_cast<int>(mCurrentImage->size.x),
                static_cast<int>(mCurrentImage->size.y),
                texture
            ));

            mCurrentImage = nullptr;

            mMode = Mode::DRAW;
            mParentWidgetModeUpdater();
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

static void blending(bool enable) {
    if (enable) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glBlendFunc(GL_SRC_COLOR, GL_ZERO);
        glDisable(GL_BLEND);
    }
}

QColor BoardWidget::themeColor() {
    return mTheme == Theme::Dark ? QColor(0, 0, 0) : QColor(0xff, 0xff, 0xff);
}

void BoardWidget::paintPointsSet(DrawnPointsSet* /*nullable*/ pointsSet) {
    if (pointsSet != nullptr) {
        int j = 0;
        for (const auto& i : pointsSet->points) {
            if (j < pointsSet->points.size() - 1) {
                const auto startPos = glm::vec2(static_cast<float>(i.x), static_cast<float>(i.y));
                const auto endPos = glm::vec2(static_cast<float>(pointsSet->points.operator[](j + 1).x), static_cast<float>(pointsSet->points.operator[](j + 1).y));
                const auto color = makeGlColor(pointsSet->erase ? themeColor() : pointsSet->color);
                const auto width = static_cast<float>(pointsSet->width);

                mRenderer->drawLine(startPos, endPos, width, color);
                mRenderer->drawPoint(startPos, width * 0.7f, color);
                mRenderer->drawHollowCircle(startPos, static_cast<int>(width / 2.0f), color);
            }
            j++;
        }
    } else {
        if (mCurrentPointsSet == nullptr) return;
        for (const auto& i : mCurrentPointsSet->points) {
            const auto pos = glm::vec2(static_cast<float>(i.x), static_cast<float>(i.y));
            const auto color = makeGlColor(mCurrentPointsSet->erase ? themeColor() : mCurrentPointsSet->color);

            mRenderer->drawPoint(pos, static_cast<float>(mCurrentPointsSet->width) * 0.7f, color);
            mRenderer->drawHollowCircle(pos, static_cast<int>(static_cast<float>(mCurrentPointsSet->width) / 2.0f), color);
        }
    }
}

void BoardWidget::paintLine(DrawnLine* /*nullable*/ line) {
    if (line == nullptr)
        line = mCurrentLine;
    if (line == nullptr)
        return;

    const auto startPos = glm::vec2(static_cast<float>(line->start.x), static_cast<float>(line->start.y));
    const auto endPos = glm::vec2(static_cast<float>(line->end.x), static_cast<float>(line->end.y));
    mRenderer->drawLine(startPos, endPos, static_cast<float>(line->width), makeGlColor(line->color));
}

void BoardWidget::paintText(DrawnText* /*nullable*/ text) {
    blending(true);

    if (text != nullptr)
        mRenderer->drawText(text->text, text->size, text->pos, makeGlColor(text->color));
    else if (mCurrentText != nullptr) {
        const auto textSize = mRenderer->textMetrics(mCurrentText->text, mCurrentText->size);
        const auto textHeight = textSize.height() > mCurrentText->size ? textSize.height() : mCurrentText->size;
        const auto textWidth = textSize.width() > 20 ? textSize.width() : 20;

        const auto color = makeGlColor(mCurrentText->color);

        mRenderer->drawLine(
            mCurrentText->pos,
            mCurrentText->pos + glm::vec2(0.0f, static_cast<float>(textHeight)),
            1.0f,
            color
        );
        mRenderer->drawLine(
            mCurrentText->pos + glm::vec2(0.0f, static_cast<float>(textHeight)),
            mCurrentText->pos + glm::vec2(static_cast<float>(textWidth), static_cast<float>(textHeight)),
            1.0f,
            color
        );

        mRenderer->drawText(mCurrentText->text, mCurrentText->size, mCurrentText->pos, color);
    }

    blending(false);
}

void BoardWidget::paintImage(DrawnImage* /*nullable*/ image) {
    blending(true);

    if (image != nullptr)
        mRenderer->drawTexture(*(image->texture), image->pos, image->size, 0.0f, glm::vec4(1.0f));
    else if (mDrawCurrentImage) {
        assert(mCurrentImage != nullptr);
        mRenderer->drawTexture(*(mCurrentImage->texture), mCurrentImage->pos, mCurrentImage->size, 0.0f, glm::vec4(1.0f));
    }

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

void BoardWidget::setCurrentTexture(const glm::vec2& size, const uchar* data) {
    auto* texture = new Texture(*this, static_cast<int>(size.x), static_cast<int>(size.y), data);
    mCurrentImage = new DrawnImage(glm::vec2(0.0f), size, texture);
}

void BoardWidget::undo() {
    if (mElements.isEmpty()) return;

    delete mElements.pop();
    update();

    emit lastElementRemoved();
}

void BoardWidget::clear() {
    for (auto i : mElements)
        delete i;

    mElements.clear();

    update();
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

std::vector<uchar> BoardWidget::pixels() {
    const auto size = this->size();
    std::vector<uchar> bytes(4 * size.width() * size.height(), 0);
    glReadPixels(0, 0, size.width(), size.height(), GL_RGBA, GL_UNSIGNED_BYTE, bytes.data());
    return bytes;
}

void BoardWidget::addPointsSet(const PointsSetDto& pointsSetDto) {
    auto pointsSet = new DrawnPointsSet(pointsSetDto.erase(), pointsSetDto.width(), Helpers::unpackQColor(pointsSetDto.color()));

    for (auto point : pointsSetDto.points())
        pointsSet->points.append(glm::vec2(point.x(), point.y()));

    mElements.push(pointsSet);

    update();
}

void BoardWidget::addLine(const LineDto& lineDto) {
    mElements.push(new DrawnLine(
        glm::vec2(lineDto.start().x(), lineDto.start().y()),
        glm::vec2(lineDto.end().x(), lineDto.end().y()),
        lineDto.width(),
        Helpers::unpackQColor(lineDto.color())
    ));

    update();
}

void BoardWidget::addText(const TextDto& textDto) {
    QByteArray bytes(textDto.text().size(), 0);
    for (int i = 0; i < textDto.text().size(); i++)
        bytes[i] = textDto.text()[i];

    mElements.push(new DrawnText(
        QString::fromUtf8(bytes),
        glm::vec2(textDto.pos().x(), textDto.pos().y()),
        textDto.fontSize(),
        Helpers::unpackQColor(textDto.color())
    ));

    update();
}

void BoardWidget::addImage(const ImageDto& imageDto) {
    mElements.push(new DrawnImage(
        glm::vec2(imageDto.pos().x(), imageDto.pos().y()),
        glm::vec2(imageDto.width(), imageDto.height()),
        new Texture(
            *this,
            imageDto.width(),
            imageDto.height(),
            reinterpret_cast<const uchar*>(imageDto.texture().data())
        )
    ));

    update();
}

void BoardWidget::removeLastElement() {
    if (mElements.isEmpty()) return;

    delete mElements.pop();
    update();
}

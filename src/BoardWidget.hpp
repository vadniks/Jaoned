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

#pragma once

#include "defs.hpp"
#include "Mode.hpp"
#include "Theme.hpp"
#include "Renderer.hpp"
#include "dto.hpp"
#include <functional>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QStack>
#include <glm/glm.hpp>

struct DrawnElement;
struct DrawnPointsSet;
struct DrawnLine;
struct DrawnText;
struct DrawnImage;

class BoardWidget final : public QOpenGLWidget, private QOpenGLFunctions_3_3_Core {
    Q_OBJECT
private:
    Mode mMode;
    Theme mTheme;
    QColor mColor;
    int mPointWidth;
    glm::mat4 mProjection;
    Renderer* mRenderer;
    int mOffsetX, mOffsetY;
    QStack<DrawnElement*> mElements;
    DrawnPointsSet* mCurrentPointsSet; // nullable
    DrawnLine* mCurrentLine; // nullable
    DrawnText* mCurrentText; // nullable
    DrawnImage* mCurrentImage; // nullable
    bool mDrawCurrentImage;
    std::function<void ()> mParentWidgetModeUpdater;
public:
    static inline int MAX_POINT_WIDTH = 100;
public:
    explicit BoardWidget(const std::function<void ()>& parentWidgetModeUpdater);
    ~BoardWidget() override;

    QSize minimumSizeHint() const override;

    DISABLE_COPY(BoardWidget)
    DISABLE_MOVE(BoardWidget)
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
private:
    void updateProjection(); // TODO: glm::look_at (camera and zoom)
    QColor themeColor();
    void paintPointsSet(DrawnPointsSet* /*nullable*/ pointsSet);
    void paintLine(DrawnLine* /*nullable*/ line);
    void paintText(DrawnText* /*nullable*/ text);
    void paintImage(DrawnImage* /*nullable*/ image);
public slots:
    void setMode(Mode mode);
    void setTheme(Theme theme);
    void setColor(const QColor& color);
    void setPointWidth(int width);
    void setCurrentTexture(const glm::vec2& size, const uchar* data);
    void undo();
    void clear();
public:
    Mode mode() const;
    Theme theme() const;
    QColor color() const;
    int pointWidth() const;
    std::vector<uchar> pixels();
signals:
    void pointsSetAdded(const PointsSetDto& pointsSetDto);
    void lineAdded(const LineDto& lineDto);
    void textAdded(const TextDto& textDto);
    void imageAdded(const ImageDto& imageDto);
    void lastElementRemoved();
};

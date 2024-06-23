
#pragma once

#include "defs.hpp"
#include "Mode.hpp"
#include "Theme.hpp"
#include "Renderer.hpp"
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>

class BoardWidget final : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
private:
    struct LineCoordinates {
        glm::vec2 start, end;
        LineCoordinates(const glm::vec2& start, const glm::vec2& end);
    };
private:
    Mode mMode;
    Theme mTheme;
    int mColor;
    int mPointWidth;
    glm::mat4 mProjection;
    Renderer* mRenderer;
    int mOffsetX, mOffsetY;
    QVector<QVector<glm::vec2>*> mMouseDrawnPoints;
    QVector<glm::vec2>* mCurrentMouseDrawnPoints; // nullable
    QVector<LineCoordinates*> mLines;
    LineCoordinates* mCurrentLine; // nullable
public:
    BoardWidget();
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
    void updateProjection();
    void paintDrawn();
    void paintLines();
    glm::vec4 makeGlColor() const;
public slots:
    void setMode(Mode mode);
    void setTheme(Theme theme);
    void setColor(int color);
    void setPointWidth(int width);
public:
    Mode mode() const;
    Theme theme() const;
    int color() const;
    int pointWidth() const;
};

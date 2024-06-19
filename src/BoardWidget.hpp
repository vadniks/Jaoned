
#pragma once

#include "defs.hpp"
#include "Renderer.hpp"
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class BoardWidget final : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
private:
    glm::mat4 mProjection;
    Renderer* mRenderer;
    int mOffsetX, mOffsetY;
    bool mMousePressed;
    QVector<glm::ivec2> mMouseDrawnPoints;
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
    void updateProjection(int width, int height);
};

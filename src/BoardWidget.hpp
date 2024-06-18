
#pragma once

#include "defs.hpp"
#include "Renderer.hpp"
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class BoardWidget final : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
private:
    Renderer* mRenderer;
public:
    BoardWidget();
    ~BoardWidget() override;

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    DISABLE_COPY(BoardWidget)
    DISABLE_MOVE(BoardWidget)
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
};

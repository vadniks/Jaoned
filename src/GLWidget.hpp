
#pragma once

#include "defs.hpp"
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
private:
    QOpenGLContext* mContext;
public:
    GLWidget();
    ~GLWidget() override;

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    DISABLE_COPY(GLWidget)
    DISABLE_MOVE(GLWidget)
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
};

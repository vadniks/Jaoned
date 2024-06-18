
#pragma once

#include "defs.hpp"
#include "GLWidget.hpp"
#include <QWidget>
#include <QVBoxLayout>

class MainWidget final : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout mLayout;
    GLWidget* mGLWidget;
public:
    MainWidget();
    ~MainWidget() override;

    DISABLE_COPY(MainWidget)
    DISABLE_MOVE(MainWidget)
};

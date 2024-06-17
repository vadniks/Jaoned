
#include "MainWidget.hpp"

MainWidget::MainWidget() : mLayout(this), mGLWidget(new GLWidget()) {
    mLayout.addWidget(mGLWidget);
}

MainWidget::~MainWidget() {
    delete mGLWidget;
}

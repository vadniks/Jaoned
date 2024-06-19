
#include "MainWidget.hpp"

MainWidget::MainWidget() : mLayout(this), mBoardWidget(new BoardWidget()) {
    mLayout.addWidget(&mControlsWidget, 0, Qt::AlignTop);
    mLayout.addWidget(mBoardWidget, 0, Qt::AlignCenter);
}

MainWidget::~MainWidget() {
    delete mBoardWidget;
}

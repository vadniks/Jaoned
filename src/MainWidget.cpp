
#include "MainWidget.hpp"

MainWidget::MainWidget() : mLayout(this), mBoardWidget(new BoardWidget()) {
    mLayout.addWidget(mBoardWidget);
}

MainWidget::~MainWidget() {
    delete mBoardWidget;
}

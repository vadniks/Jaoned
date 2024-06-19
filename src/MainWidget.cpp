
#include "MainWidget.hpp"
#include <QResizeEvent>

MainWidget::MainWidget() : mLayout(this), mBoardWidget(new BoardWidget()) {
    mLayout.addWidget(&mControlsWidget, 0, Qt::AlignTop);
    mLayout.addWidget(mBoardWidget, 0, Qt::AlignVCenter);
    mLayout.addStretch();
}

MainWidget::~MainWidget() {
    delete mBoardWidget;
}

void MainWidget::resizeEvent(QResizeEvent* event) {
    const auto size = event->size();
    const auto probe = mControlsWidget.size();
    const auto margin = (size.width() - probe.width()) / 2;

    mBoardWidget->resize(probe.width(), size.height() - probe.height() - margin * 3);

    QWidget::resizeEvent(event);
}

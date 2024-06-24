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

#include "MainWidget.hpp"
#include <QResizeEvent>

MainWidget::MainWidget() : mLayout(this), mBoardWidget(new BoardWidget()), mControlsWidget(mBoardWidget) {
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

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

#include "MainWindow.hpp"
#include <QMessageBox>

MainWindow::MainWindow() : mCurrentWidget(Widget::AUTH), mMainWidget() {
    setCentralWidget(&mAuthWidget);
    connect(Network::instance(), &Network::eventOccurred, this, &MainWindow::eventOccurred);
}

void MainWindow::setCurrentWidget(Widget widget) {
    switch ((mCurrentWidget = widget)) {
        case AUTH:
            setCentralWidget(&mAuthWidget);
            break;
        case HOME:
            setCentralWidget(&mHomeWidget);
            break;
        case MAIN:
            setCentralWidget(&mMainWidget);
            break;
    }
}

void MainWindow::eventOccurred(Network::Event event) {
    if (event == Network::Event::CONNECTED) return;

    if (event == Network::Event::DISCONNECTED)
        setCurrentWidget(Widget::AUTH);

    QMessageBox box(this);
    box.setModal(true);
    box.setText(event == Network::Event::ERROR_OCCURRED ? "Error occurred" : "Disconnected");
    box.exec();
}

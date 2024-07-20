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

#pragma once

#include "AuthWidget.hpp"
#include "HomeWidget.hpp"
#include "MainWidget.hpp"
#include "defs.hpp"
#include "Network.hpp"
#include <QMainWindow>

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    enum Widget {
        AUTH,
        HOME,
        MAIN
    };
private:
    Widget mCurrentWidget;
    AuthWidget mAuthWidget;
    HomeWidget mHomeWidget;
    MainWidget mMainWidget;
public:
    MainWindow();

    DISABLE_COPY(MainWindow)
    DISABLE_MOVE(MainWindow)

    void setCurrentWidget(Widget widget);
private slots:
    void eventOccurred(Network::Event event);
};

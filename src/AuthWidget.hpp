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

#include "defs.hpp"
#include "Network.hpp"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>

class AuthWidget : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout mLayout;
    QLabel mAppNameLabel;
    QWidget mFieldsWidget;
    QVBoxLayout mFieldsLayout;
    QLineEdit mUsernameField;
    QLineEdit mPasswordField;
    QWidget mButtonsWidget;
    QHBoxLayout mButtonsLayout;
    QPushButton mLogInButton;
    QPushButton mRegisterButton;
    QProgressBar mProgressBar;
    bool loggingIn;
public:
    AuthWidget();
    ~AuthWidget() override;

    QSize minimumSizeHint() const override;

    DISABLE_COPY(AuthWidget)
    DISABLE_MOVE(AuthWidget)
public slots:
    void logInClicked();
    void registerClicked();
    void networkEventOccurred(Network::Event event);
signals: // those are implemented elsewhere
    void loggedIn();
    void registered();
    void erroOccurred();
};

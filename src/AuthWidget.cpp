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

#include "AuthWidget.hpp"

AuthWidget::AuthWidget() : mLayout(this), mFieldsLayout(&mFieldsWidget), mButtonsLayout(&mButtonsWidget) {
    mLayout.setAlignment(Qt::AlignCenter);

    mAppNameLabel.setText("Jaoned");
    mAppNameLabel.setFont(QFont("Roboto", 24));
    mAppNameLabel.setAlignment(Qt::AlignCenter);
    mLayout.addWidget(&mAppNameLabel);

    mUsernameField.setPlaceholderText("Username");
    mUsernameField.setMaxLength(MAX_CREDENTIAL_SIZE);
    mFieldsLayout.addWidget(&mUsernameField);

    mPasswordField.setPlaceholderText("Password");
    mPasswordField.setMaxLength(MAX_CREDENTIAL_SIZE);
    mPasswordField.setEchoMode(QLineEdit::EchoMode::Password);
    mFieldsLayout.addWidget(&mPasswordField);

    mFieldsWidget.setFixedSize(16 * 20, 9 * 10);
    mLayout.addWidget(&mFieldsWidget);

    mLogInButton.setText("Log In");
    connect(&mLogInButton, &QPushButton::clicked, this, &AuthWidget::logInCLicked);
    mButtonsLayout.addWidget(&mLogInButton);

    mRegisterButton.setText("Register");
    connect(&mRegisterButton, &QPushButton::clicked, this, &AuthWidget::registerClicked);
    mButtonsLayout.addWidget(&mRegisterButton);

    mLayout.addWidget(&mButtonsWidget);

    mProgressBar.setMaximum(0);
    mProgressBar.setMinimum(0);
    mProgressBar.setValue(0);
    mProgressBar.setFixedSize(16 * 20, 9 * 5);
    mLayout.addWidget(&mProgressBar);
}

QSize AuthWidget::minimumSizeHint() const {
    return {16 * 75, 9 * 75};
}

void AuthWidget::logInCLicked() {

}

void AuthWidget::registerClicked() {

}

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

#include "BoardCreationDialog.hpp"
#include <QColorDialog>

BoardCreationDialog::BoardCreationDialog(QWidget* parent) :
    QDialog(parent),
    mLayout(this),
    mColorLayout(&mColorWidget),
    mButtonsLayout(&mButtonsWidget)
{
    setLayout(&mLayout);

    mLayout.addStretch();

    mHintLabel.setFont(QFont("Roboto", 16));
    mHintLabel.setText("Board creation");
    mLayout.addWidget(&mHintLabel, 0, Qt::AlignCenter);

    QPixmap pixmap(25, 25);
    pixmap.fill(Qt::black);

    mColorLabel.setPixmap(pixmap);
    mColorLayout.addWidget(&mColorLabel);

    mColorButton.setText("Select color");
    connect(&mColorButton, &QPushButton::clicked, this, &BoardCreationDialog::colorClicked);
    mColorLayout.addWidget(&mColorButton);

    mLayout.addWidget(&mColorWidget, 0, Qt::AlignCenter);

    mTitleField.setPlaceholderText("Title");
    mTitleField.setMaxLength(Board::MAX_TITLE_SIZE);
    mLayout.addWidget(&mTitleField, 0, Qt::AlignCenter);

    mCreateButton.setText("Create");
    connect(&mCreateButton, &QPushButton::clicked, this, &BoardCreationDialog::createClicked);
    mButtonsLayout.addWidget(&mCreateButton);

    mExitButton.setText("Exit");
    connect(&mExitButton, &QPushButton::clicked, this, &BoardCreationDialog::exitClicked);
    mButtonsLayout.addWidget(&mExitButton);

    mLayout.addWidget(&mButtonsWidget, 0, Qt::AlignCenter);

    mLayout.addStretch();

    setFixedSize(200, 200);
}

void BoardCreationDialog::colorClicked() {
    QColorDialog dialog(this);
    dialog.setModal(true);
    connect(&dialog, &QColorDialog::colorSelected, this, [this](const QColor& color){ mColor = color; });
    dialog.exec();
}

void BoardCreationDialog::createClicked() {
    emit resultFormed(Board(Board::INVALID_ID, mColor, mTitleField.text()));
    accept();
}

void BoardCreationDialog::exitClicked() {
    reject();
}

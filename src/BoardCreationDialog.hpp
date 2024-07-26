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

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class BoardCreationDialog final : public QDialog {
    Q_OBJECT
private:
    QVBoxLayout mLayout;
    QLabel mHintLabel;
    QWidget mColorWidget;
    QHBoxLayout mColorLayout;
    QLabel mColorLabel;
    QPushButton mColorButton;
    QLineEdit mTitleField;
    QWidget mButtonsWidget;
    QHBoxLayout mButtonsLayout;
    QPushButton mCreateButton;
    QPushButton mExitButton;
public:
    BoardCreationDialog();
};

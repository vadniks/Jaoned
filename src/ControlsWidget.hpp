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

#include "Mode.hpp"
#include "BoardWidget.hpp"
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>

class ControlsWidget final : public QWidget {
    Q_OBJECT
private:
    BoardWidget* mBoardWidget; // allocated elsewhere
    QHBoxLayout mLayout;
    QPushButton mThemeButton;
    QPushButton mColorButton;
    QWidget mPointWidthWidget;
    QHBoxLayout mPointWidthLayout;
    QLabel mPointWidthLabel;
    QSlider mPointWidthSlider;
    QPushButton mDrawButton;
    QPushButton mLineButton;
    QPushButton mTextButton;
    QPushButton mImageButton;
    QPushButton mEraseButton;
    QLabel mModeLabel;
    QPushButton mUndoButton;
    QPushButton mClearButton;
    QPushButton mExportButton;
public:
    explicit ControlsWidget(BoardWidget* boardWidget);
    void updateMode();
private slots:
    void themeSwitchClicked();
    void colorChangeClicked();
    void colorSelected(QColor color);
    void pointWidthChanged(int width);
    void modeSelected(Mode mode);
    void imageSelectClicked();
    void imageSelected(const QString& path);
    void undoCLicked();
    void clearClicked();
    void exportClicked();
    void outputFileSelected(const QString& path);
signals:
    void updated(); // implemented elsewhere by QtMoc automatically
};

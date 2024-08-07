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

#include "ControlsWidget.hpp"
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>

static QString makeModeString(Mode mode) {
    const QString prefix = "Currently: ";
    switch (mode) {
        case Mode::DRAW:
            return prefix + "draw";
        case Mode::LINE:
            return prefix + "line";
        case Mode::TEXT:
            return prefix + "text";
        case Mode::IMAGE:
            return prefix + "image";
        case Mode::ERASE:
            return prefix + "erase";
    }
}

ControlsWidget::ControlsWidget(BoardWidget* boardWidget) :
    mBoardWidget(boardWidget),
    mLayout(this),
    mPointWidthLayout(&mPointWidthWidget),
    mPointWidthSlider(Qt::Orientation::Horizontal)
{
    mLayout.addStretch();

    mThemeButton.setText("Switch theme");
    connect(&mThemeButton, &QPushButton::clicked, this, &ControlsWidget::themeSwitchClicked);
    mLayout.addWidget(&mThemeButton);

    mColorButton.setText("Color");
    connect(&mColorButton, &QPushButton::clicked, this, &ControlsWidget::colorChangeClicked);
    mLayout.addWidget(&mColorButton);

    QFont pointWidthFont = mPointWidthLabel.font();
    pointWidthFont.setPointSize(10);

    mPointWidthLabel.setText("Width:");
    mPointWidthLabel.setFont(pointWidthFont);

    mPointWidthSlider.setTickInterval(1);
    mPointWidthSlider.setMinimum(1);
    mPointWidthSlider.setMaximum(BoardWidget::MAX_POINT_WIDTH);
    mPointWidthSlider.setValue(mBoardWidget->pointWidth());
    mPointWidthSlider.setTickPosition(QSlider::TicksAbove);
    mPointWidthSlider.setFixedSize(300, 20);
    connect(&mPointWidthSlider, &QSlider::valueChanged, this, &ControlsWidget::pointWidthChanged);

    mPointWidthLayout.addWidget(&mPointWidthLabel);
    mPointWidthLayout.addWidget(&mPointWidthSlider);

    mLayout.addWidget(&mPointWidthWidget);

    mLayout.addStretch();

    mDrawButton.setText("Draw");
    connect(&mDrawButton, &QPushButton::clicked, this, [this](){ modeSelected(Mode::DRAW); });
    mLayout.addWidget(&mDrawButton);

    mLineButton.setText("Line");
    connect(&mLineButton, &QPushButton::clicked, this, [this](){ modeSelected(Mode::LINE); });
    mLayout.addWidget(&mLineButton);

    mTextButton.setText("Text");
    connect(&mTextButton, &QPushButton::clicked, this, [this](){ modeSelected(Mode::TEXT); });
    mLayout.addWidget(&mTextButton);

    mImageButton.setText("Image");
    connect(&mImageButton, &QPushButton::clicked, this, &ControlsWidget::imageSelectClicked);
    mLayout.addWidget(&mImageButton);

    mEraseButton.setText("Erase");
    connect(&mEraseButton, &QPushButton::clicked, this, [this](){ modeSelected(Mode::ERASE); });
    mLayout.addWidget(&mEraseButton);

    mModeLabel.setText(makeModeString(mBoardWidget->mode()));
    mLayout.addWidget(&mModeLabel);

    mLayout.addStretch();

    mUndoButton.setText("Undo");
    connect(&mUndoButton, &QPushButton::clicked, this, &ControlsWidget::undoCLicked);
    mLayout.addWidget(&mUndoButton);

    mClearButton.setText("Clear");
    connect(&mClearButton, &QPushButton::clicked, this, &ControlsWidget::clearClicked);
    mLayout.addWidget(&mClearButton);

    mLayout.addStretch();

    mExportButton.setText("Export");
    connect(&mExportButton, &QPushButton::clicked, this, &ControlsWidget::exportClicked);
    mLayout.addWidget(&mExportButton);

    mLayout.addStretch();

    emit updated();
}

void ControlsWidget::updateMode() {
    modeSelected(mBoardWidget->mode());
}

void ControlsWidget::themeSwitchClicked() {
    mBoardWidget->setTheme(mBoardWidget->theme() == Theme::Dark ? Theme::Light : Theme::Dark);
    emit updated();
}

void ControlsWidget::colorChangeClicked() {
    QColorDialog dialog(mBoardWidget->color(), this);
    dialog.setModal(true);
    connect(&dialog, &QColorDialog::colorSelected, this, &ControlsWidget::colorSelected);
    dialog.exec();
}

void ControlsWidget::colorSelected(QColor color) {
    color.setAlpha(0xff);
    mBoardWidget->setColor(color);

    emit updated();
}

void ControlsWidget::pointWidthChanged(int width) {
    mBoardWidget->setPointWidth(width);
    emit updated();
}

void ControlsWidget::modeSelected(Mode mode) {
    mBoardWidget->setMode(mode);
    mModeLabel.setText(makeModeString(mode));

    if (mode == Mode::TEXT) {
        mPointWidthSlider.setValue(24);
        mBoardWidget->setPointWidth(24);
    }

    emit updated();
}

void ControlsWidget::imageSelectClicked() {
    QFileDialog dialog(this);
    dialog.setModal(true);
    dialog.setFileMode(QFileDialog::FileMode::ExistingFile);
    connect(&dialog, &QFileDialog::fileSelected, this, &ControlsWidget::imageSelected);
    dialog.exec();
}

void ControlsWidget::imageSelected(const QString& path) {
    QPixmap image(path);

    if (image.isNull() || image.size().isNull()) {
        QMessageBox messageBox(this);
        messageBox.setModal(true);
        messageBox.setText("File is not an image");
        messageBox.exec();
        return;
    }

    modeSelected(Mode::IMAGE);
    mBoardWidget->setCurrentTexture(glm::vec2(image.width(), image.height()), image.toImage().convertToFormat(QImage::Format::Format_RGBA8888).constBits());

    emit updated();
}

void ControlsWidget::undoCLicked() {
    mBoardWidget->undo();
    emit updated();
}

void ControlsWidget::clearClicked() {
    mBoardWidget->clear();
    emit updated();
}

void ControlsWidget::exportClicked() {
    QFileDialog dialog(this);
    dialog.setModal(true);
    dialog.setFileMode(QFileDialog::FileMode::AnyFile);
    connect(&dialog, &QFileDialog::fileSelected, this, &ControlsWidget::outputFileSelected);
    dialog.exec();
}

void ControlsWidget::outputFileSelected(const QString& path) {
    const auto size = mBoardWidget->size();
    auto pixels = mBoardWidget->pixels();

    for(int line = 0; line != size.height()/2; ++line)
        std::swap_ranges(
            pixels.begin() + 4 * size.width() * line,
            pixels.begin() + 4 * size.width() * (line + 1),
            pixels.begin() + 4 * size.width() * (size.height() - line - 1)
        );

    QImage image(reinterpret_cast<const uchar*>(pixels.data()), size.width(), size.height(), QImage::Format::Format_RGBA8888);

    QFile file(path);
    file.open(QIODevice::WriteOnly);
    image.save(&file, "PNG");
    file.close();
}

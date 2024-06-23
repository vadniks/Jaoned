
#include "ControlsWidget.hpp"
#include <QColorDialog>

ControlsWidget::ControlsWidget(BoardWidget* boardWidget) : mBoardWidget(boardWidget), mLayout(this) {
    mLayout.addStretch();

    mThemeButton.setText("Switch theme");
    connect(&mThemeButton, &QPushButton::clicked, this, &ControlsWidget::themeSwitchClicked);
    mLayout.addWidget(&mThemeButton);

    mColorButton.setText("Color");
    connect(&mColorButton, &QPushButton::clicked, this, &ControlsWidget::colorChangeClicked);
    mLayout.addWidget(&mColorButton);

    mPointWidthButton.setText("Point width");
    mLayout.addWidget(&mPointWidthButton);

    mLayout.addStretch();

    mDrawButton.setText("Draw");
    mLayout.addWidget(&mDrawButton);

    mTextButton.setText("Text");
    mLayout.addWidget(&mTextButton);

    mLineButton.setText("Line");
    mLayout.addWidget(&mLineButton);

    mLayout.addStretch();
}

void ControlsWidget::themeSwitchClicked() {
    mBoardWidget->setTheme(mBoardWidget->theme() == Theme::Dark ? Theme::Light : Theme::Dark);
}

void ControlsWidget::colorChangeClicked() {
    QColorDialog dialog(mBoardWidget->color(), this);
    dialog.setModal(true);
    connect(&dialog, &QColorDialog::colorSelected, this, &ControlsWidget::colorSelected);
    dialog.exec();
}

void ControlsWidget::colorSelected(QColor color) {
    mBoardWidget->setColor(color);
}


#include "ControlsWidget.hpp"

ControlsWidget::ControlsWidget(BoardWidget* boardWidget) : mBoardWidget(boardWidget), mLayout(this) {
    mLayout.addStretch();

    mThemeButton.setText("Switch theme");
    connect(&mThemeButton, &QPushButton::clicked, this, &ControlsWidget::themeChanged);
    mLayout.addWidget(&mThemeButton);

    mColorButton.setText("Color");
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

void ControlsWidget::themeChanged() {
    mBoardWidget->setTheme(mBoardWidget->theme() == Theme::Dark ? Theme::Light : Theme::Dark);
}

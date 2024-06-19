
#include "ControlsWidget.hpp"

ControlsWidget::ControlsWidget() : mLayout(this), mDarkTheme(true), mColor(0), mPointSize(1), mFontSize(24) {
    mThemeButton.setText("Switch theme");
    mLayout.addWidget(&mThemeButton);

    mDrawButton.setText("Draw");
    mLayout.addWidget(&mDrawButton);

    mTextButton.setText("Text");
    mLayout.addWidget(&mTextButton);

    mLineButton.setText("Line");
    mLayout.addWidget(&mLineButton);
}

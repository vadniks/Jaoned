
#include "ControlsWidget.hpp"

ControlsWidget::ControlsWidget() : mLayout(this) {
    mLayout.addStretch();

    mThemeButton.setText("Switch theme");
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


#include "ControlsWidget.hpp"

ControlsWidget::ControlsWidget() : mLayout(this) {
    mThemeButton.setText("Switch theme");
    mLayout.addWidget(&mThemeButton);

    mDrawButton.setText("Draw");
    mLayout.addWidget(&mDrawButton);

    mTextButton.setText("Text");
    mLayout.addWidget(&mTextButton);

    mLineButton.setText("Line");
    mLayout.addWidget(&mLineButton);
}

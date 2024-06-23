
#include "ControlsWidget.hpp"
#include <QColorDialog>

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

void ControlsWidget::pointWidthChanged(int width) {
    mBoardWidget->setPointWidth(width);
}

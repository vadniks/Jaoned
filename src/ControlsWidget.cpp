
#include "ControlsWidget.hpp"
#include <QColorDialog>

static QString makeDrawModeString(Mode mode) {
    const QString prefix = "Currently: ";
    switch (mode) {
        case DRAW:
            return prefix + "draw";
        case LINE:
            return prefix + "line";
        case TEXT:
            return prefix + "text";
        case IMAGE:
            return prefix + "image";
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
    mLayout.addWidget(&mDrawButton);

    mLineButton.setText("Line");
    mLayout.addWidget(&mLineButton);

    mTextButton.setText("Text");
    mLayout.addWidget(&mTextButton);

    mImageButton.setText("Image");
    mLayout.addWidget(&mImageButton);

    mDrawModeLabel.setText(makeDrawModeString(mBoardWidget->mode()));
    mLayout.addWidget(&mDrawModeLabel);

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
    color.setAlpha(0xff);
    mBoardWidget->setColor(color);
}

void ControlsWidget::pointWidthChanged(int width) {
    mBoardWidget->setPointWidth(width);
}


#pragma once

#include "Mode.hpp"
#include "BoardWidget.hpp"
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton> // TODO: optimize includes
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
    QLabel mDrawModeLabel;
public:
    explicit ControlsWidget(BoardWidget* boardWidget);
private slots:
    void themeSwitchClicked();
    void colorChangeClicked();
    void colorSelected(QColor color);
    void pointWidthChanged(int width);
    void modeSelected(Mode mode);
};

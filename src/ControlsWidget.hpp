
#pragma once

#include "Mode.hpp"
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

class ControlsWidget final : public QWidget {
    Q_OBJECT
private:
    QHBoxLayout mLayout;
    QPushButton mThemeButton;
    QPushButton mColorButton;
    QPushButton mPointWidthButton;
    QPushButton mDrawButton;
    QPushButton mTextButton;
    QPushButton mLineButton;
public:
    ControlsWidget();
signals:
    void themeChanged(bool dark);
    void colorChanged(int color);
    void pointWidthChanged(int width);
    void modeChanged(Mode mode);
};

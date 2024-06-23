
#pragma once

#include "Mode.hpp"
#include "BoardWidget.hpp"
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

class ControlsWidget final : public QWidget {
    Q_OBJECT
private:
    BoardWidget* mBoardWidget; // allocated elsewhere
    QHBoxLayout mLayout;
    QPushButton mThemeButton;
    QPushButton mColorButton;
    QPushButton mPointWidthButton;
    QPushButton mDrawButton;
    QPushButton mTextButton;
    QPushButton mLineButton;
public:
    ControlsWidget(BoardWidget* boardWidget);
private slots:
    void themeChanged();
//    void colorChanged(int color);
//    void pointWidthChanged(int width);
//    void modeChanged(Mode mode);
};


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
    explicit ControlsWidget(BoardWidget* boardWidget);
private slots:
    void themeSwitchClicked();
    void colorChangeClicked();
    void colorSelected(QColor color);
//    void pointWidthChanged(int width);
//    void modeChanged(Mode mode);
};

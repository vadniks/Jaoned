
#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

class ControlsWidget final : public QWidget {
    Q_OBJECT
private:
    QHBoxLayout mLayout;
    QPushButton mThemeButton;
    QPushButton mDrawButton;
    QPushButton mTextButton;
    QPushButton mLineButton;
    bool mDarkTheme;
    int mColor;
    int mPointSize;
    int mFontSize;
public:
    ControlsWidget();
signals:
    void themeChanged(bool dark);
};

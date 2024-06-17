
#pragma once

#include "MainWidget.hpp"
#include "defs.hpp"
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    MainWidget mMainWidget;
public:
    MainWindow();

    DISABLE_COPY(MainWindow)
    DISABLE_MOVE(MainWindow)
};

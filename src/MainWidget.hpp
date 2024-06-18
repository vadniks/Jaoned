
#pragma once

#include "defs.hpp"
#include "BoardWidget.hpp"
#include <QWidget>
#include <QVBoxLayout>

class MainWidget final : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout mLayout;
    BoardWidget* mBoardWidget;
public:
    MainWidget();
    ~MainWidget() override;

    DISABLE_COPY(MainWidget)
    DISABLE_MOVE(MainWidget)
};

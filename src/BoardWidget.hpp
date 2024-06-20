
#pragma once

#include "defs.hpp"
#include <QWidget>

class BoardWidget final : public QWidget {
    Q_OBJECT
private:
    struct Coordinate {
        int x, y;

        Coordinate(int x, int y) {
            this->x = x;
            this->y = y;
        }
    };
private:
    int mOffsetX, mOffsetY;
    QVector<QVector<Coordinate>*> mMouseDrawnPoints;
    QVector<Coordinate>* mCurrentMouseDrawnPoints;
public:
    BoardWidget();
    ~BoardWidget() override;

    QSize minimumSizeHint() const override;

    DISABLE_COPY(BoardWidget)
    DISABLE_MOVE(BoardWidget)
protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};

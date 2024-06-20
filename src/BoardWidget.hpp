
#pragma once

#include "defs.hpp"
#include <QWidget>

class BoardWidget final : public QWidget {
    Q_OBJECT
public:
    enum Mode {
        DRAW, LINE, TEXT
    };
private:
    struct Coordinate {
        int x, y;
        Coordinate(int x, int y);
    };
    struct LineCoordinates {
        Coordinate start, end;
        LineCoordinates(Coordinate start, Coordinate end);
    };
private:
    Mode mMode;
    QColor mCanvasColor;
    int mColor;
    int mWidth;
    int mOffsetX, mOffsetY;
    QVector<QVector<Coordinate>*> mMouseDrawnPoints;
    QVector<Coordinate>* mCurrentMouseDrawnPoints; // nullable
    QVector<LineCoordinates*> mLines;
    LineCoordinates* mCurrentLine; // nullable
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
private:
    void paintDrawn(QPainter& painter);
    void paintLines(QPainter& painter);
};

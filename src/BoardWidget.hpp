
#pragma once

#include "defs.hpp"
#include "Mode.hpp"
#include <QWidget>
#include <glm/glm.hpp>

class BoardWidget final : public QWidget {
    Q_OBJECT
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
    bool mTheme; // true - dark
    int mColor;
    int mPointWidth;
    glm::mat4 mProjection;
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
    void updateProjection();
    void paintDrawn(QPainter& painter);
    void paintLines(QPainter& painter);
public slots:
    void setMode(Mode mode);
    void setTheme(bool theme);
    void setColor(int color);
    void setPointWidth(int width);
public:
    Mode mode() const;
    bool theme() const;
    int color() const;
    int pointWidth() const;
};

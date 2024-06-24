
#pragma once

#include "defs.hpp"
#include "Mode.hpp"
#include "Theme.hpp"
#include "Renderer.hpp"
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>

class DrawnElement;
class DrawnPoint;
class DrawnLine;
class DrawnText;

class BoardWidget final : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
private:
    Mode mMode;
    Theme mTheme;
    QColor mColor;
    int mPointWidth;
    glm::mat4 mProjection;
    Renderer* mRenderer;
    int mOffsetX, mOffsetY;
    QVector<QVector<DrawnPoint>*> mMouseDrawnPoints;
    QVector<DrawnPoint>* mCurrentMouseDrawnPoints; // nullable
    QVector<DrawnLine*> mLines;
    DrawnLine* mCurrentLine; // nullable
    QVector<DrawnText*> mTexts;
    DrawnText* mCurrentText; // nullable
public:
    static inline int MAX_POINT_WIDTH = 100;
public:
    BoardWidget();
    ~BoardWidget() override;

    QSize minimumSizeHint() const override;

    DISABLE_COPY(BoardWidget)
    DISABLE_MOVE(BoardWidget)
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
private:
    void updateProjection();
    void paintDrawn();
    void paintLines();
    void paintTexts();
public slots:
    void setMode(Mode mode);
    void setTheme(Theme theme);
    void setColor(const QColor& color);
    void setPointWidth(int width);
public:
    Mode mode() const;
    Theme theme() const;
    QColor color() const;
    int pointWidth() const;
};

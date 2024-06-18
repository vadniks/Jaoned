
#include "BoardWidget.hpp"
#include <QFontMetrics>
#include <QPainter>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

BoardWidget::BoardWidget() : mProjection(1.0f), mRenderer(nullptr) {

}

BoardWidget::~BoardWidget() {
    delete mRenderer;
}

QSize BoardWidget::minimumSizeHint() const {
    return {16 * 75, 9 * 75};
}

QSize BoardWidget::sizeHint() const {
    return minimumSizeHint();
}

void BoardWidget::initializeGL() {
    QOpenGLFunctions_3_3_Core::initializeOpenGLFunctions();
    mRenderer = new Renderer(*this);
    glViewport(0, 0, 100, 100);
}

void BoardWidget::paintGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mRenderer->drawRectangle(glm::vec2(100, 100), glm::vec2(100, 800), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), false);

    QString text("Hello World!");
    QFont font("Sans", 15);
    QFontMetrics metrics(font);
    auto size = metrics.size(Qt::TextFlag::TextSingleLine, text);

    QImage image(size.width(), size.height(), QImage::Format::Format_RGB16);
    QPainter painter(&image);
    painter.fillRect(0, 0, size.width(), size.height(), QColor(0, 0, 0, 0));
    painter.setBrush(QBrush(QColor(255, 255, 255)));
    painter.setPen(QPen(QColor(255, 255, 255)));
    painter.setFont(font);
    painter.drawText(0, 0, text);

    Texture texture(*this, size.width(), size.height(), image.constBits(), GL_RGB16);
    mRenderer->drawTexture(texture, glm::vec2(100, 100), glm::vec2(size.width(), size.height()), 0.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void BoardWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    mProjection = glm::ortho(0.0f, static_cast<float>(w), static_cast<float>(h), 0.0f, -1.0f, 1.0f);
    mRenderer->setProjection(mProjection);
}

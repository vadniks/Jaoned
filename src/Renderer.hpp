
#pragma once

#include "defs.hpp"
#include "Texture.hpp"
#include "CompoundShader.hpp"
#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <freetype2/ft2build.h>
#include <freetype/freetype.h>

class Renderer final {
private:
    QOpenGLFunctions_3_3_Core& mGl;
    CompoundShader* mShapeShader, * mSpriteShader;
    unsigned mVbo, mEbo, mVao;
    glm::mat4 mProjection;
    FT_Library mFtLib;
    FT_Face mFtFace;
public:
    explicit Renderer(QOpenGLFunctions_3_3_Core& gl);
    ~Renderer();

    DISABLE_COPY(Renderer)
    DISABLE_MOVE(Renderer)

    void setProjection(const glm::mat4& projection);

    void drawPoint(const glm::vec2& position, float pointSize, const glm::vec4& color);
    void drawPoints(int count, const QVector<float>& vertices, float pointSize, const glm::vec4& color, int drawMode);
    void drawLine(const glm::vec2& positionStart, const glm::vec2& positionEnd, float lineWidth, const glm::vec4& color);
    void drawFilledCircle(const glm::vec2& positionCenter, int radius, const glm::vec4& color);
    void drawTexture(Texture& texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, bool isMono = false);
    void drawText(const QString& text, int size, const glm::vec2& position, const glm::vec4& color);
};

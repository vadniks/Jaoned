/*
 * Jaoned - an OpenGL & QT based drawing board
 * Copyright (C) 2024 Vadim Nikolaev (https://github.com/vadniks).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "Renderer.hpp"
#include <QSize>
#include <glm/ext/matrix_transform.hpp>

static const char* const gShapeVertexShader = R"(
    #version 330 core
    layout (location = 0) in vec2 pos;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * vec4(pos, 0.0, 1.0);
    }
)";

static const char* const gShapeFragmentShader = R"(
    #version 330 core
    out vec4 colorOut;
    uniform vec4 color;
    void main() {
        colorOut = color;
    }
)";

static const char* const gSpriteVertexShader = R"(
    #version 330 core
    layout (location = 0) in vec4 vertex;
    out vec2 textureCoords;
    uniform mat4 model;
    uniform mat4 projection;
    void main() {
        textureCoords = vertex.zw;
        gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
    }
)";

static const char* const gSpriteFragmentShader = R"(
    #version 330 core
    in vec2 textureCoords;
    out vec4 color;
    uniform sampler2D sprite;
    uniform vec4 spriteColor;
    uniform int isMono;
    void main() {
        if (isMono == 0)
            color = spriteColor * texture(sprite, textureCoords);
        else {
            vec4 sampled = texture(sprite, textureCoords);
            color = spriteColor * vec4(sampled.r, sampled.r, sampled.r, sampled.r);
        }
    }
)";

static const char* FONT_FILE = "res/Roboto-Regular.ttf";
static const char* PROJECTION = "projection";
static const char* COLOR = "color";
static const char* MODEL = "model";
static const char* SPRITE_COLOR = "spriteColor";
static const char* IS_MONO = "isMono";

Renderer::Renderer(QOpenGLFunctions_3_3_Core& gl) :
    mGl(gl),
    mVbo(0),
    mEbo(0),
    mVao(0),
    mProjection(1.0f),
    mFtLib(),
    mFtFace()
{
    mShapeShader = new CompoundShader(gl, gShapeVertexShader, gShapeFragmentShader);
    mSpriteShader = new CompoundShader(gl, gSpriteVertexShader, gSpriteFragmentShader);
    mGl.glGenBuffers(1, &mVbo);
    mGl.glGenBuffers(1, &mEbo);
    mGl.glGenVertexArrays(1, &mVao);

    assert(FT_Init_FreeType(&mFtLib) == 0);
    assert(FT_New_Face(mFtLib, FONT_FILE, 0, &mFtFace) == 0);
}

Renderer::~Renderer() {
    delete mShapeShader;
    delete mSpriteShader;
    mGl.glDeleteBuffers(1, &mVbo);
    mGl.glDeleteBuffers(1, &mEbo);
    mGl.glDeleteVertexArrays(1, &mVao);

    assert(FT_Done_Face(mFtFace) == 0);
    assert(FT_Done_FreeType(mFtLib) == 0);
}

void Renderer::setProjection(const glm::mat4& projection) {
    mProjection = projection;
}

void Renderer::drawPoint(const glm::vec2& position, float pointSize, const glm::vec4& color) {
    mGl.glBindVertexArray(mVao);

    const float vertices[] = {
        position[0], position[1]
    };

    mGl.glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    mGl.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    mGl.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(0));
    mGl.glEnableVertexAttribArray(0);

    mShapeShader->use();
    mShapeShader->setValue(PROJECTION, mProjection);
    mShapeShader->setValue(COLOR, color);

    mGl.glPointSize(pointSize);
    mGl.glDrawArrays(GL_POINTS, 0, 1);

    mGl.glBindBuffer(GL_ARRAY_BUFFER, 0);
    mGl.glBindVertexArray(0);
}

void Renderer::drawPoints(int count, const QVector<float>& vertices, float pointSize, const glm::vec4& color, int drawMode) {
    assert(drawMode == GL_POINTS || drawMode == GL_TRIANGLES);

    mGl.glBindVertexArray(mVao);

    mGl.glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    mGl.glBufferData(GL_ARRAY_BUFFER, static_cast<long>(count * sizeof(float)), vertices.data(), GL_DYNAMIC_DRAW);

    mGl.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(0));
    mGl.glEnableVertexAttribArray(0);

    mShapeShader->use();
    mShapeShader->setValue(PROJECTION, mProjection);
    mShapeShader->setValue(COLOR, color);

    mGl.glPointSize(pointSize);
    mGl.glDrawArrays(drawMode, 0, count);

    mGl.glBindBuffer(GL_ARRAY_BUFFER, 0);
    mGl.glBindVertexArray(0);
}

void Renderer::drawLine(const glm::vec2& positionStart, const glm::vec2& positionEnd, float lineWidth, const glm::vec4& color) {
    const float dx = positionEnd.x - positionStart.x;
    const float dy = positionEnd.y - positionStart.y;

    glm::vec2 normal(-dy, dx);
    normal = glm::normalize(normal);

    normal.x *= lineWidth * 0.5f;
    normal.y *= lineWidth * 0.5f;

    const glm::vec2 c1 = positionStart - normal;
    const glm::vec2 c2 = positionStart + normal;
    const glm::vec2 c3 = positionEnd - normal;
    const glm::vec2 c4 = positionEnd + normal;

    mGl.glBindVertexArray(mVao);

    const float vertices[] = {
        c1.x, c1.y,
        c2.x, c2.y,
        c3.x, c3.y,
        c4.x, c4.y,
    };

    mGl.glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    mGl.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    mGl.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(0));
    mGl.glEnableVertexAttribArray(0);

    const unsigned indices[] = {
        0, 1, 3,
        3, 0, 2
    };

    mGl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    mGl.glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_DYNAMIC_DRAW
    );

    mShapeShader->use();
    mShapeShader->setValue(PROJECTION, mProjection);
    mShapeShader->setValue(COLOR, color);

    mGl.glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));

    mGl.glBindBuffer(GL_ARRAY_BUFFER, 0);
    mGl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    mGl.glBindVertexArray(0);
}

void Renderer::drawHollowCircle(const glm::vec2& positionCenter, int radius, const glm::vec4& color) {
    int count = 0;
    QVector<float> vertices;

    const auto addVertex = [&](float x, float y) {
        count += 2;
        vertices.resize(static_cast<long>(count * sizeof(float)));
        vertices[count - 2] = positionCenter.x + x; vertices[count - 1] = positionCenter.y + y;
    };

    float x, y;
    for (int i = 0; i < 180; i++) {
        x = static_cast<float>(radius) * cosf(static_cast<float>(i));
        y = static_cast<float>(radius) * sinf(static_cast<float>(i));
        addVertex(x, y);
    }

    if (count > 0)
        drawPoints(count, vertices, 1.0f, color, GL_TRIANGLES);
}

void Renderer::drawTexture(Texture& texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, bool isMono) {
    mGl.glBindVertexArray(mVao);

    float vertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    mGl.glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    mGl.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    mGl.glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
    mGl.glEnableVertexAttribArray(0);

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position[0], position[1], 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size[0], 0.5f * size[1], 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size[0], -0.5f * size[1], 0.0f));

    model = glm::scale(model, glm::vec3(size[0], size[1], 1.0f));

    mSpriteShader->use();
    mSpriteShader->setValue(PROJECTION, mProjection);
    mSpriteShader->setValue(MODEL, model);
    mSpriteShader->setValue(SPRITE_COLOR, color);
    mSpriteShader->setValue(IS_MONO, isMono ? 1 : 0);

    mGl.glActiveTexture(GL_TEXTURE0);
    texture.bind();

    mGl.glBindVertexArray(mVao);
    mGl.glDrawArrays(GL_TRIANGLES, 0, 6);

    mGl.glBindBuffer(GL_ARRAY_BUFFER, 0);
    mGl.glBindVertexArray(0);
}

void Renderer::drawText(const QString& text, int size, const glm::vec2& position, const glm::vec4& color) {
    assert(FT_Set_Pixel_Sizes(mFtFace, 0, size) == 0);

    int maxHeight = 0;
    for (auto i : text) {
        assert(FT_Load_Char(mFtFace, i.unicode(), FT_LOAD_RENDER) == 0);
        const int height = static_cast<int>(mFtFace->glyph->bitmap.rows);
        if (height > maxHeight)
            maxHeight = height;
    }

    int offset = 0;
    for (auto i : text) {
        assert(FT_Load_Char(mFtFace, i.unicode(), FT_LOAD_RENDER) == 0);

        const auto xSize = glm::ivec2(mFtFace->glyph->bitmap.width, mFtFace->glyph->bitmap.rows);
        const auto bearing = glm::ivec2(mFtFace->glyph->bitmap_left, mFtFace->glyph->bitmap_top);
        const auto advance = static_cast<int>(mFtFace->glyph->advance.x);

        Texture texture(mGl, xSize[0], xSize[1], mFtFace->glyph->bitmap.buffer, GL_RED);
        drawTexture(texture, glm::vec2(
            position.x + static_cast<float>(bearing.x) + static_cast<float>(offset),
            position.y - static_cast<float>(bearing.y) + static_cast<float>(maxHeight)
        ), xSize, 0.0f, color, true);

        offset += advance >> 6;
    }
}

QSize Renderer::textMetrics(const QString& text, int size) {
    assert(FT_Set_Pixel_Sizes(mFtFace, 0, size) == 0);

    int width = 0, height = 0;

    for (auto i : text) {
        assert(FT_Load_Char(mFtFace, i.unicode(), FT_LOAD_RENDER) == 0);
        width += static_cast<int>(mFtFace->glyph->advance.x) >> 6;

        const auto xHeight = static_cast<int>(mFtFace->glyph->bitmap.rows);
        if (height < xHeight)
            height = xHeight;
    }

    return {width, height};
}

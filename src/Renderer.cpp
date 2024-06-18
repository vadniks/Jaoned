
#include "Renderer.hpp"
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
    void main() {
        color = spriteColor * texture(sprite, textureCoords);
    }
)";

Renderer::Renderer(QOpenGLFunctions_3_3_Core& gl) : mGl(gl), mVbo(0), mEbo(0), mVao(0), mProjection(1.0f), mFtLib(), mFtFace() {
    mShapeShader = new CompoundShader(gl, gShapeVertexShader, gShapeFragmentShader);
    mSpriteShader = new CompoundShader(gl, gSpriteVertexShader, gSpriteFragmentShader);
    mGl.glGenBuffers(1, &mVbo);
    mGl.glGenBuffers(1, &mEbo);
    mGl.glGenVertexArrays(1, &mVao);

    assert(FT_Init_FreeType(&mFtLib) == 0);
    assert(FT_New_Face(mFtLib, "res/Roboto-Regular.ttf", 0, &mFtFace) == 0);
}

Renderer::~Renderer() {
    delete mShapeShader;
    delete mSpriteShader;
    mGl.glDeleteBuffers(1, &mVbo);
    mGl.glDeleteBuffers(1, &mEbo);
    mGl.glDeleteVertexArrays(1, &mVao);

    assert(FT_Done_FreeType(mFtLib) == 0);
//    assert(FT_Done_Face(mFtFace) == 0);
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
    mShapeShader->setValue("projection", mProjection);
    mShapeShader->setValue("color", color);

    mGl.glPointSize(static_cast<float>(pointSize));
    mGl.glDrawArrays(GL_POINTS, 0, 1);

    mGl.glBindBuffer(GL_ARRAY_BUFFER, 0);
    mGl.glBindVertexArray(0);
}

void Renderer::drawPoints(int count, const QVector<float>& vertices, float pointSize, const glm::vec4& color) {
    mGl.glBindVertexArray(mVao);

    mGl.glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    mGl.glBufferData(GL_ARRAY_BUFFER, static_cast<long>(count * sizeof(float)), vertices.data(), GL_DYNAMIC_DRAW);

    mGl.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(0));
    mGl.glEnableVertexAttribArray(0);

    mShapeShader->use();
    mShapeShader->setValue("projection", mProjection);
    mShapeShader->setValue("color", color);

    mGl.glPointSize((float) pointSize);
    mGl.glDrawArrays(GL_POINTS, 0, count);

    mGl.glBindBuffer(GL_ARRAY_BUFFER, 0);
    mGl.glBindVertexArray(0);
}

void Renderer::drawLine(const glm::vec2& positionStart, const glm::vec2& positionEnd, float lineWidth, const glm::vec4& color) {
    mGl.glBindVertexArray(mVao);

    const float vertices[] = {
        positionStart[0], positionStart[1],
        positionEnd[0], positionEnd[1]
    };

    mGl.glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    mGl.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    mGl.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(0));
    mGl.glEnableVertexAttribArray(0);

    mShapeShader->use();
    mShapeShader->setValue("projection", mProjection);
    mShapeShader->setValue("color", color);

    mGl.glLineWidth(static_cast<float>(lineWidth));
    mGl.glDrawArrays(GL_LINES, 0, 2);

    mGl.glBindBuffer(GL_ARRAY_BUFFER, 0);
    mGl.glBindVertexArray(0);
}

void Renderer::drawRectangle(const glm::vec2& position, const glm::vec2& dimension, float lineWidth, const glm::vec4& color, bool filled) {
    mGl.glBindVertexArray(mVao);

    const float vertices[] = {
        position[0] + dimension[0], position[1],
        position[0] + dimension[0], position[1] + dimension[1],
        position[0], position[1] + dimension[1],
        position[0], position[1]
    };

    mGl.glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    mGl.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    mGl.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(0));
    mGl.glEnableVertexAttribArray(0);

    const unsigned indicesFilled[] = {
        0, 1, 3,
        1, 2, 3
    };

    const unsigned indicesUnfilled[] = {
        0, 1, 2, 3,
        1, 2, 0, 3
    };

    mGl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    mGl.glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        filled ? sizeof(indicesFilled) : sizeof(indicesUnfilled),
        filled ? indicesFilled : indicesUnfilled,
        GL_DYNAMIC_DRAW
    );

    mShapeShader->use();
    mShapeShader->setValue("projection", mProjection);
    mShapeShader->setValue("color", color);

    if (!filled)
        mGl.glLineWidth(static_cast<float>(lineWidth));

    mGl.glDrawElements(filled ? GL_TRIANGLES : GL_LINES, filled ? 6 : 8, GL_UNSIGNED_INT, static_cast<void*>(0));

    mGl.glBindBuffer(GL_ARRAY_BUFFER, 0);
    mGl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    mGl.glBindVertexArray(0);
}

static void drawUnfilledCircle(Renderer* renderer, const glm::vec2 positionCenter, int radius, float pointSize, const glm::vec4 color) {
    const int diameter = (radius * 2);

    int x = (radius - 1);
    int y = 0;
    int tx = 1;
    int ty = 1;
    int error = (tx - diameter);

    int count = 0;
    QVector<float> vertices(0);

    while (x >= y) {
        count += 16;
        vertices.resize(static_cast<long>(count * sizeof(float)));
        vertices[count - 16] = positionCenter[0] + (float) x; vertices[count - 15] = positionCenter[1] - (float) y;
        vertices[count - 14] = positionCenter[0] + (float) x; vertices[count - 13] = positionCenter[1] + (float) y;
        vertices[count - 12] = positionCenter[0] - (float) x; vertices[count - 11] = positionCenter[1] - (float) y;
        vertices[count - 10] = positionCenter[0] - (float) x; vertices[count - 9] = positionCenter[1] + (float) y;
        vertices[count - 8] = positionCenter[0] + (float) y; vertices[count - 7] = positionCenter[1] - (float) x;
        vertices[count - 6] = positionCenter[0] + (float) y; vertices[count - 5] = positionCenter[1] + (float) x;
        vertices[count - 4] = positionCenter[0] - (float) y; vertices[count - 3] = positionCenter[1] - (float) x;
        vertices[count - 2] = positionCenter[0] - (float) y; vertices[count - 1] = positionCenter[1] + (float) x;

        if (error <= 0) {
            y++;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            x--;
            tx += 2;
            error += (tx - diameter);
        }
    }

    if (count > 0)
        renderer->drawPoints(count, vertices, pointSize, color);
}

static void drawFilledCircle(Renderer* renderer, const glm::vec2 position, int radius, float pointSize, const glm::vec4 color) {
    int count = 0;
    QVector<float> vertices;

    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            const int dx = radius - w, dy = radius - h;

            if ((dx * dx + dy * dy) <= (radius * radius)) {
                count += 2;
                vertices.resize(static_cast<long>(count * sizeof(float)));
                vertices[count - 2] = position[0] + (float) dx; vertices[count - 1] = position[1] + (float) dy;
            }
        }
    }

    if (count > 0)
        renderer->drawPoints(count, vertices, pointSize, color);
}

void Renderer::drawCircle(const glm::vec2& positionCenter, int radius, float pointSize, const glm::vec4& color, bool filled) {
    if (!filled)
        drawUnfilledCircle(this, positionCenter, radius, pointSize, color);
    else
        drawFilledCircle(this, positionCenter, radius, pointSize, color);
}

void Renderer::drawTexture(Texture& texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
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
    mGl.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    mGl.glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
    mGl.glEnableVertexAttribArray(0);

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position[0], position[1], 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size[0], 0.5f * size[1], 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size[0], -0.5f * size[1], 0.0f));

    model = glm::scale(model, glm::vec3(size[0], size[1], 1.0f));

    mSpriteShader->use();
    mSpriteShader->setValue("projection", mProjection);
    mSpriteShader->setValue("model", model);
    mSpriteShader->setValue("spriteColor", color);

    mGl.glActiveTexture(GL_TEXTURE0);
    texture.bind();

    mGl.glBindVertexArray(mVao);
    mGl.glDrawArrays(GL_TRIANGLES, 0, 6);

    mGl.glBindBuffer(GL_ARRAY_BUFFER, 0);
    mGl.glBindVertexArray(0);
}

void Renderer::drawText(const QString& text, int size, const glm::vec2& position, const glm::vec4& color) {
    assert(FT_Set_Pixel_Sizes(mFtFace, 0, size) == 0);

    int offset = 0;
    for (auto i : text) {
        assert(FT_Load_Char(mFtFace, i.unicode(), FT_LOAD_RENDER) == 0);

        auto xSize = glm::vec2(mFtFace->glyph->bitmap.width, mFtFace->glyph->bitmap.rows);
        auto bearing = glm::vec2(mFtFace->glyph->bitmap_left, mFtFace->glyph->bitmap_top);
        auto advance = static_cast<int>(mFtFace->glyph->advance.x);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        Texture texture(mGl, static_cast<int>(xSize[0]), static_cast<int>(xSize[1]), mFtFace->glyph->bitmap.buffer, GL_RED);
        drawTexture(texture, glm::vec2(position.x + bearing.x + static_cast<float>(offset), position.y - xSize.y + bearing.y), xSize, 0.0f, color);

        offset += advance >> 6;
    }
}

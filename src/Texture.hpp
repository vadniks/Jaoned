
#pragma once

#include "defs.hpp"
#include <QOpenGLFunctions_3_3_Core>

class Texture final {
private:
    QOpenGLFunctions_3_3_Core& mGl;
    unsigned mId;
    int mWidth, mHeight;
public:
    Texture(QOpenGLFunctions_3_3_Core& gl, int width, int height, const uchar* data, int format = GL_RGBA);
    ~Texture();

    DISABLE_COPY(Texture)
    DISABLE_MOVE(Texture)

    void bind();
    QSize size();
};

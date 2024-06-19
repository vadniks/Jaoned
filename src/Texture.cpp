
#include "Texture.hpp"
#include <QSize>

Texture::Texture(QOpenGLFunctions_3_3_Core& gl, int width, int height, const uchar* data, int format, bool mono) : mGl(gl), mId(0), mWidth(width), mHeight(height) {
    mGl.glGenTextures(1, &mId);
    mGl.glBindTexture(GL_TEXTURE_2D, mId);
    if (mono) mGl.glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    mGl.glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );
    mGl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    mGl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    mGl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    mGl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    mGl.glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    mGl.glDeleteTextures(1, &mId);
}

void Texture::bind() {
    mGl.glBindTexture(GL_TEXTURE_2D, mId);
}

QSize Texture::size() {
    return {mWidth, mHeight};
}


#include <dnealar/dnealar.h>
#include <dnealar/primitives.h>
#include <dnealar/texture.h>
#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

enum FontSize {
    FONT_SIZE_DEFAULT = 16
};

static SDL_Window* gWindow = nullptr;
static TTF_Font* gFont = nullptr;

static void render() {
    dlrPrimitivesPoint(100, 100, 5, 255, 255, 255, 255);
    dlrPrimitivesLine(10, 10, 90, 90, 5, 255, 255, 255, 255);
    dlrPrimitivesRectangle(110, 110, 100, 50, 1, 255, 255, 255, 255, true);
    dlrPrimitivesRectangle(220, 170, 100, 50, 5, 255, 255, 255, 255, false);
    dlrPrimitivesCircle(300, 300, 50, 5, 255, 255, 255, 255, false);
    dlrPrimitivesCircle(600, 300, 50, 1, 255, 255, 255, 255, true);

    SDL_Surface* surface = TTF_RenderUTF8_Blended(gFont, "Hello World!", (SDL_Color) {255, 255, 255, 255});
    assert(surface != nullptr);
    SDL_Surface* xSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surface);
    assert(xSurface != nullptr);
    DlrTexture* texture = dlrTextureCreate(xSurface->w, xSurface->h, reinterpret_cast<dlrByte*>(xSurface->pixels));
    SDL_FreeSurface(xSurface);
    dlrPrimitivesTexture(texture, 500, 0, dlrTextureWidth(texture), dlrTextureHeight(texture), 0.0f, 255, 255, 255, 255);
    dlrTextureDestroy(texture);
}

static void loop() {
    int width, height;
    SDL_Event event;

    while (true) {
        SDL_GL_GetDrawableSize(gWindow, &width, &height);
        dlrSetViewport(width, height);

        while (SDL_PollEvent(&event) == 1) {
            switch (event.type) {
                case SDL_QUIT:
                    goto end;
                case SDL_MOUSEMOTION:
                    dlrUpdateMousePosition(event.motion.x, event.motion.y);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    dlrUpdateMouseButtonState(true);
                    break;
                case SDL_MOUSEBUTTONUP:
                    dlrUpdateMouseButtonState(false);
                    break;
                case SDL_KEYDOWN:
                    dlrKeyDown(event.key.keysym.sym == SDLK_BACKSPACE);
                    break;
                case SDL_KEYUP:
                    dlrKeyUp();
                    break;
                case SDL_TEXTINPUT:
                    dlrTextInput(event.text.text);
                    break;
            }
        }

        dlrUpdateFrame();
        render();
        SDL_GL_SwapWindow(gWindow);
    }
    end:
    (void) 0;
}

static void* DLR_NONNULL textTextureCreate(const char* DLR_NONNULL text, int fontSize, int r, int g, int b, int a) {
    assert(fontSize == FONT_SIZE_DEFAULT);

    SDL_Surface* surface = TTF_RenderUTF8_Blended(gFont, text, (SDL_Color) {static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b), static_cast<unsigned char>(a)});
    assert(surface != nullptr);

    SDL_Surface* xSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    assert(xSurface != nullptr);

    SDL_FreeSurface(surface);
    return xSurface;
}

static void* DLR_NONNULL wrappedTextTextureCreate(const char* DLR_NONNULL text, int width, int fontSize, int r, int g, int b, int a) {
    assert(fontSize == FONT_SIZE_DEFAULT);

    SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(gFont, text, (SDL_Color) {static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b), static_cast<unsigned char>(a)}, width);
    assert(surface != nullptr);

    SDL_Surface* xSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    assert(xSurface != nullptr);

    SDL_FreeSurface(surface);
    return xSurface;
}

static void textureDestroy(void* DLR_NONNULL texture) {
    SDL_FreeSurface(reinterpret_cast<SDL_Surface*>(texture));
}

static void textureMetrics(void* DLR_NONNULL texture, int* DLR_NONNULL width, int* DLR_NONNULL height) {
    *width = ((SDL_Surface*) texture)->w;
    *height = ((SDL_Surface*) texture)->h;
}

static void* DLR_NONNULL textureData(void* DLR_NONNULL texture) {
    return ((SDL_Surface*) texture)->pixels;
}

static void textMetrics(const char* DLR_NONNULL text, int fontSize, int* DLR_NONNULL width, int* DLR_NONNULL height) {
    assert(fontSize == FONT_SIZE_DEFAULT);
    TTF_SizeUTF8(gFont, text, width, height);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    gFont = TTF_OpenFont("res/Roboto-Regular.ttf", 16);

    SDL_version version;
    SDL_GetVersion(&version);
    assert(version.major == 2);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    const int windowMinimalWidth = 16 * 50, windowMinimalHeight = 9 * 50;

    gWindow = SDL_CreateWindow(
        "Dnealar",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowMinimalWidth,
        windowMinimalHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    assert(gWindow != nullptr);
    SDL_SetWindowMinimumSize(gWindow, windowMinimalWidth, windowMinimalHeight);

    SDL_GLContext glContext = SDL_GL_CreateContext(gWindow);

    SDL_GL_SetSwapInterval(1);

    dlrInit(
        SDL_malloc,
        SDL_realloc,
        SDL_free,
        textTextureCreate,
        wrappedTextTextureCreate,
        textureDestroy,
        textureMetrics,
        textureData,
        textMetrics
    );
    loop();
    dlrQuit();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(gWindow);

    TTF_CloseFont(gFont);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    assert(SDL_GetNumAllocations() == 0);
    return EXIT_SUCCESS;
}

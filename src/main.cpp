
#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

static SDL_Window* gWindow = nullptr;

static void render() {

}

static void loop() {
    int width, height;
    SDL_Event event;

    while (true) {
        SDL_GL_GetDrawableSize(gWindow, &width, &height);

        while (SDL_PollEvent(&event) == 1) {
            switch (event.type) {
                case SDL_QUIT:
                    goto end;
                case SDL_MOUSEMOTION:
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    break;
                case SDL_MOUSEBUTTONUP:
                    break;
                case SDL_KEYDOWN:
                    break;
                case SDL_KEYUP:
                    break;
                case SDL_TEXTINPUT:
                    break;
            }
        }

        render();
        SDL_GL_SwapWindow(gWindow);
    }
    end:
    (void) 0;
}

int main() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();

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
        "Jaoned",
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

    loop();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(gWindow);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    assert(SDL_GetNumAllocations() == 0);
    return EXIT_SUCCESS;
}

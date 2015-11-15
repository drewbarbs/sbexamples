#include <exception>
#include <iostream>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <system_error>
#include <chrono>
#include <thread>

template <typename T, typename D, D Deleter> struct stateless_deleter { using pointer = T; void operator()(T x) { Deleter(x); } };

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

    try {
        auto window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> (
                SDL_CreateWindow("OpenGL",
                                 SDL_WINDOWPOS_CENTERED, // intial x position
                                 SDL_WINDOWPOS_CENTERED, // initial y position
                                 800,                    // width (px)
                                 600,                    // height (px)
                                 SDL_WINDOW_OPENGL       // flags
                ),
                &SDL_DestroyWindow);
        if (!window) {
            throw std::runtime_error(std::string("Failed to create SDL window: ") + SDL_GetError());
        }

        std::unique_ptr<SDL_GLContext,
                stateless_deleter<SDL_GLContext, decltype(&SDL_GL_DeleteContext), &SDL_GL_DeleteContext>
        > context(SDL_GL_CreateContext(window.get()));
        if (!context) {
            throw std::runtime_error(std::string("Failed to create SDL window: ") + SDL_GetError());
        }

        SDL_Event windowEvent;
        bool run = true;
        while (run)
        {
            SDL_WaitEventTimeout(&windowEvent, 50);

            do {
                if (windowEvent.type == SDL_QUIT) { run = false;  break; }
            } while (SDL_PollEvent(&windowEvent));

            SDL_GL_SwapWindow(window.get());
        }

    }  catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    SDL_Quit();
    return 0;
}
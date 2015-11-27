#ifndef SBEXAMPLES_APP_H
#define SBEXAMPLES_APP_H

#include <memory>
#include <iostream>
#include <chrono>
#include "stateless_deleter.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

namespace sb {
    class app {
    public:
        app() { }

        void render(double current_time) {
            do_render(current_time);
        }
        virtual ~app() { }

    private:
        virtual void do_render(double current_time) {}
    };

    template<typename T, int GL_MAJOR=4, int GL_MINOR=4>
    int appmain();
}

#define DECLARE_APP(app_t)     \
int main() {                   \
  return sb::appmain<app_t>(); \
}

#define DECLARE_APP_GL_VER(app_t, GL_MAJOR, GL_MINOR) \
int main() {                                          \
  return sb::appmain<app_t, GL_MAJOR, GL_MINOR>();    \
}

template<typename T, int GL_MAJOR, int GL_MINOR>
int sb::appmain() {
    using namespace std;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    try {
        auto window = unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> (
                SDL_CreateWindow("OpenGL",
                                 SDL_WINDOWPOS_CENTERED, // intial x position
                                 SDL_WINDOWPOS_CENTERED, // initial y position
                                 800,                    // width (px)
                                 600,                    // height (px)
                                 SDL_WINDOW_OPENGL       // flags
                ),
                &SDL_DestroyWindow);
        if (!window) {
            throw runtime_error(string("Failed to create SDL window: ") + SDL_GetError());
        }

        std::unique_ptr<SDL_GLContext,
        stateless_deleter<SDL_GLContext, decltype(&SDL_GL_DeleteContext), &SDL_GL_DeleteContext>
        > context(SDL_GL_CreateContext(window.get()));
        if (!context) {
            throw std::runtime_error(string("Failed to create SDL window: ") + SDL_GetError());
        }

        // Initialize GLEW
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            throw std::runtime_error(string("Failed to initialize GLEW: ") + reinterpret_cast<const char *> (glewGetErrorString(err)));
        }
        cout << "Successfully initialized GLEW: " << glewGetString(GLEW_VERSION);

        T instance;
        SDL_Event window_event;
        bool run = true;
        auto start_time = chrono::steady_clock::now();
        while (run) {
            SDL_WaitEventTimeout(&window_event, 50);

            do {
                if (window_event.type == SDL_QUIT) { run = false;  break; }
            } while (SDL_PollEvent(&window_event));

            chrono::duration<double> elapsed = chrono::steady_clock::now() - start_time;
            instance.render(elapsed.count());
            SDL_GL_SwapWindow(window.get());
        }
    }  catch (const exception &ex) {
        cerr << ex.what() << endl;
        SDL_Quit();
        return 1;
    } catch (...) {
        cerr << "Unrecognized error" << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Quit();
    return 0;
}

#endif //SBEXAMPLES_APP_H

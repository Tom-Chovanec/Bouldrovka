#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>

#include "include/app.hpp"


int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    SDL_SetHint(SDL_HINT_ORIENTATIONS, "Portrait");

    App app;

    app.init();

    while (app.isRunning()) {
        app.update();
    }

    return 0;
}

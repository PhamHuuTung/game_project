#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Game.h"
#include <SDL_mixer.h>


Game *game = nullptr;

int main(int argc, char *argv[]) {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();
    // Sửa fullscreen thành true để chạy toàn màn hình
    game->init("SDL Shooting Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, true);

    while(game->running()) {
        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    game->clean();

    return 0;
}

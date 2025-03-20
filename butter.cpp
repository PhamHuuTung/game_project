#include "Bullet.h"
#include "Game.h"

Bullet::Bullet(int x, int y, int width, int height) {
    collider.x = x;
    collider.y = y;
    collider.w = width;
    collider.h = height;

    speed = 10;
    active = true;
}

Bullet::~Bullet() {
}

void Bullet::update() {
    collider.y -= speed;

    // Check if bullet is off screen
    if(collider.y < 0) {
        active = false;
    }
}

void Bullet::render() {
    SDL_SetRenderDrawColor(Game::renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(Game::renderer, &collider);
}

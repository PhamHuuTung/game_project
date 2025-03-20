#include "Bullet.h"
#include "Game.h"
#include "Config.h"
#include <cmath>

Bullet::Bullet(int x, int y, int width, int height, float angle, bool enemy) {
    collider.x = x;
    collider.y = y;
    collider.w = width;
    collider.h = height;
    posX = x;
    posY = y;
    speed = 10;
    active = true;
    enemyBullet = enemy;
    this->angle = angle;
}

Bullet::~Bullet() { }

void Bullet::update() {
    // Update position based on angle
    posX += speed * cos(angle);
    posY += speed * sin(angle);
    collider.x = static_cast<int>(posX);
    collider.y = static_cast<int>(posY);

    // Deactivate if outside the play area instead of full window
    if(collider.x < PLAY_AREA_X || collider.x > PLAY_AREA_X + PLAY_AREA_SIZE ||
       collider.y < PLAY_AREA_Y || collider.y > PLAY_AREA_Y + PLAY_AREA_SIZE) {
        active = false;
    }
}


void Bullet::setAngle(float newAngle) {
    angle = newAngle;
}


void Bullet::render() {
    // Sử dụng màu vàng cho đạn nhân vật, xanh cho đạn quái vật
    if(enemyBullet)
        SDL_SetRenderDrawColor(Game::renderer, 0, 0, 255, 255);
    else
        SDL_SetRenderDrawColor(Game::renderer, 255, 255, 0, 255);

    SDL_RenderFillRect(Game::renderer, &collider);
}

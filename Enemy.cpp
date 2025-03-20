#include "Enemy.h"
#include "Game.h"
#include "Config.h"   // Dùng để lấy WINDOW_WIDTH, WINDOW_HEIGHT
#include <cstdlib>
#include <ctime>

Enemy::Enemy(int x, int y, int width, int height) {
    collider.x = x;
    collider.y = y;
    collider.w = width;
    collider.h = height;

    health = 15;
    speed = 3;
    active = true;

    // Khởi tạo dx, dy ngẫu nhiên
    dx = (rand() % 3 - 1) * speed;
    dy = (rand() % 3 - 1) * speed;
    if (dx == 0 && dy == 0)
        dx = speed;

    // Khởi tạo thời điểm thay đổi hướng ban đầu
    lastDirectionChangeTime = SDL_GetTicks();
}

Enemy::~Enemy() {
}

void Enemy::update() {
    Uint32 currentTime = SDL_GetTicks();
    // Direction change logic remains the same
    if (currentTime - lastDirectionChangeTime >= 2500) {
        dx = (rand() % 3 - 1) * speed;
        dy = (rand() % 3 - 1) * speed;
        if (dx == 0 && dy == 0)
            dx = speed;
        lastDirectionChangeTime = currentTime;
    }

    // Update position
    collider.x += dx;
    collider.y += dy;

    // Constrain to play area instead of full window
    if (collider.x < PLAY_AREA_X) {
        collider.x = PLAY_AREA_X;
        dx = -dx;
    } else if (collider.x > PLAY_AREA_X + PLAY_AREA_SIZE - collider.w) {
        collider.x = PLAY_AREA_X + PLAY_AREA_SIZE - collider.w;
        dx = -dx;
    }

    if (collider.y < PLAY_AREA_Y) {
        collider.y = PLAY_AREA_Y;
        dy = -dy;
    } else if (collider.y > PLAY_AREA_Y + PLAY_AREA_SIZE - collider.h) {
        collider.y = PLAY_AREA_Y + PLAY_AREA_SIZE - collider.h;
        dy = -dy;
    }

}

void Enemy::changeDirection() {
    // Cập nhật dx, dy ngẫu nhiên từ -speed đến +speed
    dx = (rand() % 3 - 1) * speed;
    dy = (rand() % 3 - 1) * speed;
    if (dx == 0 && dy == 0) {
        dx = speed;
    }
}

void Enemy::moveBy(int deltaX, int deltaY) {
    // Di chuyển enemy theo khoảng cách deltaX, deltaY
    collider.x += deltaX;
    collider.y += deltaY;
}

void Enemy::render() {
    // Vẽ enemy (màu đỏ)
    SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(Game::renderer, &collider);

    // Vẽ thanh máu cho enemy phía trên
    // Thanh nền: màu đen
    SDL_Rect healthBarBack = { collider.x, collider.y - health_max_E, collider.w, 5 };
    SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(Game::renderer, &healthBarBack);

    // Thanh máu: màu xanh (tỷ lệ theo health, max = 10)
    int healthBarWidth = (health * collider.w) / health_max_E;
    SDL_Rect healthBar = { collider.x, collider.y - health_max_E, healthBarWidth, 5 };
    SDL_SetRenderDrawColor(Game::renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(Game::renderer, &healthBar);
}


void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        active = false;
    }
}

void Enemy::reverseDirection() {
    // Đảo ngược hướng di chuyển
    directionX = -directionX;
    directionY = -directionY;
}

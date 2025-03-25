#include "Enemy.h"
#include "Game.h"
#include "Config.h"   // Dùng để lấy WINDOW_WIDTH, WINDOW_HEIGHT
#include <cstdlib>
#include <SDL_mixer.h>
#include <ctime>
#include "TextureManager.h"

Enemy::Enemy(int x, int y, int width, int height) {
    collider.x = x;
    collider.y = y;
    collider.w = width;
    collider.h = height;

    health = health_max_E;
    speed = 3;
    active = true;

    // Khởi tạo dx, dy ngẫu nhiên
    dx = (rand() % 3 - 1) * speed;
    dy = (rand() % 3 - 1) * speed;
    if (dx == 0 && dy == 0)
        dx = speed;

    // Khởi tạo thời điểm thay đổi hướng ban đầu
    lastDirectionChangeTime = SDL_GetTicks();
    // --- Load animation enemy ---
    for (int i = 0; i < ENEMY_ANIM_FRAMES; i++) {
        std::string key = "enemyplus_" + std::to_string(i);
        animTextures[i] = TextureManager::GetTexture(key);
        if (animTextures[i] == nullptr) {
            std::cout << "Failed to load enemy animation texture: " << key << std::endl;
        }
    }
    // Khởi tạo hướng mặt dựa vào giá trị dx ban đầu
    facingRight = (dx >= 0);
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

     // Cập nhật hướng mặt dựa trên hướng di chuyển
    if (dx > 0) facingRight = true;
    else if (dx < 0) facingRight = false;

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
    // --- Cập nhật animation ---
    if (currentTime - lastFrameTime >= frameDelay) {
        lastFrameTime = currentTime;
        currentFrame = (currentFrame + 1) % ENEMY_ANIM_FRAMES;
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
    // Khai báo biến kích thước sprite ở phạm vi ngoài cùng của hàm
    int spriteWidth = static_cast<int>(collider.w * 2.2);
    int spriteHeight = static_cast<int>(collider.h * 2.2);
    int offsetX = (spriteWidth - collider.w) / 2;

    // Vị trí Y thực của sprite
    int spriteY = collider.y - (spriteHeight - collider.h);

    // Sử dụng texture animation của enemy
    SDL_Texture* tex = animTextures[currentFrame];
    if (tex) {
        SDL_Rect srcRect;
        SDL_QueryTexture(tex, nullptr, nullptr, &srcRect.w, &srcRect.h);
        srcRect.x = 0;
        srcRect.y = 0;

        SDL_Rect destRect = {
            collider.x - offsetX,
            spriteY,
            spriteWidth,
            spriteHeight
        };

        SDL_RendererFlip flip = facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        SDL_RenderCopyEx(Game::renderer, tex, &srcRect, &destRect, 0, NULL, flip);
    } else {
        SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(Game::renderer, &collider);
    }

    // Điều chỉnh chiều rộng của thanh máu là 75% của sprite width
    int healthBarWidth = static_cast<int>(spriteWidth * 0.75);

    // Tính toán vị trí X để thanh máu được căn giữa với sprite
    int healthBarX = collider.x - offsetX + (spriteWidth - healthBarWidth) / 2;

    // Đặt thanh máu với khoảng cách cố định (8 pixel) phía trên sprite
    int healthBarY = spriteY - 10;

    // Vẽ nền đen của thanh máu
    SDL_Rect healthBarBack = {
        healthBarX,
        healthBarY,
        healthBarWidth,
        5
    };

    SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(Game::renderer, &healthBarBack);

    // Vẽ thanh máu màu xanh
    int currentHealthWidth = (health * healthBarWidth) / health_max_E;
    SDL_Rect healthBar = {
        healthBarX,
        healthBarY,
        currentHealthWidth,
        5
    };

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

#include "Bullet.h"
#include "Game.h"
#include "Config.h"
#include <SDL_mixer.h>

#include <cmath>
#include "TextureManager.h"


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
    frameDelay = flyingFrameDelay;
    lastFrameTime = SDL_GetTicks();

    // Load textures based on bullet type (player or enemy)
    if (!enemyBullet) {
        // Player bullet textures
        for (int i = 0; i < FLYING_FRAMES; i++) {
            std::string texName = "bullet_player_" + std::to_string(i + 1);
            flyingTextures[i] = TextureManager::GetTexture(texName);
        }

        // Explosion textures for player bullets
        for (int i = 0; i < EXPLOSION_FRAMES; i++) {
            std::string texName = "bullet_explosion_" + std::to_string(i + 1);
            explosionTextures[i] = TextureManager::GetTexture(texName);
        }
    } else {
        // Enemy bullet textures - use the enemy textures you added
        for (int i = 0; i <= FLYING_FRAMES_Enemy; i++) {
            // Make sure you don't exceed the available textures
            if (i < FLYING_FRAMES_Enemy) { // Since you have enemy_0 through enemy_7
                std::string texName = "enemy_" + std::to_string(i);
                flyingTextures[i] = TextureManager::GetTexture(texName);
            } else {
                // For any index beyond available textures, reuse the last one
                flyingTextures[i] = flyingTextures[i % FLYING_FRAMES_Enemy];
            }
        }

        // For explosion textures for enemies
        // Option 1: Reuse player explosion textures
        for (int i = 0; i < EXPLOSION_FRAMES; i++) {
            std::string texName = "bullet_explosion_" + std::to_string(i + 1);
            explosionTextures[i] = TextureManager::GetTexture(texName);
        }

        // Option 2: If you have specific enemy explosion textures, use those instead
        // For example:
        // for (int i = 0; i < EXPLOSION_FRAMES; i++) {
        //     std::string texName = "enemy_explosion_" + std::to_string(i);
        //     explosionTextures[i] = TextureManager::GetTexture(texName);
        // }
    }
}

Bullet::~Bullet() { }

void Bullet::update() {
    Uint32 currentTime = SDL_GetTicks();
//     std::cout << "exploding = " << (exploding ? "true" : "false") << std::endl;
    if (isExploding) {
        // Xử lý hiệu ứng nổ
  //       std::cout << "Updating explosion animation, frame: " << currentFrame << std::endl;
        if (currentTime - lastFrameTime >= frameDelay) {
            lastFrameTime = currentTime;
            currentFrame++;
          //  std::cout << "Advancing to explosion frame: " << currentFrame << std::endl;

            // Hủy đạn khi hiệu ứng nổ hoàn thành
            if (currentFrame >= EXPLOSION_FRAMES) {
                active = false;
          //      std :: cout << "boom" << std :: endl;
            }
        }
    } else {
        // Update flying animation
        if (currentTime - lastFrameTime >= frameDelay) {
            lastFrameTime = currentTime;
            currentFrame = (currentFrame + 1) % FLYING_FRAMES;
        }

        // Cập nhật vị trí dựa trên góc (đạn bay)
        posX += speed * cos(angle);
        posY += speed * sin(angle);
        collider.x = static_cast<int>(posX);
        collider.y = static_cast<int>(posY);

        // Vô hiệu hóa nếu đạn ra ngoài khu vực chơi
        if (collider.x < PLAY_AREA_X || collider.x > PLAY_AREA_X + PLAY_AREA_SIZE ||
            collider.y < PLAY_AREA_Y || collider.y > PLAY_AREA_Y + PLAY_AREA_SIZE) {
            active = false;
        }
    }
}

void Bullet::startExplosion() {
  //  std::cout << "Starting explosion animation!" << std::endl;
    isExploding = true; // Sử dụng biến isExploding thay vì exploding
    currentFrame = 0;
    lastFrameTime = SDL_GetTicks();
    // Bạn có thể điều chỉnh frameDelay cho hiệu ứng nổ nếu muốn nó chạy nhanh hơn hoặc chậm hơn
    frameDelay = explosionFrameDelay; // Chuyển sang sử dụng frameDelay của hoạt ảnh nổ
    // Stop moving
    speed = 0;
}


void Bullet::setAngle(float newAngle) {
    angle = newAngle;
}


void Bullet::render() {


    SDL_Texture* tex = nullptr;

    if (isExploding) {
        // Sử dụng texture nổ nếu đang trong trạng thái nổ
 //           std::cout << "bum" << std::endl;

        if (currentFrame < EXPLOSION_FRAMES) {
            tex = explosionTextures[currentFrame];
        }
    } else {
        // Sử dụng texture bay
        if (currentFrame < FLYING_FRAMES) {
            tex = flyingTextures[currentFrame];
        }
    }
    // Cho đạn địch
//    if (enemyBullet) {
//        SDL_Rect largerRect = collider;
//        largerRect.w *= 1;
//        largerRect.h *= 1;
//        largerRect.x = collider.x - (largerRect.w - collider.w) / 2;
//        largerRect.y = collider.y - (largerRect.h - collider.h) / 2;
//
//        SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 125);
//        SDL_RenderFillRect(Game::renderer, &largerRect);
//        return;
//    }

    if (tex) {
        SDL_Rect srcRect;
        SDL_QueryTexture(tex, nullptr, nullptr, &srcRect.w, &srcRect.h);
        srcRect.x = 0;
        srcRect.y = 0;

        // Tạo hình chữ nhật đích lớn hơn collider
        SDL_Rect destRect = collider;

        // Làm cho hiệu ứng nổ lớn hơn đạn thường
        float scaleFactor;
        if (enemyBullet) {
            // Smaller values for enemy bullets
            scaleFactor = isExploding ? 7.0f : 15.0f;
        } else {
            // Original values for player bullets
            scaleFactor = isExploding ? 15.0f : 18.0f;
        }

        destRect.w = static_cast<int>(destRect.w * scaleFactor);
        destRect.h = static_cast<int>(destRect.h * scaleFactor);

        // Căn giữa destRect xung quanh vị trí collider gốc
        destRect.x = collider.x - (destRect.w - collider.w) / 2;
        destRect.y = collider.y - (destRect.h - collider.h) / 2;

        // Xoay đạn để khớp với hướng bay
        double rotation = exploding ? 0.0 : angle * (180.0 / M_PI);

        SDL_RenderCopyEx(Game::renderer, tex, &srcRect, &destRect,
                         rotation, nullptr, SDL_FLIP_NONE);
    } else {
        // Fallback nếu không có texture
        SDL_Rect largerRect = collider;
        largerRect.w *= 2;
        largerRect.h *= 2;
        largerRect.x = collider.x - (largerRect.w - collider.w) / 2;
        largerRect.y = collider.y - (largerRect.h - collider.h) / 2;

        if (enemyBullet)
            SDL_SetRenderDrawColor(Game::renderer, 0, 0, 255, 255);
        else
            SDL_SetRenderDrawColor(Game::renderer, 255, 255, 0, 255);

        SDL_RenderFillRect(Game::renderer, &largerRect);
    }
}

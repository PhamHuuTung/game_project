#include "Player.h"
#include "Game.h"
#include "Config.h"
#include <SDL_mixer.h>
#include <cmath>  // để sử dụng sqrt, cos, sin
#include <iostream>
#include <SDL.h>
#include "TextureManager.h"

Player::Player(int x, int y, int width, int height) {
    collider.x = x;
    collider.y = y;
    collider.w = width;
    collider.h = height;
    posX = x;
    posY = y;
    health = health_max_P;
    speed = 5;
    active = true;
    lastRegenTime = SDL_GetTicks();  // Khởi tạo thời điểm hồi phục ban đầu

    // Lấy texture nhân vật từ TextureManager với ID "mage"
    runTextures[0] = TextureManager::GetTexture("mage");
    runTextures[1] = TextureManager::GetTexture("mage_run1");
    runTextures[2] = TextureManager::GetTexture("mage_run2");
    runTextures[3] = TextureManager::GetTexture("mage_run3");
    runTextures[4] = TextureManager::GetTexture("mage_run4");
    runTextures[5] = TextureManager::GetTexture("mage_run5");
    runTextures[6] = TextureManager::GetTexture("mage_run6");


}

Player::~Player() {
}
void Player::update() {
    // Hồi phục 1 máu mỗi 1000ms (1 giây)
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastRegenTime >= 1000) {
        if (health < health_max_P) {  // Giới hạn tối đa là 100
            health+= 3;
            std::cout << "Player health increased to: " << health << std::endl;
        }
        lastRegenTime = currentTime;
    }
    // Các logic update khác của player (nếu có)
     // === Cập nhật frame animation ===
    // Animation update
    if (isMoving) {
        // If moving, change frame every frameDelay ms
        if (currentTime - lastFrameTime >= frameDelay) {
            lastFrameTime = currentTime;
            currentFrame = (currentFrame + 1) % RUN_FRAMES;
        }
    } else {
        // Standing still, reset to frame 0
        currentFrame = 0;
        // Reset last frame time when starting to move again
        lastFrameTime = currentTime;
    }
}




void Player::handleEvent() {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Lấy kích thước cửa sổ vật lý từ renderer
    int physW, physH;
    SDL_GetRendererOutputSize(Game::renderer, &physW, &physH);
    // Tính hệ số scale chuyển từ kích thước vật lý sang kích thước logic
    float scaleX = static_cast<float>(WINDOW_WIDTH) / physW;
    float scaleY = static_cast<float>(WINDOW_HEIGHT) / physH;

    // Chuyển đổi tọa độ chuột sang hệ logic
    float logicalMouseX = mouseX * scaleX;
    float logicalMouseY = mouseY * scaleY;

    // Tính vị trí mục tiêu sao cho trung tâm nhân vật trùng với chuột
    float targetX = logicalMouseX - collider.w / 2;
    float targetY = logicalMouseY - collider.h / 2;

    // Lưu vị trí ban đầu trước khi cập nhật
    float oldPosX = posX;
    float oldPosY = posY;

    // Tính vector hiệu giữa vị trí hiện tại (posX, posY) và vị trí mục tiêu
    float diffX = targetX - posX;
    float diffY = targetY - posY;
    float distance = std::sqrt(diffX * diffX + diffY * diffY);

     // Store the X direction for deterFmining which way to face
    if (std::fabs(diffX) > 0.3f) {
        lastDiffX = diffX;
    }

    if (distance < 3) {
        posX = targetX;
        posY = targetY;
    } else if (distance > 1) {
        // Di chuyển theo vector đã chuẩn hóa với tốc độ nhất định
        float step = static_cast<float>(speed);
        posX += step * diffX / distance;
        posY += step * diffY / distance;
    }


    // Ràng buộc vùng chơi sử dụng posX, posY (theo hệ logic)
    if (posX < PLAY_AREA_X)
        posX = PLAY_AREA_X;
    else if (posX + collider.w > PLAY_AREA_X + PLAY_AREA_SIZE)
        posX = PLAY_AREA_X + PLAY_AREA_SIZE - collider.w;

    if (posY < PLAY_AREA_Y)
        posY = PLAY_AREA_Y;
    else if (posY + collider.h > PLAY_AREA_Y + PLAY_AREA_SIZE)
        posY = PLAY_AREA_Y + PLAY_AREA_SIZE - collider.h;

     // Add hysteresis to direction changes
    if (std::fabs(diffX) > 3.0f) {  // Only change direction on significant movement
        if (diffX < -2.0f) {
            facingLeft = true;
        } else if (diffX > 2.0f) {
            facingLeft = false;
        }
    }
    // Cập nhật lại vị trí của collider để render chính xác
    collider.x = static_cast<int>(posX);
    collider.y = static_cast<int>(posY);

    // Sau khi cập nhật, kiểm tra xem có di chuyển không
    isMoving = (std::fabs(posX - oldPosX) > 0.1f || std::fabs(posY - oldPosY) > 0.1f);
}







void Player::render() {
    SDL_Texture* tex = runTextures[currentFrame];
    if (tex) {
        SDL_Rect srcRect;
        SDL_QueryTexture(tex, nullptr, nullptr, &srcRect.w, &srcRect.h);
        srcRect.x = 0;
        srcRect.y = 0;

        // Create a dest rect that maintains the correct aspect ratio
        SDL_Rect destRect = collider;

        // Determine if player is moving left or right
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (isMoving) {
            // If moving left (negative x difference), flip the texture
            if (lastDiffX < 0) {
                flip = SDL_FLIP_HORIZONTAL;
            }
        }

        // Use SDL_RenderCopyEx instead of Draw to support flipping
        SDL_RenderCopyEx(Game::renderer, tex, &srcRect, &destRect, 0, nullptr, flip);
    } else {
        // Fallback if texture is missing
        SDL_SetRenderDrawColor(Game::renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(Game::renderer, &collider);
    }
//        SDL_RendererFlip flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    // Vẽ thanh máu như cũ...
    SDL_Rect healthBarBack = { collider.x, collider.y - 10, collider.w, 5 };
    SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(Game::renderer, &healthBarBack);

    int healthBarWidth = (health * collider.w) / health_max_P;
    SDL_Rect healthBar = { collider.x, collider.y - 10, healthBarWidth, 5 };
    SDL_SetRenderDrawColor(Game::renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(Game::renderer, &healthBar);
}



// Chỉ khai báo, không định nghĩa
void Player::takeDamage(int amount) {
    health -= amount;
    //std::cout << "Player health: " << health << std::endl;
    if (health <= 0) {
        active = false;
        std::cout << "Player is dead!" << std::endl;
    }
}


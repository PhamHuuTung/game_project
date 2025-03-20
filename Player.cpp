#include "Player.h"
#include "Game.h"
#include "Config.h"
#include <cmath>  // để sử dụng sqrt, cos, sin
#include <iostream>
#include <SDL.h>

Player::Player(int x, int y, int width, int height) {
    collider.x = x;
    collider.y = y;
    collider.w = width;
    collider.h = height;
    posX = x;
    posY = y;
    health = 100;
    speed = 5;
    active = true;
    lastRegenTime = SDL_GetTicks();  // Khởi tạo thời điểm hồi phục ban đầu
}

Player::~Player() {
}
void Player::update() {
    // Hồi phục 1 máu mỗi 1000ms (1 giây)
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastRegenTime >= 1000) {
        if (health < health_max_P) {  // Giới hạn tối đa là 100
            health++;
            std::cout << "Player health increased to: " << health << std::endl;
        }
        lastRegenTime = currentTime;
    }
    // Các logic update khác của player (nếu có)
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

    // Tính vector hiệu giữa vị trí hiện tại (posX, posY) và vị trí mục tiêu
    float diffX = targetX - posX;
    float diffY = targetY - posY;
    float distance = std::sqrt(diffX * diffX + diffY * diffY);

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

    // Cập nhật lại vị trí của collider để render chính xác
    collider.x = static_cast<int>(posX);
    collider.y = static_cast<int>(posY);
}







void Player::render() {
    // Vẽ nhân vật (màu xanh lá)
    SDL_SetRenderDrawColor(Game::renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(Game::renderer, &collider);

    // Vẽ thanh máu cho nhân vật ở phía trên sprite
    // Thanh nền: màu đỏ (hoặc bạn có thể chọn màu khác)
    SDL_Rect healthBarBack = { collider.x, collider.y - 10, collider.w, 5 };
    SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(Game::renderer, &healthBarBack);

    // Thanh máu: màu xanh (tỷ lệ theo health)
    int healthBarWidth = (health * collider.w) / 100;  // health từ 0 đến 100
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


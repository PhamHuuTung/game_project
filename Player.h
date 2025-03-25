#ifndef PLAYER_H
#define PLAYER_H

#include <array>   // Để dùng std::array
#include <SDL.h>
#include "Bullet.h"

class Player {
public:
    Player(int x, int y, int width, int height);
    ~Player();
    void handleEvent();
    void update();
    void render();
    void takeDamage(int amount);

    bool isActive() const { return active; }
    SDL_Rect getCollider() const { return collider; }
    int getX() const { return collider.x; }
    int getY() const { return collider.y; }
    int getWidth() const { return collider.w; }
    int getHeight() const { return collider.h; }

private:
    SDL_Rect collider;
    float posX, posY;  // Lưu vị trí chính xác bằng số thực
    int health;
    int speed;
    bool active;
    Uint32 lastRegenTime;
    float lastDiffX = 0; // To track horizontal movement direction
    bool facingLeft = false;  // Track facing direction with a boolean

    // ========== PHẦN THÊM CHO ANIMATION ==========

    // Số frame cho animation chạy
    static const int RUN_FRAMES = 7;

    // Mảng chứa 4 texture cho các frame chạy
    std::array<SDL_Texture*, RUN_FRAMES> runTextures;

    // Chỉ số frame hiện tại (0..RUN_FRAMES-1)
    int currentFrame = 0;

    // Thời điểm frame cuối cùng được cập nhật
    Uint32 lastFrameTime = 0;

    // Thời gian chờ giữa hai frame (ms)
    Uint32 frameDelay = 120;

    // Biến kiểm tra trạng thái đang di chuyển hay đứng yên
    bool isMoving = false;

    // Thêm biến lưu texture cho nhân vật:
    SDL_Texture* playerTexture;
};


#endif // PLAYER_H

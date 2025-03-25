#include "EnemyPlus.h"
#include "Game.h"
#include <cstdlib>
#include <cmath>
#include <SDL_mixer.h>


// Khởi tạo EnemyPlus với vị trí và kích thước cho trước, thiết lập cooldown bắn (ví dụ 1000ms)
EnemyPlus::EnemyPlus(int x, int y, int width, int height)
    : Enemy(x, y, width, height)  // Gọi constructor của lớp cơ sở
{
    lastShotTime = 0;
    shootCooldown = 800; // bắn mỗi 1000ms (1 giây)
    // Load các texture animation cho EnemyPlus từ TextureManager
    // Các key đã được định nghĩa trong TextureManager::LoadTextures()
//    for (int i = 0; i < ANIM_FRAMES; i++) {
//        std::string key = "enemyplus_" + std::to_string(i);
//        hurtTextures[i] = TextureManager::GetTexture(key);
//        if (hurtTextures[i] == nullptr) {
//            std::cout << "Failed to load EnemyPlus animation texture: " << key << std::endl;
//        }
//    }
}

EnemyPlus::~EnemyPlus() {
}

void EnemyPlus::update() {
    // Có thể dùng chuyển động của Enemy hiện có
    Enemy::update();

    // Bạn có thể thêm chuyển động riêng của EnemyPlus nếu cần
    // Cập nhật frame animation dựa vào thời gian
    Uint32 currentTime = SDL_GetTicks();
//    if (currentTime - lastFrameTime >= frameDelay) {
//        lastFrameTime = currentTime;
//        currentFrame = (currentFrame + 1) % ANIM_FRAMES;
//    }
}

std::vector<Bullet*> EnemyPlus::shoot() {
    std::vector<Bullet*> newBullets;
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastShotTime >= shootCooldown) {
        lastShotTime = currentTime;
        // Lấy tâm của quái vật
        SDL_Rect collider = getCollider();
        int centerX = collider.x + collider.w / 2;
        int centerY = collider.y + collider.h / 2;
        // Tạo 4 viên đạn theo hình dấu +
        // Lưu ý: Bullet phải hỗ trợ tham số angle (đơn vị radian) như đã hướng dẫn ở phần trước.
        newBullets.push_back(new Bullet(centerX, centerY, 10, 10, 0.0f, true));         // sang phải (0 rad)
        newBullets.push_back(new Bullet(centerX, centerY, 10, 10, M_PI, true));           // sang trái (π rad)
        newBullets.push_back(new Bullet(centerX, centerY, 10, 10, -M_PI/2, true));        // lên (–π/2 rad)
        newBullets.push_back(new Bullet(centerX, centerY, 10, 10, M_PI/2, true));         // xuống (π/2 rad)
    }
    return newBullets;
}


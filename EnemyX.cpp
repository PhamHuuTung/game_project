#include "EnemyX.h"
#include "Game.h"
#include <cmath>
#include <iostream>

EnemyX::EnemyX(int x, int y, int width, int height)
    : Enemy(x, y, width, height)  // Gọi constructor của lớp cơ sở
{
    lastShotTime = 0;
    shootCooldown = 888; // Ví dụ: bắn mỗi 1.5 giây
}

EnemyX::~EnemyX() {
}

void EnemyX::update() {
    // Dùng chuyển động của Enemy (bạn có thể thêm logic riêng nếu cần)
    Enemy::update();
}

std::vector<Bullet*> EnemyX::shoot() {
    std::vector<Bullet*> newBullets;
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastShotTime >= shootCooldown) {
        lastShotTime = currentTime;
        // Lấy tâm của quái vật
        SDL_Rect coll = getCollider();
        int centerX = coll.x + coll.w / 2;
        int centerY = coll.y + coll.h / 2;
        // Các góc chéo: 45°, 135°, 225°, 315° (tính bằng radian)
        float angles[4] = { M_PI / 4, 3 * M_PI / 4, 5 * M_PI / 4, 7 * M_PI / 4 };
        for (int i = 0; i < 4; i++) {
            newBullets.push_back(new Bullet(centerX, centerY, 10, 10, angles[i], true));
        }
        // Debug: in ra thông báo bắn đạn
        //std::cout << "EnemyX shooting at time: " << currentTime << std::endl;
    }
    return newBullets;
}


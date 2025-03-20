#include "EnemyStar.h"
#include "Game.h"
#include <cmath>
#include <iostream>

EnemyStar::EnemyStar(int x, int y, int width, int height)
    : Enemy(x, y, width, height)
{
    lastShotTime = 0;
    shootCooldown = 888; // Ví dụ: bắn mỗi 1.5 giây
}

EnemyStar::~EnemyStar() {
}

void EnemyStar::update() {
    Enemy::update();  // Dùng chuyển động của Enemy, hoặc thêm chuyển động riêng nếu cần
}

std::vector<Bullet*> EnemyStar::shoot() {
    std::vector<Bullet*> newBullets;
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastShotTime >= shootCooldown) {
        //std::cout << "EnemyStar shooting at time: " << currentTime << std::endl;
        lastShotTime = currentTime;
        SDL_Rect coll = getCollider();
        int centerX = coll.x + coll.w / 2;
        int centerY = coll.y + coll.h / 2;
        // Ví dụ: bắn 8 viên đạn cách đều nhau (star pattern)
        int numBullets = 8;
        double angleIncrement = 2 * M_PI / numBullets;
        for (int i = 0; i < numBullets; i++) {
            float angle = i * angleIncrement;
            newBullets.push_back(new Bullet(centerX, centerY, 10, 10, angle, true));
        }
    }
    return newBullets;
}

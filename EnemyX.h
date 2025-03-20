#ifndef ENEMYX_H
#define ENEMYX_H

#include "Enemy.h"
#include "Bullet.h"
#include <vector>
#include <SDL.h>

class EnemyX : public Enemy {
public:
    EnemyX(int x, int y, int width, int height);
    virtual ~EnemyX();

    // Override update() nếu cần (hoặc dùng luôn Enemy::update())
    virtual void update();

    // Hàm bắn đạn theo mẫu "X"
    std::vector<Bullet*> shoot();

private:
    Uint32 lastShotTime;    // Thời điểm bắn đạn cuối cùng
    Uint32 shootCooldown;   // Khoảng thời gian chờ giữa các lần bắn (ms)
};

#endif // ENEMYX_H


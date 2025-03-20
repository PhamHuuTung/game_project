#ifndef ENEMYSTAR_H
#define ENEMYSTAR_H

#include "Enemy.h"
#include "Bullet.h"
#include <vector>
#include <SDL.h>

class EnemyStar : public Enemy {
public:
    EnemyStar(int x, int y, int width, int height);
    virtual ~EnemyStar();

    // Có thể override update() nếu muốn thay đổi chuyển động riêng
    virtual void update();

    // Phương thức bắn đạn theo mẫu star (hình dấu *)
    std::vector<Bullet*> shoot();

private:
    Uint32 lastShotTime;    // Thời điểm bắn cuối cùng (ms)
    Uint32 shootCooldown;   // Thời gian chờ giữa các lần bắn (ms)
};

#endif // ENEMYSTAR_H


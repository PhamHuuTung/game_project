#ifndef ENEMYPLUS_H
#define ENEMYPLUS_H

#include "Enemy.h"
#include "Bullet.h"
#include <vector>
#include <SDL.h>

class EnemyPlus : public Enemy {
public:
    EnemyPlus(int x, int y, int width, int height);
    virtual ~EnemyPlus();

    // Override hàm update để kết hợp chuyển động (có thể giữ nguyên hoặc custom) và cơ chế bắn
    virtual void update();

    // Phương thức bắn đạn theo hình dấu +
    std::vector<Bullet*> shoot();

private:
    Uint32 lastShotTime;    // Thời điểm bắn cuối cùng (ms)
    Uint32 shootCooldown;   // Thời gian chờ giữa các lần bắn (ms)
};

#endif // ENEMYPLUS_H


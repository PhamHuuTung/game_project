#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>

class Bullet {
public:
    // Thêm tham số angle (đơn vị radian) và enemy (mặc định false)
    Bullet(int x, int y, int width, int height, float angle = 0.0f, bool enemy = false);
    ~Bullet();

    void update();
    void render();
    void setAngle(float newAngle);  // Thêm dòng này
    bool isActive() const { return active; }
    void setActive(bool isActive) { active = isActive; }
    SDL_Rect getCollider() const { return collider; }
    bool isEnemyBullet() const { return enemyBullet; }

private:
    SDL_Rect collider;
    float posX, posY;  // Lưu vị trí dạng float để di chuyển mượt
    int speed;
    bool active;
    bool enemyBullet;  // true: đạn của quái vật; false: đạn của nhân vật
    float angle;       // Góc bắn tính bằng radian

};

#endif // BULLET_H

#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>

class Enemy {
public:
    Enemy(int x, int y, int width, int height);
    virtual ~Enemy();  // Khai báo destructor là virtual nếu có lớp dẫn xuất

    virtual void update();
    void render();
    void takeDamage(int amount);

    bool isActive() const { return active; }
    void setActive(bool isActive) { active = isActive; }
    SDL_Rect getCollider() const { return collider; }

    // Hàm thay đổi hướng (đã có)
    void changeDirection();
    void reverseDirection();

    // Thêm hàm moveBy để thay đổi vị trí enemy khi va chạm
    void moveBy(int deltaX, int deltaY);

protected:
    SDL_Rect collider;
    int health;
    int speed;
    bool active;
    int dx;
    int dy;
    Uint32 lastDirectionChangeTime;  // Theo dõi thời gian thay đổi hướng cuối cùng
    int directionX;  // Hướng di chuyển ngang (1: phải, -1: trái)
    int directionY;  // Hướng di chuyển dọc (1: xuống, -1: lên)
};

#endif // ENEMY_H

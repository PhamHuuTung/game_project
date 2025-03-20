#ifndef PLAYER_H
#define PLAYER_H

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
};


#endif // PLAYER_H

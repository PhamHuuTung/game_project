#include <SDL2/SDL.h>

class Bullet {
public:
    Bullet(int x, int y, int width, int height);
    ~Bullet();

    void update();
    void render();

    bool isActive() const { return active; }
    void setActive(bool isActive) { active = isActive; }

    SDL_Rect getCollider() const { return collider; }

private:
    SDL_Rect collider;
    int speed;
    bool active;
};

#endif // BULLET_H

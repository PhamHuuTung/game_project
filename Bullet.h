#ifndef BULLET_H
#define BULLET_H
#include <array>
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
    bool isEnemyBullet() const { return enemyBullet; }

    SDL_Rect getCollider() const { return collider; }
    // Method to trigger explosion animation
    void startExplosion();

private:
    SDL_Rect collider;
    float posX, posY;  // Lưu vị trí dạng float để di chuyển mượt
    float angle;       // Góc bắn tính bằng radian
    int speed;
    bool active;
    bool enemyBullet;  // true: đạn của quái vật; false: đạn của nhân vật


    // Animation properties
    static const int FLYING_FRAMES = 5;
    static const int EXPLOSION_FRAMES = 10;
        static const int FLYING_FRAMES_Enemy = 4;

    std::array<SDL_Texture*, FLYING_FRAMES> flyingTextures;
    std::array<SDL_Texture*, EXPLOSION_FRAMES> explosionTextures;
    int currentFrame = 0;
    Uint32 lastFrameTime = 0;
    Uint32 flyingFrameDelay = 350; // Giá trị mặc định cho hoạt ảnh bay
    Uint32 explosionFrameDelay = 50; // Giá trị mặc định cho hoạt ảnh nổ
    Uint32 frameDelay; // Biến hiện tại đang sử dụng
     Uint32 flyingFrameDelayEnemy = 60; // Giá trị mặc định cho hoạt ảnh bay

     bool exploding = false;
    bool isExploding = false;  // Đổi tên từ exploding thành isExploding

};

#endif // BULLET_H

#ifndef GAME_H
#define GAME_H
#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "Player.h"
#include "Enemy.h"

class Game {
public:
    Game();
    ~Game();


    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() { return isRunning; }

    static SDL_Renderer *renderer;
    static SDL_Event event;

private:
    SDL_Texture* backgroundTexture;  // Texture nền
    bool paused;                  // Current pause state
    Uint32 pauseStartTime;        // When the pause started
    Uint32 lastPauseEndTime;      // When the last pause ended
    const Uint32 PAUSE_DURATION = 3000;   // 3 seconds (in ms)
    const Uint32 PAUSE_COOLDOWN = 5000;  // 20 seconds (in ms)
    bool pauseOnCooldown;         // Whether pause ability is on cooldown

    // Thêm vào trong lớp Game
    bool skillActive = false;         // Trạng thái skill đang được kích hoạt
    Uint32 skillStartTime = 0;        // Thời điểm bắt đầu skill
    Uint32 lastSkillSpawnTime = 0;
    Uint32 lastEnemySpawnTime;  // Thời điểm spawn quái vật lần cuối    // Để kiểm soát tần số tạo đạn trong lúc skill
    std::vector<Bullet*> skillBullets; // Chứa các viên đạn tạo ra trong lúc skill


    bool isRunning;
    SDL_Window *window;
    Player *player;
//     bool paused;  // Thêm biến trạng thái tạm dừng
    std::vector<Enemy*> enemies;
    std::vector<Bullet*> bullets;

    int level;            // Cấp độ hiện tại
    int defeatedEnemies;  // Số enemy đã bị hạ trong level hiện tại
     int score; // Biến lưu điểm số
    TTF_Font* font; // Font chữ để hiển thị điểm (nếu dùng SDL_ttf)

    void createEnemies(int count);
    void checkCollisions();
    void cleanupInactiveObjects();


};

#endif // GAME_H

#include "Game.h"
#include "TextureManager.h"
#include "Collision.h"
#include "EnemyPlus.h"
#include "EnemyStar.h"
#include "EnemyX.h"
#include "Config.h"
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdlib>    // cho rand()
#include <ctime>      // cho time()

// Đảm bảo rằng srand được gọi ở đâu đó (ví dụ trong main)
// srand(time(0));

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Game::Game() {
    isRunning = false;
    window = nullptr;
    player = nullptr;
    paused = false;

    pauseStartTime = 0;
    lastPauseEndTime = 0;
    pauseOnCooldown = false;

    level = 1;
    defeatedEnemies = 0;
}

Game::~Game() {
    clean();
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    score = 0;
    if (fullscreen) {
        // Sử dụng fullscreen desktop để cửa sổ chiếm toàn màn hình
        flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystems Initialized..." << std::endl;
    // Khởi tạo SDL_ttf ngay sau SDL_Init()
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! Error: " << TTF_GetError() << std::endl;
        isRunning = false;
        return;
    }
    // Tải font từ folder "font"
    font = TTF_OpenFont("font/courbd.ttf", 24); // Đảm bảo file font nằm trong folder "font"
    if (!font) {
        std::cout << "Failed to load font! Error: " << TTF_GetError() << std::endl;
        isRunning = false;
        return;
    }

        // Sử dụng WINDOW_WIDTH, WINDOW_HEIGHT từ Config.h làm kích thước logic
        window = SDL_CreateWindow(title, xpos, ypos, WINDOW_WIDTH, WINDOW_HEIGHT, flags);
        if (window) {
            std::cout << "Window created!" << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer) {
            // Đặt kích thước logic cho renderer
            SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            std::cout << "Renderer created!" << std::endl;
        }
        //SDL_ShowCursor(SDL_DISABLE); //làm mất hình chuột


        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
            isRunning = false;
            return;
        }
         // Load ảnh nền (đường dẫn thay đổi theo cấu trúc folder của bạn)
        backgroundTexture = IMG_LoadTexture(renderer, "paint/backgroud.png");
        if (backgroundTexture == nullptr) {
            std::cout << "Failed to load background texture! SDL Error: " << SDL_GetError() << std::endl;
        }

        isRunning = true;
        player = new Player(WINDOW_WIDTH / 2 - 25, WINDOW_HEIGHT - 60, 50, 30);
        createEnemies(5);
        lastEnemySpawnTime = SDL_GetTicks();
    } else {
        isRunning = false;
    }
}



void Game::createEnemies(int count) {
    const int enemyWidth = 40;
    const int enemyHeight = 40;
    for (int i = 0; i < count; i++) {
        int x, y;
        bool valid;
        int attempts = 0;
        do {
            // Spawn within the square play area
            x = PLAY_AREA_X + rand() % (PLAY_AREA_SIZE - enemyWidth);
            y = PLAY_AREA_Y + rand() % (PLAY_AREA_SIZE / 2);  // still in top half
            valid = true;
            // Check for overlap with existing enemies
            for (auto enemy : enemies) {
                SDL_Rect existing = enemy->getCollider();
                SDL_Rect newRect = { x, y, enemyWidth, enemyHeight };
                if (Collision::checkCollision(existing, newRect)) {
                    valid = false;
                    break;
                }
            }
            attempts++;
        } while (!valid && attempts < 10);

        // Rest of the function remains the same
        if (i % 5 == 0) {
            enemies.push_back(new EnemyX(x, y, enemyWidth, enemyHeight));
        } else if (i % 4 == 0 || i % 1 == 0) {
            enemies.push_back(new EnemyStar(x, y, enemyWidth, enemyHeight));
        } else if (i % 3 == 0 || i % 2 == 0) {
            enemies.push_back(new EnemyPlus(x, y, enemyWidth, enemyHeight));
        }
    }
}

// Modify the handleEvents function to add a dedicated pause key (e.g., 'P')
void Game::handleEvents() {
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
            // Vẫn cho phép bắn đạn bình thường khi nhấn SPACE (nếu cần)
            if (event.key.keysym.sym == SDLK_SPACE && player->isActive() && !skillActive) {
                bullets.push_back(new Bullet(player->getX() + player->getWidth() / 2 - 5, player->getY(), 10, 20));
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            // Kiểm tra double click chuột trái
            if (event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 2) {
                Uint32 currentTime = SDL_GetTicks();
                if (!pauseOnCooldown && !skillActive) {
                    skillActive = true;
                    skillStartTime = currentTime;
                    lastSkillSpawnTime = currentTime;  // Khởi tạo thời gian spawn đầu tiên cho skill
                    skillBullets.clear();              // Xóa hết các đạn skill trước đó nếu có
                    std::cout << "Skill activated for 3 seconds!" << std::endl;
                } else if (pauseOnCooldown) {
                    Uint32 remainingCooldown = (lastPauseEndTime + PAUSE_COOLDOWN - currentTime) / 1000;
                    std::cout << "Skill on cooldown. " << remainingCooldown << " seconds remaining." << std::endl;
                }
            }
            break;
        default:
            break;
    }

    // Luôn cho phép player di chuyển
    player->handleEvent();
}




void Game::update() {
    Uint32 currentTime = SDL_GetTicks();
    // Luôn update player (player được phép di chuyển)
    player->update();

    if (skillActive) {
        // Trong thời gian skill active, chỉ spawn các viên đạn skill
        if (currentTime - lastSkillSpawnTime >= 200) {
            lastSkillSpawnTime = currentTime;
            int centerX = player->getX() + player->getWidth() / 2;
            int centerY = player->getY() + player->getHeight() / 2;
            // Tạo viên đạn skill ban đầu với góc = 0 (sẽ được cập nhật sau)
            Bullet* skillBullet = new Bullet(centerX, centerY, 10, 10, 0.0f, false);
            skillBullets.push_back(skillBullet);
        }
        // Không update enemy, không bắn đạn của enemy, không update các viên đạn đang bay
        // (tức là “đóng băng” các đối tượng ngoài player)

        // Kiểm tra nếu đã hết 3 giây skill
                // Kiểm tra nếu đã hết 3 giây skill
        if (currentTime - skillStartTime >= 3000) {
            int count = skillBullets.size();
            if (count > 0) {
                double angleIncrement = 2 * M_PI / count;
                for (int i = 0; i < count; i++) {
                    float angle = i * angleIncrement;
                    skillBullets[i]->setAngle(angle);
                    // Chuyển đạn skill sang vector đạn bình thường để chúng được update & render sau này
                    bullets.push_back(skillBullets[i]);
                }
            }
            skillBullets.clear();
            skillActive = false;
            // Cập nhật lại bộ đếm spawn enemy, trừ đi thời gian chiêu đã dùng
            lastEnemySpawnTime += (currentTime - skillStartTime);
            lastPauseEndTime = currentTime;
            pauseOnCooldown = true;
            //std::cout << "Skill ended, activated " << count << " skill bullets!" << std::endl;
        }

    }
    else {
        // Khi không active skill, update enemy và xử lý bắn đạn của enemy
        for (auto& enemy : enemies) {
            if (enemy->isActive()) {
                enemy->update();
                // Kiểm tra nếu enemy là EnemyStar
                EnemyStar* starEnemy = dynamic_cast<EnemyStar*>(enemy);
                if (starEnemy) {
                    std::vector<Bullet*> starBullets = starEnemy->shoot();
                    for (auto bullet : starBullets) {
                        bullets.push_back(bullet);
                    }
                }
                // Kiểm tra nếu enemy là EnemyPlus
                else {
                    EnemyPlus* plusEnemy = dynamic_cast<EnemyPlus*>(enemy);
                    if (plusEnemy) {
                        std::vector<Bullet*> plusBullets = plusEnemy->shoot();
                        for (auto bullet : plusBullets) {
                            bullets.push_back(bullet);
                        }
                    }
                    // Kiểm tra nếu enemy là EnemyX
                    else {
                        EnemyX* xEnemy = dynamic_cast<EnemyX*>(enemy);
                        if (xEnemy) {
                            std::vector<Bullet*> xBullets = xEnemy->shoot();
                            for (auto bullet : xBullets) {
                                bullets.push_back(bullet);
                            }
                        }
                    }
                }
            }
        }
        // Update bắn đạn tự động của player
        if (player->isActive()) {
            static Uint32 lastShotTime = 0;
            if (currentTime - lastShotTime >= 500) {
                lastShotTime = currentTime;
                int numBullets = 12;
                double angleIncrement = 2 * M_PI / numBullets;
                int centerX = player->getX() + player->getWidth() / 2;
                int centerY = player->getY() + player->getHeight() / 2;
                for (int i = 0; i < numBullets; i++) {
                    float angle = i * angleIncrement;
                    bullets.push_back(new Bullet(centerX, centerY, 10, 10, angle, false));
                }
            }
        }
        // Update các viên đạn đã tồn tại (cả của player và enemy)
        for (auto& bullet : bullets) {
            if (bullet->isActive()) {
                bullet->update();
            }
        }
    }

    // Xử lý các va chạm và loại bỏ các đối tượng không active (các collision vẫn được kiểm tra nếu cần)
    checkCollisions();
    cleanupInactiveObjects();

    if (!player->isActive()) {
        std::cout << "Game Over!" << std::endl;
        isRunning = false;
    }
    // --- THÊM CODE SPWAN ENEMY MỖI 10 GIÂY ---
    if (!skillActive && SDL_GetTicks() - lastEnemySpawnTime >= 10000) {  // mỗi 10 giây
    int missing = 5 - enemies.size();  // Nếu chưa đủ 5 con, tạo thêm số còn thiếu
    if (missing > 0) {
        createEnemies(missing);
    }
    lastEnemySpawnTime = SDL_GetTicks();

    }

    // Đảm bảo luôn có ít nhất 2 quái vật trên màn hình
    if (enemies.size() < 2) {
        int missing = 2 - enemies.size();
        createEnemies(missing);
    }

    // Xử lý cooldown skill, vv.
    if (pauseOnCooldown && (currentTime - lastPauseEndTime >= PAUSE_COOLDOWN)) {
        pauseOnCooldown = false;
        std::cout << "Skill ability is ready!" << std::endl;
    }

}


void Game::cleanupInactiveObjects() {
    // Loại bỏ các viên đạn không active
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](Bullet* b) {
                if (!b->isActive()) {
                    delete b;
                    return true;
                }
                return false;
            }),
        bullets.end()
    );
        // Loại bỏ enemy không active và cập nhật điểm số cũng như số enemy bị tiêu diệt
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [this](Enemy* e) {
                if (!e->isActive()) {
                    // Cộng điểm dựa trên loại enemy
                    if(dynamic_cast<EnemyStar*>(e) != nullptr)
                        score += 10;
                    else if(dynamic_cast<EnemyX*>(e) != nullptr)
                        score += 8;
                    else if(dynamic_cast<EnemyPlus*>(e) != nullptr)
                        score += 5;
                    else
                        score += 5;  // Các loại enemy khác (nếu có)

                    defeatedEnemies++;
                    // Khi tiêu diệt đủ 5 enemy, tăng level
                    if (defeatedEnemies >= 5) {
                        level++;
                        defeatedEnemies = 0;
                        std::cout << "Level Up! Current level: " << level << std::endl;
                    }
                    delete e;
                    return true;
                }
                return false;
            }),
        enemies.end()
    );
}

void Game::checkCollisions() {
    for (auto& bullet : bullets) {
        if (bullet->isActive()) {
            if (bullet->isEnemyBullet()) {
                if (Collision::checkCollision(player->getCollider(), bullet->getCollider())) {
                    player->takeDamage(5);
                    bullet->setActive(false);
                }
            } else {
                for (auto& enemy : enemies) {
                    if (enemy->isActive() && Collision::checkCollision(bullet->getCollider(), enemy->getCollider())) {
                        enemy->takeDamage(5);
                        bullet->setActive(false);
                    }
                }
            }
        }
    }
    for (auto& enemy : enemies) {
        if (enemy->isActive() && Collision::checkCollision(player->getCollider(), enemy->getCollider())) {
            player->takeDamage(10);
            enemy->setActive(false);
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Vẽ ảnh nền trong khung chơi game (square có kích thước PLAY_AREA_SIZE x PLAY_AREA_SIZE)
    if (backgroundTexture != nullptr) {
        SDL_Rect bgRect = { PLAY_AREA_X, PLAY_AREA_Y, PLAY_AREA_SIZE, PLAY_AREA_SIZE };
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &bgRect);
    }

    // Vẽ khung vùng chơi
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_Rect playArea = { PLAY_AREA_X, PLAY_AREA_Y, PLAY_AREA_SIZE, PLAY_AREA_SIZE };
    SDL_RenderDrawRect(renderer, &playArea);

    // Hiển thị điểm số bên trái màn hình
    SDL_Color textColor = {255, 255, 255}; // Màu trắng cho text
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {20, 20, surface->w, surface->h}; // Vị trí bạn muốn hiển thị, ví dụ (20,20)
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    player->render();
    for (auto& enemy : enemies) {
        if (enemy->isActive()) {
            enemy->render();
        }
    }
    for (auto& bullet : bullets) {
        if (bullet->isActive()) {
            bullet->render();
        }
    }
    // Render các viên đạn của skill (nếu còn)
    for (auto& bullet : skillBullets) {
        if (bullet->isActive()) {
            bullet->render();
        }
    }

    SDL_RenderPresent(renderer);
}


void Game::clean() {
    if (player) {
        delete player;
        player = nullptr;
    }
    for (auto& enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
    for (auto& bullet : bullets) {
        delete bullet;
    }
    bullets.clear();

    // Giải phóng texture nền
    if (backgroundTexture != nullptr) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }
    if (font) {
    TTF_CloseFont(font);
    font = nullptr;
    }
    TTF_Quit();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    SDL_Quit();
    std::cout << "Game cleaned!" << std::endl;
}

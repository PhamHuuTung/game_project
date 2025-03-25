#include "TextureManager.h"
#include "Game.h"
#include <SDL_mixer.h>


std::map<std::string, SDL_Texture*> TextureManager::textures;

SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
    SDL_Surface* tempSurface = IMG_Load(fileName);
    SDL_Texture* tex = nullptr;

    if(tempSurface == nullptr) {
        std::cout << "Failed to load image: " << fileName << " Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest) {
    SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}

void TextureManager::LoadTextures() {
    // Load all game textures here and store them in the map
    // Example:
    // textures["player"] = LoadTexture("assets/images/player.png");
    // textures["enemy"] = LoadTexture("assets/images/enemy.png");
    // textures["bullet"] = LoadTexture("assets/images/bullet.png");
    // In TextureManager::LoadTextures(), change:
    textures["mage"] = LoadTexture("paint/player.png");
    textures["mage_run1"] = LoadTexture("paint/3_RUN_000o.png");  // Changed from wizard_run1
    textures["mage_run2"] = LoadTexture("paint/3_RUN_001o.png");  // Changed from wizard_run2
    textures["mage_run3"] = LoadTexture("paint/3_RUN_002o.png");  // Changed from wizard_run3
    textures["mage_run4"] = LoadTexture("paint/3_RUN_003o.png");  // Changed from wizard_run4
    textures["mage_run5"] = LoadTexture("paint/3_RUN_004o.png");  // Changed from wizard_run5
    textures["mage_run6"] = LoadTexture("paint/3_RUN_005o.png");  // Changed from wizard_run6

    // Player bullet flying animation textures
    textures["bullet_player_1"] = LoadTexture("paint/bullet/shot6_asset.png");
    textures["bullet_player_2"] = LoadTexture("paint/bullet/shot6_1.png");
    textures["bullet_player_3"] = LoadTexture("paint/bullet/shot6_2.png");
    textures["bullet_player_4"] = LoadTexture("paint/bullet/shot6_3.png");
    textures["bullet_player_5"] = LoadTexture("paint/bullet/shot6_4.png");

    // Bullet explosion animation textures
    textures["bullet_explosion_1"] = LoadTexture("paint/bullet/shot6_exp1.png");
    textures["bullet_explosion_2"] = LoadTexture("paint/bullet/shot6_exp2.png");
    textures["bullet_explosion_3"] = LoadTexture("paint/bullet/shot6_exp3.png");
    textures["bullet_explosion_4"] = LoadTexture("paint/bullet/shot6_exp4.png");
    textures["bullet_explosion_5"] = LoadTexture("paint/bullet/shot6_exp5.png");
    textures["bullet_explosion_6"] = LoadTexture("paint/bullet/shot6_exp6.png");
    textures["bullet_explosion_7"] = LoadTexture("paint/bullet/shot6_exp7.png");
    textures["bullet_explosion_8"] = LoadTexture("paint/bullet/shot6_exp8.png");
    textures["bullet_explosion_9"] = LoadTexture("paint/bullet/shot6_exp9.png");
    textures["bullet_explosion_10"] = LoadTexture("paint/bullet/shot6_exp10.png");
// After loading explosion textures in TextureManager::LoadTextures()
    for (int i = 1; i <= 10; i++) {
        std::string texName = "bullet_explosion_" + std::to_string(i);
        if (textures[texName] == nullptr) {
            std::cout << "Failed to load explosion texture: " << texName << std::endl;
        } else {
            std::cout << "Successfully loaded explosion texture: " << texName << std::endl;
        }
    }
    // Load EnemyPlus animation textures (Wraith_01_Hurt_000.png đến Wraith_01_Hurt_011.png)
    textures["enemyplus_0"] = LoadTexture("paint/EnemyStar/Wraith_01_Attack_000.png");
    textures["enemyplus_1"] = LoadTexture("paint/EnemyStar/Wraith_01_Attack_001.png");
    textures["enemyplus_2"] = LoadTexture("paint/EnemyStar/Wraith_01_Attack_002.png");
    textures["enemyplus_3"] = LoadTexture("paint/EnemyStar/Wraith_01_Attack_003.png");
    textures["enemyplus_4"] = LoadTexture("paint/EnemyStar/Wraith_01_Attack_004.png");
    textures["enemyplus_5"] = LoadTexture("paint/EnemyStar/Wraith_01_Attack_005.png");
    textures["enemyplus_6"] = LoadTexture("paint/EnemyStar/Wraith_01_Attack_006.png");
    textures["enemyplus_7"] = LoadTexture("paint/EnemyStar/Wraith_01_Attack_007.png");
    textures["enemyplus_8"] = LoadTexture("paint/EnemyStar/Wraith_01_Attack_008.png");
    textures["enemyplus_9"] = LoadTexture("paint/EnemyStar/Wraith_01_Attack_009.png");
    textures["enemyplus_10"] = LoadTexture("paint/EnemyStar/Wraith_01_Attack_001.png");
    textures["enemyplus_11"] = LoadTexture("paint/EnemyStar/Wraith_01_Attack_011.png");

    // đạn enemy
    textures["enemy_0"] = LoadTexture("paint/EnemyX/1.png");
    textures["enemy_1"] = LoadTexture("paint/EnemyX/2.png");
    textures["enemy_2"] = LoadTexture("paint/EnemyX/3.png");
    textures["enemy_3"] = LoadTexture("paint/EnemyX/4.png");


}

SDL_Texture* TextureManager::GetTexture(const std::string& id) {
    return textures[id];
}

// Add this implementation to TextureManager.cpp
void TextureManager::DrawEx(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, 0, nullptr, flip);
}

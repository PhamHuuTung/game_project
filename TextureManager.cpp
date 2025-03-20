#include "TextureManager.h"
#include "Game.h"

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
}

SDL_Texture* TextureManager::GetTexture(const std::string& id) {
    return textures[id];
}

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <map>

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const char* fileName);
    static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);

    static void LoadTextures();
    static SDL_Texture* GetTexture(const std::string& id);

private:
    static std::map<std::string, SDL_Texture*> textures;
    // Add this function to TextureManager.h
    static void DrawEx(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip = SDL_FLIP_NONE);
};

#endif // TEXTUREMANAGER_H

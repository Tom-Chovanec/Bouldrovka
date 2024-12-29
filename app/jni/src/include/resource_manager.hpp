#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <map>

#include "common.hpp"

class ResourceManager {
private:
    std::map<const char*, SDL_Texture*> m_Textures;
    std::map<const char*, TTF_Font*> m_Fonts;

public:
    ~ResourceManager();

    bool loadTexture(const Context& context, const char* path, const char* name);
    bool loadFont(const Context& context, const char* path, const char* name, float size);

    void makeColoredTexture(const char* srcTextureName, const char* dstTextureName, SDL_Color& color);

    SDL_Texture* getTexture(const char* textureName);
    TTF_Font* getFont(const char* fontName);

    void clear();

};

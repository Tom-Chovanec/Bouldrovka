#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <map>
#include <string>

#include "common.hpp"

class ResourceManager {
private:
    std::map<std::string, SDL_Texture*> m_Textures;
    std::map<std::string, TTF_Font*> m_Fonts;

public:
    ~ResourceManager();

    bool loadTexture(const Context& context, const char* path, const std::string& name);
    bool loadFont(const Context& context, const char* path, const std::string& name, float size);

    SDL_Texture* getTexture(const std::string& textureName);
    TTF_Font* getFont(const std::string& fontName);

    void clear();

};

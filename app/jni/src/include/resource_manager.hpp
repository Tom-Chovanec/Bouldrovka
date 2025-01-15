#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <filesystem>
#include <map>
#include <string_view>

#include "common.hpp"

class ResourceManager {
private:
    std::map<std::string_view, SDL_Texture*> m_Textures;
    std::map<std::string_view, TTF_Font*> m_Fonts;

public:
    ~ResourceManager();

    bool loadTexture(const Context& context, std::filesystem::path path, std::string_view name);
    bool loadFont(const Context& context, std::filesystem::path path, std::string_view name, float size);

    SDL_Texture* getTexture(std::string_view textureName);
    TTF_Font* getFont(std::string_view fontName);

    void clear();

};

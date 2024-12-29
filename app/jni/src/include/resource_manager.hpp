#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <filesystem>
#include <map>

#include "common.hpp"

class ResourceManager {
private:
    std::map<const char*, SDL_Texture*> m_Textures;

public:
    ~ResourceManager();

    bool loadTexture(const Context& context, std::__fs::filesystem::path path, const char* name);

    void makeColoredTexture(const char* srcTextureName, const char* dstTextureName, SDL_Color& color);

    SDL_Texture* getTexture(const char* textureName);

    void clear();

};

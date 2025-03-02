#include "include/resource_manager.hpp"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <filesystem>
#include <string_view>

bool ResourceManager::loadTexture(const Context& context, std::filesystem::path path, std::string_view name) {
    m_Textures[name] = IMG_LoadTexture(context.renderer, path.c_str());
    if (m_Textures[name] == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture %s, error: %s", path.c_str(), SDL_GetError());
        return false;
    }
    return true;
}

bool ResourceManager::loadFont(const Context& context, std::filesystem::path path, std::string_view name, float size) {
    m_Fonts[name] = TTF_OpenFont(path.c_str(), size * context.windowSize.x);
    if (m_Fonts[name] == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load font %s, error: %s", path.c_str(), SDL_GetError());
        return false;
    }
    return true;
}

SDL_Texture* ResourceManager::getTexture(std::string_view textureName) {
    if (m_Textures.count(textureName) == 0) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Texture %s does not exits!", textureName.data());
        return nullptr;
    }
    return m_Textures[textureName];
}

TTF_Font* ResourceManager::getFont(std::string_view fontName) {
    if (m_Fonts.count(fontName) == 0) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Font %s does not exits!", fontName.data());
        return nullptr;
    }
    return m_Fonts[fontName];
}

void ResourceManager::clear() {
    for (auto x : m_Textures) {
        SDL_DestroyTexture(x.second);
    }
    for (auto x : m_Fonts) {
        TTF_CloseFont(x.second);
    }
    m_Textures.clear();
    m_Fonts.clear();
}

ResourceManager::~ResourceManager() {
    this->clear();
}


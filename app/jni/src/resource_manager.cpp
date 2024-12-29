
#include "include/resource_manager.hpp"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>

bool ResourceManager::loadTexture(const Context& context, const char* path, const char* name) {
    m_Textures[name] = IMG_LoadTexture(context.renderer, path);
    if (m_Textures[name] == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture %s, error: %s", path, SDL_GetError());
        return false;
    }
    return true;
}

SDL_Texture* ResourceManager::getTexture(const char* textureName) {
    if (m_Textures.count(textureName) == 0) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Texture %s does not exits!", textureName);
        return nullptr;
    }
    return m_Textures[textureName];
}

void ResourceManager::clear() {
    for (auto x : m_Textures) {
        SDL_DestroyTexture(x.second);
    }
    m_Textures.clear();
}

ResourceManager::~ResourceManager() {
    this->clear();
}


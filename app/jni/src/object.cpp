#include "include/object.hpp"
#include <SDL3/SDL_log.h>

void Object::render(const Context& context, Renderer& renderer) {
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "What are you doing here");
}

void Object::update(const Context& context) {
}

void Object::handleClick(const Context& context) {

}


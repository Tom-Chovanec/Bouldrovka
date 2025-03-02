#pragma once

#include <SDL3/SDL.h>
#include "common.hpp"
#include "renderer.hpp"

class Object {
public:
    virtual void render(const Context& context, Renderer& renderer);
    virtual void update(const Context& context);
    virtual void handleClick(const Context& context);

    virtual ~Object() = default;
};

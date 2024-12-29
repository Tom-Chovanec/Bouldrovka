#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <vector>

#include "common.hpp"
#include "object.hpp"

class Scene {
private:
    std::vector<std::unique_ptr<Object>> m_Objects;

public:
    ~Scene();

    template <typename T, typename... Args>
    void addObject(Args&&... args) {
        static_assert(std::is_base_of<Object, T>::value, "T must derive from Object");
        m_Objects.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    void render(const Context& context, Renderer& renderer);

    void update(const Context& context);

};


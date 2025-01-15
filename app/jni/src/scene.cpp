#include "include/scene.hpp"
#include <memory>


Scene::~Scene() {
    m_Objects.clear();
}

void Scene::setBackgroundColor(Colors::COLOR color) {
    m_BackgroundColor = color;
}

void Scene::render(const Context& context, Renderer& renderer) {
    renderer.clearScreen(getColor(m_BackgroundColor));
    for (auto& object : m_Objects) {
        object->render(context, renderer);
    }
}

void Scene::update(const Context& context) {
    for (const auto& object : m_Objects) {
        object->update(context);
    }
}

void Scene::handleClick(const Context& context) {
    for (const auto& object : m_Objects) {
        object->handleClick(context);
    }
}

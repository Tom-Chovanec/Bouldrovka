#include "include/scene.hpp"
#include <memory>


Scene::~Scene() {
    m_Objects.clear();
}


void Scene::render(const Context& context, Renderer& renderer) {
    for (auto& object : m_Objects) {
        object->render(context, renderer);
    }
}

void Scene::update(const Context& context) {
    for (const auto& object : m_Objects) {
        object->update(context);
    }
}

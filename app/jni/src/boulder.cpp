#include "include/boulder.hpp"
#include "include/hold.hpp"
#include <algorithm>


Boulder::Boulder(std::string_view name, std::string_view description, std::string_view imageName) 
: m_Name(name), m_Description(description), m_ImageName(imageName) {

}

void Boulder::addHold(Float2 pos, DIFFICULTY difficulty, HOLD_TYPE type) {
    m_Holds.emplace_back(pos, difficulty, type);
}

bool pointInCircle(Float2 point, Float2 center, float radius) {
    float dx = point.x - center.x;
    float dy = point.y - center.y;
    float distanceSquared = dx * dx + dy * dy;

    return distanceSquared <= radius * radius;
}

constexpr float removeRadius = 0.1;
void Boulder::removeHold(Float2 pos) {
    auto tmp = std::remove_if(
        m_Holds.begin(),
        m_Holds.end(),
        [pos](const Hold& x) {
            return pointInCircle(pos, x.getPos(), removeRadius);
        }
    );
    m_Holds.erase(tmp, m_Holds.end());
}

void Boulder::setGenerationSettings(HoldGenerationSettings settings) {
    m_GenerationSettings = settings;
}

void Boulder::setName(std::string_view name) {
    m_Name = name;
}

void Boulder::setDescription(std::string_view description) {
    m_Description = description;
}
 
void Boulder::setImageName(std::string_view imageName) {
    m_ImageName = imageName;
}

const std::vector<Hold>& Boulder::getHolds() const {
    return m_Holds;
}

const std::vector<Hold>& Boulder::getGeneratedHolds() const {
    return m_GeneratedHolds;
}

std::string_view Boulder::getName()  const {
    return m_Name;
}
 
std::string_view Boulder::getDescription()  const {
    return m_Description;
}

std::string_view Boulder::getImageName() const {
    return m_ImageName;
}

HoldGenerationSettings Boulder::getGenerationSettings()  const {
    return m_GenerationSettings;
}

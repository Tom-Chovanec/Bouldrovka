#pragma once

#include "hold.hpp"
#include <string_view>
#include <vector>

struct HoldGenerationSettings {
    int topHoldAmount = 1;
    int upperHoldAmount = 3;
    int lowerHoldAmount = 3;
    int startHoldAmount = 1;
    int footHoldAmount = 1;
};

class Boulder {
private:
    std::string_view m_Name;
    std::string_view m_Description;
    std::string_view m_ImageName;

    std::vector<Hold> m_Holds;
    std::vector<Hold> m_GeneratedHolds;

    HoldGenerationSettings m_GenerationSettings;

public:
    Boulder(std::string_view name, std::string_view description, std::string_view imageName);

    void addHold(Float2 pos, DIFFICULTY difficulty, HOLD_TYPE type);
    void removeHold(Float2 pos);

    void setGenerationSettings(HoldGenerationSettings settings);
    void setName(std::string_view name);
    void setDescription(std::string_view description);
    void setImageName(std::string_view imageName);

    const std::vector<Hold>& getHolds() const;
    const std::vector<Hold>& getGeneratedHolds() const;
    std::string_view getName() const;
    std::string_view getDescription() const;
    std::string_view getImageName() const;
    HoldGenerationSettings getGenerationSettings() const;
};

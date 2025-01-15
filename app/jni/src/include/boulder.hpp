#pragma once

#include "hold.hpp"
#include <string_view>
#include <vector>

class Boulder {
private:
    std::string_view m_Name;
    std::string_view m_Description;
    std::string_view m_ImageName;

    std::vector<Hold> m_Holds;

public:

};

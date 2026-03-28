#include "FloatField.h"

void FloatField::Draw()
{
    ImGui::SliderFloat(m_name.c_str(), m_value, m_min, m_max);
}

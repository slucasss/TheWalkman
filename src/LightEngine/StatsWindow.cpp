#include "StatField.h"
#include "FloatField.h"
#include "StatsWindow.h"
#include "Entity.h"

void StatsWindow::AddFloat(const std::string& name, float* value, float min, float max)
{
    m_fields.push_back(new FloatField(name, value, min, max));
}

void StatsWindow::Draw()
{
    ImGui::Begin(m_name.c_str());

    for (StatField* field : m_fields)
        field->Draw();

    m_targetEntity->Inspect();

    ImGui::End();
}

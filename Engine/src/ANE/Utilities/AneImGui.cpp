#include "anepch.h"
#include "AneImGui.h"

namespace Engine::AneImGui
{
    AneImGuiContext* _aneImGui = nullptr;

    AneImGuiContext* Initialize()
    {
        _aneImGui = new AneImGuiContext();
        if(ImGui::GetCurrentContext() == nullptr) ImGui::CreateContext();

        return _aneImGui;
    }

    AneImGuiContext* GetContext()
    {
        return _aneImGui;
    }

    void Cleanup()
    {
        delete _aneImGui;
        ImGui::DestroyContext();
    }

    void SetNextTableDecoratorFlags(const ImGuiTableFlags decoratorFlags)
    {
        _aneImGui->NextTableDecoratorFlags = decoratorFlags;
    }

    void SetNextTableFieldWidth(const float fieldWidth)
    {
        _aneImGui->NextTableFieldWidth = fieldWidth;
    }

    bool LabelSliderFloat(const char* label, float* v, const float vMin, const float vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelSliderScalar(label, ImGuiDataType_Float, v, &vMin, &vMax, format, flags);
    }

    bool LabelSliderFloat2(const char* label, float v[2], const float vMin, const float vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelSliderScalarN(label, ImGuiDataType_Float, v, 2, &vMin, &vMax, format, flags);
    }

    bool LabelSliderFloat3(const char* label, float v[3], const float vMin, const float vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelSliderScalarN(label, ImGuiDataType_Float, v, 3, &vMin, &vMax, format, flags);
    }

    bool LabelSliderFloat4(const char* label, float v[4], const float vMin, const float vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelSliderScalarN(label, ImGuiDataType_Float, v, 4, &vMin, &vMax, format, flags);
    }

    bool LabelSliderInt(const char* label, int* v, const int vMin, const int vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelSliderScalar(label, ImGuiDataType_S32, v, &vMin, &vMax, format, flags);
    }

    bool LabelSliderInt2(const char* label, int v[2], const int vMin, const int vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelSliderScalarN(label, ImGuiDataType_S32, v, 2, &vMin, &vMax, format, flags);
    }

    bool LabelSliderInt3(const char* label, int v[3], const int vMin, const int vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelSliderScalarN(label, ImGuiDataType_S32, v, 3, &vMin, &vMax, format, flags);
    }

    bool LabelSliderInt4(const char* label, int v[4], const int vMin, const int vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelSliderScalarN(label, ImGuiDataType_S32, v, 4, &vMin, &vMax, format, flags);
    }

    bool LabelSliderScalar(const char* label, const ImGuiDataType dataType, void* pData, const void* pMin, const void* pMax, const char* format, const ImGuiSliderFlags flags)
    {
        bool change = false;

        if (BeginLabelTable(label))
        {
            if (ImGui::SliderScalar(std::format("##{}",label).c_str(), dataType, pData, pMin, pMax, format, flags))
            {
                change =  true;
            }

            ImGui::EndTable();
        }

        EndItem();

        return change;
    }

    bool LabelSliderScalarN(const char* label, const ImGuiDataType dataType, void* pData, const int components, const void* pMin, const void* pMax, const char* format, const ImGuiSliderFlags flags)
    {
        bool change = false;

        if (BeginLabelTable(label))
        {
            if (ImGui::SliderScalarN(std::format("##{}",label).c_str(), dataType, pData, components, pMin, pMax, format, flags))
            {
                change =  true;
            }

            ImGui::EndTable();
        }

        EndItem();

        return change;
    }

    bool LabelColorEdit3(const char* label, float col[3], const ImGuiColorEditFlags flags)
    {
        return LabelColorEdit4(label, col, flags | ImGuiColorEditFlags_NoAlpha);
    }

    bool LabelColorEdit4(const char* label, float col[4], const ImGuiColorEditFlags flags)
    {
        bool change = false;

        if (BeginLabelTable(label))
        {
            if (ImGui::ColorEdit4(std::format("##{}",label).c_str(), col, flags))
            {
                change =  true;
            }

            ImGui::EndTable();
        }

        EndItem();

        return change;
    }

    bool LabelColorPicker3(const char* label, float col[3], const ImGuiColorEditFlags flags)
    {
        float col4[4] = { col[0], col[1], col[2], 1.0f };
        if (!LabelColorPicker4(label, col4, flags | ImGuiColorEditFlags_NoAlpha))
            return false;
        col[0] = col4[0]; col[1] = col4[1]; col[2] = col4[2];
        return true;
    }

    bool LabelColorPicker4(const char* label, float col[4], const ImGuiColorEditFlags flags, const float* refCol)
    {
        bool change = false;

        if (BeginLabelTable(label))
        {
            if (ImGui::ColorPicker4(std::format("##{}",label).c_str(), col, flags, refCol))
            {
                change =  true;
            }

            ImGui::EndTable();
        }

        EndItem();

        return change;
    }

    bool BeginLabelTable(const char* label, const char* strId)
    {
        const AneImGuiContext& g = *_aneImGui;

        if (ImGui::BeginTable(strId, 2, ImGuiTableFlags_SizingFixedSame | g.NextTableDecoratorFlags))
        {
            ImGui::TableSetupColumn("Table Label", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Table Field");
            ImGui::TableNextRow(ImGuiTableRowFlags_None);

            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::Text(label);

            ImGui::TableSetColumnIndex(1);
            ImGui::PushItemWidth(g.NextTableFieldWidth);

            return true;
        }
        return false;
    }


    void EndItem()
    {
        _aneImGui->NextTableDecoratorFlags = 0;
        _aneImGui->NextTableFieldWidth = 0;
    }
}

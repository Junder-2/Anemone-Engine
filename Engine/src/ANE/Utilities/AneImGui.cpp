#include "anepch.h"
#include "AneImGui.h"

#include "ANE/Math/FMath.h"

namespace Engine::AneImGui
{
    AneImGuiContext* _aneImGui = nullptr;

    AneImGuiContext* Initialize()
    {
        _aneImGui = new AneImGuiContext();
        EndItem();
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

    float CalculateTableFieldWidth()
    {
        constexpr float fieldWidthRatio = 0.61803398875f; // 1 / Phi.
        constexpr float fieldMinWidth = 80, fieldMaxWidth = 5000;

        const float width = ImGui::GetColumnWidth();
        const float fieldWidth = FMath::Clamp(width * fieldWidthRatio, fieldMinWidth, fieldMaxWidth);

        return fieldWidth;
    }

    bool LabelDragFloat(const char* label, float* v, const float vSpeed, const float vMin, const float vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelDragScalar(label, ImGuiDataType_Float, v, vSpeed, &vMin, &vMax, format, flags);
    }

    bool LabelDragFloat2(const char* label, float v[2], const float vSpeed, const float vMin, const float vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelDragScalarN(label, ImGuiDataType_Float, v, 2, vSpeed, &vMin, &vMax, format, flags);
    }

    bool LabelDragFloat3(const char* label, float v[3], const float vSpeed, const float vMin, const float vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelDragScalarN(label, ImGuiDataType_Float, v, 3, vSpeed, &vMin, &vMax, format, flags);
    }

    bool LabelDragFloat4(const char* label, float v[4], const float vSpeed, const float vMin, const float vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelDragScalarN(label, ImGuiDataType_Float, v, 4, vSpeed, &vMin, &vMax, format, flags);
    }

    bool LabelDragInt(const char* label, int* v, const float vSpeed, const int vMin, const int vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelDragScalar(label, ImGuiDataType_S32, v, vSpeed, &vMin, &vMax, format, flags);
    }

    bool LabelDragInt2(const char* label, int v[2], const float vSpeed, const int vMin, const int vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelDragScalarN(label, ImGuiDataType_S32, v, 2, vSpeed, &vMin, &vMax, format, flags);
    }

    bool LabelDragInt3(const char* label, int v[3], const float vSpeed, const int vMin, const int vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelDragScalarN(label, ImGuiDataType_S32, v, 3, vSpeed, &vMin, &vMax, format, flags);
    }

    bool LabelDragInt4(const char* label, int v[4], const float vSpeed, const int vMin, const int vMax, const char* format, const ImGuiSliderFlags flags)
    {
        return LabelDragScalarN(label, ImGuiDataType_S32, v, 4, vSpeed, &vMin, &vMax, format, flags);
    }

    bool LabelDragScalar(const char* label, const ImGuiDataType dataType, void* pData, const float vSpeed, const void* pMin, const void* pMax, const char* format, const ImGuiSliderFlags flags)
    {
        bool change = false;

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

        if (BeginLabelField(label))
        {
            if (ImGui::DragScalar(std::format("##{}",label).c_str(), dataType, pData, vSpeed, pMin, pMax, format, flags))
            {
                change =  true;
            }

            EndLabelField();
        }

        ImGui::PopStyleVar();

        return change;
    }

    bool LabelDragScalarN(const char* label, const ImGuiDataType dataType, void* pData, const int components, const float vSpeed, const void* pMin, const void* pMax, const char* format, const ImGuiSliderFlags flags)
    {
        bool change = false;

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

        if (BeginLabelField(label))
        {
            if (ImGui::DragScalarN(std::format("##{}",label).c_str(), dataType, pData, components, vSpeed, pMin, pMax, format, flags))
            {
                change =  true;
            }

            EndLabelField();
        }

        ImGui::PopStyleVar();

        return change;
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

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

        if (BeginLabelField(label))
        {
            if (ImGui::SliderScalar(std::format("##{}",label).c_str(), dataType, pData, pMin, pMax, format, flags))
            {
                change =  true;
            }

            EndLabelField();
        }

        ImGui::PopStyleVar();

        return change;
    }

    bool LabelSliderScalarN(const char* label, const ImGuiDataType dataType, void* pData, const int components, const void* pMin, const void* pMax, const char* format, const ImGuiSliderFlags flags)
    {
        bool change = false;

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

        if (BeginLabelField(label))
        {
            if (ImGui::SliderScalarN(std::format("##{}",label).c_str(), dataType, pData, components, pMin, pMax, format, flags))
            {
                change =  true;
            }

            EndLabelField();
        }

        ImGui::PopStyleVar();

        return change;
    }

    bool LabelColorEdit3(const char* label, float col[3], const ImGuiColorEditFlags flags)
    {
        return LabelColorEdit4(label, col, flags | ImGuiColorEditFlags_NoAlpha);
    }

    bool LabelColorEdit4(const char* label, float col[4], const ImGuiColorEditFlags flags)
    {
        bool change = false;

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

        if (BeginLabelField(label))
        {
            if (ImGui::ColorEdit4(std::format("##{}",label).c_str(), col, flags))
            {
                change =  true;
            }

            EndLabelField();
        }

        ImGui::PopStyleVar();

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

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

        if (BeginLabelField(label))
        {
            if (ImGui::ColorPicker4(std::format("##{}",label).c_str(), col, flags, refCol))
            {
                change =  true;
            }

            EndLabelField();
        }

        ImGui::PopStyleVar();

        return change;
    }

    bool BeginLabelField(const char* label, const char* strId)
    {
        AneImGuiContext& g = *_aneImGui;

        if(g.NextTableFieldWidth < 0)
        {
            g.NextTableFieldWidth = CalculateTableFieldWidth();
        }

        const float labelWidth = FMath::Clamp(ImGui::GetColumnWidth() - g.NextTableFieldWidth - 15.f, 10, 100);

        constexpr ImGuiTableFlags constFlags = ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_NoPadOuterX;

        if (ImGui::BeginTable(strId, 2, g.NextTableDecoratorFlags | constFlags))
        {
            ImGui::TableSetupColumn("Table Label", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Table Field");
            ImGui::TableNextRow(ImGuiTableRowFlags_None);

            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::PushItemWidth(labelWidth);
            ImGui::Text(label);

            ImGui::TableSetColumnIndex(1);
            ImGui::PushItemWidth(g.NextTableFieldWidth);

            return true;
        }

        return true;
    }

    void EndLabelField()
    {
        ImGui::EndTable();
        EndItem();
    }

    void EndItem()
    {
        _aneImGui->NextTableDecoratorFlags = 0;
        _aneImGui->NextTableFieldWidth = -1.f;
    }
}

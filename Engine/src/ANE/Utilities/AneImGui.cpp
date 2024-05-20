#include "anepch.h"
#include "AneImGui.h"

#include "imgui_internal.h"
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
        constexpr float fieldMinWidth = 65, fieldMaxWidth = 5000;

        const float width = ImGui::GetColumnWidth();
        const float fieldWidth = FMath::Clamp(width * fieldWidthRatio, fieldMinWidth, fieldMaxWidth);

        return fieldWidth;
    }

    bool LabelCheckbox(const char* label, bool* v)
    {
        bool change = false;

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

        if (BeginLabelField(label))
        {
            if (ImGui::Checkbox(std::format("##{}", label).c_str(), v))
            {
                change =  true;
            }

            // Left align checkbox
            ImGui::SameLine(0, 0);
            ImGui::Dummy(ImVec2(_aneImGui->NextTableFieldWidth - ImGui::GetFrameHeight(), 0));

            EndLabelField();
        }

        ImGui::PopStyleVar();

        return change;
    }

    bool LabelRadioButton(const char* label, const bool active)
    {
        bool change = false;

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

        if (BeginLabelField(label))
        {
            if (ImGui::RadioButton(std::format("##{}",label).c_str(), active))
            {
                change =  true;
            }

            EndLabelField();
        }

        ImGui::PopStyleVar();

        return change;
    }

    bool LabelRadioButton(const char* label, int* v, const int vButton)
    {
        const bool pressed = LabelRadioButton(label, *v == vButton);
        if (pressed)
            *v = vButton;
        return pressed;
    }

    bool LabelCombo(const char* label, int* currentItem, const char* const items[], const int itemsCount)
    {
        bool change = false;

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

        if (BeginLabelField(label))
        {
            if (ImGui::Combo(std::format("##{}",label).c_str(), currentItem, items, itemsCount))
            {
                change =  true;
            }

            EndLabelField();
        }

        ImGui::PopStyleVar();

        return change;
    }

    void LabelText(const char* label, const char* value)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

        if (BeginLabelField(label))
        {
            ImGui::Text(value);

            EndLabelField();
        }
        ImGui::PopStyleVar();
    }

    bool LabelInputText(const char* label, char* buf, const size_t bufSize, const ImGuiInputTextFlags flags, const ImGuiInputTextCallback callback, void* userData)
    {
        IM_ASSERT(!(flags & ImGuiInputTextFlags_Multiline)); // call InputTextMultiline()
        return LabelInputTextEx(label, nullptr, buf, (int)bufSize, ImVec2(0, 0), flags, callback, userData);
    }

    bool LabelInputTextMultiline(const char* label, char* buf, const size_t bufSize, const ImVec2& size, const ImGuiInputTextFlags flags, const ImGuiInputTextCallback callback, void* userData)
    {
        return LabelInputTextEx(label, NULL, buf, (int)bufSize, size, flags | ImGuiInputTextFlags_Multiline, callback, userData);
    }

    bool LabelInputTextWithHint(const char* label, const char* hint, char* buf, const size_t bufSize, const ImGuiInputTextFlags flags, const ImGuiInputTextCallback callback, void* userData)
    {
        IM_ASSERT(!(flags & ImGuiInputTextFlags_Multiline)); // call InputTextMultiline() or  InputTextEx() manually if you need multi-line + hint.
        return LabelInputTextEx(label, hint, buf, (int)bufSize, ImVec2(0, 0), flags, callback, userData);
    }

    bool LabelInputTextEx(const char* label, const char* hint, char* buf, const int bufSize, const ImVec2& sizeArg, const ImGuiInputTextFlags flags, const ImGuiInputTextCallback callback, void* userData)
    {
        bool change = false;

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

        if (BeginLabelField(label))
        {
            if (ImGui::InputTextEx(std::format("##{}",label).c_str(), hint, buf, bufSize, sizeArg, flags, callback, userData))
            {
                change =  true;
            }

            EndLabelField();
        }

        ImGui::PopStyleVar();

        return change;
    }

    bool LabelInputFloat(const char* label, float* v, float step, float stepFast, const char* format, const ImGuiInputTextFlags flags)
    {
        return LabelInputScalar(label, ImGuiDataType_Float, (void*)v, (void*)(step > 0.0f ? &step : nullptr), (void*)(stepFast > 0.0f ? &stepFast : nullptr), format, flags);
    }

    bool LabelInputFloat2(const char* label, float v[2], const char* format, const ImGuiInputTextFlags flags)
    {
        return LabelInputScalarN(label, ImGuiDataType_Float, v, 2, nullptr, nullptr, format, flags);
    }

    bool LabelInputFloat3(const char* label, float v[3], const char* format, const ImGuiInputTextFlags flags)
    {
        return LabelInputScalarN(label, ImGuiDataType_Float, v, 3, nullptr, nullptr, format, flags);
    }

    bool LabelInputFloat4(const char* label, float v[4], const char* format, const ImGuiInputTextFlags flags)
    {
        return LabelInputScalarN(label, ImGuiDataType_Float, v, 4, nullptr, nullptr, format, flags);
    }

    bool LabelInputInt(const char* label, int* v, int step, int stepFast, const ImGuiInputTextFlags flags)
    {
        // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
        const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
        return LabelInputScalar(label, ImGuiDataType_S32, (void*)v, (void*)(step > 0 ? &step : nullptr), (void*)(stepFast > 0 ? &stepFast : nullptr), format, flags);
    }

    bool LabelInputInt2(const char* label, int v[2], const ImGuiInputTextFlags flags)
    {
        return LabelInputScalarN(label, ImGuiDataType_S32, v, 2, nullptr, nullptr, "%d", flags);
    }

    bool LabelInputInt3(const char* label, int v[3], const ImGuiInputTextFlags flags)
    {
        return LabelInputScalarN(label, ImGuiDataType_S32, v, 3, nullptr, nullptr, "%d", flags);
    }

    bool LabelInputInt4(const char* label, int v[4], const ImGuiInputTextFlags flags)
    {
        return LabelInputScalarN(label, ImGuiDataType_S32, v, 4, nullptr, nullptr, "%d", flags);
    }

    bool LabelInputDouble(const char* label, double* v, double step, double stepFast, const char* format, const ImGuiInputTextFlags flags)
    {
        return LabelInputScalar(label, ImGuiDataType_Double, (void*)v, (void*)(step > 0.0 ? &step : nullptr), (void*)(stepFast > 0.0 ? &stepFast : nullptr), format, flags);
    }

    bool LabelInputScalar(const char* label, const ImGuiDataType dataType, void* pData, const void* pStep, const void* pStepFast, const char* format, const ImGuiInputTextFlags flags)
    {
        bool change = false;

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

        if (BeginLabelField(label))
        {
            if (ImGui::InputScalar(std::format("##{}",label).c_str(), dataType, pData, pStep, pStepFast, format, flags))
            {
                change =  true;
            }

            EndLabelField();
        }

        ImGui::PopStyleVar();

        return change;
    }

    bool LabelInputScalarN(const char* label, const ImGuiDataType dataType, void* pData, const int components, const void* pStep, const void* pStepFast, const char* format, const ImGuiInputTextFlags flags)
    {
        bool change = false;

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

        if (BeginLabelField(label))
        {
            if (ImGui::InputScalarN(std::format("##{}",label).c_str(), dataType, pData, components, pStep, pStepFast, format, flags))
            {
                change =  true;
            }

            EndLabelField();
        }

        ImGui::PopStyleVar();

        return change;
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

        if (ImGui::BeginTable(std::format("{} Table", label).c_str(), 2, g.NextTableDecoratorFlags | constFlags))
        {
            ImGui::TableSetupColumn(std::format("{} Table Label", label).c_str(), ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn(std::format("{} Table Field", label).c_str());
            ImGui::TableNextRow(ImGuiTableRowFlags_None);

            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::Text(label);

            ImGui::TableSetColumnIndex(1);
            ImGui::PushItemWidth(g.NextTableFieldWidth);

            return true;
        }

        return true;
    }

    void EndLabelField()
    {
        const float column = ImGui::GetContentRegionMax().x;
        const float padding = FMath::Min(FMath::Max(column*.25f-40.f, FLT_MIN), 5.f);

        ImGui::SameLine(0, 0);
        ImGui::Dummy(ImVec2(padding, 0));

        ImGui::EndTable();
        EndItem();
    }

    void EndItem()
    {
        _aneImGui->NextTableDecoratorFlags = 0;
        _aneImGui->NextTableFieldWidth = -1.f;
    }
}

#pragma once
#include "imgui.h"

namespace Engine::AneImGui
{
    struct AneImGuiContext
    {
        float NextTableFieldWidth = 0;
        ImGuiTableFlags NextTableDecoratorFlags = 0;
    };

    extern AneImGuiContext* _aneImGui;

    AneImGuiContext* Initialize();
    AneImGuiContext* GetContext();
    void Cleanup();


    void SetNextTableDecoratorFlags(ImGuiTableFlags decoratorFlags);
    void SetNextTableFieldWidth(float fieldWidth);

    bool LabelSliderFloat(const char* label, float* v, float vMin, float vMax, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    bool LabelSliderFloat2(const char* label, float v[2], float vMin, float vMax, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    bool LabelSliderFloat3(const char* label, float v[3], float vMin, float vMax, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    bool LabelSliderFloat4(const char* label, float v[4], float vMin, float vMax, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    bool LabelSliderInt(const char* label, int* v, int vMin, int vMax, const char* format = "%d", ImGuiSliderFlags flags = 0);
    bool LabelSliderInt2(const char* label, int v[2], int vMin, int vMax, const char* format = "%d", ImGuiSliderFlags flags = 0);
    bool LabelSliderInt3(const char* label, int v[3], int vMin, int vMax, const char* format = "%d", ImGuiSliderFlags flags = 0);
    bool LabelSliderInt4(const char* label, int v[4], int vMin, int vMax, const char* format = "%d", ImGuiSliderFlags flags = 0);
    bool LabelSliderScalar(const char* label, ImGuiDataType dataType, void* pData, const void* pMin, const void* pMax, const char* format = nullptr, ImGuiSliderFlags flags = 0);
    bool LabelSliderScalarN(const char* label, ImGuiDataType dataType, void* pData, int components, const void* pMin, const void* pMax, const char* format = nullptr, ImGuiSliderFlags flags = 0);

    bool LabelColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
    bool LabelColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
    bool LabelColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
    bool LabelColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* refCol = nullptr);

    bool BeginLabelTable(const char* label, const char* strId = "Table");
    void EndItem();
}

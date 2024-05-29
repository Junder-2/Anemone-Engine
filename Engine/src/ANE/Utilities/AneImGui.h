#pragma once
#include "imgui.h"

namespace Engine::AneImGui
{
    struct AneImGuiContext // TODO replace pushing methods with one similar to ImGui
    {
        float NextTableFieldWidth;
        ImGuiTableFlags NextTableDecoratorFlags;
    };

    extern AneImGuiContext* _aneImGui;

    AneImGuiContext* Initialize();
    AneImGuiContext* GetContext();
    void Cleanup();

    void SetNextTableDecoratorFlags(ImGuiTableFlags decoratorFlags);
    void SetNextTableFieldWidth(float fieldWidth);

    float CalculateTableFieldWidth();

    bool LabelCheckbox(const char* label, bool* v);
    bool LabelRadioButton(const char* label, bool active);                    // use with e.g. if (RadioButton("one", my_value==1)) { my_value = 1; }
    bool LabelRadioButton(const char* label, int* v, int vButton);           // shortcut to handle the above pattern when value is an integer
    bool LabelCombo(const char* label, int* currentItem, const char* const items[], int itemsCount);

    void LabelText(const char* label, const char* value);

    bool LabelInputText(const char* label, char* buf, size_t bufSize, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* userData = nullptr);
    bool LabelInputTextMultiline(const char* label, char* buf, size_t bufSize, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* userData = nullptr);
    bool LabelInputTextWithHint(const char* label, const char* hint, char* buf, size_t bufSize, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* userData = nullptr);
    bool LabelInputTextEx(const char* label, const char* hint, char* buf, int bufSize, const ImVec2& sizeArg, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback = nullptr, void* userData = nullptr);
    bool LabelInputFloat(const char* label, float* v, float step = 0.0f, float stepFast = 0.0f, const char* format = "%.2f", ImGuiInputTextFlags flags = 0);
    bool LabelInputFloat2(const char* label, float v[2], const char* format = "%.2f", ImGuiInputTextFlags flags = 0);
    bool LabelInputFloat3(const char* label, float v[3], const char* format = "%.2f", ImGuiInputTextFlags flags = 0);
    bool LabelInputFloat4(const char* label, float v[4], const char* format = "%.2f", ImGuiInputTextFlags flags = 0);
    bool LabelInputInt(const char* label, int* v, int step = 1, int stepFast = 100, ImGuiInputTextFlags flags = 0);
    bool LabelInputInt2(const char* label, int v[2], ImGuiInputTextFlags flags = 0);
    bool LabelInputInt3(const char* label, int v[3], ImGuiInputTextFlags flags = 0);
    bool LabelInputInt4(const char* label, int v[4], ImGuiInputTextFlags flags = 0);
    bool LabelInputDouble(const char* label, double* v, double step = 0.0, double stepFast = 0.0, const char* format = "%.6f", ImGuiInputTextFlags flags = 0);
    bool LabelInputScalar(const char* label, ImGuiDataType dataType, void* pData, const void* pStep = nullptr, const void* pStepFast = nullptr, const char* format = nullptr, ImGuiInputTextFlags flags = 0);
    bool LabelInputScalarN(const char* label, ImGuiDataType dataType, void* pData, int components, const void* pStep = nullptr, const void* pStepFast = nullptr, const char* format = nullptr, ImGuiInputTextFlags flags = 0);

    bool LabelDragFloat(const char* label, float* v, float vSpeed = 1.0f, float vMin = 0.0f, float vMax = 0.0f, const char* format = "%.2f", ImGuiSliderFlags flags = 0);     // If v_min >= vMax we have no bound
    bool LabelDragFloat2(const char* label, float v[2], float vSpeed = 1.0f, float vMin = 0.0f, float vMax = 0.0f, const char* format = "%.2f", ImGuiSliderFlags flags = 0);
    bool LabelDragFloat3(const char* label, float v[3], float vSpeed = 1.0f, float vMin = 0.0f, float vMax = 0.0f, const char* format = "%.2f", ImGuiSliderFlags flags = 0);
    bool LabelDragFloat4(const char* label, float v[4], float vSpeed = 1.0f, float vMin = 0.0f, float vMax = 0.0f, const char* format = "%.2f", ImGuiSliderFlags flags = 0);
    bool LabelDragInt(const char* label, int* v, float vSpeed = 1.0f, int vMin = 0, int vMax = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);  // If v_min >= vMax we have no bound
    bool LabelDragInt2(const char* label, int v[2], float vSpeed = 1.0f, int vMin = 0, int vMax = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
    bool LabelDragInt3(const char* label, int v[3], float vSpeed = 1.0f, int vMin = 0, int vMax = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
    bool LabelDragInt4(const char* label, int v[4], float vSpeed = 1.0f, int vMin = 0, int vMax = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
    bool LabelDragScalar(const char* label, ImGuiDataType dataType, void* pData, float vSpeed = 1.0f, const void* pMin = nullptr, const void* pMax = nullptr, const char* format = nullptr, ImGuiSliderFlags flags = 0);
    bool LabelDragScalarN(const char* label, ImGuiDataType dataType, void* pData, int components, float vSpeed = 1.0f, const void* pMin = nullptr, const void* pMax = nullptr, const char* format = nullptr, ImGuiSliderFlags flags = 0);

    bool LabelSliderFloat(const char* label, float* v, float vMin, float vMax, const char* format = "%.2f", ImGuiSliderFlags flags = 0);
    bool LabelSliderFloat2(const char* label, float v[2], float vMin, float vMax, const char* format = "%.2f", ImGuiSliderFlags flags = 0);
    bool LabelSliderFloat3(const char* label, float v[3], float vMin, float vMax, const char* format = "%.2f", ImGuiSliderFlags flags = 0);
    bool LabelSliderFloat4(const char* label, float v[4], float vMin, float vMax, const char* format = "%.2f", ImGuiSliderFlags flags = 0);
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

    bool BeginLabelField(const char* label, const char* strId = "Table");
    void EndLabelField();
    void EndItem();
}

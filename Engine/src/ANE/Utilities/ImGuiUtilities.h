#pragma once

struct ImGuiStyle;

namespace Engine
{
    class ImGuiUtilities
    {
    public:
        static void StyleAnemoneDark(ImGuiStyle* dst = nullptr);
        static void StyleAnemoneExperimental();

        // Flags for ImGui::DockSpace()
        enum ImGuiDockNodeFlags
        {
            ImGuiDockNodeFlags_None = 0,
            ImGuiDockNodeFlags_KeepAliveOnly = 1 << 0, // Don't display the dockspace node but keep it alive. Windows docked into this dockspace node won't be undocked.
            //ImGuiDockNodeFlags_NoCentralNode = 1 << 1,   // Disable Central Node (the node which can stay empty)
            ImGuiDockNodeFlags_NoDockingInCentralNode = 1 << 2, // Disable docking inside the Central Node, which will be always kept empty. Note: when turned off, existing docked nodes will be preserved.
            ImGuiDockNodeFlags_NoSplit = 1 << 3, // Disable splitting the node into smaller nodes. Useful e.g. when embedding dockspaces into a main root one (the root one may have splitting disabled to reduce confusion). Note: when turned off, existing splits will be preserved.
            ImGuiDockNodeFlags_NoResize = 1 << 4, // Disable resizing child nodes using the splitter/separators. Useful with programatically setup dockspaces.
            ImGuiDockNodeFlags_PassthruCentralNode = 1 << 5, // Enable passthru dockspace: 1) DockSpace() will render a ImGuiCol_WindowBg background covering everything excepted the Central Node when empty. Meaning the host window should probably use SetNextWindowBgAlpha(0.0f) prior to Begin() when using this. 2) When Central Node is empty: let inputs pass-through + won't display a DockingEmptyBg background. See demo for details.
            ImGuiDockNodeFlags_AutoHideTabBar = 1 << 6 // Tab bar will automatically hide when there is a single window in the dock node.
        };

        // Flags for ImGui::BeginTabBar()
        enum ImGuiTabBarFlags
        {
            ImGuiTabBarFlags_None = 0,
            ImGuiTabBarFlags_Reorderable = 1 << 0, // Allow manually dragging tabs to re-order them + New tabs are appended at the end of list
            ImGuiTabBarFlags_AutoSelectNewTabs = 1 << 1, // Automatically select new tabs when they appear
            ImGuiTabBarFlags_NoCloseWithMiddleMouseButton = 1 << 2, // Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button. You can still repro this behavior on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
            ImGuiTabBarFlags_NoTabListPopupButton = 1 << 3,
            ImGuiTabBarFlags_NoTabListScrollingButtons = 1 << 4,
            ImGuiTabBarFlags_FittingPolicyResizeDown = 1 << 5, // Resize tabs when they don't fit
            ImGuiTabBarFlags_FittingPolicyScroll = 1 << 6, // Add scroll buttons when tabs don't fit
            ImGuiTabBarFlags_FittingPolicyMask_ = ImGuiTabBarFlags_FittingPolicyResizeDown | ImGuiTabBarFlags_FittingPolicyScroll,
            ImGuiTabBarFlags_FittingPolicyDefault_ = ImGuiTabBarFlags_FittingPolicyResizeDown
        };

        // Flags for ImGui::BeginTabItem()
        enum ImGuiTabItemFlags
        {
            ImGuiTabItemFlags_None = 0,
            ImGuiTabItemFlags_UnsavedDocument = 1 << 0, // Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator. Also: tab is selected on closure and closure is deferred by one frame to allow code to undo it without flicker.
            ImGuiTabItemFlags_SetSelected = 1 << 1, // Trigger flag to programatically make the tab selected when calling BeginTabItem()
            ImGuiTabItemFlags_NoCloseWithMiddleMouseButton = 1 << 2, // Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button. You can still repro this behavior on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
            ImGuiTabItemFlags_NoPushId = 1 << 3 // Don't call PushID(tab->ID)/PopID() on BeginTabItem()/EndTabItem()
        };
    };
}

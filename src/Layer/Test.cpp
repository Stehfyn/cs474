#include "..\cs474.pch.h"
#include "Test.h"
#include "..\Widgets\Markdown.h"
namespace cs474 {
Test::Test() {
}
Test::~Test() {
}
void Test::OnAttach() {
}
void Test::OnDetach() {
}
void Test::OnUpdate(float ts) {
}
void Test::OnUIRender() {
    DoMarkdownTest();
}
void Test::DoMarkdownTest() {
    ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName("Dear ImGui Demo"));
    /*
    ImGui::Begin("TweenWindow");
    static float settingsButtonWidth = 52.0f;
    static float rectwidth = 65.f;
    static float closeButtonWidth = 51.0f;
    ImVec2 size1 = { settingsButtonWidth, 100. };
    ImVec2 size2 = { closeButtonWidth, 100. };
    ImGui::Button("settingsButton", size1);
    bool settingsButtonIsHovered = ImGui::IsItemHovered();
    ImGui::Button("closeButton", size2);
    bool closeButtonIsHovered = ImGui::IsItemHovered();
    if (ImGui::IsWindowFocused())
    {
        ImTween<float>::Tween(
            std::tuple { 50.0f, 100.0f, & settingsButtonWidth },
            std::tuple { 50.0f, 100.0f, & closeButtonWidth })
            .OfType(ImTween<float>::TweenType::PingPong)
            .Speed(ImGui::GetIO().DeltaTime * 8.0f)
            .When(
                [&]()
                {
                    return settingsButtonIsHovered || closeButtonIsHovered;
                })
            .OnComplete( //Optional
                [&]()
                {
                    // Do something when tween completes
                }).Tick();
    }

        ImGui::Text("close: %f", closeButtonWidth);
        ImGui::Text("settings: %f", settingsButtonWidth);
        ImGui::End();
        */
}
}
// https://github.com/pthom/imgui_bundle/tree/main/external/imgui_md/imgui_md_wrapper
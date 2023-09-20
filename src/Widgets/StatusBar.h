#pragma once
namespace cs474 {
namespace widgets {
static void DrawStatusBar() {
    ImGuiWindowFlags window_flags = utils::GetHorizontalBarFlags();
    float height = ImGui::GetFrameHeight();

    if (ImGui::BeginViewportSideBar("##MainStatusBar", NULL, ImGuiDir_Down, height, window_flags)) {
        if (ImGui::BeginMenuBar()) {
            float frame_time_ms = global::GetResourceUnwrapped("g_FrameTimeInMilliSeconds");
            float fps = global::GetResourceUnwrapped("g_FramesPerSecond");

            ImGui::Text("%.3f ms/frame (%.1f FPS)", frame_time_ms, fps);
            ImGui::EndMenuBar();
        }
    }
    ImGui::End();
}
}
}
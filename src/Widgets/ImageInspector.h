#pragma once
namespace cs474 {
namespace widgets {
static bool ImageInspector(const char* id, const cs474::graphics::Texture& img, bool* inspect = (bool*)0, 
    ImVec2 inspect_offset = {0, 0}, ImVec2 not_hovered_inspect_offset = { 0, 0 }) {
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);

    ImVec2 size{ (float)img->GetWidth(), (float)img->GetHeight() };

    ImGui::Image((void*)(intptr_t)(img->GetRendererID()), size);
    auto& io = ImGui::GetIO();

    // This is_hovered replacement works ... why does IsHovered() not?
    ImRect img_rect({ pos.x, pos.y }, {pos.x + size.x, pos.y + size.y});
    bool is_hovered = img_rect.Contains(io.MousePos) && ImGui::IsWindowFocused() && !io.MouseDown[ImGuiMouseButton_Left];

    if ((*inspect || is_hovered))
    {
        ImVec2 offset{ 0,0 };
        if (is_hovered) {
            offset = inspect_offset;
        }
        else {
            offset = not_hovered_inspect_offset;
        }
        
        ImGui::SetNextWindowPos({ offset.x + io.MousePos.x, offset.y + io.MousePos.y });
        ImGui::Begin(id, NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
        float my_tex_w = size.x;
        //float my_tex_w = img->GetWidth();
        float my_tex_h = size.y;
        //float my_tex_h = img->GetHeight();

        float region_sz = 32.0f;
        float region_x = offset.x + io.MousePos.x - pos.x - region_sz * 0.5f;
        float region_y = offset.y + io.MousePos.y - pos.y - region_sz * 0.5f;
        
        float zoom = 5.0f;

        if (region_x < 0.0f) { region_x = 0.0f; }
        else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
        if (region_y < 0.0f) { region_y = 0.0f; }
        else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
        ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
        ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
        ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
        ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
        ImGui::Image((void*)(intptr_t)img->GetRendererID(), ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
        ImGui::End();

        *inspect = true;
    }
    return is_hovered;
}
}
}
#include "..\cs474.pch.h"
#include "Base.h"

#define STB_IMAGE_IMPLEMENTATION
#include "..\stb_image.h"
namespace cs474 {
Base::Base() {
}
Base::~Base() {
}
void Base::OnAttach() {
    LoadImages();
}
void Base::OnDetach() {
}
void Base::OnUpdate(float ts) {
    global::UpdateResource("g_FrameTimeInMilliSeconds", 1000.0f * ts);
    global::UpdateResource("g_FramesPerSecond", 1.0f / ts);
}
void Base::OnUIRender() {
    DoMainMenuBar();
    widgets::DrawStatusBar();

    if (global::GetResourceUnwrapped("g_FirstFrame")) {

        // 2. We want our whole dock node to be positioned in the center of the window, so we'll need to calculate that first.
        // The "work area" is the space inside the platform window created by GLFW, SDL, etc. minus the main menu bar if present.
        ImRect rect = ((ImGuiViewportP*)(void*)ImGui::GetMainViewport())->GetBuildWorkRect();   // The coordinates of the top-left corner of the work area
        ImVec2 pos = ((ImGuiViewportP*)(void*)ImGui::GetMainViewport())->CalcWorkRectPos(((ImGuiViewportP*)(void*)ImGui::GetMainViewport())->BuildWorkOffsetMin);
        ImVec2 size{ rect.Max.x - rect.Min.x, rect.Max.y - rect.Min.y };
        // v1.81 (found by git blame) adds a new function GetWorkCenter() which does these same calculations, so for any code using a newer version:
        //
        // if (firstLoop) {
        //     ImVec2 workCenter = ImGui::GetMainViewport()->GetWorkCenter();

        // 3. Now we'll need to create our dock node:
        ImGuiID id = ImGui::GetID("DockSpace"); // The string chosen here is arbitrary (it just gives us something to work with)
        ImGui::DockBuilderRemoveNode(id);             // Clear any preexisting layouts associated with the ID we just chose
        ImGui::DockBuilderAddNode(id, ImGuiDockNodeFlags_NoUndocking);                // Create a new dock node to use

        // Calculate the position of the dock node
        //
        // `DockBuilderSetNodePos()` will position the top-left corner of the node to the coordinate given.
        // This means that the node itself won't actually be in the center of the window; its top-left corner will.
        //
        // To fix this, we'll need to subtract half the node size from both the X and Y dimensions to move it left and up.
        // This new coordinate will be the position of the node's top-left corner that will center the node in the window.

        //ImVec2 workCenter = ImGui::GetMainViewport()->GetWorkCenter();
        auto& style = ImGui::GetStyle();
        ImVec2 nodePos{ 0.f, 5 * style.FramePadding.y };

        // Set the size and position:
        ImGui::DockBuilderSetNodeSize(id, size);
        ImGui::DockBuilderSetNodePos(id, pos);

        // 5. Split the dock node to create spaces to put our windows in:

        // Split the dock node in the left direction to create our first docking space. This will be on the left side of the node.
        // (The 0.5f means that the new space will take up 50% of its parent - the dock node.)
        ImGuiID dock1 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.75f, nullptr, &id);
        // +-----------+
        // |           |
        // |     1     |
        // |           |
        // +-----------+

        // Split the same dock node in the right direction to create our second docking space.
        // At this point, the dock node has two spaces, one on the left and one on the right.
        ImGuiID dock2 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.25f, nullptr, &id);
        // +-----+-----+
        // |     |     |
        // |  1  |  2  |
        // |     |     |
        // +-----+-----+
        //    split ->

        // For our last docking space, we want it to be under the second one but not under the first.
        // Split the second space in the down direction so that we now have an additional space under it.
        //
        // Notice how "dock2" is now passed rather than "id".
        // The new space takes up 50% of the second space rather than 50% of the original dock node.
        //ImGuiID dock3 = ImGui::DockBuilderSplitNode(dock2, ImGuiDir_Down, 0.5f, nullptr, &dock2);
        // +-----+-----+
        // |     |  2  |  split
        // |  1  +-----+    |
        // |     |  3  |    V
        // +-----+-----+

        // 6. Add windows to each docking space:
        ImGui::DockBuilderDockWindow("Image", dock1);
        ImGui::DockBuilderDockWindow("Hello from Layer::Base!", dock2);
        //ImGui::DockBuilderDockWindow("Three", dock3);

        // 7. We're done setting up our docking configuration:
        ImGui::DockBuilderFinish(id);
    }
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove;
    static bool open = true;
	ImGui::Begin("Hello from Layer::Base!", &open, window_flags);

    auto cd = std::filesystem::current_path();
    ImGui::Text("Current Path: {%s}", cd.generic_string().c_str());

    const FileRegistry& files = global::GetResourceUnwrapped("g_ImageFiles");
    ImGui::Text("File Count: {%zu}", files.size());

    int i = 0;
    for (const auto& entry : files) {
        ImGui::RadioButton(entry.generic_string().c_str(), &selected, i++);
    }

    const graphics::TextureRegistry& images = global::GetResourceUnwrapped("g_Images");
    if (images.size() > 0) {
        const auto& img = images.at(selected);
        ImGui::Text("Size: {%zu}x{%zu}", img->GetWidth(), img->GetHeight());
    }

	ImGui::End();

    ImGui::Begin("Image", &open, window_flags);
    {
        if (images.size() > 0) {
            //ImVec2 size(ImGui::GetContentRegionAvail());
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
            ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);

            const auto& img = images.at(selected);
            ImVec2 size{ (float)img->GetWidth(), (float)img->GetHeight() };

            ImGui::Image((void*)(intptr_t)(img->GetRendererID()), size);
            if (ImGui::IsItemHovered() && ImGui::BeginTooltip())
            {
                auto& io = ImGui::GetIO();
                float my_tex_w = size.x;
                //float my_tex_w = img->GetWidth();
                float my_tex_h = size.y;
                //float my_tex_h = img->GetHeight();

                float region_sz = 32.0f;
                float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
                float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
                float zoom = 4.0f;
                if (region_x < 0.0f) { region_x = 0.0f; }
                else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
                if (region_y < 0.0f) { region_y = 0.0f; }
                else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
                ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
                ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
                ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
                ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
                ImGui::Image((void*)(intptr_t)img->GetRendererID(), ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
                ImGui::EndTooltip();
            }
        }
    }
    ImGui::End();

    
}

void Base::DoMainMenuBar() {
    auto& style = ImGui::GetStyle();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, 5 * style.FramePadding.y));
    ImGui::BeginMainMenuBar();
    ImGui::PopStyleVar();
    if (ImGui::BeginMenu("File")) {
        ImGui::MenuItem("New");
        ImGui::MenuItem("Create");
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
        ImGui::MenuItem("New");
        ImGui::MenuItem("Create");
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View")) {
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Project")) {
        ImGui::MenuItem("New");
        ImGui::MenuItem("Create");
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Window")) {
        ImGui::MenuItem("New");
        ImGui::MenuItem("Create");
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help")) {
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}

void Base::LoadImages() {
    FileRegistry image_files = utils::Traverse("/assets", "*.gif *.jpg *.png");

    graphics::TextureRegistry images;
    for (const auto& file : image_files) {
        int image_width = 0;
        int image_height = 0;

        const auto& data = utils::SlurpFile(file);
        if (data.has_value()) {
            unsigned char* image_data = stbi_load_from_memory((unsigned char*)data.value().data(), data.value().size(), &image_width, &image_height, NULL, 4);

            if (image_data == NULL) {
                emscripten_log(EM_LOG_CONSOLE, "didnt work lol");
            }

            images.emplace_back(graphics::make_texture(image_data, image_width, image_height, 4));
            stbi_image_free(image_data);
        }
    }

    global::AddResource("g_Images", std::move(images));
    global::AddResource("g_ImageFiles", std::move(image_files));
}
}
#include "..\cs474.pch.h"
#include "Base.h"
#include "..\Widgets\Markdown.h"
namespace cs474 {
Base::Base() {
}
Base::~Base() {
}
void Base::OnAttach() {
    this->LoadImages();
}
void Base::OnDetach() {
}
void Base::OnUpdate(float ts) {
    global::UpdateResource("g_FrameTimeInMilliSeconds", 1000.0f * ts);
    global::UpdateResource("g_FramesPerSecond", 1.0f / ts);
}
void Base::OnUIRender() {
    //this->DoMainMenuBar();

    if (global::GetResourceUnwrapped("g_FirstFrame")) {
    //if (true) {
        this->BuildDockspace();
    }
    
    this->DoTableOfContents();
    this->DoLandingPage();
    this->DoImageTab();
    this->DoWidgetPanel();

    if (global::GetResourceUnwrapped("g_FirstFrame")) {
        ImGui::SetWindowFocus("LandingPage");
    }

    widgets::DrawStatusBar();
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
void Base::DoTableOfContents() {
    static bool open = true;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;
    
    static ImGuiDockNodeFlags dnf = utils::GetNoDockingPanelDockNodeFlags();
    ImGuiWindowClass wc;
    wc.DockNodeFlagsOverrideSet = dnf | ImGuiDockNodeFlags_NoResize;
    ImGui::SetNextWindowClass(&wc);
    ImGui::Begin("TableOfContents", &open, window_flags);
    {
        widgets::markdown("# Table of Contents");

        if (global::GetResourceUnwrapped("g_FirstFrame")) {
            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        }

        if (ImGui::CollapsingHeader("Assignments")) {
            widgets::markdown(R"(## Assignment 1
&nbsp; 1. [Image Sampling](#example)<br>
&nbsp; 2. [Image Quantization](#example)<br>
&nbsp; 3. [Histogram Equalization](#example)<br>
&nbsp; 4. [Histogram Specification](#example)<br>
)");
        }
    }
    ImGui::End();
}

void Base::DoLandingPage() {

    static bool open = true;
    ImGuiWindowFlags window_flags = utils::GetBaseLayerWindowFlags();
    ImGuiWindowClass wc;
    wc.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoResize;
    ImGui::SetNextWindowClass(&wc);
    
    ImGui::Begin("LandingPage", &open, window_flags);
    widgets::markdown(R"(# CS474 - Image Processing and Interpretation
A WebAssembly/Native Windows application for testing and visualizing Image Processing and Interpretation theory.

Stephen Foster: [@Stehfyn](https://github.com/Stehfyn)<br>
Joey Paschke [@DuckieOnQuacks](https://github.com/DuckieOnQuacks)
## Live Version
 A live version can be found at: [stehfyn.github.io/cs474/](https://stehfyn.github.io/cs474/)
## How to build
Build support is provided for `Visual Studio 2022`. The build system dependencies are as follows:
- `Visual Studio 2022`
- [`VSExtForEmscripten`](https://github.com/nokotan/VSExtForEmscripten/) v0.8.0 or higher.
- The specific extension is `Emscripten.Build.Definition.vsix`, and can be found [here](https://github.com/nokotan/VSExtForEmscripten/releases/download/v0.8.0/Emscripten.Build.Definition.vsix).

### Build Steps
*Note* - It is recommended to first install [`Emscripten.Build.Definition.vsix`](https://github.com/nokotan/VSExtForEmscripten/releases/download/v0.8.0/Emscripten.Build.Definition.vsix) manually, as opposed to using `setup.bat` which may take awhile.

First, you can clone the repository and its submodule:
```bash
git clone --recursive https://github.com/Stehfyn/cs474
```
Or do the following if you cloned the repo non-recursively already:
```bash
cd cs474
git submodule update --init --recursive
```
Then, if you have already installed `Emscripten.Build.Definition.vsix`, just
```
cd cs474
.\setup.bat --bypass
```
Or, if you wish to to have it automatically installed:
```
cd cs474
.\setup.bat
```
Finally, you may open `cs474.sln` with `Visual Studio 2022` and build for platform `Emscripten`.
Or, with the `Developer Command Prompt for VS`, just do:
```bash
cd cs474
msbuild /m /p:Configuration=Release /p:Platform=Emscripten .
```

### Locally Hosting the Build
The [`VSExtForEmscripten`](https://github.com/nokotan/VSExtForEmscripten/) provides support via the Visual Studio Debugger UI for testing builds on `localhost`, but this is not recommended as it is currently far from stable. A suitable alternative is just using `Python`'s http package:

To do so, first `cd` into either a Debug or Release configuration of a `cs474-client` build directory with a successful build (The build directory pattern is `bin\cs474-client\Emscripten\{Configuration}\`), then invoke the `Python` http.server:
```bash
cd bin\cs474-client\Emscripten\Release\
python3 -m http.server 8080
```
Then launch any browser and navigate to `localhost:8080/index.html`, and voila!
)");
    ImGui::End();
}

void Base::DoImageTab() {
    ImGuiWindowFlags window_flags = utils::GetBaseLayerWindowFlags();
    static bool open = true;
    ImGuiWindowClass wc;
    wc.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoResize;
    ImGui::SetNextWindowClass(&wc);
    ImGui::Begin("Image", &open, window_flags);
    {
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

void Base::DoWidgetPanel() {
    auto coolbar_button = [](const char* label) -> bool {
        float w = ImGui::GetCoolBarItemWidth();
        auto font_ptr = (ImFont*)global::GetResourceUnwrapped("g_IconFont");
        font_ptr->Scale = ImGui::GetCoolBarItemScale();
        ImGui::PushFont(font_ptr);
        bool res = ImGui::Button(label, ImVec2(w, w));
        //ImGui::Text(label);
        ImGui::PopFont();
        return res;
    };

    float widget_panel_width = 0.0f;
    if (ImGui::BeginCoolBar("##CoolBarMain", ImCoolBarFlags_Vertical, ImVec2(1.0f, 0.5f))) {
        ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
        widget_panel_width = 40.0f;
        if (ImGui::CoolBarItem()) {
            widget_panel_width = 60.0f;

            if (coolbar_button(ICON_FA_FOLDER_OPEN)) {

            }
        }
        if (ImGui::CoolBarItem()) {
            widget_panel_width = 60.0f;

            if (coolbar_button(ICON_FA_IMAGES)) {

            }
        }
        if (ImGui::CoolBarItem()) {
            widget_panel_width = 60.0f;

            if (coolbar_button(ICON_FA_CHART_SIMPLE)) {

            }
        }
        if (ImGui::CoolBarItem()) {
            widget_panel_width = 60.0f;

            if (coolbar_button(ICON_FA_GEAR)) {

            }
        }
        if (ImGui::CoolBarItem()) {
            widget_panel_width = 60.0f;

            if (coolbar_button(ICON_FA_SQUARE_XMARK)) {

            }
        }
        
        
        ImGui::EndCoolBar();
    }
    ImGuiID id = global::GetResourceUnwrapped("g_WidgetDock");
    ImVec2 new_size = { widget_panel_width, ImGui::DockBuilderGetNode(id)->Size.y };
    ImGui::DockBuilderSetNodeSize(id, new_size);

    ImGuiWindowFlags window_flags = utils::GetBaseLayerWindowFlags();
    ImGuiWindowClass wc;
    wc.DockNodeFlagsOverrideSet = utils::GetNoDockingPanelDockNodeFlags() | ImGuiDockNodeFlags_NoResize;
    ImGui::SetNextWindowClass(&wc);
    static bool open = true;
    ImVec2 zero_padded = {new_size.x * 2, new_size.y * 2};
    ImGui::SetNextWindowSize(zero_padded);
    ImGui::Begin("CoolBarPanel", &open, window_flags);
    ImGui::End();
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
            else {
                images.emplace_back(graphics::make_texture(image_data, image_width, image_height, 4));
                stbi_image_free(image_data);
            }
        }
    }

    global::AddResource("g_Images", std::move(images));
    global::AddResource("g_ImageFiles", std::move(image_files));
}
void Base::BuildDockspace() {
    ImRect rect = ((ImGuiViewportP*)(void*)ImGui::GetMainViewport())->GetBuildWorkRect();
    ImVec2 pos = ((ImGuiViewportP*)(void*)ImGui::GetMainViewport())->CalcWorkRectPos(((ImGuiViewportP*)(void*)ImGui::GetMainViewport())->BuildWorkOffsetMin);
    ImVec2 size{ (rect.Max.x - rect.Min.x) + 10.0f, rect.Max.y - rect.Min.y };

    ImGuiID id = ImGui::GetID("DockSpace");
    ImGui::DockBuilderRemoveNode(id);
    ImGui::DockBuilderAddNode(id, ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoResize);

    auto& style = ImGui::GetStyle();
    ImVec2 nodePos{ 0.f, 5 * style.FramePadding.y };

    // Set the size and position:
    ImGui::DockBuilderSetNodeSize(id, size);
    ImGui::DockBuilderSetNodePos(id, pos);

    ImGuiID dock3 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.10f , nullptr, &id);
    ImGuiID dock1 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.15f, nullptr, &id);
    ImGuiID dock2 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.75f, nullptr, &id);

    ImGui::DockBuilderDockWindow("LandingPage", dock2);
    ImGui::DockBuilderDockWindow("Dear ImGui Demo", dock2);
    ImGui::DockBuilderDockWindow("Image", dock2);
    ImGui::DockBuilderDockWindow("TableOfContents", dock1);
    ImGui::DockBuilderDockWindow("CoolBarPanel", dock3);

    global::AddResource("g_WidgetDock", dock3);
    ImGui::DockBuilderFinish(id);
}
}
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
    global::AddResource("g_ShowAssignment1", false);
    utils::fetch_bytes("readme", "https://raw.githubusercontent.com/stehfyn/cs474/main/README.md");
}
void Base::OnDetach() {
}
void Base::OnUpdate(float ts) {
    global::UpdateResource("g_FrameTimeInMilliSeconds", 1000.0f * ts);
    global::UpdateResource("g_FramesPerSecond", 1.0f / ts);
}
void Base::OnUIRender() {
    widgets::DrawStatusBar();

    if (global::GetResourceUnwrapped("g_FirstFrame")) {
        this->BuildDockspace();
    }
    
    this->DoTableOfContents();
    this->DoLandingPage();
    //this->DoWidgetPanel();

    if (global::GetResourceUnwrapped("g_FirstFrame")) {
        ImGui::SetWindowFocus("LandingPage");
    }

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

        static std::vector<std::string> headers = { "Image Sampling", "Image Quantization", "Histogram Equalization", "Histogram Specification" };
        if (ImGui::TreeNode("Programming Assignments")) {
            if (global::GetResourceUnwrapped("g_FirstFrame")) {
                ImGui::SetNextItemOpen(true, ImGuiCond_Once);
            }
            if (ImGui::TreeNode("Assignment 1")) {
                static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
                static int selected = -1;
                for (int i = 0; i < 4; ++i) {
                    ImGuiTreeNodeFlags node_flags = base_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                    if (i == selected) node_flags |= ImGuiTreeNodeFlags_Selected;
                    ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "%d. %s", i + 1, (headers[i]).c_str());
                    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
                        selected = i;
                        global::UpdateResource("g_ShowAssignment1", true);
                        global::UpdateResource("g_Assignment1ScrollTo" + headers[i], true);
                        ImGui::FocusWindow(ImGui::FindWindowByName("Assignment 1"));
                    }
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
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

    if (!readme.size()) {
        std::optional<std::reference_wrapper<const global::Resource>> bytes = global::GetResource("readme");
        // Readme has been fetched
        if (bytes.has_value()) {
            const std::vector<uint8_t>& readme_bytes = bytes.value().get();
            readme = std::string(reinterpret_cast<const char*>(readme_bytes.data()), readme_bytes.size());
            emscripten_log(EM_LOG_CONSOLE, "loaded readme");
        }
    }
    else {
        widgets::markdown(readme.c_str());
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
    const utils::FileList files = utils::Traverse("/assets", "*.gif *.pgm");
    const std::unordered_map<std::filesystem::path, utils::FileList> stem_map = utils::SortByStem(files);

    std::shared_ptr<graphics::ImageRegistry> image_registry = std::make_shared<graphics::ImageRegistry>();

    // add list of image files to registry
    for (const auto& kv : stem_map) {
        emscripten_log(EM_LOG_CONSOLE, "Stem: %s {%zu}", kv.first.c_str(), kv.second.size());
        image_registry->AddFileList(kv.first, kv.second);
        for (const auto& path : kv.second) {
            emscripten_log(EM_LOG_CONSOLE, "- %s", path.c_str());
        }
    }
    
    image_registry->LoadDefaultTexturesFromFileList();
    global::AddResource("g_ImageRegistry", std::move(image_registry));

    utils::FileList image_files = utils::Traverse("/assets", "*.gif *.pgm");
    global::AddResource("g_ImageFiles", std::move(image_files));
}

void Base::BuildDockspace() {
    ImRect rect = ((ImGuiViewportP*)(void*)ImGui::GetMainViewport())->GetBuildWorkRect();
    ImVec2 pos = ((ImGuiViewportP*)(void*)ImGui::GetMainViewport())->CalcWorkRectPos(((ImGuiViewportP*)(void*)ImGui::GetMainViewport())->BuildWorkOffsetMin);
    ImVec2 size{ (rect.Max.x - rect.Min.x) + 10.0f, rect.Max.y - rect.Min.y };

    ImGuiID id = ImGui::GetID("DockSpace");

    //ImGui::DockBuilderRemoveNode(id);

    auto& style = ImGui::GetStyle();
    ImVec2 nodePos{ 0.f, 5 * style.FramePadding.y };

    id = global::GetResourceUnwrapped("g_DockspaceOverViewport");
    //id = ImGui::DockBuilderAddNode(id, ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_PassthruCentralNode ImGuiDockNodeFlags_);
    // Set the size and position:
    ImGui::DockBuilderSetNodeSize(id, size);
    ImGui::DockBuilderSetNodePos(id, pos);

    ImGuiID dock1 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.15f , nullptr, &id);
    //ImGuiID dock2 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 1.0f, nullptr, &id);
    //ImGuiID dock3 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.15f, nullptr, &id);

    ImGui::DockBuilderDockWindow("TableOfContents", dock1);
    ImGui::DockBuilderDockWindow("LandingPage", id);
    ImGui::DockBuilderDockWindow("Assignment 1", id);

    /*
    ImGui::DockBuilderDockWindow("Dear ImGui Demo", dock2);
    ImGui::DockBuilderDockWindow("Image", dock2);
    ImGui::DockBuilderDockWindow("TableOfContents", dock1);
    ImGui::DockBuilderDockWindow("CoolBarPanel", dock3);
    */
    //global::AddResource("g_WidgetDock", dock3);
    ImGui::DockBuilderFinish(id);
}
}
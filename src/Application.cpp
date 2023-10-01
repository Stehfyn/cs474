#include "cs474.pch.h"
#include "Application.h"
#include "Layer/Base.h"
#include "Layer/Assignment1.h"
#include "Layer/AssignmentTest1.h"
#include "Layer/AssignmentTest2.h"

namespace cs474 {
Application::Application(const ApplicationSpecification& spec)
    : m_Spec(spec) {
    InitializeGL();
    InitializeImGui();

    global::AddResource("g_FirstFrame", true);

    PushLayer<Base>();
    PushLayer<Assignment1>();

    PushLayer<AssignmentTest1>();
    PushLayer<AssignmentTest2>();
}

Application::~Application() {
    glfwTerminate();
}

void Application::Run() {
    StartNewAppFrame(); 
    {
        // Update Layers
        {
            for (auto& layer : m_LayerStack)
                layer->OnUpdate(ImGui::GetIO().DeltaTime);
        }

        // Dockspace
        {
            ImGuiID id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(),
                ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoResize);
            if (global::GetResourceUnwrapped("g_FirstFrame")) {
                global::AddResource("g_DockspaceOverViewport", id);
            }
        }

        {
            //ImGui::ShowDemoWindow();
        }

        // Draw Layers
        {
            for (auto& layer : m_LayerStack)
                layer->OnUIRender();            
        }
    }
    RenderAppFrame();

    if (global::GetResourceUnwrapped("g_FirstFrame")) {
        global::UpdateResource("g_FirstFrame", false);
    }
}

void Application::Cleanup() {
}

void Application::InitializeGL() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        std::exit(1);
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
    m_Window = make_app_window(m_Spec.Width, m_Spec.Height, "app", (GLFWmonitor*)NULL, (GLFWwindow*)NULL);

    if (m_Window.get() == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        std::exit(-1);
    }

    glfwMakeContextCurrent(m_Window.get());
}

void Application::InitializeImGui() {
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_Window.get(), true);
    ImGui_ImplOpenGL3_Init();

    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigWindowsResizeFromEdges = false;
    // Setup style
    //ImGui::StyleColorsLight();
    ImGui::StyleColorsDark();

    auto fonts = utils::Traverse("assets/fonts/text", "*.ttf");
    for (const auto& font : fonts) {
        utils::MakeFont(font, 18.0f);
        utils::MakeFont(font, 23.0f);
        utils::MakeFont(font, 26.0f);
        utils::MakeFont(font, 32.0f);
    }
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 9.0f;
    style.FrameRounding = 9.0f;
    style.ScrollbarRounding = 9.0f;
    style.TabRounding = 9.0f;

    float baseFontSize = 30.0f; // 13.0f is the size of the default font. Change to the font size you use.
    float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = iconFontSize;
    global::AddResource("g_IconFont", io.Fonts->AddFontFromFileTTF("assets/fonts/icon/" FONT_ICON_FILE_NAME_FAS, iconFontSize, &icons_config, icons_ranges));

    io.FontDefault = (ImFont*)global::GetResourceUnwrapped("g_MyriadPro-Light_18.0");
    utils::resizeCanvas();
}
void Application::StartNewAppFrame() {
#ifdef __EMSCRIPTEN__
    ResizeCanvasElement();
#endif

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void Application::RenderAppFrame() {
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::Render();

    int display_w, display_h;
    glfwMakeContextCurrent(m_Window.get());
    glfwGetFramebufferSize(m_Window.get(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwMakeContextCurrent(m_Window.get());
}
#ifdef __EMSCRIPTEN__
void Application::ResizeCanvasElement() {
    int width = utils::canvas_get_width();
    int height = utils::canvas_get_height();

    if (width != m_Spec.Width || height != m_Spec.Height) {
        m_Spec.Width = width;
        m_Spec.Height = height;
        glfwSetWindowSize(m_Window.get(), m_Spec.Width, m_Spec.Height);
        ImGui::SetCurrentContext(ImGui::GetCurrentContext());
    }
}
#endif
}

#include "cs474.pch.h"
#include "Application.h"
#include "Layer/Base.h"
#include "Layer/Test.h"

namespace cs474 {
Application::Application(const ApplicationSpecification& spec)
    : m_Spec(spec) {
    InitializeGL();
    InitializeImGui();

    PushLayer<Base>();
    PushLayer<Test>();
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
            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(),
                ImGuiDockNodeFlags_PassthruCentralNode);
        }

        // Draw Layers
        {
            for (auto& layer : m_LayerStack)
                layer->OnUIRender();
        }
    }
    RenderAppFrame();
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
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup style
    ImGui::StyleColorsDark();

    auto fonts = utils::Traverse("assets/", "*.ttf");
    for (const auto& font : fonts) {
        io.Fonts->AddFontFromFileTTF(font.c_str(), 23.0f);
        io.Fonts->AddFontFromFileTTF(font.c_str(), 18.0f);
        io.Fonts->AddFontFromFileTTF(font.c_str(), 26.0f);
        io.Fonts->AddFontFromFileTTF(font.c_str(), 32.0f);
    }

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

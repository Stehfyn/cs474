#pragma once
#include "Layer/Layer.h"

namespace cs474 {
struct ApplicationSpecification {
    std::string Name = "cs474";
    uint32_t Width = 1600;
    uint32_t Height = 900;
};

struct WindowDeleter {
    using pointer = GLFWwindow*;
    void operator()(GLFWwindow* window) {
        glfwDestroyWindow(window);
    }
};

using ApplicationWindow = std::unique_ptr<GLFWwindow, WindowDeleter>;

template <class... Args>
ApplicationWindow make_app_window(Args&&... args) {
    return ApplicationWindow{ glfwCreateWindow(std::forward<Args>(args)...) };
}

using AppLayerStack = std::vector<std::shared_ptr<cs474::Layer>>;

class Application {
public:
    Application(
        const ApplicationSpecification& applicationSpecification = ApplicationSpecification());
    ~Application();

public:
    void Run();
    void Cleanup();

public:
    template <typename T> void PushLayer() {
        static_assert(std::is_base_of<cs474::Layer, T>::value,
            "Pushed type is not subclass of Layer!");
        m_LayerStack.emplace_back(std::make_shared<T>())->OnAttach();
    }

private:
    void InitializeGL();
    void InitializeImGui();
    void StartNewAppFrame();
    void RenderAppFrame();

#ifdef __EMSCRIPTEN__
    void ResizeCanvasElement();
#endif

private:
    AppLayerStack m_LayerStack;
    ApplicationWindow m_Window;
    ApplicationSpecification m_Spec;
};
}
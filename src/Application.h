#pragma once
#include "Layer/Layer.h"

namespace cs474 {
struct ApplicationSpecification {
    std::string Name = "cs474";
    uint32_t Width = 1600;
    uint32_t Height = 900;
};

using ApplicationWindow = std::shared_ptr<GLFWwindow>;

struct WindowDeleter {
    void operator()(GLFWwindow* window) {
        glfwDestroyWindow(window);
    }
};

template <class... Args>
ApplicationWindow make_app_window(Args&&... args) {
    return ApplicationWindow(glfwCreateWindow(std::forward<Args>(args)...),
        WindowDeleter{});
}

class Application {
public:
    Application(
        const ApplicationSpecification& applicationSpecification = ApplicationSpecification());
    ~Application();
    void Run();
private:
    void InitializeGL();
    void InitializeImGui();

private:
    ApplicationWindow m_Window;
   // GLFWwindow* m_Window;
    ApplicationSpecification m_Spec;
};
}
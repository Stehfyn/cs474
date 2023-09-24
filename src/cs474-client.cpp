#include "cs474.pch.h"
#include "Application.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
cs474::Application* g_Instance = nullptr;
namespace cs474 {
    int main(int argc, char** argv) {
        auto app = g_Instance = new Application();
        global::AddResource("g_ApplicationRunning", true);

#ifdef __EMSCRIPTEN__
        auto loop = []() {
            auto g_ApplicationRunning = global::GetResourceUnwrapped("g_ApplicationRunning");
            if (g_ApplicationRunning) {
                g_Instance->Run();
            }
            else {
                emscripten_cancel_main_loop();
            }
        };
        emscripten_set_main_loop(loop, 0, 1);
#else
        while (global::GetResourceUnwrapped("g_ApplicationRunning")) {
            app->Run();
        }
#endif
        app->Cleanup();
        delete app;

        return 0;
    }
} // namespace cs474

#if _MSC_VER && !__INTEL_COMPILER // https://stackoverflow.com/a/5850405/22468901
#include <Windows.h>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    return cs474::main(__argc, __argv);
}
#endif // MSVC (Windows Native)

#ifdef __EMSCRIPTEN__
extern "C" int main(int argc, char** argv) {
    return cs474::main(argc, argv);
}
#endif
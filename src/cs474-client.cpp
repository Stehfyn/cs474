#include "cs474.pch.h"
#include "Application.h"

bool g_ApplicationRunning = true;
void* g_Instance = nullptr;

namespace cs474 {
    int main(int argc, char** argv) {
        cs474::Application* app = new cs474::Application();

#ifdef __EMSCRIPTEN__
        auto loop = []() {
            if (g_ApplicationRunning) {
                ((cs474::Application*)g_Instance)->Run();
            }
        };

        emscripten_set_main_loop(loop, 0, 1);
#else
        while (g_ApplicationRunning) {
            app->Run();
        }
#endif
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

    return mcctp::main(__argc, __argv);
}
#endif // MSVC (Windows Native)

#ifdef __EMSCRIPTEN__
extern "C" int main(int argc, char** argv) {
    return cs474::main(argc, argv);
}
#endif
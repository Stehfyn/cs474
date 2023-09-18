#include "..\cs474.pch.h"
namespace cs474 {
namespace utils {
EM_JS(int, canvas_get_width, (), {
    return Module.canvas.width;
    });
EM_JS(int, canvas_get_height, (), {
    return Module.canvas.height;
    });
EM_JS(void, resizeCanvas, (), {
    js_resizeCanvas();
    });
}
}
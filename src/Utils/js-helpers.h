#pragma once
#ifdef __EMSCRIPTEN__
namespace cs474 {
namespace utils {
extern "C" {
	int canvas_get_width(void);
	int canvas_get_height(void);
	void resizeCanvas(void);
}
}
}
#endif

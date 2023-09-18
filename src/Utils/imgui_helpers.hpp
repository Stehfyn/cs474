#pragma once
namespace cs474 {
namespace utils {
static ImGuiWindowFlags GetHorizontalBarFlags(void) {
	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
	return window_flags;
}
}
}
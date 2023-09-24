#pragma once
namespace cs474 {
namespace utils {
static ImGuiWindowFlags GetHorizontalBarFlags(void) {
	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
	return window_flags;
}
static ImGuiWindowFlags GetBaseLayerWindowFlags(void) {
	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize;
	return window_flags;
}
static ImGuiDockNodeFlags GetNoDockingPanelDockNodeFlags(void) {
	return ImGuiDockNodeFlags_NoDockingOverOther
		| ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoSplit | ImGuiDockNodeFlags_NoTabBar
		| ImGuiDockNodeFlags_NoUndocking;
}
static void MakeFont(const std::filesystem::path& font_file, float font_size, int precision = 1) {
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImFontConfig config;
	
	std::string key = "g_" + std::string(font_file.stem()) + "_" + 
		utils::ToStringWithPrecision(font_size, precision);
	emscripten_log(EM_LOG_CONSOLE, key.c_str());
	global::AddResource(key, io.Fonts->AddFontFromFileTTF(font_file.c_str(), font_size));
}
}
}
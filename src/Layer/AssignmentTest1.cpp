#include "..\cs474.pch.h"
#include "AssignmentTest1.h"
namespace cs474 {
AssignmentTest1::AssignmentTest1() {
}
AssignmentTest1::~AssignmentTest1() {
}

void AssignmentTest1::OnAttach() {
	this->LoadImages();
}

void AssignmentTest1::OnDetach() {
}

void AssignmentTest1::OnUpdate(float ts) {
}

void AssignmentTest1::OnUIRender() {
	ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName("AssignmentTest1"));

	ImVec2 size = { 640, 480 };
	ImGui::SetNextWindowSize(size);

	ImVec2 pos = { 200, 400 };
	ImGui::SetNextWindowPos(pos, ImGuiCond_Once);

	ImGui::Begin("AssignmentTest1");
	
	const auto& img = images.at(images.size() - 1);
	ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
	ImGui::Image((void*)(intptr_t)(img->GetRendererID()), img_size);
	ImGui::SameLine();
	
	// Computed image
	ImGui::Image((void*)(intptr_t)(img->GetRendererID()), img_size);
	
	
	ImGui::Text("Original: ");
	ImGui::SameLine();
	
	//ImGui::Dummy(,)
	const auto& style = ImGui::GetStyle();

	float x_offset = 2 * style.ItemSpacing.x + ((float)img->GetWidth() - ImGui::GetCursorPosX());
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
	ImGui::Text("Processed: ");


	ImGui::Separator();

	if (ImGui::Button("Sub sample")) {
		// do sub samplig calculation
		// make the texture like how we did in loadimage()
	}

	// ImGui::ComboBox

	ImGui::End();
}
void AssignmentTest1::LoadImages() {
	int image_width = 0;
	int image_height = 0;
	int num_channels = 0;
	int desired_channels = 1;

	std::string lenna = "/assets/images/raw/lenna.gif";
	
	const auto& data = utils::SlurpFile(lenna);
	if (data.has_value()) {
		unsigned char* image_data = stbi_load_from_memory((unsigned char*)data.value().data(), data.value().size(), &image_width, &image_height, &num_channels, 4);

		if (image_data == NULL) {
			emscripten_log(EM_LOG_CONSOLE, "couldn't load %s", lenna.c_str());
		}
		else {
			images.emplace_back(graphics::make_texture(image_data, image_width, image_height, 4));
			stbi_image_free(image_data);
			emscripten_log(EM_LOG_CONSOLE, "{AssignmentTest1}: Successfully loaded: %s Channels: %d", lenna.c_str(), num_channels);
		}
	}

	/*
	std::string aerial = "/assets/images/raw/aerial.gif";

	const auto& data1 = utils::SlurpFile(aerial);
	if (data1.has_value()) {
		unsigned char* image_data = stbi_load_from_memory((unsigned char*)data1.value().data(), data1.value().size(), &image_width, &image_height, &num_channels, desired_channels);

		if (image_data == NULL) {
			emscripten_log(EM_LOG_CONSOLE, "couldn't load %s", aerial.c_str());
		}
		else {
			images.emplace_back(graphics::make_texture(image_data, image_width, image_height, 4));
			stbi_image_free(image_data);
			emscripten_log(EM_LOG_CONSOLE, "{AssignmentTest1}: Successfully loaded: %s", aerial.c_str());
		}
	}
	*/
}
}
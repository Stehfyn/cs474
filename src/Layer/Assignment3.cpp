#include "..\cs474.pch.h"
#include "Assignment3.h"
namespace cs474 {
Assignment3::Assignment3() {
}
Assignment3::~Assignment3() {
}
void Assignment3::OnAttach() {
	global::UpdateResource("g_Assignment3ScrollToExperiment 1", false);
	global::UpdateResource("g_Assignment3ScrollToExperiment 2", false);
	global::UpdateResource("g_Assignment3ScrollToExperiment 3", false);

	this->GenerateSubject2a();
	this->GenerateSubject2b();
	this->GenerateSubject2c();
}
void Assignment3::OnDetach() {
}
void Assignment3::OnUpdate(float ts) {
}
void Assignment3::OnUIRender() {
	if (global::GetResourceUnwrapped("g_ShowAssignment3")) {
		bool open = true;
		ImGuiWindowFlags window_flags = utils::GetBaseLayerWindowFlags();

		ImGui::Begin("Assignment 3", &open);
		{
			this->Experiment1();
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::NewLine();

			this->Experiment2();
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::NewLine();

			this->Experiment3();
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::NewLine();
		}
		ImGui::End();

		if (!open) global::UpdateResource("g_ShowAssignment3", false);
	}
}
void Assignment3::Experiment1() {
	if (global::GetResourceUnwrapped("g_Assignment3ScrollToExperiment 1")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment3ScrollToExperiment 1", false);
	}
	widgets::markdown("# Experiment 1");
}
void Assignment3::Experiment2() {
	if (global::GetResourceUnwrapped("g_Assignment3ScrollToExperiment 2")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment3ScrollToExperiment 2", false);
	}
	widgets::markdown("# Experiment 2");
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& twoA_opt = image_registry->GetTexture("lenna", "subject2a");
	const std::optional<graphics::Texture>& twoB_opt = image_registry->GetTexture("lenna", "subject2b");
	const std::optional<graphics::Texture>& twoC_opt = image_registry->GetTexture("lenna", "subject2c");
	if (twoA_opt.has_value() && twoB_opt.has_value() && twoC_opt.has_value()) {
		widgets::markdown("# 2.a");
		ImGui::Image((ImTextureID)twoA_opt.value()->GetRendererID(), ImVec2(512, 512));
		widgets::markdown("# 2.b");
		ImGui::Image((ImTextureID)twoB_opt.value()->GetRendererID(), ImVec2(512, 512));
		widgets::markdown("# 2.c");
		ImGui::Image((ImTextureID)twoC_opt.value()->GetRendererID(), ImVec2(512, 512));
	}
}
void Assignment3::Experiment3() {
	if (global::GetResourceUnwrapped("g_Assignment3ScrollToExperiment 3")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment3ScrollToExperiment 3", false);
	}
	widgets::markdown("# Experiment 3");
}
std::vector<uint8_t> Assignment3::GenerateSubject(unsigned int width, unsigned int height, unsigned int sq_width, unsigned int sq_height)
{
	std::vector<uint8_t> subject(width * height);
	for (size_t y = 0; y < height; ++y)
		for (size_t x = 0; x < width; ++x) {
			size_t low = (width / 2) - (sq_width / 2);
			size_t high = (width / 2) + (sq_width / 2);
			if ((x == std::clamp(x, low, high)) && (y == std::clamp(y, low, high))) {
				subject[(y * width) + x] = UINT8_MAX;
			}
			else {
				subject[(y * width) + x] = 0;
			}
		}
	return subject;
}
void Assignment3::GenerateSubject2a() {
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	auto data = GenerateSubject(512, 512, 32, 32);
	image_registry->AddTexture("lenna", "subject2a", graphics::make_texture(data, 512, 512, 1));
}
void Assignment3::GenerateSubject2b() {
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	auto data = GenerateSubject(512, 512, 64, 64);
	image_registry->AddTexture("lenna", "subject2b", graphics::make_texture(data, 512, 512, 1));
}
void Assignment3::GenerateSubject2c() {
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	auto data = GenerateSubject(512, 512, 128, 128);
	image_registry->AddTexture("lenna", "subject2c", graphics::make_texture(data, 512, 512, 1));
}
}
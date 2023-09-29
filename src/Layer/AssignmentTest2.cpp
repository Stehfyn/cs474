#include "..\cs474.pch.h"
#include "AssignmentTest2.h"
#include <imgui_demo.cpp>
#include "..\Graphics\Image.h"
namespace cs474 {
	AssignmentTest2::AssignmentTest2() {
	}
	AssignmentTest2::~AssignmentTest2() {
	}

	void AssignmentTest2::OnAttach() {
	}

	void AssignmentTest2::OnDetach() {
	}

	void AssignmentTest2::OnUpdate(float ts) {
	}

	void AssignmentTest2::OnUIRender() {

		ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName("AssignmentTest2"));

		ImVec2 size = { 640, 480 };
		ImGui::SetNextWindowSize(size);

		ImVec2 pos = { 200, 400 };
		ImGui::SetNextWindowPos(pos, ImGuiCond_Once);

		ImGui::Begin("AssignmentTest2");

		std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
		const std::optional<graphics::Texture>& img_opt = image_registry->GetTexture("lenna", ".pgm");

		if (img_opt.has_value()) {
			const auto& img = img_opt.value();
			const std::vector<uint8_t>& rawData = img->GetRawData();
			ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
			ImGui::Image((void*)(intptr_t)(img->GetRendererID()), img_size);
			ImGui::SameLine();

			// Computed image
			
			const std::optional<graphics::Texture>& sub_opt = image_registry->GetTexture("lenna", "sub");

			if (sub_opt.has_value()) {
				const auto& img_sub = sub_opt.value();
				ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
				ImGui::Image((void*)(intptr_t)(img_sub->GetRendererID()), img_sub_size);
			} else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}

			ImGui::Text("Original: ");
			ImGui::SameLine();

			//ImGui::Dummy(,)
			const auto& style = ImGui::GetStyle();

			float x_offset = 2 * style.ItemSpacing.x + ((float)img->GetWidth() - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
			ImGui::Text("Processed: ");


			ImGui::Separator();

			static bool init_sample = true;
			if (ImGui::Button("Sub sample") || init_sample) {
				auto subsampled = subSample(rawData, factor, img->GetWidth(), img->GetHeight());
				if (subsampled.has_value()) {
					auto data = subsampled.value();
					auto scaled = scale(data, factor, img->GetWidth() / factor, img->GetHeight() / factor);
					if (scaled.has_value()) {
						auto scaled_data = scaled.value();
						bool success = image_registry->AddTexture("lenna", "sub", graphics::make_texture(scaled_data, img->GetWidth(), img->GetHeight(), 1));
						emscripten_log(EM_LOG_CONSOLE, "%d", success);
					}
				}
				if (init_sample) {
					init_sample = false;
				}
			}

			//Creating a Combo menu for the factor choices
			int items[] = { 1,2,4,8 };
			static int item_current_idx = 0;
			ImGui::SetNextItemWidth(100);
			if (ImGui::BeginCombo("Factor", std::to_string(items[item_current_idx]).c_str()))
			{
				ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(std::to_string(items[n]).c_str(), is_selected))
					{
						item_current_idx = n;
						this->factor = items[n];
					}
					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::End();
		}
	}
}
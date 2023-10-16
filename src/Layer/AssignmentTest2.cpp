#include "..\cs474.pch.h"
#include "AssignmentTest2.h"
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

		
		//this->Question1();
		this->Question2();
		this->Question3();
		this->Question4();
		
	}
	void AssignmentTest2::Question1() {
		ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName("Question1"));

		ImVec2 size = { 640, 480 };
		ImGui::SetNextWindowSize(size);

		ImVec2 pos = { 200, 0 };
		ImGui::SetNextWindowPos(pos, ImGuiCond_Once);

		ImGui::Begin("Question1");
		std::shared_ptr<graphics::ImageRegistry> image_registry = std::shared_ptr<graphics::ImageRegistry>(global::GetResourceMutUnwrapped("g_ImageRegistry"));
		const std::optional<graphics::Texture>& img_opt = image_registry->GetTexture("lenna", ".pgm");

		if (img_opt.has_value()) {
			const auto& style = ImGui::GetStyle();
			const auto& img = img_opt.value();
			const std::vector<uint8_t>& rawData = img->GetRawData();
			ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
			//ImGui::Image((void*)(intptr_t)(img->GetRendererID()), img_size);
			bool is_hovered1 = widgets::ImageInspector("inspect1", img, &inspect_sub, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + img->GetWidth()), 0.0f });
			ImGui::SameLine();

			// Computed image

			const std::optional<graphics::Texture>& sub_opt = image_registry->GetTexture("lenna", "sub");

			if (sub_opt.has_value()) {
				const auto& img_sub = sub_opt.value();
				ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
				bool is_hovered2 = widgets::ImageInspector("inspect2", img_sub, &inspect_sub, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_sub->GetWidth(), 0.0f });
				if ((!is_hovered1) && (!is_hovered2)) inspect_sub = false;
				//ImGui::Image((void*)(intptr_t)(img_sub->GetRendererID()), img_sub_size);
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}

			ImGui::Text("Original: ");
			ImGui::SameLine();

			//ImGui::Dummy(,)

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
		}
		ImGui::End();
	}
	void AssignmentTest2::Question2() {
		ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName("Question2"));

		ImVec2 size = { 1200, 1200 };
		ImGui::SetNextWindowSize(size);

		ImVec2 pos = { 200, 0 };
		ImGui::SetNextWindowPos(pos, ImGuiCond_Once);

		ImGui::Begin("Question2");
		std::shared_ptr<graphics::ImageRegistry> image_registry = std::shared_ptr<graphics::ImageRegistry>(global::GetResourceMutUnwrapped("g_ImageRegistry"));
		const std::optional<graphics::Texture>& img_opt = image_registry->GetTexture("lenna", ".pgm");

		std::vector<double> avgMask7x7(49, 1.0 / 49.0);
		std::vector<double> avgMask15x15(225, 1.0 / 225.0);
		std::vector<double> gausMask7x7 = {1,1,2,2,2,1,1,1,2,2,4,2,2,1,2,2,4,8,4,2,2,2,4,8,16,8,4,2,2,2,4,8,4,2,2,1,2,2,4,2,2,1,1,1,2,2,2,1,1};
		std::vector<double> gausMask15x15 = {2,2,3,4,5,5,6,6,6,5,5,4,3,2,2,2,3,4,5,7,7,8,8,8,7,7,5,4,3,2,3,4,6,9,10,10,11,10,10,9,7,5,4,3,2,4,5,7,9,10,12,13,13,13,12,10,9,7,6,4,3,
		5,7,9,11,13,14,15,16,15,14,13,11,9,7,5,5,7,10,12,14,16,17,18,17,16,14,12,10,7,5,6,8,10,13,15,17,19,19,19,17,15,13,10,8,6,6,8,11,13,15,17,19,20,19,18,16,13,11,8,6,6,8,10,13,15,17,19,19,19,17,15,13,10,8,6,
		5,7,10,12,14,16,17,18,17,16,14,12,10,7,5,5,7,9,11,13,14,15,16,15,14,13,11,9,7,5,4,5,7,9,10,12,13,13,13,12,10,9,7,6,4,3,3,4,6,9,10,10,11,10,10,9,7,5,4,3,2,2,3,4,5,7,7,8,8,8,7,7,5,4,3,2,2,2,3,4,5,5,6,6,6,5,5,4,3,2,2};

		if (img_opt.has_value()) {
			const auto& style = ImGui::GetStyle();
			const auto& img = img_opt.value();
			const std::vector<uint8_t>& rawData = img->GetRawData();
			ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
			bool is_hovered1 = widgets::ImageInspector("inspect1", img, &inspect_sub, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + img->GetWidth()), 0.0f });
			ImGui::SameLine();

			// Computed image
			const std::optional<graphics::Texture>& sub_opt = image_registry->GetTexture("lenna", "average");

			if (sub_opt.has_value()) {
				const auto& img_sub = sub_opt.value();
				ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
				bool is_hovered2 = widgets::ImageInspector("inspect2", img_sub, &inspect_sub, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_sub->GetWidth(), 0.0f });
				if ((!is_hovered1) && (!is_hovered2)) inspect_sub = false;
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}
			ImGui::SameLine();

			const std::optional<graphics::Texture>& sub_opt_blur = image_registry->GetTexture("lenna", "blur");

			if (sub_opt_blur.has_value()) {
				const auto& img_sub = sub_opt_blur.value();
				ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
				bool is_hovered2 = widgets::ImageInspector("inspect3", img_sub, &inspect_sub, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_sub->GetWidth(), 0.0f });
				if ((!is_hovered1) && (!is_hovered2)) inspect_sub = false;
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}

			ImGui::Text("Original: ");

			ImGui::SameLine();

			float x_offset = 2 * style.ItemSpacing.x + ((float)img->GetWidth() - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
			ImGui::Text("Averaged: ");

			ImGui::SameLine();

			float x_offset2 = 3 * style.ItemSpacing.x + ((float)img->GetWidth()*2 - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset2);
			ImGui::Text("Guassian Blurred: ");


			ImGui::Separator();

			static bool init_sample = true;
			if (ImGui::Button("Average") || init_sample) {
				if (filterSize == 7) {
					auto averagedData = smoothImage(img->GetRawData(), img->GetWidth(), img->GetHeight(), filterSize, avgMask7x7);
					if (averagedData.has_value()) {
						auto data = averagedData.value();
						bool success = image_registry->AddTexture("lenna", "average", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
						emscripten_log(EM_LOG_CONSOLE, "%d", success);
					}
				}
				else {
					auto averagedData = smoothImage(img->GetRawData(), img->GetWidth(), img->GetHeight(), filterSize, avgMask15x15);
					if (averagedData.has_value()) {
						auto data = averagedData.value();
						bool success = image_registry->AddTexture("lenna", "average", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
						emscripten_log(EM_LOG_CONSOLE, "%d", success);
					}
				}
				if (init_sample) {
					init_sample = false;
				}
			}
			ImGui::SameLine();
			static bool init_sample2 = true;
			if (ImGui::Button("Guassian Blur") || init_sample2) {
				if (filterSize == 7) {
					auto averagedData = smoothImage(img->GetRawData(), img->GetWidth(), img->GetHeight(), filterSize, gausMask7x7);
					if (averagedData.has_value()) {
						auto data = averagedData.value();
						bool success = image_registry->AddTexture("lenna", "blur", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
						emscripten_log(EM_LOG_CONSOLE, "%d", success);
					}
				}
				else {
					auto averagedData = smoothImage(img->GetRawData(), img->GetWidth(), img->GetHeight(), filterSize, gausMask15x15);
					if (averagedData.has_value()) {
						auto data = averagedData.value();
						bool success = image_registry->AddTexture("lenna", "blur", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
						emscripten_log(EM_LOG_CONSOLE, "%d", success);
					}
				}
				if (init_sample2) {
					init_sample2 = false;
				}
			}

				std::vector<std::string> items = { "7x7", "15x15" };
				static int item_current_idx = 0;
				ImGui::SetNextItemWidth(100);
				if (ImGui::BeginCombo("Filter Size", items[item_current_idx].c_str()))
				{
					ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
					for (int n = 0; n < items.size(); n++)
					{
						const bool is_selected = (item_current_idx == n);
						if (ImGui::Selectable(items[n].c_str(), is_selected)) {
							item_current_idx = n;
							if (items[n] == "7x7") {
								filterSize = 7;
							}
							else {
								filterSize = 15;
							}
						}
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				ImGui::End();
			}
		}
	void AssignmentTest2::Question3() {
		ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName("Question3"));

		ImVec2 size = { 1200, 480 };
		ImGui::SetNextWindowSize(size);

		ImVec2 pos = { 200, 0 };
		ImGui::SetNextWindowPos(pos, ImGuiCond_Once);

		ImGui::Begin("Question3");
		std::shared_ptr<graphics::ImageRegistry> image_registry = std::shared_ptr<graphics::ImageRegistry>(global::GetResourceMutUnwrapped("g_ImageRegistry"));
		const std::optional<graphics::Texture>& img_opt = image_registry->GetTexture("lenna", ".pgm");

		std::vector<double> avgMask7x7(49, 1.0 / 49.0);
		std::vector<double> avgMask15x15(225, 1.0 / 225.0);

		if (img_opt.has_value()) {
			const auto& style = ImGui::GetStyle();
			const auto& img = img_opt.value();
			const std::vector<uint8_t>& rawData = img->GetRawData();
			ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
			bool is_hovered1 = widgets::ImageInspector("inspect1", img, &inspect_sub, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + img->GetWidth()), 0.0f });
			ImGui::SameLine();

			// Computed image
			const std::optional<graphics::Texture>& sub_opt = image_registry->GetTexture("lenna", "salty");
			const std::optional<graphics::Texture>& sub_median = image_registry->GetTexture("lenna", "median");
			const std::optional<graphics::Texture>& sub_average = image_registry->GetTexture("lenna", "average");

			if (sub_opt.has_value()) {
				const auto& img_sub = sub_opt.value();
				ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
				bool is_hovered2 = widgets::ImageInspector("inspect2", img_sub, &inspect_sub, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_sub->GetWidth(), 0.0f });
				if ((!is_hovered1) && (!is_hovered2)) inspect_sub = false;
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}

			ImGui::SameLine();

			if (sub_median.has_value()) {
				const auto& img_sub = sub_median.value();
				ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
				bool is_hovered2 = widgets::ImageInspector("inspect3", img_sub, &inspect_sub, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_sub->GetWidth(), 0.0f });
				if ((!is_hovered1) && (!is_hovered2)) inspect_sub = false;
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}

			ImGui::SameLine();

			if (sub_average.has_value()) {
				const auto& img_sub = sub_average.value();
				ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
				bool is_hovered2 = widgets::ImageInspector("inspect4", img_sub, &inspect_sub, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_sub->GetWidth(), 0.0f });
				if ((!is_hovered1) && (!is_hovered2)) inspect_sub = false;
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}

			ImGui::Text("Original: ");

			ImGui::SameLine();

			float x_offset = 2 * style.ItemSpacing.x + ((float)img->GetWidth() - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
			ImGui::Text("Salt And Pepper: ");

			ImGui::SameLine();

			float x_offset2 = 3 * style.ItemSpacing.x + ((float)img->GetWidth()*2 - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset2);
			ImGui::Text("Median Filtered: ");

			ImGui::SameLine();

			float x_offset3 = 4 * style.ItemSpacing.x + ((float)img->GetWidth() * 3 - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset3);
			ImGui::Text("Average Filtered: ");

			ImGui::Separator();

			static bool init_sample = true;
			if (ImGui::Button("Salt And Pepper") || init_sample) {
					auto averagedData = addNoise(img->GetRawData(), img->GetWidth(), img->GetHeight(), percentSaltPep);
					if (averagedData.has_value()) {
						auto data = averagedData.value();
						bool success = image_registry->AddTexture("lenna", "salty", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
						emscripten_log(EM_LOG_CONSOLE, "%d", success);
					}
				if (init_sample) {
					init_sample = false;
				}
			}
			ImGui::SameLine();

			//Creating a Combo menu for the factor choices
			std::vector<std::string> items = {"30%", "50%"};
			static int item_current_idx = 0;
			ImGui::SetNextItemWidth(100);
			if (ImGui::BeginCombo("% Salt and Pepper", items[item_current_idx].c_str()))
			{
				ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
				for (int n = 0; n < items.size(); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n].c_str(), is_selected)) {
						item_current_idx = n;
						if (items[n] == "30%") {
							percentSaltPep = 30.0;
						}
						else {
							percentSaltPep = 50.0;
						}
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Separator();

			static bool init_sample2 = true;
			if (ImGui::Button("Median Filtered") || init_sample2) {
				const std::optional<graphics::Texture>& sub_optNew = image_registry->GetTexture("lenna", "salty");
				if (filterSize == 7) {
					auto medianData = medianFilter(sub_optNew.value()->GetRawData(), sub_optNew.value()->GetWidth(), sub_optNew.value()->GetHeight(), 7);
					if (medianData.has_value()) {
						auto data = medianData.value();
						bool success = image_registry->AddTexture("lenna", "median", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
						emscripten_log(EM_LOG_CONSOLE, "%d", success);
					}
				}
				else {
					auto medianData = medianFilter(sub_optNew.value()->GetRawData(), sub_optNew.value()->GetWidth(), sub_optNew.value()->GetHeight(), 15);
					if (medianData.has_value()) {
						auto data = medianData.value();
						bool success = image_registry->AddTexture("lenna", "median", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
						emscripten_log(EM_LOG_CONSOLE, "%d", success);
					}
				}
				if (init_sample2) {
					init_sample2 = false;
				}
			}

			ImGui::SameLine();

			std::vector<std::string> items2 = {"7x7", "15x15"};
			static int item_current_idx2 = 0;
			ImGui::SetNextItemWidth(100);
			if (ImGui::BeginCombo("Median Filter Size", items2[item_current_idx2].c_str()))
			{
				ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
				for (int n = 0; n < items2.size(); n++)
				{
					const bool is_selected = (item_current_idx2 == n);
					if (ImGui::Selectable(items2[n].c_str(), is_selected)) {
						item_current_idx2 = n;
						if (items2[n] == "7x7") {
							filterSize = 7;
						}
						else {
							filterSize = 15;
						}
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Separator();

			static bool init_sample3 = true;
			if (ImGui::Button("Average Filtering") || init_sample3) {
				const std::optional<graphics::Texture>& sub_optNew = image_registry->GetTexture("lenna", "median");
				if (filterSizeAvg == 7) {
					auto averagedData = smoothImage(sub_optNew.value()->GetRawData(), sub_optNew.value()->GetWidth(), sub_optNew.value()->GetHeight(), filterSizeAvg, avgMask7x7);
					if (averagedData.has_value()) {
						auto data = averagedData.value();
						bool success = image_registry->AddTexture("lenna", "average", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
						emscripten_log(EM_LOG_CONSOLE, "%d", success);
					}
				}
				else {
					auto medianData = smoothImage(sub_optNew.value()->GetRawData(), sub_optNew.value()->GetWidth(), sub_optNew.value()->GetHeight(), filterSizeAvg, avgMask15x15);
					if (medianData.has_value()) {
						auto data = medianData.value();
						bool success = image_registry->AddTexture("lenna", "average", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
						emscripten_log(EM_LOG_CONSOLE, "%d", success);
					}
				}
				if (init_sample3) {
					init_sample3 = false;
				}
			}
			ImGui::SameLine();


			static int item_current_idx3 = 0;
			ImGui::SetNextItemWidth(100);
			if (ImGui::BeginCombo("Average Filter Size", items2[item_current_idx3].c_str()))
			{
				ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
				for (int n = 0; n < items2.size(); n++)
				{
					const bool is_selected = (item_current_idx3 == n);
					if (ImGui::Selectable(items2[n].c_str(), is_selected)) {
						item_current_idx3 = n;
						if (items2[n] == "7x7") {
							filterSizeAvg = 7;
						}
						else {
							filterSizeAvg = 15;
						}
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Separator();

			ImGui::End();
		}
	}
	void AssignmentTest2::Question4() {
		ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName("Question4"));

		ImVec2 size = { 1200, 1200 };
		ImGui::SetNextWindowSize(size);

		ImVec2 pos = { 200, 0 };
		ImGui::SetNextWindowPos(pos, ImGuiCond_Once);

		ImGui::Begin("Question4");
		std::shared_ptr<graphics::ImageRegistry> image_registry = std::shared_ptr<graphics::ImageRegistry>(global::GetResourceMutUnwrapped("g_ImageRegistry"));
		const std::optional<graphics::Texture>& img_opt = image_registry->GetTexture("lenna", ".pgm");

		std::vector<double> gausMask7x7 = { 1,1,2,2,2,1,1,1,2,2,4,2,2,1,2,2,4,8,4,2,2,2,4,8,16,8,4,2,2,2,4,8,4,2,2,1,2,2,4,2,2,1,1,1,2,2,2,1,1 };

		if (img_opt.has_value()) {
			const auto& style = ImGui::GetStyle();
			const auto& img = img_opt.value();
			const std::vector<uint8_t>& rawData = img->GetRawData();
			ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
			bool is_hovered1 = widgets::ImageInspector("inspect1", img, &inspect_sub, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + img->GetWidth()), 0.0f });
			ImGui::SameLine();

			// Computed image
			const std::optional<graphics::Texture>& sub_opt = image_registry->GetTexture("lenna", "gaus");

			if (sub_opt.has_value()) {
				const auto& img_sub = sub_opt.value();
				ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
				bool is_hovered2 = widgets::ImageInspector("inspect2", img_sub, &inspect_sub, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_sub->GetWidth(), 0.0f });
				if ((!is_hovered1) && (!is_hovered2)) inspect_sub = false;
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}

			ImGui::SameLine();

			const std::optional<graphics::Texture>& sub_opt_unsharp = image_registry->GetTexture("lenna", "unsharp");

			if (sub_opt_unsharp.has_value()) {
				const auto& img_sub = sub_opt_unsharp.value();
				ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
				bool is_hovered2 = widgets::ImageInspector("inspect3", img_sub, &inspect_sub, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_sub->GetWidth(), 0.0f });
				if ((!is_hovered1) && (!is_hovered2)) inspect_sub = false;
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}

			ImGui::SameLine();

			const std::optional<graphics::Texture>& sub_opt_boost = image_registry->GetTexture("lenna", "boost");

			if (sub_opt_boost.has_value()) {
				const auto& img_sub = sub_opt_boost.value();
				ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
				bool is_hovered2 = widgets::ImageInspector("inspect4", img_sub, &inspect_sub, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_sub->GetWidth(), 0.0f });
				if ((!is_hovered1) && (!is_hovered2)) inspect_sub = false;
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}

			ImGui::Text("Original: ");

			ImGui::SameLine();

			float x_offset = 2 * style.ItemSpacing.x + ((float)img->GetWidth() - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
			ImGui::Text("Gaussian Blurred 7x7: ");

			ImGui::SameLine();

			float x_offset2 = 3 * style.ItemSpacing.x + ((float)img->GetWidth() * 2 - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset2);
			ImGui::Text("Unsharp Mask: ");

			ImGui::SameLine();

			float x_offset3 = 4 * style.ItemSpacing.x + ((float)img->GetWidth() * 3 - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset3);
			ImGui::Text("Boost Filter K = 5: ");

			ImGui::Separator();
		
			static bool init_sample2 = true;
			if (init_sample2) {
				auto smoothData = smoothImage(img->GetRawData(), img->GetWidth(), img->GetHeight(), 7, gausMask7x7);
				if (smoothData.has_value()) {
					const auto& data = smoothData.value();
					image_registry->AddTexture("lenna", "gaus", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					auto boostData = unsharpAndBoostFilter(img->GetRawData(), data, img->GetHeight(), img->GetWidth(), 5);
					auto unsharpData = unsharpAndBoostFilter(img->GetRawData(), data, img->GetHeight(), img->GetWidth(), 1);
					if (unsharpData.has_value() && boostData.has_value()) {
						image_registry->AddTexture("lenna", "boost", graphics::make_texture(boostData.value(), img->GetWidth(), img->GetHeight(), 1));
						image_registry->AddTexture("lenna", "unsharp", graphics::make_texture(unsharpData.value(), img->GetWidth(), img->GetHeight(), 1));
					}
					if (init_sample2) {
						init_sample2 = false;
					}
				}
			}
			ImGui::End();
		}
	}
}
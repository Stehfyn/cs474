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
		this->Question1();
		/*
		this->Question2();
		this->Question3();
		this->Question4();
		*/
	}
	void AssignmentTest2::Question1() {
		ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName("Question1"));

		ImVec2 size = { 1000, 480 };
		ImGui::SetNextWindowSize(size);

		ImVec2 pos = { 200, 0 };
		ImGui::SetNextWindowPos(pos, ImGuiCond_Once);

		ImGui::Begin("Question1");
		std::shared_ptr<graphics::ImageRegistry> image_registry = std::shared_ptr<graphics::ImageRegistry>(global::GetResourceMutUnwrapped("g_ImageRegistry"));
		const std::optional<graphics::Texture>& img_opt = image_registry->GetTexture("ImagePadded", ".pgm");

		if (img_opt.has_value()) {
			const auto& style = ImGui::GetStyle();
			const auto& img = img_opt.value();
			const std::vector<uint8_t>& rawData = img->GetRawData();
			ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
			ImGui::Image((void*)(intptr_t)(img->GetRendererID()), img_size);
			//bool is_hovered1 = widgets::ImageInspector("inspect1", img, &inspect_sub, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + img->GetWidth()), 0.0f });
			ImGui::SameLine();

			// Computed image

			const std::optional<graphics::Texture>& sub_opt = image_registry->GetTexture("PatternPadded", ".pgm");

			if (sub_opt.has_value()) {
				const auto& img_sub = sub_opt.value();
				//ImVec2 img_sub_size{(float)img_sub->GetWidth(),(float)img_sub->GetHeight() };
				ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
				//bool is_hovered2 = widgets::ImageInspector("inspect2", img_sub, &inspect_sub, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_sub->GetWidth(), 0.0f });
				//if ((!is_hovered1) && (!is_hovered2)) inspect_sub = false;
				ImGui::Image((void*)(intptr_t)(img_sub->GetRendererID()), img_sub_size);
			}
			else {

				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}

			ImGui::Text("Original: %zu %zu", img->GetWidth(), img->GetHeight());
			ImGui::SameLine();

			//ImGui::Dummy(,)
			ImGui::Text("%zu", rawData.size());

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
			std::vector<std::string> items = { "pattern" };
			static int item_current_idx = 0;
			ImGui::SetNextItemWidth(100);
			if (ImGui::BeginCombo("Original Image", items[item_current_idx].c_str()))
			{
				ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
				for (int n = 0; n < items.size(); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n].c_str(), is_selected))
					{
						item_current_idx = n;
						imageChoice = items[n].c_str();  // Or simply use items[n] if specifiedHisto is a std::string
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

		ImVec2 size = { 640, 480 };
		ImGui::SetNextWindowSize(size);

		ImVec2 pos = { 200 + 640, 0 };
		ImGui::SetNextWindowPos(pos, ImGuiCond_Once);

		ImGui::Begin("Question2");
		std::shared_ptr<graphics::ImageRegistry> image_registry = std::shared_ptr<graphics::ImageRegistry>(global::GetResourceMutUnwrapped("g_ImageRegistry"));
		const std::optional<graphics::Texture>& img_opt = image_registry->GetTexture("lenna", ".pgm");

		if (img_opt.has_value()) {
			const auto& style = ImGui::GetStyle();
			const auto& img = img_opt.value();
			const std::vector<uint8_t>& rawData = img->GetRawData();
			ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
			//ImGui::Image((void*)(intptr_t)(img->GetRendererID()), img_size);
			bool is_hovered1 = widgets::ImageInspector("inspect3", img, &inspect_quant, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + img->GetWidth()), 0.0f });
			ImGui::SameLine();

			// Computed image

			const std::optional<graphics::Texture>& quant_opt = image_registry->GetTexture("lenna", "quant");

			if (quant_opt.has_value()) {
				const auto& img_quant = quant_opt.value();
				ImVec2 img_quant_size{ (float)img_quant->GetWidth(), (float)img_quant->GetHeight() };
				bool is_hovered2 = widgets::ImageInspector("inspect4", img_quant, &inspect_quant, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_quant->GetWidth(), 0.0f });
				if ((!is_hovered1) && (!is_hovered2)) inspect_quant = false;
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

			static bool init_quant = true;

			auto do_quantize = [&]() {
				auto quantized = quantize(rawData, (uint8_t)levels, img->GetWidth(), img->GetHeight());
				if (quantized.has_value()) {
					auto data = quantized.value();
					bool success = image_registry->AddTexture("lenna", "quant", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
				if (init_quant) {
					init_quant = false;
				}
			};

			if (ImGui::Button("Quantize") || init_quant) {
				do_quantize();
			}

			//Creating a Combo menu for the factor choices
			int items[] = { (int)((uint8_t)-1),128,32,8,2};
			static int item_current_idx = 0;
			ImGui::SetNextItemWidth(100);
			if (ImGui::BeginCombo("Levels", std::to_string(items[item_current_idx]).c_str()))
			{
				ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(std::to_string(items[n]).c_str(), is_selected))
					{
						item_current_idx = n;
						this->levels = items[n];
						do_quantize();
					}
					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (ImGui::SliderFloat("##Levels", &levels, 2.0f, (float)(uint8_t)-1, "%.0f")) {
				do_quantize();
			}
			
		}
		ImGui::End();
	}
	void AssignmentTest2::Question3() {
		ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName("Question3"));

		ImVec2 size = { 640, 480 };
		ImGui::SetNextWindowSize(size);

		ImVec2 pos = { 200, 480 };
		ImGui::SetNextWindowPos(pos, ImGuiCond_Once);

		ImGui::Begin("Question3");
		std::shared_ptr<graphics::ImageRegistry> image_registry = std::shared_ptr<graphics::ImageRegistry>(global::GetResourceMutUnwrapped("g_ImageRegistry"));
		const std::optional<graphics::Texture>& img_opt = image_registry->GetTexture("f_16", ".pgm");

		if (img_opt.has_value()) {
			const auto& style = ImGui::GetStyle();
			const auto& img = img_opt.value();
			const std::vector<uint8_t>& rawData = img->GetRawData();
			ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
			//ImGui::Image((void*)(intptr_t)(img->GetRendererID()), img_size);
			bool is_hovered1 = widgets::ImageInspector("inspect5", img, &inspect_eq, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + img->GetWidth()), 0.0f });
			ImGui::SameLine();

			// Computed image

			const std::optional<graphics::Texture>& eq_opt = image_registry->GetTexture("f_16", "eq");

			if (eq_opt.has_value()) {
				const auto& img_eq = eq_opt.value();
				ImVec2 img_eq_size{ (float)img_eq->GetWidth(), (float)img_eq->GetHeight() };
				bool is_hovered2 = widgets::ImageInspector("inspect6", img_eq, &inspect_eq, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_eq->GetWidth(), 0.0f });
				if ((!is_hovered1) && (!is_hovered2)) inspect_eq = false;
				//ImGui::Image((void*)(intptr_t)(img_sub->GetRendererID()), img_sub_size);
			}
			else {
				auto equalized = hist_eq(rawData, img->GetWidth(), img->GetHeight());
				bool success = image_registry->AddTexture("f_16", "eq", graphics::make_texture(equalized, img->GetWidth(), img->GetHeight(), 1));
			}

			ImGui::Text("Original: ");
			ImGui::SameLine();

			//ImGui::Dummy(,)

			float x_offset = 2 * style.ItemSpacing.x + ((float)img->GetWidth() - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
			ImGui::Text("Equalized: ");

		}
		ImGui::End();
	}
	void AssignmentTest2::Question4() {
		ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName("Question4"));

		ImVec2 size = { 640, 750 };
		ImGui::SetNextWindowSize(size);

		ImVec2 pos = { 200 + 640, 480 };
		ImGui::SetNextWindowPos(pos, ImGuiCond_Once);

		ImGui::Begin("Question4");
		std::shared_ptr<graphics::ImageRegistry> image_registry = std::shared_ptr<graphics::ImageRegistry>(global::GetResourceMutUnwrapped("g_ImageRegistry"));
		const std::optional<graphics::Texture>& img_opt = image_registry->GetTexture(imageChoice, ".pgm");
		const std::optional<graphics::Texture>& specHisto_opt = image_registry->GetTexture(spechistoChoice, ".pgm");

		if (img_opt.has_value()) {
			const auto& style = ImGui::GetStyle();
			const auto& img = img_opt.value();
			const std::vector<uint8_t>& rawData = img->GetRawData();
			ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
			//ImGui::Image((void*)(intptr_t)(img->GetRendererID()), img_size);
			bool is_hovered1 = widgets::ImageInspector("inspect1", img, &inspect_spec, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + img->GetWidth()), 0.0f });
			ImGui::SameLine();

			// Computed image

			const std::optional<graphics::Texture>& spec_opt = image_registry->GetTexture(imageChoice, "spec");

			if (spec_opt.has_value()) {
				const auto& img_sub = spec_opt.value();
				ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
				bool is_hovered2 = widgets::ImageInspector("inspect2", img_sub, &inspect_spec, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_sub->GetWidth(), 0.0f });
				if ((!is_hovered1) && (!is_hovered2)) inspect_spec= false;
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
			ImGui::Text("Histogram Specification with Image: ");


			ImGui::Separator();
			
			static bool init_sample = true;
			if (ImGui::Button("Histogram Specification") || init_sample) {
				const auto& specHistoImgData = specHisto_opt.value();
				auto specifiedHistogram = hist_spec(rawData, specHistoImgData->GetRawData(), img->GetWidth(), img->GetHeight());
				if (specifiedHistogram.has_value()) {
					std::vector<uint8_t> specImagedata = specifiedHistogram.value();
					auto scaled = scale(specImagedata, factor, img->GetWidth() / factor, img->GetHeight() / factor);
					if (scaled.has_value()) {
						auto scaled_data = scaled.value();
						bool success = image_registry->AddTexture(imageChoice, "spec", graphics::make_texture(scaled_data, img->GetWidth(), img->GetHeight(), 1));
						emscripten_log(EM_LOG_CONSOLE, "%d", success);
					}
				}
				if (init_sample) {
					init_sample = false;
				}
				
			}

			//Creating a Combo menu for the factor choices
			std::vector<std::string> items = {"boat", "aerial", "f_16", "lenna", "tools", "peppers", "sf", "wheel", "lax" };
			static int item_current_idx = 0;
			ImGui::SetNextItemWidth(100);
			if (ImGui::BeginCombo("Original Image", items[item_current_idx].c_str()))
			{
				ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
				for (int n = 0; n < items.size(); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n].c_str(), is_selected))
					{
						item_current_idx = n;
						imageChoice = items[n].c_str();  // Or simply use items[n] if specifiedHisto is a std::string
					}
					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			//Creating a Combo menu for the factor choices
			std::vector<std::string> items2 = {"boat", "aerial", "f_16", "lenna", "tools", "peppers", "sf", "wheel", "lax"};
			static int item_current_idx2 = 0;
			ImGui::SetNextItemWidth(100);
			if (ImGui::BeginCombo("Specified Histogram", items2[item_current_idx2].c_str()))
			{
				ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
				for (int n = 0; n < items2.size(); n++)
				{
					const bool is_selected2 = (item_current_idx2 == n);
					if (ImGui::Selectable(items2[n].c_str(), is_selected2))
					{
						item_current_idx2 = n;
						spechistoChoice = items2[n].c_str();  // Or simply use items[n] if specifiedHisto is a std::string
					}
					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected2)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			std::vector<float> originalImgHisto = hist(img_opt.value()->GetRawData());
			ImGui::PlotHistogram("Orignal Image Histogram",
				originalImgHisto.data(),           // Pointer to the data
				originalImgHisto.size(),           // Number of data points
				0,                           // Index offset (not needed in this case)
				nullptr,                     // Overlay text
				0.0f,                        // Scale min (0 for automatic scaling)
				*std::max_element(originalImgHisto.begin(), originalImgHisto.end()), // Scale max
				ImVec2(0, 80)                 // Graph size (0 for automatic sizing)
			);

			/*std::vector<float> specifiedImgHisto = hist(specImageData);
			ImGui::PlotHistogram("Specified Image Histogram",
				specifiedImgHisto.data(),           // Pointer to the data
				specifiedImgHisto.size(),           // Number of data points
				0,                           // Index offset (not needed in this case)
				nullptr,                     // Overlay text
				0.0f,                        // Scale min (0 for automatic scaling)
				*std::max_element(specifiedImgHisto.begin(), specifiedImgHisto.end()), // Scale max
				ImVec2(0, 80)                 // Graph size (0 for automatic sizing)
			);*/
		}
		ImGui::End();
	}
}
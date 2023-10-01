#include "..\cs474.pch.h"
#include "Assignment1.h"
namespace cs474 {
Assignment1::Assignment1() {
}
Assignment1::~Assignment1() {
}
void Assignment1::OnAttach() {
	global::UpdateResource("g_Assignment1ScrollToImage Sampling", false);
	global::UpdateResource("g_Assignment1ScrollToImage Quantization", false);
	global::UpdateResource("g_Assignment1ScrollToHistogram Equalization", false);
	global::UpdateResource("g_Assignment1ScrollToHistogram Specification", false);
}
void Assignment1::OnDetach() {
}
void Assignment1::OnUpdate(float ts) {
}
void Assignment1::OnUIRender() {
	if (global::GetResourceUnwrapped("g_ShowAssignment1")) {
		bool open = true;
		ImGuiWindowFlags window_flags = utils::GetBaseLayerWindowFlags();
		//ImGuiWindowClass wc;
		//wc.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_NoUndocking;
		//ImGui::SetNextWindowClass(&wc);

		ImGui::Begin("Assignment 1", &open);
		{
			this->Question1();
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::NewLine();

			this->Question2();
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::NewLine();

			this->Question3();
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::NewLine();
			
			this->Question4();
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::NewLine();
		}
		ImGui::End();

		if (!open) global::UpdateResource("g_ShowAssignment1", false);
	}
}
void Assignment1::Question1() {
	widgets::markdown("# 1. Image Sampling");
	if (global::GetResourceUnwrapped("g_Assignment1ScrollToImage Sampling")) {
		ImGui::SetScrollHereY();
		global::UpdateResource("g_Assignment1ScrollToImage Sampling", false);
	}

	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");

	const std::optional<graphics::Texture>& img_opt_lenna = image_registry->GetTexture("lenna", ".pgm");
	const std::optional<graphics::Texture>& img_opt_peppers = image_registry->GetTexture("peppers", ".pgm");

	if (img_opt_lenna.has_value() && img_opt_peppers.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& lenna_img = img_opt_lenna.value();
		const std::vector<uint8_t>& rawDataLenna = lenna_img->GetRawData();
		
		ImVec2 img_size{ (float)lenna_img->GetWidth(), (float)lenna_img->GetHeight() };
		bool is_hovered1 = widgets::ImageInspector("inspect1", 
			lenna_img, &inspect_sub1, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + lenna_img->GetWidth()), 0.0f });
		
		ImGui::SameLine();

		const std::optional<graphics::Texture>& lenna_sub_opt = image_registry->GetTexture("lenna", "sub");
		if (lenna_sub_opt.has_value()) {
			const auto& lenna_img_sub = lenna_sub_opt.value();
			ImVec2 img_sub_size{ (float)lenna_img_sub->GetWidth(), (float)lenna_img_sub->GetHeight() };
			bool is_hovered2 = widgets::ImageInspector("inspect2", 
				lenna_img_sub, &inspect_sub1, { 0.0f, 0.0f }, { style.ItemSpacing.x + lenna_img_sub->GetWidth(), 0.0f });
			if ((!is_hovered1) && (!is_hovered2)) inspect_sub1 = false;
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SameLine();

		const auto& peppers_img = img_opt_peppers.value();
		const std::vector<uint8_t>& rawDataPeppers = peppers_img->GetRawData();
		float orignal_x_2nd = ImGui::GetCursorPosX();
		bool is_hovered3 = widgets::ImageInspector("inspect3",
			peppers_img, &inspect_sub2, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + peppers_img->GetWidth()), 0.0f });


		ImGui::SameLine();

		float processed_x_2nd = 0.0f;
		const std::optional<graphics::Texture>& peppers_sub_opt = image_registry->GetTexture("peppers", "sub");
		if (peppers_sub_opt.has_value()) {
			const auto& peppers_img_sub = peppers_sub_opt.value();
			ImVec2 img_sub_size{ (float)peppers_img_sub->GetWidth(), (float)peppers_img_sub->GetHeight() };
			processed_x_2nd = ImGui::GetCursorPosX();
			bool is_hovered4 = widgets::ImageInspector("inspect4",
				peppers_img_sub, &inspect_sub2, { 0.0f, 0.0f }, { style.ItemSpacing.x + peppers_img_sub->GetWidth(), 0.0f });
			if ((!is_hovered3) && (!is_hovered4)) inspect_sub2 = false;
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::Text("Original: ");
		ImGui::SameLine();

		//ImGui::Dummy(,)

		float x_offset = 2 * style.ItemSpacing.x + ((float)lenna_img->GetWidth() - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
		ImGui::Text("Processed: ");
		ImGui::SameLine();

		ImGui::SetCursorPosX(orignal_x_2nd);

		ImGui::Text("Original: ");
		ImGui::SameLine();
		
		ImGui::SetCursorPosX(processed_x_2nd);

		ImGui::Text("Processed: ");

		ImGui::Separator();

		static bool init_sample1 = true;
		if (ImGui::Button("Subsample##lenna") || init_sample1) {
			auto subsampled = subSample(rawDataLenna, factor1, lenna_img->GetWidth(), lenna_img->GetHeight());
			if (subsampled.has_value()) {
				auto data = subsampled.value();
				auto scaled = scale(data, factor1, lenna_img->GetWidth() / factor1, lenna_img->GetHeight() / factor1);
				if (scaled.has_value()) {
					auto scaled_data = scaled.value();
					bool success = image_registry->AddTexture("lenna", "sub", graphics::make_texture(scaled_data, lenna_img->GetWidth(), lenna_img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
			if (init_sample1) {
				init_sample1 = false;
			}
		}
		ImGui::SameLine();
		//Creating a Combo menu for the factor choices
		int items1[] = { 1,2,4,8 };
		static int item_current_idx_1 = 0;
		ImGui::SetNextItemWidth(100);
		if (ImGui::BeginCombo("Factor##lennacombo", std::to_string(items1[item_current_idx_1]).c_str()))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			for (int n = 0; n < IM_ARRAYSIZE(items1); n++)
			{
				const bool is_selected = (item_current_idx_1 == n);
				if (ImGui::Selectable(std::to_string(items1[n]).c_str(), is_selected))
				{
					item_current_idx_1 = n;
					this->factor1 = items1[n];
				}
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		static bool init_sample2 = true;
		ImGui::SetCursorPosX(orignal_x_2nd);
		if (ImGui::Button("Subsample##peppers") || init_sample2) {
			auto subsampled = subSample(rawDataPeppers, factor2, peppers_img->GetWidth(), peppers_img->GetHeight());
			if (subsampled.has_value()) {
				auto data = subsampled.value();
				auto scaled = scale(data, factor2, peppers_img->GetWidth() / factor2, peppers_img->GetHeight() / factor2);
				if (scaled.has_value()) {
					auto scaled_data = scaled.value();
					bool success = image_registry->AddTexture("peppers", "sub", graphics::make_texture(scaled_data, peppers_img->GetWidth(), peppers_img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
			if (init_sample2) {
				init_sample2 = false;
			}
		}
		ImGui::SameLine();
		//Creating a Combo menu for the factor choices
		int items2[] = { 1,2,4,8 };
		static int item_current_idx_2 = 0;
		ImGui::SetNextItemWidth(100);
		if (ImGui::BeginCombo("Factor##pepperscombo", std::to_string(items2[item_current_idx_2]).c_str()))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			for (int n = 0; n < IM_ARRAYSIZE(items2); n++)
			{
				const bool is_selected = (item_current_idx_2 == n);
				if (ImGui::Selectable(std::to_string(items2[n]).c_str(), is_selected))
				{
					item_current_idx_2 = n;
					this->factor2 = items2[n];
				}
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}

}
void Assignment1::Question2() {
	widgets::markdown("# 2. Image Quantization");
	if (global::GetResourceUnwrapped("g_Assignment1ScrollToImage Quantization")) {
		ImGui::SetScrollHereY();
		global::UpdateResource("g_Assignment1ScrollToImage Quantization", false);
	}

	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");

	const std::optional<graphics::Texture>& img_opt_lenna = image_registry->GetTexture("lenna", ".pgm");
	const std::optional<graphics::Texture>& img_opt_peppers = image_registry->GetTexture("peppers", ".pgm");

	if (img_opt_lenna.has_value() && img_opt_peppers.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& lenna_img = img_opt_lenna.value();
		const std::vector<uint8_t>& rawDataLenna = lenna_img->GetRawData();
		ImVec2 img_size{ (float)lenna_img->GetWidth(), (float)lenna_img->GetHeight() };
		//ImGui::Image((void*)(intptr_t)(img->GetRendererID()), img_size);
		bool is_hovered1 = widgets::ImageInspector("inspect5", lenna_img, &inspect_quant1, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + lenna_img->GetWidth()), 0.0f });
		ImGui::SameLine();

		// Computed image

		const std::optional<graphics::Texture>& lenna_quant_opt = image_registry->GetTexture("lenna", "quant");

		float quant_x_pos = 0.0f;
		if (lenna_quant_opt.has_value()) {
			const auto& img_quant = lenna_quant_opt.value();
			ImVec2 img_quant_size{ (float)img_quant->GetWidth(), (float)img_quant->GetHeight() };
			quant_x_pos = ImGui::GetCursorPosX() + img_quant->GetWidth();
			bool is_hovered2 = widgets::ImageInspector("inspect6", img_quant, &inspect_quant1, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_quant->GetWidth(), 0.0f });
			if ((!is_hovered1) && (!is_hovered2)) inspect_quant1 = false;
			//ImGui::Image((void*)(intptr_t)(img_sub->GetRendererID()), img_sub_size);
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SameLine();
		const auto& peppers_img = img_opt_peppers.value();
		const std::vector<uint8_t>& rawDataPeppers = peppers_img->GetRawData();
		float peppers_x = ImGui::GetCursorPosX();
		bool is_hovered3 = widgets::ImageInspector("inspect7", peppers_img, &inspect_quant2, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + peppers_img->GetWidth()), 0.0f });
		ImGui::SameLine();

		const std::optional<graphics::Texture>& peppers_quant_opt = image_registry->GetTexture("peppers", "quant");
		float quant_x_pos2 = 0.0f;
		if (peppers_quant_opt.has_value()) {
			const auto& img_quant = peppers_quant_opt.value();
			ImVec2 img_quant_size{ (float)img_quant->GetWidth(), (float)img_quant->GetHeight() };
			quant_x_pos2 = ImGui::GetCursorPosX() + img_quant->GetWidth();
			bool is_hovered4 = widgets::ImageInspector("inspect8", img_quant, &inspect_quant2, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_quant->GetWidth(), 0.0f });
			if ((!is_hovered3) && (!is_hovered4)) inspect_quant2 = false;
			//ImGui::Image((void*)(intptr_t)(img_sub->GetRendererID()), img_sub_size);
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::Text("Original: ");
		ImGui::SameLine();

		//ImGui::Dummy(,)

		float x_offset = 2 * style.ItemSpacing.x + ((float)lenna_img->GetWidth() - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
		ImGui::Text("Processed: ");
		ImGui::SameLine();

		ImGui::SetCursorPosX(peppers_x);
		ImGui::Text("Original: ");
		ImGui::SameLine();

		ImGui::SetCursorPosX(quant_x_pos2 - peppers_img->GetWidth());
		ImGui::Text("Processed: ");


		ImGui::Separator();

		static bool init_quant1 = true;

		auto do_quantize1 = [&]() {
			auto quantized = quantize(rawDataLenna, (uint8_t)levels1, lenna_img->GetWidth(), lenna_img->GetHeight());
			if (quantized.has_value()) {
				auto data = quantized.value();
				bool success = image_registry->AddTexture("lenna", "quant", graphics::make_texture(data, lenna_img->GetWidth(), lenna_img->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);
			}
			if (init_quant1) {
				init_quant1 = false;
			}
		};

		static bool init_quant2 = true;

		auto do_quantize2 = [&]() {
			auto quantized = quantize(rawDataPeppers, (uint8_t)levels2, peppers_img->GetWidth(), peppers_img->GetHeight());
			if (quantized.has_value()) {
				auto data = quantized.value();
				bool success = image_registry->AddTexture("peppers", "quant", graphics::make_texture(data, peppers_img->GetWidth(), peppers_img->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);
			}
			if (init_quant2) {
				init_quant2 = false;
			}
		};

		if (ImGui::Button("Quantize##quant1") || init_quant1) {
			do_quantize1();
		}

		ImGui::SameLine();

		//Creating a Combo menu for the factor choices
		int items[] = { (int)((uint8_t)-1),128,32,8,2 };
		static int item_current_idx1 = 0;
		ImGui::SetNextItemWidth(100);
		if (ImGui::BeginCombo("##Levels1", std::to_string(items[item_current_idx1]).c_str()))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_current_idx1 == n);
				if (ImGui::Selectable(std::to_string(items[n]).c_str(), is_selected))
				{
					item_current_idx1 = n;
					this->levels1 = items[n];
					do_quantize1();
				}
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		
		ImGui::SameLine();

		ImGui::SetNextItemWidth(quant_x_pos - ImGui::GetCursorPosX() - (std::string("Levels").size() * ImGui::GetFontSize())/2.0);
		if (ImGui::SliderFloat("Levels##LevelsSlider1", &levels1, 2.0f, (float)(uint8_t)-1, "%.0f")) {
			do_quantize1();
		}

		ImGui::SameLine();

		if (ImGui::Button("Quantize##quant2") || init_quant2) {
			do_quantize2();
		}

		ImGui::SameLine();

		static int item_current_idx2 = 0;
		ImGui::SetNextItemWidth(100);
		if (ImGui::BeginCombo("##Levels2", std::to_string(items[item_current_idx2]).c_str()))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_current_idx2 == n);
				if (ImGui::Selectable(std::to_string(items[n]).c_str(), is_selected))
				{
					item_current_idx2 = n;
					this->levels2 = items[n];
					do_quantize2();
				}
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();

		ImGui::SetNextItemWidth(quant_x_pos2 - ImGui::GetCursorPosX() - (std::string("Levels").size() * ImGui::GetFontSize()) / 2.0);
		if (ImGui::SliderFloat("Levels##LevelsSlider2", &levels2, 2.0f, (float)(uint8_t)-1, "%.0f")) {
			do_quantize2();
		}

	}

}
void Assignment1::Question3() {
	widgets::markdown("# 3. Histogram Equalization");
	if (global::GetResourceUnwrapped("g_Assignment1ScrollToHistogram Equalization")) {
		ImGui::SetScrollHereY();
		global::UpdateResource("g_Assignment1ScrollToHistogram Equalization", false);
	}

	std::shared_ptr<graphics::ImageRegistry> image_registry = std::shared_ptr<graphics::ImageRegistry>(global::GetResourceMutUnwrapped("g_ImageRegistry"));
	const std::optional<graphics::Texture>& boat_img_opt = image_registry->GetTexture("boat", ".pgm");
	const std::optional<graphics::Texture>& f16_img_opt = image_registry->GetTexture("f_16", ".pgm");

	if (boat_img_opt.has_value() && f16_img_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& boat_img = boat_img_opt.value();
		const std::vector<uint8_t>& rawDataBoat = boat_img->GetRawData();
		ImVec2 img_size{ (float)boat_img->GetWidth(), (float)boat_img->GetHeight() };
		//ImGui::Image((void*)(intptr_t)(img->GetRendererID()), img_size);
		bool is_hovered1 = widgets::ImageInspector("inspect9", boat_img, &inspect_eq1, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + boat_img->GetWidth()), 0.0f });
		ImGui::SameLine();

		// Computed image

		const std::optional<graphics::Texture>& boat_eq_opt = image_registry->GetTexture("boat", "eq");

		if (boat_eq_opt.has_value()) {
			const auto& boat_img_eq = boat_eq_opt.value();
			ImVec2 img_eq_size{ (float)boat_img_eq->GetWidth(), (float)boat_img_eq->GetHeight() };
			bool is_hovered2 = widgets::ImageInspector("inspect10", boat_img_eq, &inspect_eq1, { 0.0f, 0.0f }, { style.ItemSpacing.x + boat_img_eq->GetWidth(), 0.0f });
			if ((!is_hovered1) && (!is_hovered2)) inspect_eq1 = false;
			//ImGui::Image((void*)(intptr_t)(img_sub->GetRendererID()), img_sub_size);
		}
		else {
			auto equalized = hist_eq(rawDataBoat, boat_img->GetWidth(), boat_img->GetHeight());
			bool success = image_registry->AddTexture("boat", "eq", graphics::make_texture(equalized, boat_img->GetWidth(), boat_img->GetHeight(), 1));
		}

		ImGui::SameLine();
		const auto& f16_img = f16_img_opt.value();
		const std::vector<uint8_t>& rawDataf16 = f16_img->GetRawData();
		float f16_x = ImGui::GetCursorPosX();
		bool is_hovered3 = widgets::ImageInspector("inspect11", f16_img, &inspect_eq2, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + boat_img->GetWidth()), 0.0f });
		ImGui::SameLine();
		
		const std::optional<graphics::Texture>& f16_eq_opt = image_registry->GetTexture("f_16", "eq");
		if (f16_eq_opt.has_value()) {
			const auto& f16_img_eq = f16_eq_opt.value();
			ImVec2 img_eq_size{ (float)f16_img_eq->GetWidth(), (float)f16_img_eq->GetHeight() };
			bool is_hovered4 = widgets::ImageInspector("inspect12", f16_img_eq, &inspect_eq2, { 0.0f, 0.0f }, { style.ItemSpacing.x + f16_img_eq->GetWidth(), 0.0f });
			if ((!is_hovered3) && (!is_hovered4)) inspect_eq2 = false;
			//ImGui::Image((void*)(intptr_t)(img_sub->GetRendererID()), img_sub_size);
		}
		else {
			auto equalized = hist_eq(rawDataf16, f16_img->GetWidth(), f16_img->GetHeight());
			bool success = image_registry->AddTexture("f_16", "eq", graphics::make_texture(equalized, f16_img->GetWidth(), f16_img->GetHeight(), 1));
		}

		std::vector<float> originalImgHistoBoat = hist(rawDataBoat);
		ImGui::PlotHistogram("##Orignal Boat Histo",
			originalImgHistoBoat.data(),           // Pointer to the data
			originalImgHistoBoat.size(),           // Number of data points
			0,                           // Index offset (not needed in this case)
			nullptr,                     // Overlay text
			0.0f,                        // Scale min (0 for automatic scaling)
			*std::max_element(originalImgHistoBoat.begin(), originalImgHistoBoat.end()), // Scale max
			ImVec2(boat_img->GetWidth(), 80)                 // Graph size (0 for automatic sizing)
		);

		ImGui::SameLine();

		if (boat_eq_opt.has_value()) {
			std::vector<float> equalizedImgHistoBoat = hist(boat_eq_opt.value()->GetRawData());
			ImGui::PlotHistogram("##Equalized Boat Histo",
				equalizedImgHistoBoat.data(),           // Pointer to the data
				equalizedImgHistoBoat.size(),           // Number of data points
				0,                           // Index offset (not needed in this case)
				nullptr,                     // Overlay text
				0.0f,                        // Scale min (0 for automatic scaling)
				*std::max_element(equalizedImgHistoBoat.begin(), equalizedImgHistoBoat.end()), // Scale max
				ImVec2(boat_img->GetWidth(), 80)                 // Graph size (0 for automatic sizing)
			);
		}

		ImGui::SameLine();

		std::vector<float> originalImgHistof16 = hist(rawDataf16);
		ImGui::PlotHistogram("##Orignal f16 Histo",
			originalImgHistof16.data(),           // Pointer to the data
			originalImgHistof16.size(),           // Number of data points
			0,                           // Index offset (not needed in this case)
			nullptr,                     // Overlay text
			0.0f,                        // Scale min (0 for automatic scaling)
			*std::max_element(originalImgHistof16.begin(), originalImgHistof16.end()), // Scale max
			ImVec2(f16_img->GetWidth(), 80)                 // Graph size (0 for automatic sizing)
		);

		ImGui::SameLine();

		if (f16_eq_opt.has_value()) {
			std::vector<float> equalizedImgHistof16 = hist(f16_eq_opt.value()->GetRawData());
			ImGui::PlotHistogram("##Equalized f16 Histo",
				equalizedImgHistof16.data(),           // Pointer to the data
				equalizedImgHistof16.size(),           // Number of data points
				0,                           // Index offset (not needed in this case)
				nullptr,                     // Overlay text
				0.0f,                        // Scale min (0 for automatic scaling)
				*std::max_element(equalizedImgHistof16.begin(), equalizedImgHistof16.end()), // Scale max
				ImVec2(f16_img->GetWidth(), 80)                 // Graph size (0 for automatic sizing)
			);
		}

		ImGui::Text("Original: ");
		ImGui::SameLine();
		
		//ImGui::Dummy(,)
		
		float x_offset = 2 * style.ItemSpacing.x + ((float)boat_img->GetWidth() - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
		ImGui::Text("Equalized: ");
		ImGui::SameLine();

		ImGui::SetCursorPosX(f16_x);
		ImGui::Text("Original: ");
		ImGui::SameLine();

		ImGui::SetCursorPosX(f16_x + f16_img->GetWidth() + style.ItemSpacing.x);
		ImGui::Text("Equalized: ");

		ImGui::Separator();
	}
}
void Assignment1::Question4() {
	widgets::markdown("# 4. Histogram Specification");
	if (global::GetResourceUnwrapped("g_Assignment1ScrollToHistogram Specification")) {
		ImGui::SetScrollHereY();
		global::UpdateResource("g_Assignment1ScrollToHistogram Specification", false);
	}

	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& boat_img_opt = image_registry->GetTexture("boat", ".pgm");
	const std::optional<graphics::Texture>& f16_img_opt = image_registry->GetTexture("f_16", ".pgm");
	const std::optional<graphics::Texture>& sf_img_opt = image_registry->GetTexture("sf", ".pgm");
	const std::optional<graphics::Texture>& peppers_img_opt = image_registry->GetTexture("peppers", ".pgm");

	if (boat_img_opt.has_value() && f16_img_opt.has_value()
		&& sf_img_opt.has_value() && peppers_img_opt.has_value()) {
		const auto& style = ImGui::GetStyle();

		const auto& boat_img = boat_img_opt.value();
		const std::vector<uint8_t>& rawDataBoat = boat_img->GetRawData();
		const auto& f16_img = f16_img_opt.value();
		const std::vector<uint8_t>& rawDataf16 = f16_img->GetRawData();
		const auto& sf_img = sf_img_opt.value();
		const std::vector<uint8_t>& rawDataSf = sf_img->GetRawData();
		const auto& peppers_img = peppers_img_opt.value();
		const std::vector<uint8_t>& rawDataPeppers = peppers_img->GetRawData();
		
		auto do_specification = [&](const std::string& src_name, const std::string& spec_name, const std::vector<uint8_t>& src, int width, int height) {
			const std::optional<graphics::Texture>& spec_img_opt = image_registry->GetTexture(spec_name, ".pgm");
			if (spec_img_opt.has_value()) {
				const std::vector<uint8_t>& rawDataSpec = spec_img_opt.value()->GetRawData();
				auto specified = hist_spec(src, rawDataSpec, width, height);
				if (specified.has_value()) {
					bool success = image_registry->AddTexture(src_name, "spec", graphics::make_texture(specified.value(), width, height, 1));
				}
			}
		};

		ImVec2 img_size{ (float)boat_img->GetWidth(), (float)boat_img->GetHeight() };
		//ImGui::Image((void*)(intptr_t)(img->GetRendererID()), img_size);
		bool is_hovered1 = widgets::ImageInspector("inspect13", boat_img, &inspect_spec1, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + boat_img->GetWidth()), 0.0f });
		ImGui::SameLine();

		const std::optional<graphics::Texture>& boat_spec_opt = image_registry->GetTexture("boat", "spec");

		if (boat_spec_opt.has_value()) {
			const auto& boat_img_spec = boat_spec_opt.value();
			ImVec2 img_eq_size{ (float)boat_img_spec->GetWidth(), (float)boat_img_spec->GetHeight() };
			bool is_hovered2 = widgets::ImageInspector("inspect14", boat_img_spec, &inspect_spec1, { 0.0f, 0.0f }, { style.ItemSpacing.x + boat_img_spec->GetWidth(), 0.0f });
			if ((!is_hovered1) && (!is_hovered2)) inspect_spec1 = false;
			//ImGui::Image((void*)(intptr_t)(img_sub->GetRendererID()), img_sub_size);
		}
		else {
			do_specification("boat", "sf", rawDataBoat, boat_img->GetWidth(), boat_img->GetHeight());
		}

		ImGui::SameLine();

		float f16_x = ImGui::GetCursorPosX();
		bool is_hovered3 = widgets::ImageInspector("inspect15", f16_img, &inspect_spec2, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + boat_img->GetWidth()), 0.0f });
		ImGui::SameLine();

		const std::optional<graphics::Texture>& f16_spec_opt = image_registry->GetTexture("f_16", "spec");
		if (f16_spec_opt.has_value()) {
			const auto& f16_img_spec = f16_spec_opt.value();
			ImVec2 img_eq_size{ (float)f16_img_spec->GetWidth(), (float)f16_img_spec->GetHeight() };
			bool is_hovered2 = widgets::ImageInspector("inspect16", f16_img_spec, &inspect_spec2, { 0.0f, 0.0f }, { style.ItemSpacing.x + f16_img_spec->GetWidth(), 0.0f });
			if ((!is_hovered1) && (!is_hovered2)) inspect_spec2 = false;
			//ImGui::Image((void*)(intptr_t)(img_sub->GetRendererID()), img_sub_size);
		}
		else {
			do_specification("f_16", "peppers", rawDataf16, f16_img->GetWidth(), f16_img->GetHeight());
		}

		std::vector<float> originalImgHistoBoat = hist(rawDataBoat);
		ImGui::PlotHistogram("##Orignal Boat Histo2",
			originalImgHistoBoat.data(),           // Pointer to the data
			originalImgHistoBoat.size(),           // Number of data points
			0,                           // Index offset (not needed in this case)
			nullptr,                     // Overlay text
			0.0f,                        // Scale min (0 for automatic scaling)
			*std::max_element(originalImgHistoBoat.begin(), originalImgHistoBoat.end()), // Scale max
			ImVec2(boat_img->GetWidth(), 80)                 // Graph size (0 for automatic sizing)
		);

		ImGui::SameLine();

		if (boat_spec_opt.has_value()) {
			std::vector<float> specifiedImgHistoBoat = hist(boat_spec_opt.value()->GetRawData());
			ImGui::PlotHistogram("##Specified Boat Histo",
				specifiedImgHistoBoat.data(),           // Pointer to the data
				specifiedImgHistoBoat.size(),           // Number of data points
				0,                           // Index offset (not needed in this case)
				nullptr,                     // Overlay text
				0.0f,                        // Scale min (0 for automatic scaling)
				*std::max_element(specifiedImgHistoBoat.begin(), specifiedImgHistoBoat.end()), // Scale max
				ImVec2(boat_img->GetWidth(), 80)                 // Graph size (0 for automatic sizing)
			);
		}

		ImGui::SameLine();

		std::vector<float> originalImgHistof16 = hist(rawDataf16);
		ImGui::PlotHistogram("##Orignal f16 Histo2",
			originalImgHistof16.data(),           // Pointer to the data
			originalImgHistof16.size(),           // Number of data points
			0,                           // Index offset (not needed in this case)
			nullptr,                     // Overlay text
			0.0f,                        // Scale min (0 for automatic scaling)
			*std::max_element(originalImgHistof16.begin(), originalImgHistof16.end()), // Scale max
			ImVec2(f16_img->GetWidth(), 80)                 // Graph size (0 for automatic sizing)
		);

		ImGui::SameLine();

		if (f16_spec_opt.has_value()) {
			std::vector<float> specifiedImgHistof16 = hist(f16_spec_opt.value()->GetRawData());
			ImGui::PlotHistogram("##Specified f16 Histo",
				specifiedImgHistof16.data(),           // Pointer to the data
				specifiedImgHistof16.size(),           // Number of data points
				0,                           // Index offset (not needed in this case)
				nullptr,                     // Overlay text
				0.0f,                        // Scale min (0 for automatic scaling)
				*std::max_element(specifiedImgHistof16.begin(), specifiedImgHistof16.end()), // Scale max
				ImVec2(f16_img->GetWidth(), 80)                 // Graph size (0 for automatic sizing)
			);
		}

		ImGui::Text("Original: ");
		ImGui::SameLine();

		//ImGui::Dummy(,)

		float x_offset = 2 * style.ItemSpacing.x + ((float)boat_img->GetWidth() - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
		ImGui::Text("Specified: ");
		ImGui::SameLine();

		ImGui::SetCursorPosX(f16_x);
		ImGui::Text("Original: ");
		ImGui::SameLine();

		ImGui::SetCursorPosX(f16_x + f16_img->GetWidth() + style.ItemSpacing.x);
		ImGui::Text("Specified: ");

		ImGui::Separator();

		std::vector<std::string> items2 = { "boat", "aerial", "f_16", "lenna", "tools", "peppers", "sf", "wheel", "lax" };
		static int item_current_idx1 = 5;
		ImGui::SetNextItemWidth(100);
		if (ImGui::BeginCombo("Specified Histogram##specification1", items2[item_current_idx1].c_str()))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			for (int n = 0; n < items2.size(); n++)
			{
				const bool is_selected2 = (item_current_idx1 == n);
				if (ImGui::Selectable(items2[n].c_str(), is_selected2))
				{
					item_current_idx1 = n;
					spec1 = items2[n]; // Or simply use items[n] if specifiedHisto is a std::string
					do_specification("boat", spec1, rawDataBoat, boat_img->GetWidth(), boat_img->GetHeight());
				}
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected2)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();

		ImGui::SetCursorPosX(f16_x);
		static int item_current_idx2 = 4;
		ImGui::SetNextItemWidth(100);
		if (ImGui::BeginCombo("Specified Histogram##specification2", items2[item_current_idx2].c_str()))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			for (int n = 0; n < items2.size(); n++)
			{
				const bool is_selected2 = (item_current_idx2 == n);
				if (ImGui::Selectable(items2[n].c_str(), is_selected2))
				{
					item_current_idx2 = n;
					spec2 = items2[n]; // Or simply use items[n] if specifiedHisto is a std::string
					do_specification("f_16", spec2, rawDataf16, f16_img->GetWidth(), f16_img->GetHeight());
				}
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected2)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
}
} // namespace cs474
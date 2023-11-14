#include "..\cs474.pch.h"
#include "Assignment2.h"
namespace cs474 {
Assignment2::Assignment2() {
}
Assignment2::~Assignment2() {
}
void Assignment2::OnAttach() {
	global::UpdateResource("g_Assignment2ScrollToCorrelation", false);
	global::UpdateResource("g_Assignment2ScrollToAveraging and Gaussian Smoothing", false);
	global::UpdateResource("g_Assignment2ScrollToMedian Filtering", false);
	global::UpdateResource("g_Assignment2ScrollToUnsharp Masking and High Boost Filtering", false);
	global::UpdateResource("g_Assignment2ScrollToGradient and Laplacian", false);

	this->DoSpatialFiltering();
}
void Assignment2::OnDetach() {
}
void Assignment2::OnUpdate(float ts) {
}
void Assignment2::OnUIRender() {
	if (global::GetResourceUnwrapped("g_ShowAssignment2")) {
		bool open = true;
		ImGuiWindowFlags window_flags = utils::GetBaseLayerWindowFlags();
		//ImGuiWindowClass wc;
		//wc.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_NoUndocking;
		//ImGui::SetNextWindowClass(&wc);

		ImGui::Begin("Assignment 2", &open);
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

			this->Question5();
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::NewLine();

		}
		ImGui::End();

		if (!open) global::UpdateResource("g_ShowAssignment2", false);
		//this->Test();
	}
}

void Assignment2::Question1() {
	if (global::GetResourceUnwrapped("g_Assignment2ScrollToCorrelation")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment2ScrollToCorrelation", false);
	}
	widgets::markdown("# 1. Correlation");

	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");

	const std::optional<graphics::Texture>& og_opt = image_registry->GetTexture("ImagePadded", ".pgm");
	const std::optional<graphics::Texture>& filt_opt = image_registry->GetTexture("ImagePadded", "filt");
	if (og_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		ImVec2 start = ImGui::GetCursorScreenPos();
		ImVec2 start_pos(start.x + 28, start.y + 52);
		ImVec2 start_text(start.x + 28, start.y + 52 - ImGui::GetFontSize());
		ImVec2 end_pos(start_pos.x + 84, start_pos.y + 53);
		ImGui::Image((void*)(intptr_t)(og_opt.value()->GetRendererID()), ImVec2(444, 333));
		//ImGui::Image((void*)(intptr_t)(filt_opt.value()->GetRendererID()), ImVec2(444, 333));
		if (ImGui::IsWindowFocused()) {
			ImGui::GetForegroundDrawList()->AddRect(start_pos, end_pos, ImU32(0xff0000ff));
			ImGui::GetForegroundDrawList()->AddText(start_text, ImU32(0xff0000ff), "Mask");
		}
	}
	static float size = 32.0f;
	static float thickness = 8.0f;
	if (filt_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		static bool inspect_filt = false;
		ImGui::SameLine();
		ImVec2 start = ImGui::GetCursorScreenPos();
		inspect_filt = widgets::ImageInspector("inspectfilt", filt_opt.value(), &inspect_filt, { 0.0f, 0.0f }, { style.ItemSpacing.x + 444, 0.0f });
		inspect_filt = false;
		if (ImGui::IsWindowFocused()) {
			this->DoMaskAnimation(start);
		}
	}
	else { 
		TryMakeSpatialFilteringTexture(false);
		ImGui::SameLine();
		static float nextdot = 0, nextdot2;
		static bool show_number = false;
		static float velocity = 1.f;
		nextdot -= 0.07f;
		float start_x = ImGui::GetCursorPosX();
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + (444 / 2.0f) - (size / 2.0f), ImGui::GetCursorPosY() + (333 / 2.0f) - (size / 2.0f)));
		float start_y = ImGui::GetCursorPosY();
		ImSpinner::SpinnerDots("spinner0", &nextdot, size, thickness);
		ImGui::SameLine();
		const auto& style = ImGui::GetStyle();
		std::string loading = "Processing spatial filter ...";
		ImGui::SetCursorPosX(start_x + (444 / 2.0f) - (ImGui::GetFontSize() * (loading.size() / 8.0f)));
		ImGui::SetCursorPosY(start_y + (size * 2.0f) + ImGui::GetFontSize());
		ImGui::Text(loading.c_str());
	}
}

void Assignment2::DoMaskAnimation(ImVec2 start) {
	static float pixps = 888.0f;
	static float x = 0.0f;
	static float y = 0.0f;
	ImVec2 offset{x, y};
	ImVec2 start_pos(start.x + offset.x, start.y + offset.y);
	ImVec2 end_pos(start_pos.x + 84, start_pos.y + 53);
	//70 76
	//133 188
	//379 268

	x += (pixps * ImGui::GetIO().DeltaTime);

	if (x >= 444.0f) {
		y += 22.0f;
		x = 0.0f;
	}
	if (y >= 333.0f) {
		y = 0.0f;
		x = 0.0f;
	}

	if ((y >= (77 - (53 / 2))) || ((y == (76 - (53 / 2))) && (x >= (70 - (84 / 2))))) {
		ImVec2 start_pos_c(start.x + 70 - (84 / 2.0), start.y + 76 - (53 / 2.0));
		ImVec2 start_text_c(start_pos_c.x - 28, start_pos_c.y - (52 - ImGui::GetFontSize()));
		ImVec2 end_pos_c(start_pos_c.x + 84, start_pos_c.y + 53);
		ImGui::GetForegroundDrawList()->AddRect(start_pos_c, end_pos_c, ImU32(0xffffffff));
		ImGui::GetForegroundDrawList()->AddText(start_text_c, ImU32(0xffffffff), "(70, 76)");
	}
	if ((y >= (189 - (53 / 2))) || ((y == (188 - (53 / 2))) && (x >= (133 - (84 / 2))))) {
		ImVec2 start_pos_c(start.x + 133 - (84 / 2), start.y + 189 - (53 / 2));
		ImVec2 start_text_c(start_pos_c.x - 28, start_pos_c.y - (52 - ImGui::GetFontSize()));
		ImVec2 end_pos_c(start_pos_c.x + 84, start_pos_c.y + 53);
		ImGui::GetForegroundDrawList()->AddRect(start_pos_c, end_pos_c, ImU32(0xffffffff));
		ImGui::GetForegroundDrawList()->AddText(start_text_c, ImU32(0xffffffff), "(133, 188)");
	}
	if ((y >= (269 - (53 / 2))) || ((y == (268 - (53 / 2))) && (x >= (379 - (84 / 2))))) {
		ImVec2 start_pos_c(start.x + 379 - (84 / 2), start.y + 268 - (53 / 2));
		ImVec2 start_text_c(start_pos_c.x - 28, start_pos_c.y - (52 - ImGui::GetFontSize()));
		ImVec2 end_pos_c(start_pos_c.x + 84, start_pos_c.y + 53);
		ImGui::GetForegroundDrawList()->AddRect(start_pos_c, end_pos_c, ImU32(0xffffffff));
		ImGui::GetForegroundDrawList()->AddText(start_text_c, ImU32(0xffffffff), "(379, 268)");
	}

	ImVec2 start_text(start_pos.x, start_pos.y - ImGui::GetFontSize());
	ImGui::GetForegroundDrawList()->AddRect(start_pos, end_pos, ImU32(0xff0000ff));
	ImGui::GetForegroundDrawList()->AddText(start_text, ImU32(0xff0000ff), "Mask");
}

void Assignment2::Question2() {
	if (global::GetResourceUnwrapped("g_Assignment2ScrollToAveraging and Gaussian Smoothing")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment2ScrollToAveraging and Gaussian Smoothing", false);
	}
	widgets::markdown("# 2. Averaging and Gaussian Smoothing");

	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& lenna_opt = image_registry->GetTexture("lenna", ".pgm");
	const std::optional<graphics::Texture>& sf_opt = image_registry->GetTexture("sf", ".pgm");

	if (lenna_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img = lenna_opt.value();
		ImVec2 img_size{ (float)lenna_opt.value()->GetWidth(), (float)lenna_opt.value()->GetHeight() };
		is_hovered_avg1 = widgets::ImageInspector("inspect_avg", lenna_opt.value(), &inspect_avg, { 0.0f, 0.0f }, { ((is_hovered_avg2) ? 1.0f : 2.0f) * -1.0f * (style.ItemSpacing.x + lenna_opt.value()->GetWidth()), 0.0f });
		ImGui::SameLine();

		// averaged image
		const std::optional<graphics::Texture>& sub_opt = image_registry->GetTexture("lenna", "average1");
		float start_x = 0.0f;
		if (sub_opt.has_value()) {
			const auto& img_sub = sub_opt.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			start_x = ImGui::GetCursorPosX();
			is_hovered_avg2 = widgets::ImageInspector("inspect_avg2", img_sub, &inspect_avg, { 0.0f, 0.0f }, { ((is_hovered_avg3) ? -1.0f : 1.0f) * (style.ItemSpacing.x + img_sub->GetWidth()), 0.0f });
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SameLine();
		// gaussian image
		const std::optional<graphics::Texture>& sub_opt_blur = image_registry->GetTexture("lenna", "blur1");
		float start_x2 = 0.0f;
		if (sub_opt_blur.has_value()) {
			const auto& img_sub_blur = sub_opt_blur.value();
			ImVec2 img_sub_size{ (float)img_sub_blur->GetWidth(), (float)img_sub_blur->GetHeight() };
			start_x2 = ImGui::GetCursorPosX();
			is_hovered_avg3 = widgets::ImageInspector("inspect_avg3", img_sub_blur, &inspect_avg, { 0.0f, 0.0f }, { ((is_hovered_avg1) ? 2.0f : 1.0f ) * ( style.ItemSpacing.x + img_sub_blur->GetWidth()), 0.0f });
			if ((!is_hovered_avg1) && (!is_hovered_avg2) && (!is_hovered_avg3)) inspect_avg = false;
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

		float x_offset2 = 3 * style.ItemSpacing.x + ((float)img->GetWidth() * 2 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset2);
		ImGui::Text("Gaussian Blurred: ");

		ImGui::Separator();

		auto do_avg_filtering = [&]() {
			if (filterSize == 7) {
				auto averagedData = smoothImage(img->GetRawData(), img->GetWidth(), img->GetHeight(), filterSize, avgMask7x7);
				if (averagedData.has_value()) {
					auto data = averagedData.value();
					bool success = image_registry->AddTexture("lenna", "average1", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
			else {
				auto averagedData = smoothImage(img->GetRawData(), img->GetWidth(), img->GetHeight(), filterSize, avgMask15x15);
				if (averagedData.has_value()) {
					auto data = averagedData.value();
					bool success = image_registry->AddTexture("lenna", "average1", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
		};

		auto do_gaus_filtering = [&]() {
			if (filterSize == 7) {
				auto averagedData = smoothImage(img->GetRawData(), img->GetWidth(), img->GetHeight(), filterSize, gausMask7x7);
				if (averagedData.has_value()) {
					auto data = averagedData.value();
					bool success = image_registry->AddTexture("lenna", "blur1", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
			else {
				auto averagedData = smoothImage(img->GetRawData(), img->GetWidth(), img->GetHeight(), filterSize, gausMask15x15);
				if (averagedData.has_value()) {
					auto data = averagedData.value();
					bool success = image_registry->AddTexture("lenna", "blur1", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
		};

		static bool init_sample = true;
		if (init_sample) {
			do_avg_filtering();
			do_gaus_filtering();
			init_sample = false;
		}

		std::vector<std::string> items = { "7x7", "15x15" };
		static int item_current_idx = 0;
		static int item_current_idx1 = 0;
		ImGui::SetCursorPosX(start_x);
		ImGui::SetNextItemWidth(100);
		if (ImGui::BeginCombo("Avg Filter", items[item_current_idx].c_str()))
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
					do_avg_filtering();
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(start_x2);
		ImGui::SetNextItemWidth(100);
		if (ImGui::BeginCombo("Gaussian Filter", items[item_current_idx1].c_str()))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			for (int n = 0; n < items.size(); n++)
			{
				const bool is_selected = (item_current_idx1 == n);
				if (ImGui::Selectable(items[n].c_str(), is_selected)) {
					item_current_idx1 = n;
					if (items[n] == "7x7") {
						filterSize = 7;
					}
					else {
						filterSize = 15;
					}
					do_gaus_filtering();
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}

	ImGui::NewLine();
	ImGui::NewLine();

	if (sf_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img = sf_opt.value();
		ImVec2 img_size{ (float)sf_opt.value()->GetWidth(), (float)sf_opt.value()->GetHeight() };
		is_hovered_avg4 = widgets::ImageInspector("inspect_avg12", sf_opt.value(), &inspect_avg2, { 0.0f, 0.0f }, { ((is_hovered_avg5) ? 1.0f : 2.0f) * -1.0f * (style.ItemSpacing.x + sf_opt.value()->GetWidth()), 0.0f });
		ImGui::SameLine();

		// averaged image
		const std::optional<graphics::Texture>& sub_opt = image_registry->GetTexture("sf", "average1");
		float start_x = 0.0f;
		if (sub_opt.has_value()) {
			const auto& img_sub = sub_opt.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			start_x = ImGui::GetCursorPosX();
			is_hovered_avg5 = widgets::ImageInspector("inspect_avg22", img_sub, &inspect_avg2, { 0.0f, 0.0f }, { ((is_hovered_avg6) ? -1.0f : 1.0f) * (style.ItemSpacing.x + img_sub->GetWidth()), 0.0f });
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SameLine();
		// gaussian image
		const std::optional<graphics::Texture>& sub_opt_blur = image_registry->GetTexture("sf", "blur1");
		float start_x2 = 0.0f;
		if (sub_opt_blur.has_value()) {
			const auto& img_sub_blur = sub_opt_blur.value();
			ImVec2 img_sub_size{ (float)img_sub_blur->GetWidth(), (float)img_sub_blur->GetHeight() };
			start_x2 = ImGui::GetCursorPosX();
			is_hovered_avg6 = widgets::ImageInspector("inspect_avg23", img_sub_blur, &inspect_avg2, { 0.0f, 0.0f }, { ((is_hovered_avg4) ? 2.0f : 1.0f) * (style.ItemSpacing.x + img_sub_blur->GetWidth()), 0.0f });
			if ((!is_hovered_avg4) && (!is_hovered_avg5) && (!is_hovered_avg6)) inspect_avg2 = false;
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

		float x_offset2 = 3 * style.ItemSpacing.x + ((float)img->GetWidth() * 2 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset2);
		ImGui::Text("Gaussian Blurred: ");

		ImGui::Separator();

		auto do_avg_filtering = [&]() {
			if (filterSize == 7) {
				auto averagedData = smoothImage(img->GetRawData(), img->GetWidth(), img->GetHeight(), filterSize, avgMask7x7);
				if (averagedData.has_value()) {
					auto data = averagedData.value();
					bool success = image_registry->AddTexture("sf", "average1", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
			else {
				auto averagedData = smoothImage(img->GetRawData(), img->GetWidth(), img->GetHeight(), filterSize, avgMask15x15);
				if (averagedData.has_value()) {
					auto data = averagedData.value();
					bool success = image_registry->AddTexture("sf", "average1", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
		};

		auto do_gaus_filtering = [&]() {
			if (filterSize == 7) {
				auto averagedData = smoothImage(img->GetRawData(), img->GetWidth(), img->GetHeight(), filterSize, gausMask7x7);
				if (averagedData.has_value()) {
					auto data = averagedData.value();
					bool success = image_registry->AddTexture("sf", "blur1", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
			else {
				auto averagedData = smoothImage(img->GetRawData(), img->GetWidth(), img->GetHeight(), filterSize, gausMask15x15);
				if (averagedData.has_value()) {
					auto data = averagedData.value();
					bool success = image_registry->AddTexture("sf", "blur1", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
		};

		static bool init_sample = true;
		if (init_sample) {
			do_avg_filtering();
			do_gaus_filtering();
			init_sample = false;
		}

		std::vector<std::string> items = { "7x7", "15x15" };
		static int item_current_idx = 0;
		static int item_current_idx1 = 0;
		ImGui::SetCursorPosX(start_x);
		ImGui::SetNextItemWidth(100);
		if (ImGui::BeginCombo("Avg Filter##sfblur", items[item_current_idx].c_str()))
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
					do_avg_filtering();
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(start_x2);
		ImGui::SetNextItemWidth(100);
		if (ImGui::BeginCombo("Gaussian Filter##sfblur", items[item_current_idx1].c_str()))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			for (int n = 0; n < items.size(); n++)
			{
				const bool is_selected = (item_current_idx1 == n);
				if (ImGui::Selectable(items[n].c_str(), is_selected)) {
					item_current_idx1 = n;
					if (items[n] == "7x7") {
						filterSize = 7;
					}
					else {
						filterSize = 15;
					}
					do_gaus_filtering();
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

	}
}

void Assignment2::Question3() {
	if (global::GetResourceUnwrapped("g_Assignment2ScrollToMedian Filtering")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment2ScrollToMedian Filtering", false);
	}
	widgets::markdown("# 3. Median Filtering");

	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& img_opt = image_registry->GetTexture("lenna", ".pgm");
	const std::optional<graphics::Texture>& img_opt_boat = image_registry->GetTexture("boat", ".pgm");

	if (img_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img = img_opt.value();
		const std::vector<uint8_t>& rawData = img->GetRawData();
		ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
		float x_off_coeff = ((is_hovered_med4) ? -3.0f : ((is_hovered_med3) ? -2.0f : -1.0f));
		is_hovered_med1 = widgets::ImageInspector("inspect_med1", img, &inspect_med, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img->GetWidth()), 0.0f });
		ImGui::SameLine();

		// Computed image
		const std::optional<graphics::Texture>& sub_opt = image_registry->GetTexture("lenna", "salty");
		const std::optional<graphics::Texture>& sub_median = image_registry->GetTexture("lenna", "median");
		const std::optional<graphics::Texture>& sub_average = image_registry->GetTexture("lenna", "average2");

		float start_x = 0.0f;
		if (sub_opt.has_value()) {
			const auto& img_sub = sub_opt.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			start_x = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_med4) ? -2.0f : ((is_hovered_med3) ? -1.0f : 1.0f));
			is_hovered_med2 = widgets::ImageInspector("inspect_med2", img_sub, &inspect_med, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sub->GetWidth()), 0.0f });
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SameLine();
		float start_x2 = 0.0f;
		if (sub_median.has_value()) {
			const auto& img_sub = sub_median.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			start_x2 = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_med4) ? -1.0f : ((is_hovered_med1) ? 2.0f : 1.0f));
			is_hovered_med3 = widgets::ImageInspector("inspect_med3", img_sub, &inspect_med, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sub->GetWidth()), 0.0f });
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SameLine();
		float start_x3 = 0.0f;
		if (sub_average.has_value()) {
			const auto& img_sub = sub_average.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			x_off_coeff = ((is_hovered_med1) ? 3.0f : ((is_hovered_med2) ? 2.0f : 1.0f));
			start_x3 = ImGui::GetCursorPosX();
			is_hovered_med4 = widgets::ImageInspector("inspect_med4", img_sub, &inspect_med, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sub->GetWidth()), 0.0f });
			if ((!is_hovered_med1) && (!is_hovered_med2) && (!is_hovered_med3) && (!is_hovered_med4)) inspect_med = false;
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

		float x_offset2 = 3 * style.ItemSpacing.x + ((float)img->GetWidth() * 2 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset2);
		ImGui::Text("Median Filtered: ");

		ImGui::SameLine();

		float x_offset3 = 4 * style.ItemSpacing.x + ((float)img->GetWidth() * 3 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset3);
		ImGui::Text("Average Filtered: ");

		ImGui::Separator();

		auto do_snp = [&]() {
			auto averagedData = addNoise(img->GetRawData(), img->GetWidth(), img->GetHeight(), percentSaltPep);
			if (averagedData.has_value()) {
				auto data = averagedData.value();
				bool success = image_registry->AddTexture("lenna", "salty", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);
			}
		};

		auto do_med_filtering = [&]() {
			const std::optional<graphics::Texture>& sub_optNew = image_registry->GetTexture("lenna", "salty");

			if (filterSizeMed == 7) {
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
		};

		auto do_avg_filtering = [&]() {
			const std::optional<graphics::Texture>& sub_optNew = image_registry->GetTexture("lenna", "median");
			if (filterSizeAvg == 7) {
				auto averagedData = smoothImage(sub_optNew.value()->GetRawData(), sub_optNew.value()->GetWidth(), sub_optNew.value()->GetHeight(), filterSizeAvg, avgMask7x7);
				if (averagedData.has_value()) {
					auto data = averagedData.value();
					bool success = image_registry->AddTexture("lenna", "average2", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
			else {
				auto medianData = smoothImage(sub_optNew.value()->GetRawData(), sub_optNew.value()->GetWidth(), sub_optNew.value()->GetHeight(), filterSizeAvg, avgMask15x15);
				if (medianData.has_value()) {
					auto data = medianData.value();
					bool success = image_registry->AddTexture("lenna", "average2", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
		};

		static bool init_sample = true;
		if (init_sample) {
			do_snp();
			do_med_filtering();
			do_avg_filtering();

			if (init_sample) {
				init_sample = false;
			}
		}

		//Creating a Combo menu for the factor choices
		std::vector<std::string> items = { "10%", "30%", "50%" };
		static int item_current_idx = 0;
		ImGui::SetNextItemWidth(100);
		ImGui::SetCursorPosX(start_x);
		if (ImGui::BeginCombo("% Noise##med1", items[item_current_idx].c_str()))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			for (int n = 0; n < items.size(); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n].c_str(), is_selected)) {
					item_current_idx = n;
					percentSaltPep = atof(items[n].c_str());
					do_snp();
					do_med_filtering();
					do_avg_filtering();
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();

		ImGui::SetCursorPosX(start_x2);
		std::vector<std::string> items2 = { "7x7", "15x15" };
		static int item_current_idx2 = 0;
		ImGui::SetNextItemWidth(100);
		if (ImGui::BeginCombo("Median Filter Size##med1", items2[item_current_idx2].c_str()))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			for (int n = 0; n < items2.size(); n++)
			{
				const bool is_selected = (item_current_idx2 == n);
				if (ImGui::Selectable(items2[n].c_str(), is_selected)) {
					item_current_idx2 = n;
					if (items2[n] == "7x7") {
						filterSizeMed = 7;
					}
					else {
						filterSizeMed = 15;
					}
					do_med_filtering();
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();

		static int item_current_idx3 = 0;
		ImGui::SetNextItemWidth(100);
		ImGui::SetCursorPosX(start_x3);
		if (ImGui::BeginCombo("Average Filter Size##med1", items2[item_current_idx3].c_str()))
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
					do_avg_filtering();
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}

	ImGui::NewLine();
	ImGui::NewLine();

	if (img_opt_boat.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img = img_opt_boat.value();
		const std::vector<uint8_t>& rawData = img->GetRawData();
		ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
		float x_off_coeff = ((is_hovered_med8) ? -3.0f : ((is_hovered_med7) ? -2.0f : -1.0f));
		is_hovered_med5 = widgets::ImageInspector("inspect_med11", img, &inspect_med2, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img->GetWidth()), 0.0f });
		ImGui::SameLine();

		// Computed image
		const std::optional<graphics::Texture>& sub_opt = image_registry->GetTexture("boat", "salty");
		const std::optional<graphics::Texture>& sub_median = image_registry->GetTexture("boat", "median");
		const std::optional<graphics::Texture>& sub_average = image_registry->GetTexture("boat", "average2");

		float start_x = 0.0f;
		if (sub_opt.has_value()) {
			const auto& img_sub = sub_opt.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			start_x = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_med8) ? -2.0f : ((is_hovered_med7) ? -1.0f : 1.0f));
			is_hovered_med6 = widgets::ImageInspector("inspect_med22", img_sub, &inspect_med2, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sub->GetWidth()), 0.0f });
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SameLine();
		float start_x2 = 0.0f;
		if (sub_median.has_value()) {
			const auto& img_sub = sub_median.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			start_x2 = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_med8) ? -1.0f : ((is_hovered_med5) ? 2.0f : 1.0f));
			is_hovered_med7 = widgets::ImageInspector("inspect_med33", img_sub, &inspect_med2, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sub->GetWidth()), 0.0f });
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SameLine();
		float start_x3 = 0.0f;
		if (sub_average.has_value()) {
			const auto& img_sub = sub_average.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			x_off_coeff = ((is_hovered_med5) ? 3.0f : ((is_hovered_med6) ? 2.0f : 1.0f));
			start_x3 = ImGui::GetCursorPosX();
			is_hovered_med8 = widgets::ImageInspector("inspect_med44", img_sub, &inspect_med2, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sub->GetWidth()), 0.0f });
			if ((!is_hovered_med5) && (!is_hovered_med6) && (!is_hovered_med7) && (!is_hovered_med8)) inspect_med2 = false;
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

		float x_offset2 = 3 * style.ItemSpacing.x + ((float)img->GetWidth() * 2 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset2);
		ImGui::Text("Median Filtered: ");

		ImGui::SameLine();

		float x_offset3 = 4 * style.ItemSpacing.x + ((float)img->GetWidth() * 3 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset3);
		ImGui::Text("Average Filtered: ");

		ImGui::Separator();

		auto do_snp = [&]() {
			auto averagedData = addNoise(img->GetRawData(), img->GetWidth(), img->GetHeight(), percentSaltPep2);
			if (averagedData.has_value()) {
				auto data = averagedData.value();
				bool success = image_registry->AddTexture("boat", "salty", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);
			}
		};

		auto do_med_filtering = [&]() {
			const std::optional<graphics::Texture>& sub_optNew = image_registry->GetTexture("boat", "salty");

			if (filterSizeMed2 == 7) {
				auto medianData = medianFilter(sub_optNew.value()->GetRawData(), sub_optNew.value()->GetWidth(), sub_optNew.value()->GetHeight(), 7);
				if (medianData.has_value()) {
					auto data = medianData.value();
					bool success = image_registry->AddTexture("boat", "median", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
			else {
				auto medianData = medianFilter(sub_optNew.value()->GetRawData(), sub_optNew.value()->GetWidth(), sub_optNew.value()->GetHeight(), 15);
				if (medianData.has_value()) {
					auto data = medianData.value();
					bool success = image_registry->AddTexture("boat", "median", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
		};

		auto do_avg_filtering = [&]() {
			const std::optional<graphics::Texture>& sub_optNew = image_registry->GetTexture("boat", "median");
			if (filterSizeAvg2 == 7) {
				auto averagedData = smoothImage(sub_optNew.value()->GetRawData(), sub_optNew.value()->GetWidth(), sub_optNew.value()->GetHeight(), filterSizeAvg, avgMask7x7);
				if (averagedData.has_value()) {
					auto data = averagedData.value();
					bool success = image_registry->AddTexture("boat", "average2", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
			else {
				auto medianData = smoothImage(sub_optNew.value()->GetRawData(), sub_optNew.value()->GetWidth(), sub_optNew.value()->GetHeight(), filterSizeAvg, avgMask15x15);
				if (medianData.has_value()) {
					auto data = medianData.value();
					bool success = image_registry->AddTexture("boat", "average2", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
		};

		static bool init_sample = true;
		if (init_sample) {
			do_snp();
			do_med_filtering();
			do_avg_filtering();

			if (init_sample) {
				init_sample = false;
			}
		}

		//Creating a Combo menu for the factor choices
		std::vector<std::string> items = { "10%", "30%", "50%" };
		static int item_current_idx = 0;
		ImGui::SetNextItemWidth(100);
		ImGui::SetCursorPosX(start_x);
		if (ImGui::BeginCombo("% Noise##med2", items[item_current_idx].c_str()))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			for (int n = 0; n < items.size(); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n].c_str(), is_selected)) {
					item_current_idx = n;
					percentSaltPep2 = atof(items[n].c_str());
					do_snp();
					do_med_filtering();
					do_avg_filtering();
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();

		ImGui::SetCursorPosX(start_x2);
		std::vector<std::string> items2 = { "7x7", "15x15" };
		static int item_current_idx2 = 0;
		ImGui::SetNextItemWidth(100);
		if (ImGui::BeginCombo("Median Filter Size##med2", items2[item_current_idx2].c_str()))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			for (int n = 0; n < items2.size(); n++)
			{
				const bool is_selected = (item_current_idx2 == n);
				if (ImGui::Selectable(items2[n].c_str(), is_selected)) {
					item_current_idx2 = n;
					if (items2[n] == "7x7") {
						filterSizeMed2 = 7;
					}
					else {
						filterSizeMed2 = 15;
					}
					do_med_filtering();
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();

		static int item_current_idx3 = 0;
		ImGui::SetNextItemWidth(100);
		ImGui::SetCursorPosX(start_x3);
		if (ImGui::BeginCombo("Average Filter Size##med2", items2[item_current_idx3].c_str()))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			for (int n = 0; n < items2.size(); n++)
			{
				const bool is_selected = (item_current_idx3 == n);
				if (ImGui::Selectable(items2[n].c_str(), is_selected)) {
					item_current_idx3 = n;
					if (items2[n] == "7x7") {
						filterSizeAvg2 = 7;
					}
					else {
						filterSizeAvg2 = 15;
					}
					do_avg_filtering();
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
}

void Assignment2::Question4() {
	if (global::GetResourceUnwrapped("g_Assignment2ScrollToUnsharp Masking and High Boost Filtering")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment2ScrollToUnsharp Masking and High Boost Filtering", false);
	}
	widgets::markdown("# 4. Unsharp Masking and High Boost Filtering");

	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& img_opt = image_registry->GetTexture("lenna", ".pgm");
	const std::optional<graphics::Texture>& img_opt_f16 = image_registry->GetTexture("f_16", ".pgm");

	if (img_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img = img_opt.value();
		const std::vector<uint8_t>& rawData = img->GetRawData();
		ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
		float x_off_coeff = ((is_hovered_hbf4) ? -3.0f : ((is_hovered_hbf3) ? -2.0f : -1.0f));
		is_hovered_hbf1 = widgets::ImageInspector("inspect_hbf1", img, &inspect_hbf, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img->GetWidth()), 0.0f });
		ImGui::SameLine();

		// Computed image
		const std::optional<graphics::Texture>& sub_opt = image_registry->GetTexture("lenna", "gaus2");
		if (sub_opt.has_value()) {
			const auto& img_sub = sub_opt.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			x_off_coeff = ((is_hovered_hbf4) ? -2.0f : ((is_hovered_hbf3) ? -1.0f : 1.0f));
			is_hovered_hbf2 = widgets::ImageInspector("inspect_hbf2", img_sub, &inspect_hbf, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sub->GetWidth()), 0.0f });
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SameLine();

		const std::optional<graphics::Texture>& sub_opt_unsharp = image_registry->GetTexture("lenna", "unsharp");

		if (sub_opt_unsharp.has_value()) {
			const auto& img_sub = sub_opt_unsharp.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			x_off_coeff = ((is_hovered_hbf4) ? -1.0f : ((is_hovered_hbf1) ? 2.0f : 1.0f));
			is_hovered_hbf3 = widgets::ImageInspector("inspect_hbf3", img_sub, &inspect_hbf, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sub->GetWidth()), 0.0f });
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SameLine();

		const std::optional<graphics::Texture>& sub_opt_boost = image_registry->GetTexture("lenna", "boost");

		if (sub_opt_boost.has_value()) {
			const auto& img_sub = sub_opt_boost.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			x_off_coeff = ((is_hovered_hbf1) ? 3.0f : ((is_hovered_hbf2) ? 2.0f : 1.0f));
			is_hovered_hbf4 = widgets::ImageInspector("inspect_hbf4", img_sub, &inspect_hbf, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sub->GetWidth()), 0.0f });
			if ((!is_hovered_hbf1) && (!is_hovered_hbf2) && (!is_hovered_hbf3) && (!is_hovered_hbf4)) inspect_hbf = false;
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
				image_registry->AddTexture("lenna", "gaus2", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
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
	}

	ImGui::NewLine();
	ImGui::NewLine();

	if (img_opt_f16.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img = img_opt_f16.value();
		const std::vector<uint8_t>& rawData = img->GetRawData();
		ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
		float x_off_coeff = ((is_hovered_hbf8) ? -3.0f : ((is_hovered_hbf7) ? -2.0f : -1.0f));
		is_hovered_hbf5 = widgets::ImageInspector("inspect_hbf11", img, &inspect_hbf2, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img->GetWidth()), 0.0f });
		ImGui::SameLine();

		// Computed image
		const std::optional<graphics::Texture>& sub_opt = image_registry->GetTexture("f_16", "gaus2");
		if (sub_opt.has_value()) {
			const auto& img_sub = sub_opt.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			x_off_coeff = ((is_hovered_hbf8) ? -2.0f : ((is_hovered_hbf7) ? -1.0f : 1.0f));
			is_hovered_hbf6 = widgets::ImageInspector("inspect_hbf22", img_sub, &inspect_hbf2, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sub->GetWidth()), 0.0f });
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SameLine();

		const std::optional<graphics::Texture>& sub_opt_unsharp = image_registry->GetTexture("f_16", "unsharp");

		if (sub_opt_unsharp.has_value()) {
			const auto& img_sub = sub_opt_unsharp.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			x_off_coeff = ((is_hovered_hbf8) ? -1.0f : ((is_hovered_hbf5) ? 2.0f : 1.0f));
			is_hovered_hbf7 = widgets::ImageInspector("inspect_hbf33", img_sub, &inspect_hbf2, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sub->GetWidth()), 0.0f });
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SameLine();

		const std::optional<graphics::Texture>& sub_opt_boost = image_registry->GetTexture("f_16", "boost");

		if (sub_opt_boost.has_value()) {
			const auto& img_sub = sub_opt_boost.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			x_off_coeff = ((is_hovered_hbf5) ? 3.0f : ((is_hovered_hbf6) ? 2.0f : 1.0f));
			is_hovered_hbf8 = widgets::ImageInspector("inspect_hbf44", img_sub, &inspect_hbf2, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sub->GetWidth()), 0.0f });
			if ((!is_hovered_hbf5) && (!is_hovered_hbf6) && (!is_hovered_hbf7) && (!is_hovered_hbf8)) inspect_hbf2 = false;
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
				image_registry->AddTexture("f_16", "gaus2", graphics::make_texture(data, img->GetWidth(), img->GetHeight(), 1));
				auto boostData = unsharpAndBoostFilter(img->GetRawData(), data, img->GetHeight(), img->GetWidth(), 5);
				auto unsharpData = unsharpAndBoostFilter(img->GetRawData(), data, img->GetHeight(), img->GetWidth(), 1);
				if (unsharpData.has_value() && boostData.has_value()) {
					image_registry->AddTexture("f_16", "boost", graphics::make_texture(boostData.value(), img->GetWidth(), img->GetHeight(), 1));
					image_registry->AddTexture("f_16", "unsharp", graphics::make_texture(unsharpData.value(), img->GetWidth(), img->GetHeight(), 1));
				}
				if (init_sample2) {
					init_sample2 = false;
				}
			}
		}
	}
}

void Assignment2::Question5() {
	if (global::GetResourceUnwrapped("g_Assignment2ScrollToGradient and Laplacian")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment2ScrollToGradient and Laplacian", false);
	}
	widgets::markdown("# 5. Gradient and Laplacian");
	widgets::markdown("## Prewitt Mask");

	this->DoPrewittLenna();
	ImGui::NewLine();
	ImGui::NewLine();

	this->DoPrewittSf();

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::NewLine();

	widgets::markdown("## Sobel Mask");

	this->DoSobelLenna();
	ImGui::NewLine();
	ImGui::NewLine();

	this->DoSobelSf();

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::NewLine();

	widgets::markdown("## Laplacian Mask");

	this->DoLaplacianLenna();
	ImGui::NewLine();
	ImGui::NewLine();

	this->DoLaplacianSf();
}

void Assignment2::DoSpatialFiltering() {
	emscripten_wasm_worker_t worker = emscripten_malloc_wasm_worker(/*stackSize: */1024);
	// wtf? - @Stehfyn 11/13/23
	// emscripten_wasm_worker_post_function_sig(worker, reinterpret_cast<std::function<void()>*>(&ProcessSpatialFilteringData), "()");
	emscripten_wasm_worker_post_function_v(worker, ProcessSpatialFilteringData);
}

void Assignment2::DoPrewittLenna() {
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& lenna_opt = image_registry->GetTexture("lenna", ".pgm");

	static bool do_again = false;
	static bool threshold_ = false;
	static bool lock_range_size = false;
	static float range_size = 0.10f;
	static bool above = true;

	float start_x = 0.0f;
	float start_x2 = 0.0f;
	float start_x3 = 0.0f;
	if (lenna_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img_lenna = lenna_opt.value();
		const std::vector<uint8_t>& rawDataLenna = img_lenna->GetRawData();

		ImVec2 img_size{ (float)img_lenna->GetWidth(), (float)img_lenna->GetHeight() };
		float x_off_coeff = ((is_hovered_lap14) ? -3.0f : ((is_hovered_lap13) ? -2.0f : -1.0f));
		is_hovered_lap11 = widgets::ImageInspector("inspect_lap11", img_lenna, &inspect_lap1, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
		ImGui::SameLine();

		const std::optional<graphics::Texture>& lenna_prewitt_opt = image_registry->GetTexture("lenna", "prewitt");
		const std::optional<graphics::Texture>& lenna_prewitt_partials1_opt = image_registry->GetTexture("lenna", "prewitt-partials1");
		const std::optional<graphics::Texture>& lenna_prewitt_partials2_opt = image_registry->GetTexture("lenna", "prewitt-partials2");

		if (lenna_prewitt_opt.has_value() && lenna_prewitt_partials1_opt.has_value() && lenna_prewitt_partials2_opt.has_value() && (!do_again)) {
			const auto& img_lenna_prewitt = lenna_prewitt_opt.value();
			const auto& img_lenna_prewitt_partials1 = lenna_prewitt_partials1_opt.value();
			const auto& img_lenna_prewitt_partials2 = lenna_prewitt_partials2_opt.value();
			start_x = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap14) ? -2.0f : ((is_hovered_lap13) ? -1.0f : 1.0f));
			is_hovered_lap12 = widgets::ImageInspector("inspect_lap12", img_lenna_prewitt_partials1, &inspect_lap1, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
			ImGui::SameLine();
			start_x2 = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap14) ? -1.0f : ((is_hovered_lap11) ? 2.0f : 1.0f));
			is_hovered_lap13 = widgets::ImageInspector("inspect_lap13", img_lenna_prewitt_partials2, &inspect_lap1, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
			ImGui::SameLine();
			start_x3 = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap11) ? 3.0f : ((is_hovered_lap12) ? 2.0f : 1.0f));
			is_hovered_lap14 = widgets::ImageInspector("inspect_lap14", img_lenna_prewitt, &inspect_lap1, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
			if ((!is_hovered_lap11) && (!is_hovered_lap12) && (!is_hovered_lap13) && (!is_hovered_lap14)) inspect_lap1 = false;
			//ImGui::SameLine();
		}
		else {
			std::vector<float> prewitt_x = convolve(rawDataLenna, (int)img_lenna->GetWidth(), (int)img_lenna->GetHeight(), prewittHorz, 3, 3);
			std::vector<float> prewitt_y = convolve(rawDataLenna, (int)img_lenna->GetWidth(), (int)img_lenna->GetHeight(), prewittVert, 3, 3);
			auto magnitude = gradient_magnitude(prewitt_x, prewitt_y, (int)img_lenna->GetWidth(), (int)img_lenna->GetHeight());

			std::vector<uint8_t> normed = normr(magnitude);
			auto partials1 = normr(prewitt_x);
			auto partials2 = normr(prewitt_y);

			if (threshold_) {
				std::vector<uint8_t> normedt = threshold_range(normed, (int)(thresh_min_prewitt1 * 255), (int)(thresh_max_prewitt1 * 255), above);
				auto partials1t = threshold_range(partials1, (int)(thresh_min_prewitt1 * 255), (int)(thresh_max_prewitt1 * 255), above);
				auto partials2t = threshold_range(partials2, (int)(thresh_min_prewitt1 * 255), (int)(thresh_max_prewitt1 * 255), above);

				image_registry->AddTexture("lenna", "prewitt-partials1", graphics::make_texture(partials1t, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
				image_registry->AddTexture("lenna", "prewitt-partials2", graphics::make_texture(partials2t, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
				image_registry->AddTexture("lenna", "prewitt", graphics::make_texture(normedt, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
			}
			else {
				image_registry->AddTexture("lenna", "prewitt-partials1", graphics::make_texture(partials1, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
				image_registry->AddTexture("lenna", "prewitt-partials2", graphics::make_texture(partials2, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
				image_registry->AddTexture("lenna", "prewitt", graphics::make_texture(normed, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
			}

			start_x = ImGui::GetCursorPosX();
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			ImGui::SameLine();
			start_x2 = ImGui::GetCursorPosX();
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			ImGui::SameLine();
			start_x3 = ImGui::GetCursorPosX();
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			do_again = false;
		}

		ImGui::Text("Original: ");

		ImGui::SameLine();

		float x_offset = 2 * style.ItemSpacing.x + ((float)img_lenna->GetWidth() - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
		ImGui::Text("Partial Derivative (x): ");

		ImGui::SameLine();

		float x_offset2 = 3 * style.ItemSpacing.x + ((float)img_lenna->GetWidth() * 2 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset2);
		ImGui::Text("Partial Derivative (y): ");

		ImGui::SameLine();

		float x_offset3 = 4 * style.ItemSpacing.x + ((float)img_lenna->GetWidth() * 3 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset3);
		ImGui::Text("Gradient Magnitude: ");

		ImGui::Separator();

		if (ImGui::Checkbox("Threshold", &threshold_)) {
			do_again = true;
		}

		ImGui::SameLine();

		if (!threshold_) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ImGui::Checkbox("Flip B/W", &above)) {
			do_again = true;
		}

		ImGui::SameLine();

		auto lock_ranges = [&]() {
			if (std::abs(thresh_max_prewitt1 - thresh_min_prewitt1) < range_size) {
				thresh_max_prewitt1 = std::clamp(thresh_min_prewitt1 + range_size, 0.0f, 1.0f);
				if (std::abs(thresh_max_prewitt1 - thresh_min_prewitt1) < range_size) {
					thresh_min_prewitt1 = std::clamp(thresh_max_prewitt1 - range_size, 0.0f, 1.0f);
				}
			}
		};
		ImGui::SetNextItemWidth(200);
		if (ImGui::SliderFloat("Min##prewitt_min1", &thresh_min_prewitt1, 0.01f, 0.99f)) {
			do_again = true;
			if (thresh_min_prewitt1 > thresh_max_prewitt1) {
				thresh_max_prewitt1 = std::clamp(thresh_min_prewitt1 + ((lock_range_size) ? range_size : 0.01f), 0.0f, 1.0f);
			}
			if (lock_range_size) lock_ranges();

		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		if (ImGui::SliderFloat("Max##prewitt_max1", &thresh_max_prewitt1, 0.01f, 0.99f)) {
			do_again = true;
			if (thresh_min_prewitt1 > thresh_max_prewitt1) {
				thresh_min_prewitt1 = std::clamp(thresh_max_prewitt1 - ((lock_range_size) ? range_size : 0.01f), 0.0f, 1.0f);
			}
			if (lock_range_size) lock_ranges();
		}
		ImGui::SameLine();


		if (ImGui::Checkbox("Lock Range Size", &lock_range_size)) {
			do_again = true;
			lock_ranges();
		}

		ImGui::SameLine();

		if (!lock_range_size) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Range Size##prewitt1", &range_size, 0.01f, 0.99f)) do_again = true;
		if (!lock_range_size) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		ImGui::SameLine();

		ImGui::SetNextItemWidth(100);
		if (ImGui::Button("Reset##prewitt1")) {
			do_again = true;
			thresh_min_prewitt1 = 0.65f;
			thresh_max_prewitt1 = 0.90f;
			range_size = 0.10f;
			lock_range_size = false;
		}

		if (!threshold_) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}
}

void Assignment2::DoPrewittSf() {
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& sf_opt = image_registry->GetTexture("sf", ".pgm");

	static bool do_again = false;
	static bool threshold_ = false;
	static bool lock_range_size = false;
	static float range_size = 0.10f;
	static bool above = true;

	float start_x = 0.0f;
	float start_x2 = 0.0f;
	float start_x3 = 0.0f;
	if (sf_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img_sf = sf_opt.value();
		const std::vector<uint8_t>& rawDataSf = img_sf->GetRawData();

		ImVec2 img_size{ (float)img_sf->GetWidth(), (float)img_sf->GetHeight() };
		float x_off_coeff = ((is_hovered_lap24) ? -3.0f : ((is_hovered_lap23) ? -2.0f : -1.0f));
		is_hovered_lap21 = widgets::ImageInspector("inspect_lap21", img_sf, &inspect_lap2, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sf->GetWidth()), 0.0f });
		ImGui::SameLine();

		const std::optional<graphics::Texture>& sf_prewitt_opt = image_registry->GetTexture("sf", "prewitt");
		const std::optional<graphics::Texture>& sf_prewitt_partials1_opt = image_registry->GetTexture("sf", "prewitt-partials1");
		const std::optional<graphics::Texture>& sf_prewitt_partials2_opt = image_registry->GetTexture("sf", "prewitt-partials2");

		if (sf_prewitt_opt.has_value() && sf_prewitt_partials1_opt.has_value() && sf_prewitt_partials2_opt.has_value() && (!do_again)) {
			const auto& img_sf_prewitt = sf_prewitt_opt.value();
			const auto& img_sf_prewitt_partials1 = sf_prewitt_partials1_opt.value();
			const auto& img_sf_prewitt_partials2 = sf_prewitt_partials2_opt.value();
			start_x = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap24) ? -2.0f : ((is_hovered_lap23) ? -1.0f : 1.0f));
			is_hovered_lap22 = widgets::ImageInspector("inspect_lap22", img_sf_prewitt_partials1, &inspect_lap2, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sf->GetWidth()), 0.0f });
			ImGui::SameLine();
			start_x2 = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap24) ? -1.0f : ((is_hovered_lap21) ? 2.0f : 1.0f));
			is_hovered_lap23 = widgets::ImageInspector("inspect_lap23", img_sf_prewitt_partials2, &inspect_lap2, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sf->GetWidth()), 0.0f });
			ImGui::SameLine();
			start_x3 = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap21) ? 3.0f : ((is_hovered_lap22) ? 2.0f : 1.0f));
			is_hovered_lap24 = widgets::ImageInspector("inspect_lap24", img_sf_prewitt, &inspect_lap2, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sf->GetWidth()), 0.0f });
			if ((!is_hovered_lap21) && (!is_hovered_lap22) && (!is_hovered_lap23) && (!is_hovered_lap24)) inspect_lap2 = false;
			//ImGui::SameLine();
		}
		else {
			std::vector<float> prewitt_x = convolve(rawDataSf, (int)img_sf->GetWidth(), (int)img_sf->GetHeight(), prewittHorz, 3, 3);
			std::vector<float> prewitt_y = convolve(rawDataSf, (int)img_sf->GetWidth(), (int)img_sf->GetHeight(), prewittVert, 3, 3);
			auto magnitude = gradient_magnitude(prewitt_x, prewitt_y, (int)img_sf->GetWidth(), (int)img_sf->GetHeight());

			std::vector<uint8_t> normed = normr(magnitude);
			auto partials1 = normr(prewitt_x);
			auto partials2 = normr(prewitt_y);

			if (threshold_) {
				std::vector<uint8_t> normedt = threshold_range(normed, (int)(thresh_min_prewitt2 * 255), (int)(thresh_max_prewitt2 * 255), above);
				auto partials1t = threshold_range(partials1, (int)(thresh_min_prewitt2 * 255), (int)(thresh_max_prewitt2 * 255), above);
				auto partials2t = threshold_range(partials2, (int)(thresh_min_prewitt2 * 255), (int)(thresh_max_prewitt2 * 255), above);

				image_registry->AddTexture("sf", "prewitt-partials1", graphics::make_texture(partials1t, img_sf->GetWidth(), img_sf->GetHeight(), 1));
				image_registry->AddTexture("sf", "prewitt-partials2", graphics::make_texture(partials2t, img_sf->GetWidth(), img_sf->GetHeight(), 1));
				image_registry->AddTexture("sf", "prewitt", graphics::make_texture(normedt, img_sf->GetWidth(), img_sf->GetHeight(), 1));
			}
			else {
				image_registry->AddTexture("sf", "prewitt-partials1", graphics::make_texture(partials1, img_sf->GetWidth(), img_sf->GetHeight(), 1));
				image_registry->AddTexture("sf", "prewitt-partials2", graphics::make_texture(partials2, img_sf->GetWidth(), img_sf->GetHeight(), 1));
				image_registry->AddTexture("sf", "prewitt", graphics::make_texture(normed, img_sf->GetWidth(), img_sf->GetHeight(), 1));
			}

			start_x = ImGui::GetCursorPosX();
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			ImGui::SameLine();
			start_x2 = ImGui::GetCursorPosX();
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			ImGui::SameLine();
			start_x3 = ImGui::GetCursorPosX();
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			do_again = false;
		}

		ImGui::Text("Original: ");

		ImGui::SameLine();

		float x_offset = 2 * style.ItemSpacing.x + ((float)img_sf->GetWidth() - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
		ImGui::Text("Partial Derivative (x): ");

		ImGui::SameLine();

		float x_offset2 = 3 * style.ItemSpacing.x + ((float)img_sf->GetWidth() * 2 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset2);
		ImGui::Text("Partial Derivative (y): ");

		ImGui::SameLine();

		float x_offset3 = 4 * style.ItemSpacing.x + ((float)img_sf->GetWidth() * 3 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset3);
		ImGui::Text("Gradient Magnitude: ");

		ImGui::Separator();

		if (ImGui::Checkbox("Threshold##prewitt2", &threshold_)) {
			do_again = true;
		}

		ImGui::SameLine();

		if (!threshold_) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ImGui::Checkbox("Flip B/W##prewitt2", &above)) {
			do_again = true;
		}

		ImGui::SameLine();

		auto lock_ranges = [&]() {
			if (std::abs(thresh_max_prewitt2 - thresh_min_prewitt2) < range_size) {
				thresh_max_prewitt2 = std::clamp(thresh_min_prewitt2 + range_size, 0.0f, 1.0f);
				if (std::abs(thresh_max_prewitt2 - thresh_min_prewitt2) < range_size) {
					thresh_min_prewitt2 = std::clamp(thresh_max_prewitt2 - range_size, 0.0f, 1.0f);
				}
			}
		};
		ImGui::SetNextItemWidth(200);
		if (ImGui::SliderFloat("Min##prewitt_min2", &thresh_min_prewitt2, 0.01f, 0.99f)) {
			do_again = true;
			if (thresh_min_prewitt2 > thresh_max_prewitt2) {
				thresh_max_prewitt2 = std::clamp(thresh_min_prewitt2 + ((lock_range_size) ? range_size : 0.01f), 0.0f, 1.0f);
			}
			if (lock_range_size) lock_ranges();

		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		if (ImGui::SliderFloat("Max##prewitt_max2", &thresh_max_prewitt2, 0.01f, 0.99f)) {
			do_again = true;
			if (thresh_min_prewitt2 > thresh_max_prewitt2) {
				thresh_min_prewitt2 = std::clamp(thresh_max_prewitt2 - ((lock_range_size) ? range_size : 0.01f), 0.0f, 1.0f);
			}
			if (lock_range_size) lock_ranges();
		}
		ImGui::SameLine();


		if (ImGui::Checkbox("Lock Range Size##prewitt2", &lock_range_size)) {
			do_again = true;
			lock_ranges();
		}

		ImGui::SameLine();

		if (!lock_range_size) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Range Size##prewitt2", &range_size, 0.01f, 0.99f)) do_again = true;
		if (!lock_range_size) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		ImGui::SameLine();

		ImGui::SetNextItemWidth(100);
		if (ImGui::Button("Reset##prewitt2")) {
			do_again = true;
			thresh_min_prewitt2 = 0.65f;
			thresh_max_prewitt2 = 0.90f;
			range_size = 0.10f;
			lock_range_size = false;
		}

		if (!threshold_) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}
}

void Assignment2::DoSobelLenna() {
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& lenna_opt = image_registry->GetTexture("lenna", ".pgm");

	static bool do_again = false;
	static bool threshold_ = false;
	static bool lock_range_size = false;
	static float range_size = 0.10f;
	static bool above = true;

	float start_x = 0.0f;
	float start_x2 = 0.0f;
	float start_x3 = 0.0f;
	if (lenna_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img_lenna = lenna_opt.value();

		ImVec2 img_size{ (float)img_lenna->GetWidth(), (float)img_lenna->GetHeight() };
		float x_off_coeff = ((is_hovered_lap34) ? -3.0f : ((is_hovered_lap33) ? -2.0f : -1.0f));
		is_hovered_lap31 = widgets::ImageInspector("inspect_lap31", img_lenna, &inspect_lap3, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
		ImGui::SameLine();

		const std::optional<graphics::Texture>& lenna_sobel_opt = image_registry->GetTexture("lenna", "sobel");
		const std::optional<graphics::Texture>& lenna_sobel_partials1_opt = image_registry->GetTexture("lenna", "sobel-partials1");
		const std::optional<graphics::Texture>& lenna_sobel_partials2_opt = image_registry->GetTexture("lenna", "sobel-partials2");

		if (lenna_sobel_opt.has_value() && lenna_sobel_partials1_opt.has_value() && lenna_sobel_partials2_opt.has_value() && (!do_again)) {
			const auto& img_lenna_sobel = lenna_sobel_opt.value();
			const auto& img_lenna_sobel_partials1 = lenna_sobel_partials1_opt.value();
			const auto& img_lenna_sobel_partials2 = lenna_sobel_partials2_opt.value();
			start_x = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap34) ? -2.0f : ((is_hovered_lap33) ? -1.0f : 1.0f));
			is_hovered_lap32 = widgets::ImageInspector("inspect_lap32", img_lenna_sobel_partials1, &inspect_lap3, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
			ImGui::SameLine();
			start_x2 = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap34) ? -1.0f : ((is_hovered_lap31) ? 2.0f : 1.0f));
			is_hovered_lap33 = widgets::ImageInspector("inspect_lap33", img_lenna_sobel_partials2, &inspect_lap3, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
			ImGui::SameLine();
			start_x3 = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap31) ? 3.0f : ((is_hovered_lap32) ? 2.0f : 1.0f));
			is_hovered_lap34 = widgets::ImageInspector("inspect_lap34", img_lenna_sobel, &inspect_lap3, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
			if ((!is_hovered_lap31) && (!is_hovered_lap32) && (!is_hovered_lap33) && (!is_hovered_lap34)) inspect_lap3 = false;
			//ImGui::SameLine();
		}
		else {
			std::vector<float> sobel_x = convolve(img_lenna->GetRawData(), (int)img_lenna->GetWidth(), (int)img_lenna->GetHeight(), sobelHorz, 3, 3);
			std::vector<float> sobel_y = convolve(img_lenna->GetRawData(), (int)img_lenna->GetWidth(), (int)img_lenna->GetHeight(), sobelVert, 3, 3);
			auto magnitude = gradient_magnitude(sobel_x, sobel_y, (int)img_lenna->GetWidth(), (int)img_lenna->GetHeight());

			std::vector<uint8_t> normed = normr(magnitude);
			auto partials1 = normr(sobel_x);
			auto partials2 = normr(sobel_y);

			if (threshold_) {
				std::vector<uint8_t> normedt = threshold_range(normed, (int)(thresh_min_sobel1 * 255), (int)(thresh_max_sobel1 * 255), above);
				auto partials1t = threshold_range(partials1, (int)(thresh_min_sobel1 * 255), (int)(thresh_max_sobel1 * 255), above);
				auto partials2t = threshold_range(partials2, (int)(thresh_min_sobel1 * 255), (int)(thresh_max_sobel1 * 255), above);

				image_registry->AddTexture("lenna", "sobel-partials1", graphics::make_texture(partials1t, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
				image_registry->AddTexture("lenna", "sobel-partials2", graphics::make_texture(partials2t, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
				image_registry->AddTexture("lenna", "sobel", graphics::make_texture(normedt, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
			}
			else {
				image_registry->AddTexture("lenna", "sobel-partials1", graphics::make_texture(partials1, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
				image_registry->AddTexture("lenna", "sobel-partials2", graphics::make_texture(partials2, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
				image_registry->AddTexture("lenna", "sobel", graphics::make_texture(normed, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
			}

			start_x = ImGui::GetCursorPosX();
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			ImGui::SameLine();
			start_x2 = ImGui::GetCursorPosX();
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			ImGui::SameLine();
			start_x3 = ImGui::GetCursorPosX();
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			do_again = false;
		}

		ImGui::Text("Original: ");

		ImGui::SameLine();

		float x_offset = 2 * style.ItemSpacing.x + ((float)img_lenna->GetWidth() - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
		ImGui::Text("Partial Derivative (x): ");

		ImGui::SameLine();

		float x_offset2 = 3 * style.ItemSpacing.x + ((float)img_lenna->GetWidth() * 2 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset2);
		ImGui::Text("Partial Derivative (y): ");

		ImGui::SameLine();

		float x_offset3 = 4 * style.ItemSpacing.x + ((float)img_lenna->GetWidth() * 3 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset3);
		ImGui::Text("Gradient Magnitude: ");

		ImGui::Separator();

		if (ImGui::Checkbox("Threshold##sobel1", &threshold_)) {
			do_again = true;
		}

		ImGui::SameLine();

		if (!threshold_) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ImGui::Checkbox("Flip B/W##sobel1", &above)) {
			do_again = true;
		}

		ImGui::SameLine();

		auto lock_ranges = [&]() {
			if (std::abs(thresh_max_sobel1 - thresh_min_sobel1) < range_size) {
				thresh_max_sobel1 = std::clamp(thresh_min_sobel1 + range_size, 0.0f, 1.0f);
				if (std::abs(thresh_max_sobel1 - thresh_min_sobel1) < range_size) {
					thresh_min_sobel1 = std::clamp(thresh_max_sobel1 - range_size, 0.0f, 1.0f);
				}
			}
		};
		ImGui::SetNextItemWidth(200);
		if (ImGui::SliderFloat("Min##sobel_min1", &thresh_min_sobel1, 0.01f, 0.99f)) {
			do_again = true;
			if (thresh_min_sobel1 > thresh_max_sobel1) {
				thresh_max_sobel1 = std::clamp(thresh_min_sobel1 + ((lock_range_size) ? range_size : 0.01f), 0.0f, 1.0f);
			}
			if (lock_range_size) lock_ranges();

		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		if (ImGui::SliderFloat("Max##sobel_max1", &thresh_max_sobel1, 0.01f, 0.99f)) {
			do_again = true;
			if (thresh_min_sobel1 > thresh_max_sobel1) {
				thresh_min_sobel1 = std::clamp(thresh_max_sobel1 - ((lock_range_size) ? range_size : 0.01f), 0.0f, 1.0f);
			}
			if (lock_range_size) lock_ranges();
		}
		ImGui::SameLine();


		if (ImGui::Checkbox("Lock Range Size##sobel1", &lock_range_size)) {
			do_again = true;
			lock_ranges();
		}

		ImGui::SameLine();

		if (!lock_range_size) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Range Size##sobel1", &range_size, 0.01f, 0.99f)) do_again = true;
		if (!lock_range_size) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		ImGui::SameLine();

		ImGui::SetNextItemWidth(100);
		if (ImGui::Button("Reset##sobel1")) {
			do_again = true;
			thresh_min_sobel1 = 0.65f;
			thresh_max_sobel1 = 0.90f;
			range_size = 0.10f;
			lock_range_size = false;
		}

		if (!threshold_) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}
}

void Assignment2::DoSobelSf() {
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& sf_opt = image_registry->GetTexture("sf", ".pgm");

	static bool do_again = false;
	static bool threshold_ = false;
	static bool lock_range_size = false;
	static float range_size = 0.10f;
	static bool above = true;

	float start_x = 0.0f;
	float start_x2 = 0.0f;
	float start_x3 = 0.0f;
	if (sf_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img_sf = sf_opt.value();
		const std::vector<uint8_t>& rawDataSf = img_sf->GetRawData();

		ImVec2 img_size{ (float)img_sf->GetWidth(), (float)img_sf->GetHeight() };
		float x_off_coeff = ((is_hovered_lap44) ? -3.0f : ((is_hovered_lap43) ? -2.0f : -1.0f));
		is_hovered_lap41 = widgets::ImageInspector("inspect_lap41", img_sf, &inspect_lap4, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sf->GetWidth()), 0.0f });
		ImGui::SameLine();

		const std::optional<graphics::Texture>& sf_sobel_opt = image_registry->GetTexture("sf", "sobel");
		const std::optional<graphics::Texture>& sf_sobel_partials1_opt = image_registry->GetTexture("sf", "sobel-partials1");
		const std::optional<graphics::Texture>& sf_sobel_partials2_opt = image_registry->GetTexture("sf", "sobel-partials2");

		if (sf_sobel_opt.has_value() && sf_sobel_partials1_opt.has_value() && sf_sobel_partials2_opt.has_value() && (!do_again)) {
			const auto& img_sf_sobel = sf_sobel_opt.value();
			const auto& img_sf_sobel_partials1 = sf_sobel_partials1_opt.value();
			const auto& img_sf_sobel_partials2 = sf_sobel_partials2_opt.value();
			start_x = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap44) ? -2.0f : ((is_hovered_lap43) ? -1.0f : 1.0f));
			is_hovered_lap42 = widgets::ImageInspector("inspect_lap42", img_sf_sobel_partials1, &inspect_lap4, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sf->GetWidth()), 0.0f });
			ImGui::SameLine();
			start_x2 = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap44) ? -1.0f : ((is_hovered_lap41) ? 2.0f : 1.0f));
			is_hovered_lap43 = widgets::ImageInspector("inspect_lap43", img_sf_sobel_partials2, &inspect_lap4, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sf->GetWidth()), 0.0f });
			ImGui::SameLine();
			start_x3 = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap41) ? 3.0f : ((is_hovered_lap42) ? 2.0f : 1.0f));
			is_hovered_lap44 = widgets::ImageInspector("inspect_lap44", img_sf_sobel, &inspect_lap4, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sf->GetWidth()), 0.0f });
			if ((!is_hovered_lap41) && (!is_hovered_lap42) && (!is_hovered_lap43) && (!is_hovered_lap44)) inspect_lap4 = false;
			//ImGui::SameLine();
		}
		else {
			std::vector<float> sobel_x = convolve(rawDataSf, (int)img_sf->GetWidth(), (int)img_sf->GetHeight(), sobelHorz, 3, 3);
			std::vector<float> sobel_y = convolve(rawDataSf, (int)img_sf->GetWidth(), (int)img_sf->GetHeight(), sobelVert, 3, 3);
			auto magnitude = gradient_magnitude(sobel_x, sobel_y, (int)img_sf->GetWidth(), (int)img_sf->GetHeight());

			std::vector<uint8_t> normed = normr(magnitude);
			auto partials1 = normr(sobel_x);
			auto partials2 = normr(sobel_y);

			if (threshold_) {
				std::vector<uint8_t> normedt = threshold_range(normed, (int)(thresh_min_sobel2 * 255), (int)(thresh_max_sobel2 * 255), above);
				auto partials1t = threshold_range(partials1, (int)(thresh_min_sobel2 * 255), (int)(thresh_max_sobel2 * 255), above);
				auto partials2t = threshold_range(partials2, (int)(thresh_min_sobel2 * 255), (int)(thresh_max_sobel2 * 255), above);

				image_registry->AddTexture("sf", "sobel-partials1", graphics::make_texture(partials1t, img_sf->GetWidth(), img_sf->GetHeight(), 1));
				image_registry->AddTexture("sf", "sobel-partials2", graphics::make_texture(partials2t, img_sf->GetWidth(), img_sf->GetHeight(), 1));
				image_registry->AddTexture("sf", "sobel", graphics::make_texture(normedt, img_sf->GetWidth(), img_sf->GetHeight(), 1));
			}
			else {
				image_registry->AddTexture("sf", "sobel-partials1", graphics::make_texture(partials1, img_sf->GetWidth(), img_sf->GetHeight(), 1));
				image_registry->AddTexture("sf", "sobel-partials2", graphics::make_texture(partials2, img_sf->GetWidth(), img_sf->GetHeight(), 1));
				image_registry->AddTexture("sf", "sobel", graphics::make_texture(normed, img_sf->GetWidth(), img_sf->GetHeight(), 1));
			}

			start_x = ImGui::GetCursorPosX();
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			ImGui::SameLine();
			start_x2 = ImGui::GetCursorPosX();
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			ImGui::SameLine();
			start_x3 = ImGui::GetCursorPosX();
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			do_again = false;
		}

		ImGui::Text("Original: ");

		ImGui::SameLine();

		float x_offset = 2 * style.ItemSpacing.x + ((float)img_sf->GetWidth() - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
		ImGui::Text("Partial Derivative (x): ");

		ImGui::SameLine();

		float x_offset2 = 3 * style.ItemSpacing.x + ((float)img_sf->GetWidth() * 2 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset2);
		ImGui::Text("Partial Derivative (y): ");

		ImGui::SameLine();

		float x_offset3 = 4 * style.ItemSpacing.x + ((float)img_sf->GetWidth() * 3 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset3);
		ImGui::Text("Gradient Magnitude: ");

		ImGui::Separator();

		if (ImGui::Checkbox("Threshold##sobel2", &threshold_)) {
			do_again = true;
		}

		ImGui::SameLine();

		if (!threshold_) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ImGui::Checkbox("Flip B/W##sobel2", &above)) {
			do_again = true;
		}

		ImGui::SameLine();

		auto lock_ranges = [&]() {
			if (std::abs(thresh_max_sobel2 - thresh_min_sobel2) < range_size) {
				thresh_max_sobel2 = std::clamp(thresh_min_sobel2 + range_size, 0.0f, 1.0f);
				if (std::abs(thresh_max_sobel2 - thresh_min_sobel2) < range_size) {
					thresh_min_sobel2 = std::clamp(thresh_max_sobel2 - range_size, 0.0f, 1.0f);
				}
			}
		};
		ImGui::SetNextItemWidth(200);
		if (ImGui::SliderFloat("Min##sobel_min2", &thresh_min_sobel2, 0.01f, 0.99f)) {
			do_again = true;
			if (thresh_min_sobel2 > thresh_max_sobel2) {
				thresh_max_sobel2 = std::clamp(thresh_min_sobel2 + ((lock_range_size) ? range_size : 0.01f), 0.0f, 1.0f);
			}
			if (lock_range_size) lock_ranges();

		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		if (ImGui::SliderFloat("Max##sobel_max2", &thresh_max_sobel2, 0.01f, 0.99f)) {
			do_again = true;
			if (thresh_min_sobel2 > thresh_max_sobel2) {
				thresh_min_sobel2 = std::clamp(thresh_max_sobel2 - ((lock_range_size) ? range_size : 0.01f), 0.0f, 1.0f);
			}
			if (lock_range_size) lock_ranges();
		}
		ImGui::SameLine();


		if (ImGui::Checkbox("Lock Range Size##sobel2", &lock_range_size)) {
			do_again = true;
			lock_ranges();
		}

		ImGui::SameLine();

		if (!lock_range_size) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Range Size##sobel2", &range_size, 0.01f, 0.99f)) do_again = true;
		if (!lock_range_size) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		ImGui::SameLine();

		ImGui::SetNextItemWidth(100);
		if (ImGui::Button("Reset##sobel2")) {
			do_again = true;
			thresh_min_sobel2 = 0.65f;
			thresh_max_sobel2 = 0.90f;
			range_size = 0.10f;
			lock_range_size = false;
		}

		if (!threshold_) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}
}

void Assignment2::DoLaplacianLenna() {
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& lenna_opt = image_registry->GetTexture("lenna", ".pgm");

	static bool do_again = false;
	static bool threshold_ = false;
	static bool lock_range_size = false;
	static float range_size = 0.10f;
	static bool above = true;

	float start_x = 0.0f;
	float start_x2 = 0.0f;
	float start_x3 = 0.0f;
	if (lenna_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img_lenna = lenna_opt.value();

		ImVec2 img_size{ (float)img_lenna->GetWidth(), (float)img_lenna->GetHeight() };
		float x_off_coeff = ((is_hovered_lap54) ? -3.0f : ((is_hovered_lap53) ? -2.0f : -1.0f));
		is_hovered_lap51 = widgets::ImageInspector("inspect_lap51", img_lenna, &inspect_lap5, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
		ImGui::SameLine();

		const std::optional<graphics::Texture>& lenna_lap_opt = image_registry->GetTexture("lenna", "lap");
		const std::optional<graphics::Texture>& lenna_blur_opt = image_registry->GetTexture("lenna", "blur3");
		const std::optional<graphics::Texture>& lenna_lap_edges_opt = image_registry->GetTexture("lenna", "lap-edges");

		if (lenna_lap_opt.has_value() && lenna_lap_edges_opt.has_value() && lenna_blur_opt.has_value() && (!do_again)) {
			const auto& img_lenna_lap = lenna_lap_opt.value();
			const auto& img_lenna_blur = lenna_blur_opt.value();
			const auto& img_lenna_lap_edges = lenna_lap_edges_opt.value();

			start_x = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap54) ? -2.0f : ((is_hovered_lap53) ? -1.0f : 1.0f));
			is_hovered_lap52 = widgets::ImageInspector("inspect_lap52", img_lenna_blur, &inspect_lap5, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
			ImGui::SameLine();
			start_x2 = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap54) ? -1.0f : ((is_hovered_lap51) ? 2.0f : 1.0f));
			is_hovered_lap53 = widgets::ImageInspector("inspect_lap53", img_lenna_lap_edges, &inspect_lap5, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
			ImGui::SameLine();
			start_x3 = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap51) ? 3.0f : ((is_hovered_lap52) ? 2.0f : 1.0f));
			is_hovered_lap54 = widgets::ImageInspector("inspect_lap54", img_lenna_lap, &inspect_lap5, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
			if ((!is_hovered_lap51) && (!is_hovered_lap52) && (!is_hovered_lap53) && (!is_hovered_lap54)) inspect_lap5 = false;
			//ImGui::SameLine();
		}
		else {

			auto averagedData = smoothImage(img_lenna->GetRawData(), img_lenna->GetWidth(), img_lenna->GetHeight(), filterSizeAvgLap1, (filterSizeAvgLap1 == 7) ? gausMask7x7 : gausMask15x15);
			if (averagedData.has_value()) {
				auto data = averagedData.value();
				bool success = image_registry->AddTexture("lenna", "blur3", graphics::make_texture(data, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);

				std::vector<float> lap_edges = convolve(averagedData.value(), (int)img_lenna->GetWidth(), (int)img_lenna->GetHeight(),laplacian, 3, 3);
				auto edges = normr(lap_edges);

				auto sharpened = sharpen(img_lenna->GetRawData(), edges, (int)img_lenna->GetWidth(), (int)img_lenna->GetHeight());

				//std::vector<uint8_t> normed = norm(sharpened, (int)(img_lenna->GetWidth() * img_lenna->GetHeight()));

				if (threshold_) {
					std::vector<uint8_t> normedt = threshold_range(sharpened, (int)(thresh_min_lap1 * 255), (int)(thresh_max_lap1 * 255), above);
					auto edgest = threshold_range(edges, (int)(thresh_min_lap1 * 255), (int)(thresh_max_lap1 * 255), above);

					image_registry->AddTexture("lenna", "lap-edges", graphics::make_texture(edgest, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
					image_registry->AddTexture("lenna", "lap", graphics::make_texture(normedt, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
				}
				else {
					image_registry->AddTexture("lenna", "lap-edges", graphics::make_texture(edges, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
					image_registry->AddTexture("lenna", "lap", graphics::make_texture(sharpened, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
				}

				start_x = ImGui::GetCursorPosX();
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
				ImGui::SameLine();
				start_x2 = ImGui::GetCursorPosX();
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
				ImGui::SameLine();
				start_x3 = ImGui::GetCursorPosX();
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
				do_again = false;
			}
		}

		ImGui::Text("Original: ");

		ImGui::SameLine();

		float x_offset = 2 * style.ItemSpacing.x + ((float)img_lenna->GetWidth() - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
		ImGui::Text("Gaussian Blur (%zu x %zu):", filterSizeAvgLap1, filterSizeAvgLap1);

		ImGui::SameLine();

		float x_offset2 = 3 * style.ItemSpacing.x + ((float)img_lenna->GetWidth() * 2 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset2);
		ImGui::Text("Laplacian Edges: ");

		ImGui::SameLine();

		float x_offset3 = 4 * style.ItemSpacing.x + ((float)img_lenna->GetWidth() * 3 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset3);
		ImGui::Text("Sharpened: ");

		ImGui::Separator();


		static int item_current_idx3 = 0;
		ImGui::SetNextItemWidth(100);
		std::vector<std::string> items2 = { "7x7", "15x15" };
		if (ImGui::BeginCombo("Gaussian Mask##lap1", items2[item_current_idx3].c_str()))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			for (int n = 0; n < items2.size(); n++)
			{
				const bool is_selected = (item_current_idx3 == n);
				if (ImGui::Selectable(items2[n].c_str(), is_selected)) {
					item_current_idx3 = n;
					if (items2[n] == "7x7") {
						filterSizeAvgLap1 = 7;
					}
					else {
						filterSizeAvgLap1 = 15;
					}
					do_again = true;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();

		if (ImGui::Checkbox("Threshold##lap1", &threshold_)) {
			do_again = true;
		}

		ImGui::SameLine();

		if (!threshold_) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ImGui::Checkbox("Flip B/W##lap1", &above)) {
			do_again = true;
		}

		ImGui::SameLine();

		auto lock_ranges = [&]() {
			if (std::abs(thresh_max_lap1 - thresh_min_lap1) < range_size) {
				thresh_max_lap1 = std::clamp(thresh_min_lap1 + range_size, 0.0f, 1.0f);
				if (std::abs(thresh_max_lap1 - thresh_min_lap1) < range_size) {
					thresh_min_lap1 = std::clamp(thresh_max_lap1 - range_size, 0.0f, 1.0f);
				}
			}
		};
		ImGui::SetNextItemWidth(200);
		if (ImGui::SliderFloat("Min##lap_min1", &thresh_min_lap1, 0.01f, 0.99f)) {
			do_again = true;
			if (thresh_min_lap1 > thresh_max_lap1) {
				thresh_max_lap1 = std::clamp(thresh_min_lap1 + ((lock_range_size) ? range_size : 0.01f), 0.0f, 1.0f);
			}
			if (lock_range_size) lock_ranges();

		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		if (ImGui::SliderFloat("Max##lap_max1", &thresh_max_lap1, 0.01f, 0.99f)) {
			do_again = true;
			if (thresh_min_lap1 > thresh_max_lap1) {
				thresh_min_lap1 = std::clamp(thresh_max_lap1 - ((lock_range_size) ? range_size : 0.01f), 0.0f, 1.0f);
			}
			if (lock_range_size) lock_ranges();
		}
		ImGui::SameLine();


		if (ImGui::Checkbox("Lock Range Size##lap1", &lock_range_size)) {
			do_again = true;
			lock_ranges();
		}

		ImGui::SameLine();

		if (!lock_range_size) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Range Size##lap1", &range_size, 0.01f, 0.99f)) do_again = true;
		if (!lock_range_size) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		ImGui::SameLine();

		ImGui::SetNextItemWidth(100);
		if (ImGui::Button("Reset##lap1")) {
			do_again = true;
			thresh_min_lap1 = 0.65f;
			thresh_max_lap1 = 0.90f;
			range_size = 0.10f;
			lock_range_size = false;
		}

		if (!threshold_) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}
}

void Assignment2::DoLaplacianSf() {
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& sf_opt = image_registry->GetTexture("sf", ".pgm");

	static bool do_again = false;
	static bool threshold_ = false;
	static bool lock_range_size = false;
	static float range_size = 0.10f;
	static bool above = true;

	float start_x = 0.0f;
	float start_x2 = 0.0f;
	float start_x3 = 0.0f;
	if (sf_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img_sf = sf_opt.value();

		ImVec2 img_size{ (float)img_sf->GetWidth(), (float)img_sf->GetHeight() };
		float x_off_coeff = ((is_hovered_lap64) ? -3.0f : ((is_hovered_lap63) ? -2.0f : -1.0f));
		is_hovered_lap61 = widgets::ImageInspector("inspect_lap61", img_sf, &inspect_lap6, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sf->GetWidth()), 0.0f });
		ImGui::SameLine();

		const std::optional<graphics::Texture>& sf_lap_opt = image_registry->GetTexture("sf", "lap");
		const std::optional<graphics::Texture>& sf_blur_opt = image_registry->GetTexture("sf", "blur3");
		const std::optional<graphics::Texture>& sf_lap_edges_opt = image_registry->GetTexture("sf", "lap-edges");

		if (sf_lap_opt.has_value() && sf_lap_edges_opt.has_value() && sf_blur_opt.has_value() && (!do_again)) {
			const auto& img_sf_lap = sf_lap_opt.value();
			const auto& img_sf_blur = sf_blur_opt.value();
			const auto& img_sf_lap_edges = sf_lap_edges_opt.value();

			start_x = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap64) ? -2.0f : ((is_hovered_lap63) ? -1.0f : 1.0f));
			is_hovered_lap62 = widgets::ImageInspector("inspect_lap62", img_sf_blur, &inspect_lap6, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sf->GetWidth()), 0.0f });
			ImGui::SameLine();
			start_x2 = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap64) ? -1.0f : ((is_hovered_lap61) ? 2.0f : 1.0f));
			is_hovered_lap63 = widgets::ImageInspector("inspect_lap63", img_sf_lap_edges, &inspect_lap6, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sf->GetWidth()), 0.0f });
			ImGui::SameLine();
			start_x3 = ImGui::GetCursorPosX();
			x_off_coeff = ((is_hovered_lap61) ? 3.0f : ((is_hovered_lap62) ? 2.0f : 1.0f));
			is_hovered_lap64 = widgets::ImageInspector("inspect_lap64", img_sf_lap, &inspect_lap6, { 0.0f, 0.0f }, { x_off_coeff * (style.ItemSpacing.x + img_sf->GetWidth()), 0.0f });
			if ((!is_hovered_lap61) && (!is_hovered_lap62) && (!is_hovered_lap63) && (!is_hovered_lap64)) inspect_lap6 = false;
			//ImGui::SameLine();
		}
		else {

			auto averagedData = smoothImage(img_sf->GetRawData(), img_sf->GetWidth(), img_sf->GetHeight(), filterSizeAvgLap2, (filterSizeAvgLap2 == 7) ? gausMask7x7 : gausMask15x15);
			if (averagedData.has_value()) {
				auto data = averagedData.value();
				bool success = image_registry->AddTexture("sf", "blur3", graphics::make_texture(data, img_sf->GetWidth(), img_sf->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);

				std::vector<float> lap_edges = convolve(averagedData.value(), (int)img_sf->GetWidth(), (int)img_sf->GetHeight(), laplacian, 3, 3);
				auto edges = normr(lap_edges);

				auto sharpened = sharpen(img_sf->GetRawData(), edges, (int)img_sf->GetWidth(), (int)img_sf->GetHeight());

				if (threshold_) {
					std::vector<uint8_t> normedt = threshold_range(sharpened, (int)(thresh_min_lap2 * 255), (int)(thresh_max_lap2 * 255), above);
					auto edgest = threshold_range(edges, (int)(thresh_min_lap2 * 255), (int)(thresh_max_lap2 * 255), above);

					image_registry->AddTexture("sf", "lap-edges", graphics::make_texture(edgest, img_sf->GetWidth(), img_sf->GetHeight(), 1));
					image_registry->AddTexture("sf", "lap", graphics::make_texture(normedt, img_sf->GetWidth(), img_sf->GetHeight(), 1));
				}
				else {
					image_registry->AddTexture("sf", "lap-edges", graphics::make_texture(edges, img_sf->GetWidth(), img_sf->GetHeight(), 1));
					image_registry->AddTexture("sf", "lap", graphics::make_texture(sharpened, img_sf->GetWidth(), img_sf->GetHeight(), 1));
				}

				start_x = ImGui::GetCursorPosX();
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
				ImGui::SameLine();
				start_x2 = ImGui::GetCursorPosX();
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
				ImGui::SameLine();
				start_x3 = ImGui::GetCursorPosX();
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
				do_again = false;
			}
		}

		ImGui::Text("Original: ");

		ImGui::SameLine();

		float x_offset = 2 * style.ItemSpacing.x + ((float)img_sf->GetWidth() - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
		ImGui::Text("Gaussian Blur (%zu x %zu):", filterSizeAvgLap2, filterSizeAvgLap2);

		ImGui::SameLine();

		float x_offset2 = 3 * style.ItemSpacing.x + ((float)img_sf->GetWidth() * 2 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset2);
		ImGui::Text("Laplacian Edges: ");

		ImGui::SameLine();

		float x_offset3 = 4 * style.ItemSpacing.x + ((float)img_sf->GetWidth() * 3 - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset3);
		ImGui::Text("Sharpened: ");

		ImGui::Separator();


		static int item_current_idx3 = 0;
		ImGui::SetNextItemWidth(100);
		std::vector<std::string> items2 = { "7x7", "15x15" };
		if (ImGui::BeginCombo("Gaussian Mask##lap2", items2[item_current_idx3].c_str()))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			for (int n = 0; n < items2.size(); n++)
			{
				const bool is_selected = (item_current_idx3 == n);
				if (ImGui::Selectable(items2[n].c_str(), is_selected)) {
					item_current_idx3 = n;
					if (items2[n] == "7x7") {
						filterSizeAvgLap2 = 7;
					}
					else {
						filterSizeAvgLap2 = 15;
					}
					do_again = true;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();

		if (ImGui::Checkbox("Threshold##lap2", &threshold_)) {
			do_again = true;
		}

		ImGui::SameLine();

		if (!threshold_) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ImGui::Checkbox("Flip B/W##lap2", &above)) {
			do_again = true;
		}

		ImGui::SameLine();

		auto lock_ranges = [&]() {
			if (std::abs(thresh_max_lap2 - thresh_min_lap2) < range_size) {
				thresh_max_lap2 = std::clamp(thresh_min_lap2 + range_size, 0.0f, 1.0f);
				if (std::abs(thresh_max_lap2 - thresh_min_lap2) < range_size) {
					thresh_min_lap2 = std::clamp(thresh_max_lap2 - range_size, 0.0f, 1.0f);
				}
			}
		};
		ImGui::SetNextItemWidth(200);
		if (ImGui::SliderFloat("Min##lap_min2", &thresh_min_lap2, 0.01f, 0.99f)) {
			do_again = true;
			if (thresh_min_lap2 > thresh_max_lap2) {
				thresh_max_lap2 = std::clamp(thresh_min_lap2 + ((lock_range_size) ? range_size : 0.01f), 0.0f, 1.0f);
			}
			if (lock_range_size) lock_ranges();

		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		if (ImGui::SliderFloat("Max##lap_max2", &thresh_max_lap2, 0.01f, 0.99f)) {
			do_again = true;
			if (thresh_min_lap2 > thresh_max_lap2) {
				thresh_min_lap2 = std::clamp(thresh_max_lap2 - ((lock_range_size) ? range_size : 0.01f), 0.0f, 1.0f);
			}
			if (lock_range_size) lock_ranges();
		}
		ImGui::SameLine();


		if (ImGui::Checkbox("Lock Range Size##lap2", &lock_range_size)) {
			do_again = true;
			lock_ranges();
		}

		ImGui::SameLine();

		if (!lock_range_size) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Range Size##lap2", &range_size, 0.01f, 0.99f)) do_again = true;
		if (!lock_range_size) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		ImGui::SameLine();

		ImGui::SetNextItemWidth(100);
		if (ImGui::Button("Reset##lap2")) {
			do_again = true;
			thresh_min_lap2 = 0.65f;
			thresh_max_lap2 = 0.90f;
			range_size = 0.10f;
			lock_range_size = false;
		}

		if (!threshold_) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}
}

void ProcessSpatialFilteringData() {
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& img_opt_padded = image_registry->GetTexture("ImagePadded", ".pgm");
	const std::optional<graphics::Texture>& pat_opt_padded = image_registry->GetTexture("PatternPadded", ".pgm");
	static bool one_attempt = true;

	if (img_opt_padded.has_value() && one_attempt) {
		one_attempt = false;
		const auto& padded = img_opt_padded.value();
		const auto& patPadded = pat_opt_padded.value();

		emscripten_log(EM_LOG_CONSOLE, "can access image_reg");

		auto filtered = spatial_filtering(padded->GetRawData(), padded->GetWidth(), padded->GetHeight(), patPadded->GetRawData(), patPadded->GetWidth(), patPadded->GetHeight());
		emscripten_log(EM_LOG_CONSOLE, "processed: {%zu}", filtered.size());
		global::AddResource("filteredimage", filtered);
		
		const std::vector<float>& opt_filtered = global::GetResourceUnwrapped("filteredimage");
		if (padded->GetRawData().size() == opt_filtered.size() == filtered.size()) {
			emscripten_log(EM_LOG_CONSOLE, "data is valid");
		}

		emscripten_log(EM_LOG_CONSOLE, "og: {%zu} filtered: {%zu}, fetched: {%zu}", padded->GetRawData().size(), filtered.size(), opt_filtered.size());
	}
}

void TryMakeSpatialFilteringTexture(bool force = false) {
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& og_opt = image_registry->GetTexture("ImagePadded", ".pgm");
	const std::optional<std::reference_wrapper<const global::Resource>>& opt_filtered = global::GetResource("filteredimage");
	
	static bool init = true;

	if ((opt_filtered.has_value() && og_opt.has_value()) && (init || force)) {
		const auto& og_image = og_opt.value();
		const std::vector<float>& filtered_ = opt_filtered.value().get();

		std::vector<uint8_t> filtered(filtered_.size());
		for (int i = 0; i < filtered.size(); ++i) {
			filtered[i] = static_cast<uint8_t>(255.0f * filtered_[i]);
		}

		image_registry->AddTexture("ImagePadded", "filt", graphics::make_texture(filtered, og_image->GetWidth(), og_image->GetHeight(), 1));

		if (init) init = false;
	}
}
} // namespace cs474

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
		ImGui::Image((void*)(intptr_t)(og_opt.value()->GetRendererID()), ImVec2(444, 333));
		//ImGui::Image((void*)(intptr_t)(filt_opt.value()->GetRendererID()), ImVec2(444, 333));
	}
	static float size = 32.0f;
	static float thickness = 8.0f;
	if (filt_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		static bool inspect_filt = false;
		ImGui::SameLine();
		inspect_filt = widgets::ImageInspector("inspectfilt", filt_opt.value(), &inspect_filt, { 0.0f, 0.0f }, { style.ItemSpacing.x + 444, 0.0f });
		inspect_filt = false;
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

void Assignment2::Question2() {
	if (global::GetResourceUnwrapped("g_Assignment2ScrollToAveraging and Gaussian Smoothing")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment2ScrollToAveraging and Gaussian Smoothing", false);
	}
	widgets::markdown("# 2. Averaging and Gaussian Smoothing");

	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& lenna_opt = image_registry->GetTexture("lenna", ".pgm");

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
}

void Assignment2::Question3() {
	if (global::GetResourceUnwrapped("g_Assignment2ScrollToMedian Filtering")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment2ScrollToMedian Filtering", false);
	}
	widgets::markdown("# 3. Median Filtering");

	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& img_opt = image_registry->GetTexture("lenna", ".pgm");

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
		if (ImGui::BeginCombo("% Noise", items[item_current_idx].c_str()))
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
		if (ImGui::BeginCombo("Median Filter Size", items2[item_current_idx2].c_str()))
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
}

void Assignment2::Question5() {
	if (global::GetResourceUnwrapped("g_Assignment2ScrollToGradient and Laplacian")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment2ScrollToGradient and Laplacian", false);
	}
	widgets::markdown("# 5. Gradient and Laplacian");

	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& lenna_opt = image_registry->GetTexture("lenna", ".pgm");
	const std::optional<graphics::Texture>& sf_opt = image_registry->GetTexture("sf", ".pgm");

	if (lenna_opt.has_value() && sf_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img_lenna = lenna_opt.value();
		const auto& img_sf = sf_opt.value();
		const std::vector<uint8_t>& rawDataLenna = img_lenna->GetRawData();
		const std::vector<uint8_t>& rawDataSf = img_sf->GetRawData();
		ImVec2 img_size{ (float)img_lenna->GetWidth(), (float)img_lenna->GetHeight() };
		//float x_off_coeff = ((is_hovered_hbf4) ? -3.0f : ((is_hovered_hbf3) ? -2.0f : -1.0f));
		is_hovered_lap11 = widgets::ImageInspector("inspect_lap11", img_lenna, &inspect_lap1, { 0.0f, 0.0f }, { /*x_off_coeff **/ (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
		ImGui::SameLine();

		const std::optional<graphics::Texture>& lenna_prewitt_opt = image_registry->GetTexture("lenna", "prewitt");
		const std::optional<graphics::Texture>& lenna_prewitt_partials1_opt = image_registry->GetTexture("lenna", "prewitt-partials1");
		const std::optional<graphics::Texture>& lenna_prewitt_partials2_opt = image_registry->GetTexture("lenna", "prewitt-partials2");

		if (lenna_prewitt_opt.has_value() && lenna_prewitt_partials1_opt.has_value() && lenna_prewitt_partials2_opt.has_value()) {
			const auto& img_lenna_prewitt = lenna_prewitt_opt.value();
			const auto& img_lenna_prewitt_partials1 = lenna_prewitt_partials1_opt.value();
			const auto& img_lenna_prewitt_partials2 = lenna_prewitt_partials2_opt.value();
			is_hovered_lap12 = widgets::ImageInspector("inspect_lap12", img_lenna_prewitt_partials1, &inspect_lap1, { 0.0f, 0.0f }, { /*x_off_coeff **/ (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
			ImGui::SameLine();
			is_hovered_lap12 = widgets::ImageInspector("inspect_lap13", img_lenna_prewitt_partials2, &inspect_lap1, { 0.0f, 0.0f }, { /*x_off_coeff **/ (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
			ImGui::SameLine();
			is_hovered_lap13 = widgets::ImageInspector("inspect_lap14", img_lenna_prewitt, &inspect_lap1, { 0.0f, 0.0f }, { /*x_off_coeff **/ (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f });
			ImGui::SameLine();
		}
		else {
			std::vector<float> prewitt_x = convolve(rawDataLenna, (int)img_lenna->GetWidth(), (int)img_lenna->GetHeight(), prewittHorz, 3, 3);
			std::vector<float> prewitt_y = convolve(rawDataLenna, (int)img_lenna->GetWidth(), (int)img_lenna->GetHeight(), prewittVert, 3, 3);
			auto magnitude = gradient_magnitude(prewitt_x, prewitt_y, (int)img_lenna->GetWidth(), (int)img_lenna->GetHeight());
			std::vector<uint8_t> normed = normr(magnitude);
			auto magnitude2 = threshold(normed, 20, false);
			auto partials1 = flip(normr(prewitt_x));
			//auto partials1 = to_uint8_max(prewitt_x);
			////auto partials1 = visualize_partials(prewitt_x);
			//auto partials2 = to_uint8_max(prewitt_y);
			auto partials2 = flip(normr(prewitt_y));
			//auto partials2 = visualize_partials(prewitt_y);

			image_registry->AddTexture("lenna", "prewitt-partials1", graphics::make_texture(partials1, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
			image_registry->AddTexture("lenna", "prewitt-partials2", graphics::make_texture(partials2, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
			image_registry->AddTexture("lenna", "prewitt", graphics::make_texture(magnitude2, img_lenna->GetWidth(), img_lenna->GetHeight(), 1));
		}
		inspect_lap1 = false;
		inspect_lap2 = false;
		
		
		
		
		
		
		
		
		
		
		
		
		is_hovered_lap21 = widgets::ImageInspector("inspect_lap21", img_sf, &inspect_lap2, { 0.0f, 0.0f }, { /*x_off_coeff **/ (style.ItemSpacing.x + img_lenna->GetWidth()), 0.0f});




	}


}

void Assignment2::DoSpatialFiltering() {
	emscripten_wasm_worker_t worker = emscripten_malloc_wasm_worker(/*stackSize: */1024);
	emscripten_wasm_worker_post_function_sig(worker, reinterpret_cast<std::function<void()>*>(&ProcessSpatialFilteringData), "void()");
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
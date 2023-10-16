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
}

void Assignment2::Question3() {
	if (global::GetResourceUnwrapped("g_Assignment2ScrollToMedian Filtering")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment2ScrollToMedian Filtering", false);
	}
	widgets::markdown("# 3. Median Filtering");
}

void Assignment2::Question4() {
	if (global::GetResourceUnwrapped("g_Assignment2ScrollToUnsharp Masking and High Boost Filtering")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment2ScrollToUnsharp Masking and High Boost Filtering", false);
	}
	widgets::markdown("# 4. Unsharp Masking and High Boost Filtering");
}

void Assignment2::Question5() {
	if (global::GetResourceUnwrapped("g_Assignment2ScrollToGradient and Laplacian")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment2ScrollToGradient and Laplacian", false);
	}
	widgets::markdown("# 5. Gradient and Laplacian");
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
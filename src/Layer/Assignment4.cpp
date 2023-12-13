#include "..\cs474.pch.h"
#include "Assignment4.h"
namespace cs474 {
Assignment4::Assignment4() {
}
Assignment4::~Assignment4() {
}
void Assignment4::OnAttach() {
	global::UpdateResource("g_Assignment4ScrollToNoise Removal", false);
	global::UpdateResource("g_Assignment4ScrollToImage Convolution", false);
	global::UpdateResource("g_Assignment4ScrollToHomomorphic Filtering", false);
}
void Assignment4::OnDetach() {
}
void Assignment4::OnUpdate(float ts) {
}
void Assignment4::OnUIRender() {
	if (global::GetResourceUnwrapped("g_ShowAssignment4")) {
		bool open = true;
		ImGuiWindowFlags window_flags = utils::GetBaseLayerWindowFlags();
		//ImGuiWindowClass wc;
		//wc.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_NoUndocking;
		//ImGui::SetNextWindowClass(&wc);

		ImGui::Begin("Assignment 4", &open);
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
		}
		ImGui::End();

		if (!open) global::UpdateResource("g_ShowAssignment4", false);
		//this->Test();
	}
}

void Assignment4::Question1() {
	if (global::GetResourceUnwrapped("g_Assignment4ScrollToNoise Removal")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment4ScrollToNoise Removal", false);
	}
	widgets::markdown("# 1. Noise Removal");

	static float lowCutoff = 35.0f;
	static float highCutoff = 38.0f;
	static const float maxCutoff = 256.0f;
	static int centerX = 224;
	static int centerY = 240;
	static int notchWidth = 3;
	static int notchHeight = 3;

	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");

	const std::optional<graphics::Texture>& img_opt_boy = image_registry->GetTexture("boy_noisy", ".pgm");

	if (img_opt_boy.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& boy_img = img_opt_boy.value();
		const std::vector<uint8_t>& rawDataBoy = boy_img->GetRawData();

		ImVec2 img_size{ (float)boy_img->GetWidth(), (float)boy_img->GetHeight() };
		//bool is_hovered1 = widgets::ImageInspector("inspect1", boy_img, &inspect_sub1, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + boy_img->GetWidth()), 0.0f });
		ImGui::Image((void*)(intptr_t)(boy_img->GetRendererID()), img_size);


		ImGui::SameLine();

		const std::optional<graphics::Texture>& boy_sub_opt = image_registry->GetTexture("boy_noisy", "fft2");
		if (boy_sub_opt.has_value()) {
			const auto& boy_img_sub = boy_sub_opt.value();
			ImVec2 img_sub_size{ (float)boy_img_sub->GetWidth(), (float)boy_img_sub->GetHeight() };
			ImGui::Image((void*)(intptr_t)(boy_img_sub->GetRendererID()), img_sub_size);
			
			//bool is_hovered2 = widgets::ImageInspector("inspect2", boy_img_sub, &inspect_sub1, { 0.0f, 0.0f }, { style.ItemSpacing.x + boy_img_sub->GetWidth(), 0.0f });
			//if ((!is_hovered1) && (!is_hovered2)) inspect_sub1 = false;
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::Text("Original Image: ");
		ImGui::SameLine();

		float x_offset = 2 * style.ItemSpacing.x + ((float)boy_img->GetWidth() - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
		ImGui::Text("Spectrum Of Original: ");

		static bool init_sample1 = true;
		if (init_sample1) {
			// Declaring single float vectors for real and imaginary parts
			std::vector<float> real_Fuv(boy_img->GetHeight() * boy_img->GetWidth());
			std::vector<float> imag_Fuv(boy_img->GetHeight() * boy_img->GetWidth(), 0.0); // Initialize with zeros

			for (int i = 0; i < boy_img->GetHeight(); i++) {
				for (int j = 0; j < boy_img->GetWidth(); j++) {
					real_Fuv[i * boy_img->GetWidth() + j] = static_cast<float>(rawDataBoy[i * boy_img->GetWidth() + j]);
				}
			}

			fft2D(real_Fuv, imag_Fuv, boy_img->GetWidth(), boy_img->GetHeight(), 1); //Forward 2dfft

			// Calculate the magnitude and apply log scaling
			std::vector<float> magnitude1(real_Fuv.size());
			for (int i = 0; i < real_Fuv.size(); i++) {
				magnitude1[i] = std::log(1 + std::sqrt(real_Fuv[i] * real_Fuv[i] + imag_Fuv[i] * imag_Fuv[i]));
			}

			fftShift(magnitude1, imag_Fuv, boy_img->GetWidth(), boy_img->GetHeight()); //Shift

			//Find min and max to normalize the real data and cast to uint8_t
			float minVal = *std::min_element(magnitude1.begin(), magnitude1.end());
			float maxVal = *std::max_element(magnitude1.begin(), magnitude1.end());

			//Dynamically allocate processdata vector
			std::vector<uint8_t> processedData;
			processedData.reserve(magnitude1.size());

			for (float val : magnitude1) {
				// Normalize the value
				float normalized = (val - minVal) / (maxVal - minVal);

				// Scale to 0-255 and convert to uint8_t
				uint8_t pixel = static_cast<uint8_t>(normalized * 255.0f);
				processedData.push_back(pixel);
			}

			// Use processedData for creating the texture
			if (!processedData.empty()) {
				bool success = image_registry->AddTexture("boy_noisy", "fft2", graphics::make_texture(processedData, boy_img->GetWidth(), boy_img->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);
			}

			if (init_sample1) {
				init_sample1 = false;
			}
		}

		float processed_x = 0.0f;
		const std::optional<graphics::Texture>& boy_sub_freq = image_registry->GetTexture("boy_noisy", "freq");
		if (boy_sub_freq.has_value()) {
			const auto& boy_img_sub = boy_sub_freq.value();
			ImVec2 img_sub_size{ (float)boy_img_sub->GetWidth(), (float)boy_img_sub->GetHeight() };
			processed_x = ImGui::GetCursorPosX();
			//bool is_hovered4 = widgets::ImageInspector("inspect4", boy_img_sub, &inspect_sub2, { 0.0f, 0.0f }, { style.ItemSpacing.x + boy_img_sub->GetWidth(), 0.0f });
			//if ((!is_hovered4)) inspect_sub2 = false;
			ImGui::Image((void*)(intptr_t)(boy_img_sub->GetRendererID()), img_sub_size);

		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SameLine();

		const std::optional<graphics::Texture>& boy_sub_spacial = image_registry->GetTexture("boy_noisy", "spatial");
		if (boy_sub_spacial.has_value()) {
			const auto& boy_img_sub = boy_sub_spacial.value();
			ImVec2 img_sub_size{ (float)boy_img_sub->GetWidth(), (float)boy_img_sub->GetHeight() };
			processed_x = ImGui::GetCursorPosX();
			//bool is_hovered4 = widgets::ImageInspector("inspect5", boy_img_sub, &inspect_sub2, { 0.0f, 0.0f }, { style.ItemSpacing.x + boy_img_sub->GetWidth(), 0.0f });
			//if ((!is_hovered4)) inspect_sub2 = false;
			ImGui::Image((void*)(intptr_t)(boy_img_sub->GetRendererID()), img_sub_size);
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::Text("Band Reject Filter Low: 35 High: 38: ");

		ImGui::SameLine();

		static bool init_sample2 = true;
		if (init_sample2) {
			// Declaring single float vectors for real and imaginary parts
			std::vector<float> real(boy_img->GetHeight() * boy_img->GetWidth());
			std::vector<float> imag(boy_img->GetHeight() * boy_img->GetWidth(), 0.0); // Initialize with zeros to set phase to 0

			//Split up data into real and imaginary in this case imaginary is 0.
			for (int i = 0; i < boy_img->GetHeight(); i++) {
				for (int j = 0; j < boy_img->GetWidth(); j++) {
					real[i * boy_img->GetWidth() + j] = static_cast<float>(rawDataBoy[i * boy_img->GetWidth() + j]);
				}
			}

			fft2D(real, imag, boy_img->GetWidth(), boy_img->GetHeight(), 1); //Forward 2dfft

			// Calculate the magnitude and apply log scaling
			std::vector<float> magnitude(real.size());
			for (int i = 0; i < real.size(); i++) {
				magnitude[i] = std::log(1 + std::sqrt(real[i] * real[i] + imag[i] * imag[i]));
			}
			fftShift(magnitude, imag, boy_img->GetWidth(), boy_img->GetHeight()); //Shift
			//low cutoff to about 1/4 of the maximum frequency and the high cutoff to about 1/2 of the maximum frequency
			bandRejectFilter(magnitude, imag, boy_img->GetWidth(), boy_img->GetHeight(), lowCutoff, highCutoff);

			// Normalize the magnitude data for visualization
			float minVal = *std::min_element(magnitude.begin(), magnitude.end());
			float maxVal = *std::max_element(magnitude.begin(), magnitude.end());

			//Dynamically allocate processdata vector
			std::vector<uint8_t> processedData;
			processedData.reserve(magnitude.size());

			for (float val : magnitude) {
				// Normalize the value
				float normalized = (val - minVal) / (maxVal - minVal);

				// Scale to 0-255 and convert to uint8_t
				uint8_t pixel = static_cast<uint8_t>(normalized * 255.0f);
				processedData.push_back(pixel);
			}

			if (!processedData.empty()) {
				bool success = image_registry->AddTexture("boy_noisy", "freq", graphics::make_texture(processedData, boy_img->GetWidth(), boy_img->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);
			}

			if (init_sample2) {
				init_sample2 = false;
			}
		}

		ImGui::SameLine();

		float x_offset3 = 2 * style.ItemSpacing.x + (((float)boy_img->GetWidth()) - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset3);
		ImGui::Text("Original Image After Applying Band Reject In Spatial Domain: ");

		static bool init_sample3 = true;
		if (init_sample3) {
			// Declaring single float vectors for real and imaginary parts
			std::vector<float> real(boy_img->GetHeight() * boy_img->GetWidth());
			std::vector<float> imag(boy_img->GetHeight() * boy_img->GetWidth(), 0.0); // Initialize with zeros to set phase to 0

			//Split up data into real and imaginary in this case imaginary is 0.
			for (int i = 0; i < boy_img->GetHeight(); i++) {
				for (int j = 0; j < boy_img->GetWidth(); j++) {
					real[i * boy_img->GetWidth() + j] = static_cast<float>(rawDataBoy[i * boy_img->GetWidth() + j]);
				}
			}

			fft2D(real, imag, boy_img->GetWidth(), boy_img->GetHeight(), 1); //Forward 2dfft

			fftShift(real, imag, boy_img->GetWidth(), boy_img->GetHeight()); //Shift

			//low cutoff to about 1/4 of the maximum frequency and the high cutoff to about 1/2 of the maximum frequency
			bandRejectFilter(real, imag, boy_img->GetWidth(), boy_img->GetHeight(), 35, 38);

			fftShift(real, imag, boy_img->GetWidth(), boy_img->GetHeight());

			fft2D(real, imag, boy_img->GetWidth(), boy_img->GetHeight(), -1); //Inverse 2dfft

			// Normalize the magnitude data for visualization
			float minVal = *std::min_element(real.begin(), real.end());
			float maxVal = *std::max_element(real.begin(), real.end());

			//Dynamically allocate processdata vector
			std::vector<uint8_t> processedData;
			processedData.reserve(real.size());

			for (float val : real) {
				// Normalize the value
				float normalized = (val - minVal) / (maxVal - minVal);

				// Scale to 0-255 and convert to uint8_t
				uint8_t pixel = static_cast<uint8_t>(normalized * 255.0f);
				processedData.push_back(pixel);
			}

			if (!processedData.empty()) {
				bool success = image_registry->AddTexture("boy_noisy", "spatial", graphics::make_texture(processedData, boy_img->GetWidth(), boy_img->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);
			}

			if (init_sample3) {
				init_sample3 = false;
			}
		}

		const std::optional<graphics::Texture>& img_opt_boynotch = image_registry->GetTexture("boy_noisy", ".pgm");
		ImGui::Separator();

		float processed_x_2nd = 0.0f;
		const std::optional<graphics::Texture>& boy_sub_freq2 = image_registry->GetTexture("boy_noisy", "freqNotch");
		if (boy_sub_freq2.has_value()) {
			const auto& boy_img_sub = boy_sub_freq2.value();
			ImVec2 img_sub_size{ (float)boy_img_sub->GetWidth(), (float)boy_img_sub->GetHeight() };
			processed_x_2nd = ImGui::GetCursorPosX();
			//bool is_hovered4 = widgets::ImageInspector("inspect6", boy_img_sub, &inspect_sub3, { 0.0f, 0.0f }, { style.ItemSpacing.x + boy_img_sub->GetWidth(), 0.0f });
			//if ((!is_hovered4)) inspect_sub3 = false;
			ImGui::Image((void*)(intptr_t)(boy_img_sub->GetRendererID()), img_sub_size);
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SameLine();

		const std::optional<graphics::Texture>& boy_sub_spacial2 = image_registry->GetTexture("boy_noisy", "spatialNotch");
		if (boy_sub_spacial2.has_value()) {
			const auto& boy_img_sub = boy_sub_spacial2.value();
			ImVec2 img_sub_size{ (float)boy_img_sub->GetWidth(), (float)boy_img_sub->GetHeight() };
			processed_x_2nd = ImGui::GetCursorPosX();
			//bool is_hovered4 = widgets::ImageInspector("inspect7", boy_img_sub, &inspect_sub3, { 0.0f, 0.0f }, { style.ItemSpacing.x + boy_img_sub->GetWidth(), 0.0f });
			//if ((!is_hovered4)) inspect_sub3 = false;
			ImGui::Image((void*)(intptr_t)(boy_img_sub->GetRendererID()), img_sub_size);
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::Text("Frequency Regect Notch W:3 Notch H:3 ");

		ImGui::SameLine();

		static bool init_sample4 = true;
		if (init_sample4) {

			// Declaring single float vectors for real and imaginary parts
			std::vector<float> real(boy_img->GetHeight() * boy_img->GetWidth());
			std::vector<float> imag(boy_img->GetHeight() * boy_img->GetWidth(), 0.0); // Initialize with zeros to set phase to 0

			//Split up data into real and imaginary in this case imaginary is 0.
			for (int i = 0; i < boy_img->GetHeight(); i++) {
				for (int j = 0; j < boy_img->GetWidth(); j++) {
					real[i * boy_img->GetWidth() + j] = static_cast<float>(rawDataBoy[i * boy_img->GetWidth() + j]);
				}
			}

			fft2D(real, imag, boy_img->GetWidth(), boy_img->GetHeight(), 1); //Forward 2dfft

			// Calculate the magnitude and apply log scaling
			std::vector<float> magnitude(real.size());
			for (int i = 0; i < real.size(); i++) {
				magnitude[i] = std::log(1 + std::sqrt(real[i] * real[i] + imag[i] * imag[i]));
			}
			fftShift(magnitude, imag, boy_img->GetWidth(), boy_img->GetHeight()); //Shift
			//low cutoff to about 1/4 of the maximum frequency and the high cutoff to about 1/2 of the maximum frequency
			applyFourNotchFilters(magnitude, imag, boy_img->GetWidth(), boy_img->GetHeight(), -centerX, -centerY, notchWidth, notchHeight,
				-centerX, centerY, notchWidth, notchHeight,
				centerX, -centerY, notchWidth, notchHeight,
				centerX, centerY, notchWidth, notchHeight);

			//bandRejectFilterMagnitude(magnitude, imag, boy_img->GetWidth(), boy_img->GetHeight(), 35, 38);

			//fft2D(magnitude, imag, boy_img->GetWidth(), boy_img->GetHeight(), -1); //Inverse 2dfft

			// Normalize the magnitude data for visualization
			float minVal = *std::min_element(magnitude.begin(), magnitude.end());
			float maxVal = *std::max_element(magnitude.begin(), magnitude.end());

			//Dynamically allocate processdata vector
			std::vector<uint8_t> processedData;
			processedData.reserve(magnitude.size());

			for (float val : magnitude) {
				// Normalize the value
				float normalized = (val - minVal) / (maxVal - minVal);

				// Scale to 0-255 and convert to uint8_t
				uint8_t pixel = static_cast<uint8_t>(normalized * 255.0f);
				processedData.push_back(pixel);
			}

			if (!processedData.empty()) {
				bool success = image_registry->AddTexture("boy_noisy", "freqNotch", graphics::make_texture(processedData, boy_img->GetWidth(), boy_img->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);
			}

			if (init_sample4) {
				init_sample4 = false;
			}
		}

		ImGui::SameLine();
		float x_offset4 = 2 * style.ItemSpacing.x + (((float)boy_img->GetWidth()) - ImGui::GetCursorPosX());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset4);
		ImGui::Text("Original Image After Applying Notch Filter In Spatial Domain:");

		ImGui::Separator();

		static bool init_sample5 = true;
		if (init_sample5) {
			// Declaring single float vectors for real and imaginary parts
			std::vector<float> real(boy_img->GetHeight() * boy_img->GetWidth());
			std::vector<float> imag(boy_img->GetHeight() * boy_img->GetWidth(), 0.0); // Initialize with zeros to set phase to 0

			//Split up data into real and imaginary in this case imaginary is 0.
			for (int i = 0; i < boy_img->GetHeight(); i++) {
				for (int j = 0; j < boy_img->GetWidth(); j++) {
					real[i * boy_img->GetWidth() + j] = static_cast<float>(rawDataBoy[i * boy_img->GetWidth() + j]);
				}
			}

			fft2D(real, imag, boy_img->GetWidth(), boy_img->GetHeight(), 1); //Forward 2dfft

			fftShift(real, imag, boy_img->GetWidth(), boy_img->GetHeight()); //Shift
			//low cutoff to about 1/4 of the maximum frequency and the high cutoff to about 1/2 of the maximum frequency
			applyFourNotchFilters(real, imag, boy_img->GetWidth(), boy_img->GetHeight(), -centerX, -centerY, notchWidth, notchHeight,
				-centerX, centerY, notchWidth, notchHeight,
				centerX, -centerY, notchWidth, notchHeight,
				centerX, centerY, notchWidth, notchHeight);

			fftShift(real, imag,  boy_img->GetWidth(), boy_img->GetHeight());

			//bandRejectFilterMagnitude(magnitude, imag, boy_img->GetWidth(), boy_img->GetHeight(), 35, 38);
			fft2D(real, imag, boy_img->GetWidth(), boy_img->GetHeight(), -1); //Inverse 2dfft

			// Normalize the magnitude data for visualization
			float minVal = *std::min_element(real.begin(), real.end());
			float maxVal = *std::max_element(real.begin(), real.end());

			//Dynamically allocate processdata vector
			std::vector<uint8_t> processedData;
			processedData.reserve(real.size());

			for (float val : real) {
				// Normalize the value
				float normalized = (val - minVal) / (maxVal - minVal);

				// Scale to 0-255 and convert to uint8_t
				uint8_t pixel = static_cast<uint8_t>(normalized * 255.0f);
				processedData.push_back(pixel);
			}

			if (!processedData.empty()) {
				bool success = image_registry->AddTexture("boy_noisy", "spatialNotch", graphics::make_texture(processedData, boy_img->GetWidth(), boy_img->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);
			}

			if (init_sample5) {
				init_sample5 = false;
			}
		}

		const std::optional<graphics::Texture>& boy_sub_blur2 = image_registry->GetTexture("boy_noisy", "blurred");
		if (boy_sub_blur2.has_value()) {
			const auto& boy_img_sub = boy_sub_blur2.value();
			ImVec2 img_sub_size{ (float)boy_img_sub->GetWidth(), (float)boy_img_sub->GetHeight() };
			processed_x_2nd = ImGui::GetCursorPosX();
			//bool is_hovered4 = widgets::ImageInspector("inspect4", boy_img_sub, &inspect_sub5, { 0.0f, 0.0f }, { style.ItemSpacing.x + boy_img_sub->GetWidth(), 0.0f });
			//if ((!is_hovered4)) inspect_sub2 = false;
			ImGui::Image((void*)(intptr_t)(boy_img_sub->GetRendererID()), img_sub_size);
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		static bool init_sample6 = true;
		if (ImGui::Button("Guassian Filter") || init_sample6) {
			if (filterSize == 7) {
				auto averagedData = smoothImage(boy_img->GetRawData(), boy_img->GetWidth(), boy_img->GetHeight(), filterSize, gausMask7x7);
				if (averagedData.has_value()) {
					auto data = averagedData.value();
					bool success = image_registry->AddTexture("boy_noisy", "blurred", graphics::make_texture(data, boy_img->GetWidth(), boy_img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
			else {
				auto averagedData = smoothImage(boy_img->GetRawData(), boy_img->GetWidth(), boy_img->GetHeight(), filterSize, gausMask15x15);
				if (averagedData.has_value()) {
					auto data = averagedData.value();
					bool success = image_registry->AddTexture("boy_noisy", "blurred", graphics::make_texture(data, boy_img->GetWidth(), boy_img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}
			}
			if (init_sample6) {
				init_sample6 = false;
			}
		}
		ImGui::SameLine();
		std::vector<std::string> items = { "7x7", "15x15" };
		static int item_current_idx = 0;
		static int item_current_idx1 = 0;
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
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Separator();

		const std::optional<graphics::Texture>& boy_sub_noise = image_registry->GetTexture("boy_noisy", "noise");
		if (boy_sub_noise.has_value()) {
			const auto& boy_img_sub = boy_sub_noise.value();
			ImVec2 img_sub_size{ (float)boy_img_sub->GetWidth(), (float)boy_img_sub->GetHeight() };
			processed_x_2nd = ImGui::GetCursorPosX();
			//bool is_hovered4 = widgets::ImageInspector("inspect8", boy_img_sub, &inspect_sub4, { 0.0f, 0.0f }, { style.ItemSpacing.x + boy_img_sub->GetWidth(), 0.0f });
			//if ((!is_hovered4)) inspect_sub4 = false;
			ImGui::Image((void*)(intptr_t)(boy_img_sub->GetRendererID()), img_sub_size);
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}
		ImGui::Text("Noise Derivation Using Notch Filter Pass: ");

		static bool init_sample7 = true;
		if (init_sample7) {
			// Declaring single float vectors for real and imaginary parts
			std::vector<float> real(boy_img->GetHeight() * boy_img->GetWidth());
			std::vector<float> imag(boy_img->GetHeight() * boy_img->GetWidth(), 0.0); // Initialize with zeros to set phase to 0

			//Split up data into real and imaginary in this case imaginary is 0.
			for (int i = 0; i < boy_img->GetHeight(); i++) {
				for (int j = 0; j < boy_img->GetWidth(); j++) {
					real[i * boy_img->GetWidth() + j] = static_cast<float>(rawDataBoy[i * boy_img->GetWidth() + j]);
				}
			}

			fft2D(real, imag, boy_img->GetWidth(), boy_img->GetHeight(), 1); //Forward 2dfft

			fftShift(real, imag, boy_img->GetWidth(), boy_img->GetHeight()); //Shift
			//low cutoff to about 1/4 of the maximum frequency and the high cutoff to about 1/2 of the maximum frequency
			applyFourNotchFiltersNoise(real, imag, boy_img->GetWidth(), boy_img->GetHeight(), -centerX, -centerY, notchWidth, notchHeight,
				-centerX, centerY, notchWidth, notchHeight,
				centerX, -centerY, notchWidth, notchHeight,
				centerX, centerY, notchWidth, notchHeight);

			fftShift(real, imag, boy_img->GetWidth(), boy_img->GetHeight());

			//bandRejectFilterMagnitude(magnitude, imag, boy_img->GetWidth(), boy_img->GetHeight(), 35, 38);
			fft2D(real, imag, boy_img->GetWidth(), boy_img->GetHeight(), -1); //Inverse 2dfft

			// Normalize the magnitude data for visualization
			float minVal = *std::min_element(real.begin(), real.end());
			float maxVal = *std::max_element(real.begin(), real.end());

			//Dynamically allocate processdata vector
			std::vector<uint8_t> processedData;
			processedData.reserve(real.size());

			for (float val : real) {
				// Normalize the value
				float normalized = (val - minVal) / (maxVal - minVal);

				// Scale to 0-255 and convert to uint8_t
				uint8_t pixel = static_cast<uint8_t>(normalized * 255.0f);
				processedData.push_back(pixel);
			}

			if (!processedData.empty()) {
				bool success = image_registry->AddTexture("boy_noisy", "noise", graphics::make_texture(processedData, boy_img->GetWidth(), boy_img->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);
			}

			if (init_sample7) {
				init_sample7 = false;
			}
		}

		ImGui::Separator();
	}
}

void Assignment4::Question2() {
	if (global::GetResourceUnwrapped("g_Assignment4ScrollToImage Convolution")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment4ScrollToImage Convolution", false);
	}
	widgets::markdown("# 2. Image Convolution");
}

void Assignment4::Question3() {
	if (global::GetResourceUnwrapped("g_Assignment4ScrollToHomomorphic Filtering")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment4ScrollToHomomorphic Filtering", false);
	}
	widgets::markdown("# 3. Homomorphic Filtering");
}
}
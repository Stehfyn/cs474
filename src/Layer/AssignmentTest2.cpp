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

		this->Question1();
		//this->Question2();
		//this->Question3();
		//this->Question4();

	}
void AssignmentTest2::Question1() {
		ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName("Question1"));

		ImVec2 size = { 2000, 1080 };
		ImGui::SetNextWindowSize(size);

		ImVec2 pos = { 0, 0 };
		ImGui::SetNextWindowPos(pos, ImGuiCond_Once);

		//widgets::markdown("# 1. Image Sampling");
		ImGui::Begin("Band Reject");

		static float lowCutoff = 64.0f;
		static float highCutoff = 128.0f;
		static const float maxCutoff = 256.0f;
		static int centerX = 224;
		static int centerY = 240;
		static int notchWidth = 3;
		static int notchHeight = 3;

		std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");

		const std::optional<graphics::Texture>& img_opt_boy = image_registry->GetTexture("boy_noisy", ".pgm");
		const std::optional<graphics::Texture>& img_opt_boynotch = image_registry->GetTexture("boy_noisy", ".pgm");

		if (img_opt_boy.has_value() && img_opt_boynotch.has_value()) {
			const auto& style = ImGui::GetStyle();
			const auto& boy_img = img_opt_boy.value();
			const std::vector<uint8_t>& rawDataBoy = boy_img->GetRawData();

			ImVec2 img_size{ (float)boy_img->GetWidth(), (float)boy_img->GetHeight() };
			bool is_hovered1 = widgets::ImageInspector("inspect1", boy_img, &inspect_sub1, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + boy_img->GetWidth()), 0.0f });

			ImGui::SameLine();

			const std::optional<graphics::Texture>& boy_sub_opt = image_registry->GetTexture("boy_noisy", "fft2");
			if (boy_sub_opt.has_value()) {
				const auto& boy_img_sub = boy_sub_opt.value();
				ImVec2 img_sub_size{ (float)boy_img_sub->GetWidth(), (float)boy_img_sub->GetHeight() };
				bool is_hovered2 = widgets::ImageInspector("inspect2", boy_img_sub, &inspect_sub1, { 0.0f, 0.0f }, { style.ItemSpacing.x + boy_img_sub->GetWidth(), 0.0f });
				if ((!is_hovered1) && (!is_hovered2)) inspect_sub1 = false;
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}


			ImGui::Text("Original ");
			ImGui::SameLine();

			//ImGui::Dummy(,)

			float x_offset = 2 * style.ItemSpacing.x + ((float)boy_img->GetWidth() - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
			ImGui::Text("Processed FFT Spectrum ");

			float x_offset3 = 2 * style.ItemSpacing.x + (((float)boy_img->GetWidth()) - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset3);
			static bool init_sample1 = true;
			ImGui::SetNextItemWidth(100.0f);
			if (ImGui::Button("Fast Fourier Transform") || init_sample1) {
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

			float processed_x_2nd = 0.0f;
			const std::optional<graphics::Texture>& boy_sub_freq = image_registry->GetTexture("boy_noisy", "freq2");
			if (boy_sub_freq.has_value()) {
				const auto& boy_img_sub = boy_sub_freq.value();
				ImVec2 img_sub_size{ (float)boy_img_sub->GetWidth(), (float)boy_img_sub->GetHeight() };
				processed_x_2nd = ImGui::GetCursorPosX();
				bool is_hovered4 = widgets::ImageInspector("inspect4", boy_img_sub, &inspect_sub2, { 0.0f, 0.0f }, { style.ItemSpacing.x + boy_img_sub->GetWidth(), 0.0f });
				if ((!is_hovered4)) inspect_sub2 = false;
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}

			ImGui::SameLine();

			const std::optional<graphics::Texture>& boy_sub_spacial = image_registry->GetTexture("boy_noisy", "spatial2");
			if (boy_sub_spacial.has_value()) {
				const auto& boy_img_sub = boy_sub_spacial.value();
				ImVec2 img_sub_size{ (float)boy_img_sub->GetWidth(), (float)boy_img_sub->GetHeight() };
				processed_x_2nd = ImGui::GetCursorPosX();
				bool is_hovered4 = widgets::ImageInspector("inspect4", boy_img_sub, &inspect_sub2, { 0.0f, 0.0f }, { style.ItemSpacing.x + boy_img_sub->GetWidth(), 0.0f });
				if ((!is_hovered4)) inspect_sub2 = false;
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}

			ImGui::Text("Frequency Regect Low: 35 High: 38 ");

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
				bandRejectFilter(magnitude, imag, boy_img->GetWidth(), boy_img->GetHeight(), 35, 38);

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
					bool success = image_registry->AddTexture("boy_noisy", "freq2", graphics::make_texture(processedData, boy_img->GetWidth(), boy_img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}

				if (init_sample2) {
					init_sample2 = false;
				}
			}

			//ImGui::SliderInt("Center X", &centerX, -512/2, 512/2);
			//ImGui::SliderInt("Center Y", &centerY, -512/2, 512/2);
			//ImGui::SliderInt("Notch Width", &notchWidth, 0, boy_img->GetWidth());
			//ImGui::SliderInt("Notch Height", &notchHeight, 0, boy_img->GetHeight());

			ImGui::SameLine();

			float x_offset4 = 2 * style.ItemSpacing.x + (((float)boy_img->GetWidth()) - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset4);
			ImGui::Text("Spatial Regect ");
			ImGui::SameLine();
			static bool init_sample3 = true;
			if (ImGui::Button("SpatialReject##boy") || init_sample3) {
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
					bool success = image_registry->AddTexture("boy_noisy", "spatial2", graphics::make_texture(processedData, boy_img->GetWidth(), boy_img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}

				if (init_sample3) {
					init_sample3 = false;
				}
			}

		}

		ImGui::Separator();
		if (img_opt_boy.has_value() && img_opt_boynotch.has_value()) {
			const auto& style = ImGui::GetStyle();
			const auto& boy_img = img_opt_boy.value();
			const std::vector<uint8_t>& rawDataBoy = boy_img->GetRawData();

			ImVec2 img_size{ (float)boy_img->GetWidth(), (float)boy_img->GetHeight() };
			bool is_hovered1 = widgets::ImageInspector("inspect1", boy_img, &inspect_sub1, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + boy_img->GetWidth()), 0.0f });

			ImGui::SameLine();

			const std::optional<graphics::Texture>& boy_sub_opt = image_registry->GetTexture("boy_noisy", "fft");
			if (boy_sub_opt.has_value()) {
				const auto& boy_img_sub = boy_sub_opt.value();
				ImVec2 img_sub_size{ (float)boy_img_sub->GetWidth(), (float)boy_img_sub->GetHeight() };
				bool is_hovered2 = widgets::ImageInspector("inspect2", boy_img_sub, &inspect_sub1, { 0.0f, 0.0f }, { style.ItemSpacing.x + boy_img_sub->GetWidth(), 0.0f });
				if ((!is_hovered1) && (!is_hovered2)) inspect_sub1 = false;
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}


			ImGui::Text("Original ");
			ImGui::SameLine();

			//ImGui::Dummy(,)

			float x_offset = 2 * style.ItemSpacing.x + ((float)boy_img->GetWidth() - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
			ImGui::Text("Processed FFT Spectrum ");

			float x_offset3 = 2 * style.ItemSpacing.x + (((float)boy_img->GetWidth()) - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset3);
			static bool init_sample1 = true;
			ImGui::SetNextItemWidth(100.0f);
			if (ImGui::Button("Fast Fourier Transform ") || init_sample1) {
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
					bool success = image_registry->AddTexture("boy_noisy", "fft", graphics::make_texture(processedData, boy_img->GetWidth(), boy_img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}

				if (init_sample1) {
					init_sample1 = false;
				}
			}

			float processed_x_2nd = 0.0f;
			const std::optional<graphics::Texture>& boy_sub_freq = image_registry->GetTexture("boy_noisy", "freq");
			if (boy_sub_freq.has_value()) {
				const auto& boy_img_sub = boy_sub_freq.value();
				ImVec2 img_sub_size{ (float)boy_img_sub->GetWidth(), (float)boy_img_sub->GetHeight() };
				processed_x_2nd = ImGui::GetCursorPosX();
				bool is_hovered4 = widgets::ImageInspector("inspect4", boy_img_sub, &inspect_sub2, { 0.0f, 0.0f }, { style.ItemSpacing.x + boy_img_sub->GetWidth(), 0.0f });
				if ((!is_hovered4)) inspect_sub2 = false;
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}

			ImGui::SameLine();

			const std::optional<graphics::Texture>& boy_sub_spacial = image_registry->GetTexture("boy_noisy", "spatial");
			if (boy_sub_spacial.has_value()) {
				const auto& boy_img_sub = boy_sub_spacial.value();
				ImVec2 img_sub_size{ (float)boy_img_sub->GetWidth(), (float)boy_img_sub->GetHeight() };
				processed_x_2nd = ImGui::GetCursorPosX();
				bool is_hovered4 = widgets::ImageInspector("inspect4", boy_img_sub, &inspect_sub2, { 0.0f, 0.0f }, { style.ItemSpacing.x + boy_img_sub->GetWidth(), 0.0f });
				if ((!is_hovered4)) inspect_sub2 = false;
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}

			ImGui::Text("Frequency Regect Notch W:3 Notch H:3 ");

			ImGui::SameLine();

			static bool init_sample2 = true;
			if (ImGui::Button("FrequencyReject##boy") || init_sample2) {


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
					bool success = image_registry->AddTexture("boy_noisy", "freq", graphics::make_texture(processedData, boy_img->GetWidth(), boy_img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}

				if (init_sample2) {
					init_sample2 = false;
				}
			}

			//ImGui::SliderInt("Center X", &centerX, -512/2, 512/2);
			//ImGui::SliderInt("Center Y", &centerY, -512/2, 512/2);
			//ImGui::SliderInt("Notch Width", &notchWidth, 0, boy_img->GetWidth());
			//ImGui::SliderInt("Notch Height", &notchHeight, 0, boy_img->GetHeight());

			ImGui::SameLine();

			float x_offset4 = 2 * style.ItemSpacing.x + (((float)boy_img->GetWidth()) - ImGui::GetCursorPosX());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset4);
			ImGui::Text("Spatial Regect ");

			ImGui::SameLine();
			static bool init_sample3 = true;
			if (ImGui::Button("SpatialReject##boy") || init_sample3) {
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
					bool success = image_registry->AddTexture("boy_noisy", "spatial", graphics::make_texture(processedData, boy_img->GetWidth(), boy_img->GetHeight(), 1));
					emscripten_log(EM_LOG_CONSOLE, "%d", success);
				}

				if (init_sample3) {
					init_sample3 = false;
				}
			}

		}

	ImGui::End();

}
void AssignmentTest2::Question2() {
	ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName("Question1"));

	ImVec2 size = { 2000, 1080 };
	ImGui::SetNextWindowSize(size);

	ImVec2 pos = { 500, 0 };
	ImGui::SetNextWindowPos(pos, ImGuiCond_Once);

	ImGui::Begin("2D-FFT with Magnitude And Phase");

	static std::string currentImage = "lenna"; // Default image
	const char* items[] = { "lenna", "boat", "f_16" };
	static int item_current_idx = 0; // Here we store our selection data as an index.

	std::shared_ptr<graphics::ImageRegistry> image_registry = std::shared_ptr<graphics::ImageRegistry>(global::GetResourceMutUnwrapped("g_ImageRegistry"));
	const std::optional<graphics::Texture>& img_opt = image_registry->GetTexture(currentImage, ".pgm");

	//Original image
	if (img_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img = img_opt.value();
		const std::vector<uint8_t>& rawData = img->GetRawData();
		ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
		bool is_hovered1 = widgets::ImageInspector("inspect1", img, &inspect_sub, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + img->GetWidth()), 0.0f });

		ImGui::SameLine();

		// Part 3A frequency representation of image
		const std::optional<graphics::Texture>& sub_3a = image_registry->GetTexture(currentImage, "2DFFT");

		if (sub_3a.has_value()) {
			const auto& img_sub = sub_3a.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			bool is_hovered2 = widgets::ImageInspector("inspect2", img_sub, &inspect_sub, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_sub->GetWidth(), 0.0f });
			if ((!is_hovered1) && (!is_hovered2)) inspect_sub = false;
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SameLine();

		// Part 3B image
		const std::optional<graphics::Texture>& sub_3b = image_registry->GetTexture(currentImage, "2DFFT3b");

		if (sub_3b.has_value()) {
			const auto& img_sub_3b = sub_3b.value();
			ImVec2 img_sub_size3{ (float)img_sub_3b->GetWidth(), (float)img_sub_3b->GetHeight() };
			bool is_hovered3 = widgets::ImageInspector("inspect3", img_sub_3b, &inspect_sub, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_sub_3b->GetWidth(), 0.0f });
			if ((!is_hovered1) && (!is_hovered3)) inspect_sub = false;
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::SetNextItemWidth(150.0f);
		if (ImGui::Combo("Select Image", &item_current_idx, items, IM_ARRAYSIZE(items))) {
			currentImage = items[item_current_idx];
		}

		ImGui::SameLine(0, 20);

		static bool init_sample3a = true;
		static float c = 0.0f;
		ImGui::SetNextItemWidth(150.0f);
		if (ImGui::Button("2D-FFT Phase = 0")) {
			// Declaring single float vectors for real and imaginary parts
			std::vector<float> real(img->GetHeight() * img->GetWidth());
			std::vector<float> imag(img->GetHeight() * img->GetWidth(), 0.0); // Initialize with zeros to set phase to 0

			//Split up data into real and imaginary in this case imaginary is 0.
			for (int i = 0; i < img->GetHeight(); i++) {
				for (int j = 0; j < img->GetWidth(); j++) {
					real[i * img->GetWidth() + j] = static_cast<float>(rawData[i * img->GetWidth() + j]);
				}
			}

			fft2D(real, imag, img->GetWidth(), img->GetHeight(), 1); //Forward 2dfft

			// Set real to magnitude and set imaginary to zero after fft
			for (size_t i = 0; i < real.size(); ++i) {
				float magnitude = std::sqrt(real[i] * real[i] + imag[i] * imag[i]);
				real[i] = abs(magnitude); // Set real part to magnitude
				imag[i] = 0.0f; // Set imaginary part to zero
			}

			fft2D(real, imag, img->GetWidth(), img->GetHeight(), -1); //Inverse 2dfft

			fftShift(real, imag, img->GetWidth(), img->GetHeight()); //Shift

			//Find min and max to normalize the real data and cast to uint8_t
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

			// Use processedData for creating the texture
			if (!processedData.empty()) {
				bool success = image_registry->AddTexture(currentImage, "2DFFT", graphics::make_texture(processedData, img->GetWidth(), img->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);
			}

			if (init_sample3a) {
				init_sample3a = false;
			}
		}

		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		ImGui::DragFloat("Shift", &c, 0.0f, 0, 128);

		ImGui::SameLine(0, 150);

		static bool init_sample3b = true;
		ImGui::SetNextItemWidth(100.0f);
		if (ImGui::Button("2D-FFT Magnitude = 1")) {
			// Declaring single float vectors for real and imaginary parts
			std::vector<float> real_Fuv(img->GetHeight() * img->GetWidth());
			std::vector<float> imag_Fuv(img->GetHeight() * img->GetWidth(), 0.0); // Initialize with zeros

			for (int i = 0; i < img->GetHeight(); i++) {
				for (int j = 0; j < img->GetWidth(); j++) {
					real_Fuv[i * img->GetWidth() + j] = static_cast<float>(rawData[i * img->GetWidth() + j]);
				}
			}

			fft2D(real_Fuv, imag_Fuv, img->GetWidth(), img->GetHeight(), 1); //Forward 2dfft

			// Normalize magnitude to 1 and retain original phase
			for (size_t i = 0; i < real_Fuv.size(); ++i) {
				float realPart = real_Fuv[i];
				float imagPart = imag_Fuv[i];

				// Calculate the phase
				float phase = atan2(imagPart, realPart); //Note: to compute tan - 1, use the function atan2().

				// Set real and imaginary parts to cos(theta) and sin(theta)
				real_Fuv[i] = cos(phase);
				imag_Fuv[i] = sin(phase);
			}

			fft2D(real_Fuv, imag_Fuv, img->GetWidth(), img->GetHeight(), -1); //Inverse 2dfft

			//Find min and max to normalize the real data and cast to uint8_t
			float minVal = *std::min_element(real_Fuv.begin(), real_Fuv.end());
			float maxVal = *std::max_element(real_Fuv.begin(), real_Fuv.end());

			//Dynamically allocate processdata vector
			std::vector<uint8_t> processedData;
			processedData.reserve(real_Fuv.size());

			for (float val : real_Fuv) {
				// Normalize the value
				float normalized = (val - minVal) / (maxVal - minVal);

				// Scale to 0-255 and convert to uint8_t
				uint8_t pixel = static_cast<uint8_t>(normalized * 255.0f);
				processedData.push_back(pixel);
			}

			// Use processedData for creating the texture
			if (!processedData.empty()) {
				bool success = image_registry->AddTexture(currentImage, "2DFFT3b", graphics::make_texture(processedData, img->GetWidth(), img->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);
			}

			if (init_sample3b) {
				init_sample3b = false;
			}
		}
	}
	ImGui::End();
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
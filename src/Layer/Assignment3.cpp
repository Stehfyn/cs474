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

	//Rectangle function stuff
	std::vector<float> originalRectWave = utils::ReadFloatData("assets/data/Rect_128.txt");
	int rectSamples = originalRectWave.size();
	std::vector<float> rectData(2 * rectSamples);

	//Cos wave stuff
	const int samples = 128;
	const int cycles = 8;
	static float cosData[2 * samples]; // Array size doubled for real and imaginary parts
	std::vector<float> originalCosWave(samples);

	//Signal stuff
	const char* signalOptions[] = { "2,3,4,4", "1,2,3,4", "5,6,7,8", "-2,-3,-4,-4", "-9,5,6,0" };
	static int currentSignal = 0;

	const unsigned long nn = 4;
	static float data[2 * nn];
	static std::vector<std::vector<float>> signals = { {-9,5,6,0}, {-2,-3,-4,-4}, {5,6,7,8}, {1,2,3,4}, {2,3,4,4} };

	auto signal = signals[currentSignal];

	static std::vector<float> realPart(nn), imagPart(nn), magnitude(nn);
	static std::vector<float> realPartCos(samples), imagPartCos(samples), magnitudeCos(samples);

	// Initialize the data array with the real signal interleaved with zeros for the imaginary parts.
	for (unsigned long i = 0; i < nn; i++) {
		data[2 * i] = signal[i];
		data[2 * i + 1] = 0;
	}

	// Generate the cosine wave
	for (int i = 0; i < samples; ++i) {
		cosData[2 * i] = cos(2 * M_PI * cycles * i / samples);
		cosData[2 * i + 1] = 0;
		originalCosWave[i] = cosData[2 * i];
	}

	// Generate rectangle wave
	for (int i = 0; i < rectSamples; ++i) {
		rectData[2 * i] = originalRectWave[i];
		rectData[2 * i + 1] = 0;
		originalRectWave[i] = rectData[2 * i];
	}

	// Perform the FFT on the selected signal
	fft1D(data, nn, 1);

	// Perform the FFT on the cosine wave
	fft1D(cosData, samples, 1);

	//Perform FFT of rectangle function
	fft1D(rectData.data(), rectSamples, 1);

	// Process the FFT data for the selected signal
	for (int i = 0; i < nn; i++) {
		realPart[i] = data[2 * i];
		imagPart[i] = data[2 * i + 1];
		magnitude[i] = std::sqrt(realPart[i] * realPart[i] + imagPart[i] * imagPart[i]);
	}

	// Process the FFT data for the cosine wave
	static int shift = 30;
	for (int i = 0; i < samples; i++) {
		realPartCos[i] = cosData[2 * i];
		imagPartCos[i] = cosData[2 * i + 1];
		magnitudeCos[i] = std::sqrt(realPartCos[i] * realPartCos[i] + imagPartCos[i] * imagPartCos[i]);
		std::rotate(magnitudeCos.rbegin(), magnitudeCos.rbegin() + shift, magnitudeCos.rend());
		//std::rotate(magnitudeCos.rbegin(), magnitudeCos.rbegin() + (magnitudeCos.size() / (2*2)), magnitudeCos.rend());
	}

	// Process the FFT data for the rectangle wave
	std::vector<float> realPartRect(rectSamples), imagPartRect(rectSamples), magnitudeRect(rectSamples);
	for (int i = 0; i < rectSamples; i++) {

		realPartRect[i] = rectData[2 * i];
		imagPartRect[i] = rectData[2 * i + 1];
		magnitudeRect[i] = std::sqrt(realPartRect[i] * realPartRect[i] + imagPartRect[i] * imagPartRect[i]);
		std::rotate(magnitudeRect.begin(), magnitudeRect.begin() + (magnitudeRect.size() / 2), magnitudeRect.end());
	}

	widgets::markdown("# 1.a");

	// Plotting the FFT results for the selected signal as dots
	ImPlot::SetNextAxesToFit();
	if (ImPlot::BeginPlot("Signal 1D-FFT Results", "X-axis", "Y-axis", ImVec2(400, 400), 0, ImPlotAxisFlags_AutoFit)) {
		ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
		ImPlot::PlotScatter("Real Part", realPart.data(), nn);
		ImPlot::PlotScatter("Imaginary Part", imagPart.data(), nn);
		ImPlot::PlotScatter("Magnitude", magnitude.data(), nn);
		ImPlot::EndPlot();
	}

	// Dropdown menu to select the signal
	ImGui::SetNextItemWidth(300);
	ImGui::Combo("Select Signal", &currentSignal, signalOptions, IM_ARRAYSIZE(signalOptions));

	ImGui::Separator();
	widgets::markdown("# 1.b");

	// Plotting the original cosine wave
	ImPlot::SetNextAxesToFit();
	if (ImPlot::BeginPlot("Original Cosine Wave \n (f(x) = cos(2πux) where u = 8 and N = 128) ", "X-axis", "Y-axis", ImVec2(400, 400))) {
		ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
		ImPlot::PlotLine("Cosine Wave", originalCosWave.data(), samples);
		ImPlot::EndPlot();
	}

	ImGui::SameLine();

	// Plotting the FFT results for the cosine wave
	ImPlot::SetNextAxesToFit();
	if (ImPlot::BeginPlot("Cosine Wave 1D-FFT", "X-axis", "Y-axis", ImVec2(400, 400))) {
		ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
		ImPlot::PlotLine("Real Part", realPartCos.data(), samples);
		ImPlot::PlotLine("Imaginary Part", imagPartCos.data(), samples);
		ImPlot::PlotLine("Magnitude", magnitudeCos.data(), samples);
		ImPlot::EndPlot();
	}

	ImGui::SameLine();

	// Plotting the FFT results for the rectangle wave
	ImPlot::SetNextAxesToFit();
	if (ImPlot::BeginPlot("Rectangle Wave 1D-FFT", "X-axis", "Y-axis", ImVec2(400, 400))) {
		ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
		ImPlot::PlotLine("Real Part", realPartRect.data(), rectSamples);
		ImPlot::PlotLine("Imaginary Part", imagPartRect.data(), rectSamples);
		ImPlot::PlotLine("Magnitude", magnitudeRect.data(), rectSamples);
		ImPlot::EndPlot();
	}

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 410);
	ImGui::SetNextItemWidth(150);
	ImGui::DragInt("Shift", &shift, 1, 0, 128);

	ImGui::Separator();
	widgets::markdown("# 2D-FFT Check");

	static std::string currentImage = "lenna"; // Default image
	const char* items[] = { "lenna", "boat", "f_16" };
	static int item_current_idx = 0; // Here we store our selection data as an index.

	//Start of 2d fft check
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& img_opt = image_registry->GetTexture(currentImage, ".pgm");

	//Original image
	if (img_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img = img_opt.value();
		const std::vector<uint8_t>& rawData = img->GetRawData();
		ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
		bool is_hovered_check1 = widgets::ImageInspector("inspect1", img, &inspect_check, { 0.0f, 0.0f }, { -1.0f * (style.ItemSpacing.x + img->GetWidth()), 0.0f });

		ImGui::SameLine();

		const std::optional<graphics::Texture>& sub_3a = image_registry->GetTexture(currentImage, "2DFFT");

		if (sub_3a.has_value()) {
			const auto& img_sub = sub_3a.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			bool is_hovered_check2 = widgets::ImageInspector("inspect2", img_sub, &inspect_check, { 0.0f, 0.0f }, { style.ItemSpacing.x + img_sub->GetWidth(), 0.0f });
			if ((!is_hovered_check1) && (!is_hovered_check2)) inspect_check = false;
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}

		ImGui::Text("Original: ");
		ImGui::SameLine();
		ImGui::SetCursorPosX(img->GetWidth() + 10);
		ImGui::Text("After Forward and Inverse 2D-FFT: ");


		static float c = 0.0f;
		ImGui::SetNextItemWidth(150.0f);
		if (ImGui::Combo("Image", &item_current_idx, items, IM_ARRAYSIZE(items))) {
			currentImage = items[item_current_idx];
			const std::optional<graphics::Texture>& next_img_opt = image_registry->GetTexture(currentImage, ".pgm");
			if (next_img_opt.has_value()) this->Process2DFFT(next_img_opt.value(), currentImage);
		}

		//ImGui::SameLine();

		static bool init_sample3a = true;
		//ImGui::SetCursorPosX(img->GetWidth() + 20);
		//ImGui::SetNextItemWidth(200.0f);
		//if (ImGui::Button("2D-FFT") || init_sample3a) {
		if (init_sample3a) {
			this->Process2DFFT(img, currentImage);

			if (init_sample3a) {
				init_sample3a = false;
			}
		}
	}

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

		const std::optional<graphics::Texture>& twoAMag_opt = image_registry->GetTexture("lenna", "subject2a_mag");
		const std::optional<graphics::Texture>& twoAMagshift_opt = image_registry->GetTexture("lenna", "subject2a_magshift");
		if (twoAMag_opt.has_value() && twoAMagshift_opt.has_value()) {
			ImGui::SameLine();
			ImGui::Image((ImTextureID)twoAMag_opt.value()->GetRendererID(), ImVec2(512, 512));
			ImGui::SameLine();
			ImGui::Image((ImTextureID)twoAMagshift_opt.value()->GetRendererID(), ImVec2(512, 512));
		}

		widgets::markdown("# 2.b");
		ImGui::Image((ImTextureID)twoB_opt.value()->GetRendererID(), ImVec2(512, 512));

		const std::optional<graphics::Texture>& twoBMag_opt = image_registry->GetTexture("lenna", "subject2b_mag");
		const std::optional<graphics::Texture>& twoBMagshift_opt = image_registry->GetTexture("lenna", "subject2b_magshift");
		if (twoBMag_opt.has_value() && twoBMagshift_opt.has_value()) {
			ImGui::SameLine();
			ImGui::Image((ImTextureID)twoBMag_opt.value()->GetRendererID(), ImVec2(512, 512));
			ImGui::SameLine();
			ImGui::Image((ImTextureID)twoBMagshift_opt.value()->GetRendererID(), ImVec2(512, 512));
		}

		widgets::markdown("# 2.c");
		ImGui::Image((ImTextureID)twoC_opt.value()->GetRendererID(), ImVec2(512, 512));

		const std::optional<graphics::Texture>& twoCMag_opt = image_registry->GetTexture("lenna", "subject2c_mag");
		const std::optional<graphics::Texture>& twoCMagshift_opt = image_registry->GetTexture("lenna", "subject2c_magshift");
		if (twoCMag_opt.has_value() && twoCMagshift_opt.has_value()) {
			ImGui::SameLine();
			ImGui::Image((ImTextureID)twoCMag_opt.value()->GetRendererID(), ImVec2(512, 512));
			ImGui::SameLine();
			ImGui::Image((ImTextureID)twoCMagshift_opt.value()->GetRendererID(), ImVec2(512, 512));
		}
	}
}
void Assignment3::Experiment3() {
	if (global::GetResourceUnwrapped("g_Assignment3ScrollToExperiment 3")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment3ScrollToExperiment 3", false);
	}
	widgets::markdown("# Experiment 3");

	static std::string currentImage = "lenna"; // Default image
	const char* items[] = { "lenna", "boat", "f_16" };
	static int item_current_idx = 0; // Here we store our selection data as an index.

	widgets::markdown("# 3.a");

	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	const std::optional<graphics::Texture>& img_opt = image_registry->GetTexture(currentImage, ".pgm");

	//Original image
	if (img_opt.has_value()) {
		const auto& style = ImGui::GetStyle();
		const auto& img = img_opt.value();
		const std::vector<uint8_t>& rawData = img->GetRawData();
		ImVec2 img_size{ (float)img->GetWidth(), (float)img->GetHeight() };
		ImVec2 not_hovered_offset1 = (is_hovered_exp3_3) ? ImVec2( 0.0f, -1.0f * (style.ItemSpacing.y + img->GetHeight() + yOffset)) : ImVec2( - 1.0f * (style.ItemSpacing.x + img->GetWidth()), 0.0f );
		is_hovered_exp3_1 = widgets::ImageInspector("inspect3", img, &inspect_exp3, { 0.0f, 0.0f }, not_hovered_offset1);
	
		ImGui::SameLine();

		// Part 3A frequency representation of image
		const std::optional<graphics::Texture>& sub_3a = image_registry->GetTexture(currentImage, "2DFFT3a");

		if (sub_3a.has_value()) {
			const auto& img_sub = sub_3a.value();
			ImVec2 img_sub_size{ (float)img_sub->GetWidth(), (float)img_sub->GetHeight() };
			ImVec2 not_hovered_offset2 = (is_hovered_exp3_3) ? ImVec2((style.ItemSpacing.x + img->GetWidth()), -1.0f * (style.ItemSpacing.y + img->GetHeight() + yOffset)) : ImVec2((style.ItemSpacing.x + img->GetWidth()), 0.0f);
			is_hovered_exp3_2 = widgets::ImageInspector("inspect4", img_sub, &inspect_exp3, { 0.0f, 0.0f }, not_hovered_offset2);
		}
		else {
			ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
		}
		yStart = ImGui::GetCursorPosY();
		ImGui::Text("Original: ");
		ImGui::SameLine();
		ImGui::SetCursorPosX(img->GetWidth() + 20);
		ImGui::Text("Reconstructed Magnitude Only: ");


		static float c = 0.0f;
		ImGui::SetNextItemWidth(150.0f);
		if (ImGui::Combo("Select Image", &item_current_idx, items, IM_ARRAYSIZE(items))) {
			currentImage = items[item_current_idx];
		}

		ImGui::SameLine();

		ImGui::SetCursorPosX(img->GetWidth() + 20);
		ImGui::SetNextItemWidth(150.0f);


		//////////////////////////////////////
		//Start of calculation for phase = 0.
		/////////////////////////////////////
		static bool init_sample3a = true;
		ImGui::SetNextItemWidth(100.0f);
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
				real[i] = magnitude; // Set real part to magnitude
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
				bool success = image_registry->AddTexture(currentImage, "2DFFT3a", graphics::make_texture(processedData, img->GetWidth(), img->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);
			}

			if (init_sample3a) {
				init_sample3a = false;
			}
		}

			ImGui::Separator();
			widgets::markdown("# 3.b");


			// Part 3B image
			const std::optional<graphics::Texture>& sub_3b = image_registry->GetTexture(currentImage, "2DFFT3b");

			yEnd = ImGui::GetCursorPosY();
			yOffset = yEnd - yStart;
			if (sub_3b.has_value()) {
				const auto& img_sub_3b = sub_3b.value();
				ImVec2 img_sub_size3{ (float)img_sub_3b->GetWidth(), (float)img_sub_3b->GetHeight() };
				ImVec2 not_hovered_offset3 = (is_hovered_exp3_1) ? ImVec2(0.0f, 1.0f * (style.ItemSpacing.y + img->GetHeight() + yOffset)) : ImVec2(-1.0f * (style.ItemSpacing.x + img->GetWidth()), 1.0f * (style.ItemSpacing.y + img->GetHeight()));
				is_hovered_exp3_3 = widgets::ImageInspector("inspect5", img_sub_3b, &inspect_exp3, { 0.0f, 0.0f }, not_hovered_offset3);
				if ((!is_hovered_exp3_1) && (!is_hovered_exp3_2) && (!is_hovered_exp3_3)) inspect_exp3 = false;
			}
			else {
				ImGui::Image((void*)(intptr_t)(size_t)-1, img_size);
			}


		ImGui::Text("Reconstructed Phase Only:");

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

}
void Assignment3::Process2DFFT(const graphics::Texture& img, const std::string& currentImage) {
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");

	// Declaring single float vectors for real and imaginary parts
	std::vector<float> real(img->GetHeight() * img->GetWidth());
	std::vector<float> imag(img->GetHeight() * img->GetWidth(), 0.0); // Initialize with zeros to set phase to 0

	//Split up data into real and imaginary in this case imaginary is 0.
	for (int i = 0; i < img->GetHeight(); i++) {
		for (int j = 0; j < img->GetWidth(); j++) {
			real[i * img->GetWidth() + j] = static_cast<float>(img->GetRawData()[i * img->GetWidth() + j]);
		}
	}

	fft2D(real, imag, img->GetWidth(), img->GetHeight(), 1); //Forward 2dfft

	fft2D(real, imag, img->GetWidth(), img->GetHeight(), -1); //Inverse 2dfft

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

	const auto& mag_n_shifted_mag = this->ProcessSubject2(data, 
				512, 512);

	const auto& mag = mag_n_shifted_mag.first;
	const auto& shifted_mag = mag_n_shifted_mag.second;

	image_registry->AddTexture("lenna", "subject2a_mag", graphics::make_texture(mag, 512, 512, 1));
	image_registry->AddTexture("lenna", "subject2a_magshift", graphics::make_texture(shifted_mag, 512, 512, 1));


}
void Assignment3::GenerateSubject2b() {
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	auto data = GenerateSubject(512, 512, 64, 64);
	image_registry->AddTexture("lenna", "subject2b", graphics::make_texture(data, 512, 512, 1));

	const auto& mag_n_shifted_mag = this->ProcessSubject2(data, 
				512, 512);

	const auto& mag = mag_n_shifted_mag.first;
	const auto& shifted_mag = mag_n_shifted_mag.second;

	image_registry->AddTexture("lenna", "subject2b_mag", graphics::make_texture(mag, 512, 512, 1));
	image_registry->AddTexture("lenna", "subject2b_magshift", graphics::make_texture(shifted_mag, 512, 512, 1));
}
void Assignment3::GenerateSubject2c() {
	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");
	auto data = GenerateSubject(512, 512, 128, 128);
	image_registry->AddTexture("lenna", "subject2c", graphics::make_texture(data, 512, 512, 1));

	const auto& mag_n_shifted_mag = this->ProcessSubject2(data, 
				512, 512);

	const auto& mag = mag_n_shifted_mag.first;
	const auto& shifted_mag = mag_n_shifted_mag.second;

	image_registry->AddTexture("lenna", "subject2c_mag", graphics::make_texture(mag, 512, 512, 1));
	image_registry->AddTexture("lenna", "subject2c_magshift", graphics::make_texture(shifted_mag, 512, 512, 1));
}
std::pair<std::vector<uint8_t>, std::vector<uint8_t>> Assignment3::ProcessSubject2(
	const std::vector<uint8_t>& data, 
	unsigned int width, 
	unsigned int height) {

	std::shared_ptr<graphics::ImageRegistry> image_registry = global::GetResourceMutUnwrapped("g_ImageRegistry");

	const auto& img = data;
	std::vector<float> real(width * height);
	std::vector<float> imag(width * height, 0.0); // Initialize with zeros to set phase to 0

	// Split up data into real and imaginary in this case imaginary is 0.
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			real[i * width + j] = static_cast<float>(data[i * width + j]);
		}
	}

	std::vector<float> mag;
	mag.reserve(real.size());

	fft2D(real, imag, width, height, 1); //Forward 2dfft
	
	for (size_t i = 0; i < real.size(); ++i) {
		float magnitude = std::sqrt(real[i] * real[i] + imag[i] * imag[i]);
		mag.push_back(0.5 * std::log(1 + magnitude)); // Set real part to magnitude
	}

	std::vector<uint8_t> processedMag;
	processedMag.reserve(real.size());
	//Find min and max to normalize the real data and cast to uint8_t
	float minVal = *std::min_element(mag.begin(), mag.end());
	float maxVal = *std::max_element(mag.begin(), mag.end());
	for (float val : mag) {
		// Normalize the value
		float normalized = (val - minVal) / (maxVal - minVal);
		uint8_t pixel = static_cast<uint8_t>(normalized * 255.0f);
		processedMag.push_back(pixel);
	}

	
	fftShift(real, imag, width, height); //Shift

	std::vector<float> mag_shifted;
	mag_shifted.reserve(real.size());

	for (size_t i = 0; i < real.size(); ++i) {
		float magnitude = std::sqrt(real[i] * real[i] + imag[i] * imag[i]);
		mag_shifted.push_back(0.5 * std::log(1 + magnitude));
	}

	std::vector<uint8_t> processedMagShifted;
	processedMagShifted.reserve(real.size());
	minVal = *std::min_element(mag_shifted.begin(), mag_shifted.end());
	maxVal = *std::max_element(mag_shifted.begin(), mag_shifted.end());
	for (float val : mag_shifted) {
		// Normalize the value
		float normalized = (val - minVal) / (maxVal - minVal);
		uint8_t pixel = static_cast<uint8_t>(normalized * 255.0f);
		processedMagShifted.push_back(pixel);
	}

	return { processedMag, processedMagShifted };
}
}
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
		this->Question2();
		//this->Question3();
		//this->Question4();

	}
void AssignmentTest2::Question1() {
    ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName("Question1"));

    ImVec2 size = { 2000, 1080 };
    ImGui::SetNextWindowSize(size);

    ImVec2 pos = { 500, 0 };
    ImGui::SetNextWindowPos(pos, ImGuiCond_Once);

    ImGui::Begin("DFT Plot");

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
    const char* signalOptions[] = { "2,3,4,4", "1,2,3,4", "5,6,7,8", "-2,-3,-4,-4", "-9,5,6,0"};
	static int currentSignal = 0;

	const unsigned long nn = 4;
    static float data[2 * nn];
	static std::vector<std::vector<float>> signals = {{-9,5,6,0}, {-2,-3,-4,-4}, {5,6,7,8}, {1,2,3,4}, {2,3,4,4}};

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

    // Plotting the FFT results for the selected signal
	ImPlot::SetNextAxesToFit();
    if (ImPlot::BeginPlot("Signal 1D-FFT Results", "X-axis", "Y-axis", ImVec2(400, 400), 0, ImPlotAxisFlags_AutoFit)) {
		ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
        ImPlot::PlotLine("Real Part", realPart.data(), nn);
        ImPlot::PlotLine("Imaginary Part", imagPart.data(), nn);
        ImPlot::PlotLine("Magnitude", magnitude.data(), nn);
        ImPlot::EndPlot();
    }
	ImGui::SameLine();

	// Plotting the original cosine wave
	ImPlot::SetNextAxesToFit();
	if (ImPlot::BeginPlot("Original Cosine Wave", "X-axis", "Y-axis", ImVec2(400, 400))) {
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

	// Dropdown menu to select the signal
	ImGui::SetNextItemWidth(300);
	ImGui::Combo("Select Signal", &currentSignal, signalOptions, IM_ARRAYSIZE(signalOptions));

	ImGui::SameLine();
	ImGui::DragInt("Shift", &shift, 1.0f, 0, 128);

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

			const float c = 1.0f;
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
				bool success = image_registry->AddTexture(currentImage, "2DFFT", graphics::make_texture(processedData, img->GetWidth(), img->GetHeight(), 1));
				emscripten_log(EM_LOG_CONSOLE, "%d", success);
			}
			
			if (init_sample3a) {
				init_sample3a = false;
			}
		}

		ImGui::SameLine(0, 150);

		static bool init_sample3b = true;
		ImGui::SetNextItemWidth(100.0f);
		if (ImGui::Button("2D-FFT Magnitude = 1")){
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
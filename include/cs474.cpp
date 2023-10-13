#include "cs474.h"
// cs474-lib.cpp : Defines the functions for the static library.
//

// TODO: This is an example of a library function

namespace cs474 {
	void fcns474lib()
	{

	}
	std::optional<std::vector<uint8_t>> subSample(const std::vector<uint8_t>& rawData, int factor, int width, int height)
	{
		if (width != height)
		{
			return std::nullopt;
		}
		if ((width / factor) % 2 != 0)
		{
			return std::nullopt;
		}
		int newWidth = width / factor;
		int newHeight = height / factor;
		std::vector<uint8_t> subsampleData(newWidth * newHeight);

		for (int y = 0; y < newHeight; ++y)
		{
			for (int x = 0; x < newWidth; ++x)
			{
				int subIndex = (y * factor * width) + (x * factor);
				subsampleData[(y * newWidth) + x] = rawData[subIndex];
			}
		}
		return { subsampleData };
	}

	std::optional<std::vector<uint8_t>> scale(const std::vector<uint8_t>& subsampleData, int factor, int width, int height)
	{
		if (width != height)
		{
			return std::nullopt;
		}
		if ((width / factor) % 2 != 0)
		{
			return std::nullopt;
		}

		int scaledWidth = width * factor;
		int scaledHeight = height * factor;

		std::vector<uint8_t> scaledData(scaledWidth * scaledHeight);

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				uint8_t value = subsampleData[y * width + x];
				for (int sy = 0; sy < factor; ++sy) {
					for (int sx = 0; sx < factor; ++sx) {
						scaledData[(y * factor + sy) * scaledWidth + (x * factor + sx)] = value;
					}
				}
			}
		}

		return { scaledData };
	}

	std::optional<std::vector<uint8_t>> quantize(const std::vector<uint8_t>& data, uint8_t levels, int width, int height) {
		if (width != height)
		{
			return std::nullopt;
		}

		std::vector<uint8_t> quantized(width * height);
		int bin_size = UINT8_MAX / levels;
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				int bin = data[(y * width) + x] / bin_size;
				if (bin == levels) bin -= 1; // force into lower bin 
				quantized[(y * width) + x] = bin * bin_size;
			}
		}

		return { quantized };
	}

	std::vector<float> hist(const std::vector<uint8_t>& data) {
		std::vector<float> hist(UINT8_MAX + 1, 0);
		for (const auto& point : data) hist[point]++;
		return hist;
	}

	// https://math.stackexchange.com/a/2021283
	std::vector<float> cdf(const std::vector<float>& data) {
		std::vector<float> _cdf(data.size());
		std::partial_sum(data.begin(), data.end(), _cdf.begin());
		return _cdf;
	}

	std::vector<uint8_t> norm(const std::vector<float>& data, int total_pixels) {
		auto firstNonZeroIter = std::find_if(data.begin(), data.end(), [](float c) { return c > 0; });
		float minCdfValue = (firstNonZeroIter != data.end()) ? *firstNonZeroIter : 0;

		std::vector<uint8_t> norm(UINT8_MAX + 1);
		for (int i = 0; i < UINT8_MAX + 1; ++i) {
			norm[i] = static_cast<uint8_t>((data[i] - minCdfValue) * UINT8_MAX / (total_pixels - minCdfValue));
		}
		
		return norm;
	}

	std::vector<uint8_t> hist_eq(const std::vector<uint8_t>& data, int width, int height) {
		auto _hist = hist(data);
		auto _cdf = cdf(_hist);
		auto _norm = norm(_cdf, width * height);

		std::vector<uint8_t> equalized(width * height);
		for (int i = 0; i < width * height; ++i) {
			equalized[i] = _norm[data[i]];
		}

		return equalized;
	}

	std::optional<std::vector<uint8_t>> hist_spec(const std::vector<uint8_t>& srcData, const std::vector<uint8_t>& specData, int width, int height) {
		// Step 1: Equalize the histogram of the source image
		auto srcEqualized = hist_eq(srcData, width, height);
		auto specifiedHist = hist(specData);

		// Step 2: Equalize the specified histogram to obtain the mapping
		std::vector<float> specCdf = cdf(specifiedHist);
		std::vector<uint8_t> specNorm = norm(specCdf, std::accumulate(specifiedHist.begin(), specifiedHist.end(), 0));

		// Step 3: Apply the mapping to the uniformly equalized source image
		std::vector<uint8_t> specifiedData(width * height);
		for (int i = 0; i < width * height; ++i) {
			uint8_t srcValue = srcEqualized[i];
			uint8_t mappedValue = 0;
			size_t minDiff = SIZE_MAX;  // Initialize to maximum possible value
        
			// Search for the closest value in the specified histogram
			for (size_t j = 0; j <= UINT8_MAX; ++j) {
				size_t diff = std::abs(static_cast<int>(srcValue) - static_cast<int>(specNorm[j]));
				if (diff < minDiff) {
					minDiff = diff;
					mappedValue = static_cast<uint8_t>(j);
				}
			}
        
			specifiedData[i] = mappedValue;
		}

		return { specifiedData };
	}

	std::vector<uint8_t> spatial_filtering(const std::vector<uint8_t>& image_data, int image_width, int image_height, const std::vector<uint8_t>& mask_data, int mask_width, int mask_height)
	{
		int output_width = image_width - mask_width + 1;
		int output_height = image_height - mask_height + 1;
		std::vector<uint8_t> output_data(output_width * output_height);

		for (int i = 0; i < output_height; ++i) {
			for (int j = 0; j < output_width; ++j) {
				int sum = 0;
				for (int mi = 0; mi < mask_height; ++mi) {
					for (int mj = 0; mj < mask_width; ++mj) {
						sum += image_data[(i + mi) * image_width + (j + mj)] * mask_data[mi * mask_width + mj];
					}
				}
				output_data[i * output_width + j] = sum;  // You might want to normalize this value
			}
		}
		return output_data;
	}
} // namespace cs474
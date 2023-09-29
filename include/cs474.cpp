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
		int bin_size = ((uint8_t)-1) / levels;
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				int bin = data[(y * width) + x] / bin_size;
				quantized[(y * width) + x] = bin * bin_size;
			}
		}

		return { quantized };
	}

	std::vector<size_t> hist(const std::vector<uint8_t>& data) {
		std::vector<size_t> hist(UINT8_MAX + 1, 0);
		for (const auto& point : data) hist[point]++;
		return hist;
	}

	// https://math.stackexchange.com/a/2021283
	std::vector<size_t> cdf(const std::vector<size_t>& data) {
		std::vector<size_t> _cdf(data.size());
		std::partial_sum(data.begin(), data.end(), _cdf.begin());
		return _cdf;
	}

	std::vector<uint8_t> norm(const std::vector<size_t>& data, int total_pixels) {
		auto firstNonZeroIter = std::find_if(data.begin(), data.end(), [](size_t c) { return c > 0; });
		size_t minCdfValue = (firstNonZeroIter != data.end()) ? *firstNonZeroIter : 0;

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
}
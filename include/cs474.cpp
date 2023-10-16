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

	std::optional<std::vector<uint8_t>> smoothImage(const std::vector<uint8_t>& srcData, int width, int height, int filterSize, const std::vector<double>& filterMask) {

		std::vector<uint8_t> smoothedData(width * height);
		int offset = filterSize / 2;  // Offset to center the filter mask on a pixel

		//Apply filter to each pixel
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				double sum = 0.0;
				double weightSum = 0.0;

				//Apply the filter mask to neighboring pixels
				for (int j = -offset; j <= offset; ++j) {
					for (int i = -offset; i <= offset; ++i) {
						int newY = y + j;
						int newX = x + i;

						//Skip pixels outside the image boundaries
						if (newX < 0 || newX >= width || newY < 0 || newY >= height) {
							continue;
						}
						double weight = filterMask[(j + offset) * filterSize + (i + offset)];

						sum += srcData[newY * width + newX] * weight;
						weightSum += weight;
					}
				}
				//Normalize and assign the new pixel value
				smoothedData[y * width + x] = static_cast<uint8_t>(std::round(sum / weightSum));
			}
		}
		return { smoothedData };
	}

	std::optional<std::vector<uint8_t>> addNoise(const std::vector<uint8_t>& data, int width, int height, float percentage) {
		std::vector<uint8_t> corrupted = data;
		int total_pixels = width * height;
		int num_corrupted = static_cast<int>(percentage / 100.0 * total_pixels);

		std::srand(std::time(nullptr));

		for (int i = 0; i < num_corrupted; ++i) {
			int rand_index = std::rand() % total_pixels;
			uint8_t salt_or_pepper = (std::rand() % 2 == 0) ? 0 : 255;
			corrupted[rand_index] = salt_or_pepper;
		}

		return { corrupted };
	}

	std::optional<std::vector<uint8_t>> medianFilter(const std::vector<uint8_t>& data, int width, int height, int filterSize) {

		std::vector<uint8_t> filtered(data.size());
		int offset = filterSize / 2;

		// Iterate over each pixel in the image
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				std::vector<uint8_t> neighbors;

				// Collect neighbors
				for (int x = -offset; x <= offset; ++x) {
					for (int y = -offset; y <= offset; ++y) {
						int ni = i + x;
						int nj = j + y;

						// Check boundary conditions
						if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
							neighbors.push_back(data[ni * width + nj]);
						}
					}
				}

				// Find the median value among the neighbors
				std::sort(neighbors.begin(), neighbors.end());
				filtered[i * width + j] = neighbors[neighbors.size() / 2];
			}
		}

		return { filtered };
	}

	std::optional<std::vector<uint8_t>> unsharpAndBoostFilter(const std::vector<uint8_t>& data, int width, int height, int kValue, int filterSize)
	{

	}
} // namespace cs474
#pragma once
#include <cstdint>
#include <vector>
#include <optional>
#include <numeric>
#include <functional>

namespace cs474 {

	static const std::vector<double> avgMask7x7(49, 1.0 / 49.0);
	static const std::vector<double> avgMask15x15(225, 1.0 / 225.0);
	static const std::vector<double> gausMask7x7 = { 1,1,2,2,2,1,1,1,2,2,4,2,2,1,2,2,4,8,4,2,2,2,4,8,16,8,4,2,2,2,4,8,4,2,2,1,2,2,4,2,2,1,1,1,2,2,2,1,1 };
	static const std::vector<double> gausMask15x15 = { 2,2,3,4,5,5,6,6,6,5,5,4,3,2,2,2,3,4,5,7,7,8,8,8,7,7,5,4,3,2,3,4,6,9,10,10,11,10,10,9,7,5,4,3,2,4,5,7,9,10,12,13,13,13,12,10,9,7,6,4,3,
	5,7,9,11,13,14,15,16,15,14,13,11,9,7,5,5,7,10,12,14,16,17,18,17,16,14,12,10,7,5,6,8,10,13,15,17,19,19,19,17,15,13,10,8,6,6,8,11,13,15,17,19,20,19,18,16,13,11,8,6,6,8,10,13,15,17,19,19,19,17,15,13,10,8,6,
	5,7,10,12,14,16,17,18,17,16,14,12,10,7,5,5,7,9,11,13,14,15,16,15,14,13,11,9,7,5,4,5,7,9,10,12,13,13,13,12,10,9,7,6,4,3,3,4,6,9,10,10,11,10,10,9,7,5,4,3,2,2,3,4,5,7,7,8,8,8,7,7,5,4,3,2,2,2,3,4,5,5,6,6,6,5,5,4,3,2,2 };
	static const std::vector<int> prewittHorz = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	static const std::vector<int> prewittVert = { -1, 0, 1, -1, 0, 1, -1, 0, 1};
	static const std::vector<int> sobelHorz = { -1, -2, -1, 0, 0, 0, 1, 2, 1};
	static const std::vector<int> sobelVert = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	static const std::vector<int> laplacian = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };

	void fcns474lib();
	std::vector<uint8_t> normr(const std::vector<float>& data);
	std::optional<std::vector<uint8_t>> subSample(const std::vector<uint8_t>& rawData, int factor, int width, int height);
	std::optional<std::vector<uint8_t>> scale(const std::vector<uint8_t>& subsampleData, int factor, int width, int height);
	std::optional<std::vector<uint8_t>> quantize(const std::vector<uint8_t>& data, uint8_t levels, int width, int height);
	std::vector<float> hist(const std::vector<uint8_t>& data);
	std::vector<float> cdf(const std::vector<float>& data);
	std::vector<uint8_t> norm(const std::vector<float>& data, int total_pixels);
	std::vector<uint8_t> hist_eq(const std::vector<uint8_t>& data, int width, int height);
	std::optional<std::vector<uint8_t>> hist_spec(const std::vector<uint8_t>& srcData, const std::vector<uint8_t>& specData, int width, int height);
	std::vector<float> spatial_filtering(const std::vector<uint8_t>& image_data, int image_width, int image_height, const std::vector<uint8_t>& mask_data, int mask_width, int mask_height);
	std::optional<std::vector<uint8_t>> smoothImage(const std::vector<uint8_t>& srcData, int width, int height, int filterSize, const std::vector<double>& filterMask);
	std::optional <std::vector<uint8_t>> addNoise(const std::vector<uint8_t>& data, int width, int height, float percentage);
	std::optional<std::vector<uint8_t>> medianFilter(const std::vector<uint8_t>& data, int width, int height, int filterSize);
	std::optional<std::vector<uint8_t>> unsharpAndBoostFilter(const std::vector<uint8_t>& originaldata, const std::vector<uint8_t>& blurredData, int width, int height, int kValue);
	std::vector<float> convolve(const std::vector<uint8_t>& data, int image_width, int image_height, const std::vector<int>& mask, int mask_width, int mask_height);
	std::vector<float> convolve(const std::vector<float>& data, int image_width, int image_height, const std::vector<int>& mask, int mask_width, int mask_height);
	std::vector<float> gradient_magnitude(const std::vector<float>& dataX, const std::vector<float>& dataY, int image_width, int image_height);
	std::vector<uint8_t> sharpen(const std::vector<uint8_t>& data, const std::vector<uint8_t>& edges, int image_width, int image_height);
	std::vector<uint8_t> clamp_overflow_to_uint8_max(std::vector<float>& data);
	std::vector<uint8_t> to_uint8_max(std::vector<float>& data);
	std::vector<uint8_t> visualize_partials(const std::vector<float>& dataX);
	std::vector<uint8_t> flip(const std::vector<uint8_t>& dataX);
	std::vector<uint8_t> threshold(const std::vector<uint8_t>& data, int thresholdValue, bool above);
	std::vector<uint8_t> threshold_range(const std::vector<uint8_t>& data, int min, int max, bool flip);
	void fft1D(float data[], unsigned long nn, int isign);
	void fft2D(std::vector<float>& realPart, std::vector<float>& imagPart, int width, int height, int isign);
	void fftShift(std::vector<float>& realPart, std::vector<float>& imagPart, int width, int height);
	void bandRejectFilter(std::vector<float>& realPart, std::vector<float>& imagPart, int width, int height, float lowCutOff, float highCutOff);
	void applyFourNotchFilters(std::vector<float>& realPart, std::vector<float>& imagPart,
			int width, int height,
			int centerX1, int centerY1, int notchWidth1, int notchHeight1,
			int centerX2, int centerY2, int notchWidth2, int notchHeight2,
			int centerX3, int centerY3, int notchWidth3, int notchHeight3,
			int centerX4, int centerY4, int notchWidth4, int notchHeight4);

	void applyFourNotchFiltersNoise(std::vector<float>& realPart, std::vector<float>& imagPart,
			int width, int height,
			int centerX1, int centerY1, int notchWidth1, int notchHeight1,
			int centerX2, int centerY2, int notchWidth2, int notchHeight2,
			int centerX3, int centerY3, int notchWidth3, int notchHeight3,
			int centerX4, int centerY4, int notchWidth4, int notchHeight4);
	void highFrequencyEmphasisFilter(std::vector<float>& realPart, std::vector<float>& imagPart, int width, int height, float D0, float c, float gammaL, float gammaH);
	std::vector<float> convolve1DHorizontal(const std::vector<uint8_t>& input, int width, int height, const std::vector<int>& kernel);
	std::vector<float> convolve1DVertical(const std::vector<float>& input, int width, int height, const std::vector<int>& kernel);
} // namespace cs474
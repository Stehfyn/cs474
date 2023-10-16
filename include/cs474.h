#pragma once
#include <cstdint>
#include <vector>
#include <optional>
#include <numeric>
#include <functional>

namespace cs474 {
	void fcns474lib();
	std::vector<uint8_t> normr(const std::vector<uint8_t>& data);
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
} // namespace cs474
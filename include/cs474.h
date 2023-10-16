#pragma once
#include <cstdint>
#include <vector>
#include <optional>
#include <numeric>

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
} // namespace cs474
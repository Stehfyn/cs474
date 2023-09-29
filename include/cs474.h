#pragma once
#include <cstdint>
#include <vector>
#include <optional>
#include <numeric>

namespace cs474 {
	void fcns474lib();
	std::optional<std::vector<uint8_t>> subSample(const std::vector<uint8_t>& rawData, int factor, int width, int height);
	std::optional<std::vector<uint8_t>> scale(const std::vector<uint8_t>& subsampleData, int factor, int width, int height);
	std::optional<std::vector<uint8_t>> quantize(const std::vector<uint8_t>& data, uint8_t levels, int width, int height);
	std::vector<size_t> hist(const std::vector<uint8_t>& data);
	std::vector<size_t> cdf(const std::vector<uint8_t>& data);
	std::vector<uint8_t> norm(const std::vector<uint8_t>& data, int total_pixels);
	std::vector<uint8_t> hist_eq(const std::vector<uint8_t>& data, int width, int height);
}
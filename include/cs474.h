#pragma once
#include <cstdint>
#include <vector>
#include <optional>

namespace cs474 {
	void fcns474lib();
	std::optional<std::vector<uint8_t>> subSample(const std::vector<uint8_t>& rawData, int factor, int width, int height);
	std::optional<std::vector<uint8_t>> scale(const std::vector<uint8_t>& subsampleData, int factor, int width, int height);
}
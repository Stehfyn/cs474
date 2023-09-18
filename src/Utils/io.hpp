#pragma once
#include "detail/io-detail.h"
namespace cs474 {
namespace utils {
static std::optional<std::vector<char>> SlurpFile(const std::filesystem::path path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(file_size);
    if (file.read(buffer.data(), file_size)) return buffer;
    return std::nullopt;
}
static bool BurpFile(const std::filesystem::path& path, std::vector<char> buffer) {
    if (buffer.size() > 0) {
        try {
            std::ofstream file(path, std::ios::out | std::ios::binary);
            file.write(&buffer.data()[0], buffer.size());
            file.close();
            return true;
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    return false;
}

enum Depth {
    Max = (size_t)-1,
};

static std::vector<std::filesystem::path> Traverse(const std::filesystem::path& dir, 
    const char* pattern = "*", size_t depth = Depth::Max) {
    std::vector<std::filesystem::path> paths;

    if (depth == 0) return paths;

    auto search_set = detail::parse_search_pattern(pattern);

    if (std::filesystem::exists(dir) && std::filesystem::is_directory(dir)) {
        std::vector<std::filesystem::path> sub_folders;
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            const std::filesystem::path& path = entry.path();
            if (detail::is_in_search_set(path, search_set))
                paths.push_back(path);
            if (std::filesystem::is_directory(path)) {
                auto sub_paths = Traverse(path, pattern, depth - 1);
                paths.insert(paths.end(), sub_paths.begin(), sub_paths.end());
            }
        }
    }
    return paths;
}
}
}
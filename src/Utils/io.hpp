#pragma once
#include "detail/io-detail.h"
namespace cs474 {
namespace utils {
using FileList = std::vector<std::filesystem::path>;

static std::optional<std::vector<char>> SlurpFile(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(file_size);
    if (file.read(buffer.data(), file_size)) return buffer;
    return std::nullopt;
}

static bool BurpFile(const std::filesystem::path& path, const std::vector<char>& buffer) {
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


static std::vector<float> ReadFloatData(const std::filesystem::path& path) {
    std::ifstream file(path);

    // If the file is not opened successfully, return an empty vector
    if (!file.is_open()) {
        return {};
    }

    std::vector<float> data;
    std::string line;

    // Read each line from the file
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        float value;

        // Read each float from the line
        while (iss >> value) {
            data.push_back(value);
        }

        // If there is an error during reading (other than end-of-file), return an empty vector
        if (iss.fail() && !iss.eof()) {
            return {};
        }
    }

    return data;
}


enum Depth {
    Max = (size_t)-1,
};

static FileList Traverse(const std::filesystem::path& dir, 
    const char* pattern = "*", size_t depth = Depth::Max) {
    auto search_set = detail::parse_search_pattern(pattern);
    return detail::traverse_impl(dir, search_set, depth);
}

static std::unordered_map<std::filesystem::path, FileList> SortByStem(const std::vector<std::filesystem::path>& paths,
    bool include_dirs = false) {
    std::unordered_map<std::filesystem::path, FileList> sorted;
    for (const auto& path : paths) {
        if (path.has_stem() && 
            (include_dirs == std::filesystem::is_directory(path))) {
            auto it = sorted.find(path.stem());
            if (it != sorted.end()) {
                it->second.push_back(path);
            } else {
                sorted.insert({ path.stem(), { path } });
            }
        }
    }
    return sorted;
}

template <typename T> //https://stackoverflow.com/a/16606128
static std::string ToStringWithPrecision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return std::move(out).str();
}
}
} // namespace cs474
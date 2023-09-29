#pragma once
namespace cs474 {
namespace utils {
namespace detail {
static std::vector<std::string> tokenize(const char* str) {
    std::vector<std::string> split;
    std::istringstream iss(str);
    std::string token;

    while (iss >> token) split.push_back(token);
    return split;
}

static std::optional<std::string> validate(const std::string& token) {
    if (token == "*") return token;

    if (token.length() >= 2) {
        auto i = token.find(".");
        if ((i != std::string::npos)
            && (token[0] == '*')
            && (token[1] == '.')
            && (i != (token.size() - 1))) {
            return std::string(++(token.begin()), token.end());
        }
    }
    return std::nullopt;
}

static std::unordered_set<std::string> parse_search_pattern(const char* pattern) {
    std::unordered_set<std::string> search_set;
    auto tokens = tokenize(pattern);

    for (const auto& token : tokens) {
        auto valid_token = validate(token);
        if (valid_token.has_value()) {
            search_set.emplace(valid_token.value());
        }
    }
    return search_set;
}

static bool is_in_search_set(const std::filesystem::path& path, const std::unordered_set<std::string>& search_set) {
    auto it = search_set.find(path.extension().generic_string());
    return !((it == search_set.end()) && (search_set.find("*") == search_set.end()));
}

static std::vector<std::filesystem::path> traverse_impl(const std::filesystem::path& dir,
    const std::unordered_set<std::string>& search_set, size_t depth) {

    std::vector<std::filesystem::path> paths;
    if (depth == 0) return paths;

    if (std::filesystem::exists(dir) && std::filesystem::is_directory(dir)) {
        std::vector<std::filesystem::path> sub_folders;
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            const std::filesystem::path& path = entry.path();
            if (detail::is_in_search_set(path, search_set))
                paths.push_back(path);
            if (std::filesystem::is_directory(path)) {
                auto sub_paths = traverse_impl(path, search_set, depth - 1);
                paths.insert(paths.end(), sub_paths.begin(), sub_paths.end());
            }
        }
    }
    return paths;
}
}
} // namespace detail
} // namespace cs474
namespace std {
    template <> struct hash<std::filesystem::path> {
        size_t operator()(const std::filesystem::path& path) const {
            return std::hash<std::string>{}(path.generic_string());
        }
    };
} //namespace std
#pragma once
namespace cs474 {
namespace utils {
namespace detail {
static std::vector<std::string> split_tokens(const char* str) {
    std::vector<std::string> split;
    std::istringstream iss(str);
    std::string token;

    while (iss >> token) split.push_back(token);
    return split;
}
static std::optional<std::string> validate_token(const std::string& token) {
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
    auto tokens = split_tokens(pattern);

    for (const auto& token : tokens) {
        auto valid_token = validate_token(token);
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
}
}
}
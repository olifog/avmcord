//
// Created by Oliver Fogelin on 15/03/2025.
//

#ifndef STRING_HASH_H
#define STRING_HASH_H

#include <string_view>

constexpr uint64_t hash(const std::string_view str) noexcept {
    uint64_t hash = 0;
    for (const char c : str) {
        hash = (hash * 131) + c;
    }
    return hash;
}

constexpr uint64_t operator"" _hash(const char* str, size_t len) {
    return hash(std::string_view(str, len));
}

#endif //STRING_HASH_H

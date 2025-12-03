#include <print>
#include <ranges>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <chrono>
#include "ctre.hpp"

namespace stdr=std::ranges;
namespace stdv=std::views;

struct Range {
    long long lower;
    long long upper;
};

std::istream& operator>>(std::istream& in, Range& r) {
    char junk;
    in >> r.lower;
    in >> junk;
    in >> r.upper;
    in >> junk;
    return in;
}

bool is_invalid(long long id) {
    const auto s = std::to_string(id);
    if (s.size() % 2) return false;
    auto mid = s.begin() + s.size()/2;
    return std::equal(s.begin(), mid, mid);
}

bool is_invalid2(long long id) {
    const auto s = std::to_string(id);
    auto s2 = s;
    for (unsigned i = 0; i < s.size() / 2; ++i) {
        std::rotate(s2.begin(), s2.begin()+1, s2.end());
        if (std::equal(s2.begin(), s2.end(), s.begin()))
            return true;
    }
    return false;
}

template<int part>
bool is_invalid_re(long long id) {
    const auto s = std::to_string(id);
    if constexpr (part == 1) {
        return ctre::match<"(\\d+)\\1">(s);
    } else {
        return ctre::match<"(\\d+)\\1+">(s);
    }
}

int main(int, char* argv[]) {
    std::ifstream file(argv[1]);
    const auto rngs = stdv::istream<Range>(file)
        | stdr::to<std::vector>();
    auto all_ids = rngs 
        | stdv::transform([](auto r) { return stdv::iota(r.lower, r.upper+1); })
        | stdv::join;
    auto invalid1 = all_ids | stdv::filter(is_invalid);
    auto invalid2 = all_ids | stdv::filter(is_invalid2);
    std::print("Part 1: {}\nPart 2: {}\n", 
            stdr::fold_left(invalid1, 0, std::plus<>()),
            stdr::fold_left(invalid2, 0, std::plus<>()));

    // auto invalid3 = all_ids | stdv::filter(is_invalid_re<1>);
    // auto invalid4 = all_ids | stdv::filter(is_invalid_re<2>);
    // std::print("Part 1: {}\nPart 2: {}\n", 
            // stdr::fold_left(invalid3, 0, std::plus<>()),
            // stdr::fold_left(invalid4, 0, std::plus<>()));
}

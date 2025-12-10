#include <print>
#include <ranges>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <string>

namespace stdr=std::ranges;
namespace stdv=std::views;

template<int digits>
auto joltage(std::string const& row) {
    std::string result;
    result.reserve(digits);
    auto it = row.begin();
    for (int i = 0; i < digits; ++i) {
        it = std::max_element(it, row.end() - (digits - i - 1));
        assert(it != row.end());
        result.push_back(*it);
        ++it;
    }
    return std::stoll(result);
}

int main(int, char* argv[]) {
    std::ifstream file(argv[1]);
    const auto rows = stdv::istream<std::string>(file) 
        | stdr::to<std::vector>();

    const auto part1 = stdr::fold_left(rows | stdv::transform(joltage<2>), 
            0ll, std::plus<>());
    const auto part2 = stdr::fold_left(rows | stdv::transform(joltage<12>), 
            0ll, std::plus<>());
    std::print("Part 1: {}\nPart 2: {}\n", part1, part2);
}

#include <print>
#include <ranges>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>

namespace stdr=std::ranges;
namespace stdv=std::views;

struct Spin {
    int value;
};

std::istream& operator>>(std::istream& in, Spin& spin) {
    char dir;
    in >> dir;
    in >> spin.value;
    if (dir == 'L')
        spin.value *= -1;
    return in;
}

int zeros_between(int a, int b) {
    // Returns the number of integers divisible by 100 in the interval (a, b]
    assert(a > 0 && b > 0);
    auto diff = std::abs(a/100 - b/100);
    if (b < a) {
        diff += b % 100 == 0;
        diff -= a % 100 == 0;
    }

    assert(diff >= 0);
    return diff;
}

int main(int, char* argv[]) {
    std::ifstream file(argv[1]);
    auto spins = stdv::istream<Spin>(file)
        | stdv::transform([](auto s) { return s.value; })
        | stdr::to<std::vector>();
    std::vector<int> positions;
    positions.push_back(50);
    std::inclusive_scan(spins.begin(), spins.end(), std::back_inserter(positions),
            std::plus<>(), 50);
    auto zero_count = stdr::count_if(positions, [](auto p) { return p % 100 == 0; });

    // [a,b,c,d,e]
    // zeros_between((a, b]) + zeros_between((b, c]) + ...
    const auto min_position = *stdr::min_element(positions);
    const auto shift = 100 * (std::abs(min_position) / 100 + 2);

    auto shifted = positions 
        | stdv::transform([=](auto p) { return p + shift; });

    auto clicks = shifted
        | stdv::adjacent_transform<2>(zeros_between);

    std::print("Pos\tClicks\n");
    for (auto [p, c] : stdv::zip(shifted, clicks))
        std::print("{}\t{}\n", p, c);

    auto total_clicks = stdr::fold_left(clicks, 0, std::plus<>());


    std::print("Part 1: {}\nPart 2: {}\n",
            zero_count, total_clicks);

    //std::print("{}\n", positions | stdv::transform([](auto x) { return x / 100; }));
}

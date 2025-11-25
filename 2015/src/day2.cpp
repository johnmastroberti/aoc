#include <print>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <vector>
#include <array>

namespace stdr=std::ranges;
namespace stdv=std::views;

struct Dims {
    int l, w, h;
    constexpr std::array<int, 3> array() const {
        return std::array{l, w, h};
    }
};

std::istream& operator>>(std::istream& s, Dims& d) {
    char junk;
    s >> d.l;
    s >> junk;
    s >> d.w;
    s >> junk;
    s >> d.h;
    return s;
}

template <>
struct std::formatter<Dims> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const Dims& d, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "[{} by {} by {}]", 
                d.l, d.w, d.h);
    }
};

int area(Dims d) {
    auto sides = d.array();
    const auto volume = d.l * d.w * d.h;
    stdr::transform(sides, sides.begin(), 
            [=](auto s) { return volume / s; });
    const auto smallest = *stdr::min_element(sides);
    return smallest + 2 * stdr::fold_left(sides, 0, std::plus<>());
}

int ribbon(Dims d) {
    auto sides = d.array();
    stdr::sort(sides);
    return 2 * (sides[0] + sides[1]) + sides[0] * sides[1] * sides[2];
}

int main(int, char* argv[]) {
    std::ifstream file(argv[1]);
    auto dims = stdv::istream<Dims>(file)
        | stdr::to<std::vector>();

    auto areas = dims | stdv::transform(area);
    auto total = stdr::fold_left(areas, 0, std::plus<>());
    std::print("Part 1: {}\n", total);

    auto ribbon_length = dims | stdv::transform(ribbon);
    auto p2 = stdr::fold_left(ribbon_length, 0, std::plus<>());
    std::print("Part 2: {}\n", p2);
}

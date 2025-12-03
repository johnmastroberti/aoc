#include <print>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <vector>
#include <array>

namespace stdr=std::ranges;
namespace stdv=std::views;


struct Vec2 {
    int x;
    int y;
    constexpr auto operator<=>(const Vec2&) const = default;
    static constexpr Vec2 from_char(char c) {
        switch (c) {
            case '<': return Vec2{-1, 0};
            case '>': return Vec2{+1, 0};
            case '^': return Vec2{0, +1};
            case 'v': return Vec2{0, -1};
            default: return Vec2{0, 0};
        }
    }
};

Vec2 operator+(Vec2 v1, Vec2 v2) {
    return Vec2{ v1.x + v2.x, v1.y + v2.y};
}

void remove_duplicates(std::vector<Vec2>& v) {
    stdr::sort(v);
    auto removed = stdr::unique(v);
    v.erase(removed.begin(), removed.end());
}

void scan(auto&& range, auto out, auto init, auto binop = std::plus<>()) {
    auto value = init;
    for (auto&& x : range) {
        *out = value;
        ++out;
        value = binop(value, x);
    }
    *out = value;
}

int main(int, char* argv[]) {
    std::ifstream file(argv[1]);
    const auto moves = stdv::istream<char>(file)
        | stdv::transform(Vec2::from_char)
        | stdr::to<std::vector>();

    std::vector visited = {Vec2{0,0}};

    std::inclusive_scan(moves.begin(), moves.end(),
            std::back_inserter(visited));

    remove_duplicates(visited);
    std::print("Part 1: {}\n", visited.size());

    visited.clear();

    const auto santa_moves = moves | stdv::stride(2);
    const auto robot_moves = moves | stdv::drop(1) | stdv::stride(2);

    scan(santa_moves, std::back_inserter(visited), Vec2{0,0}, std::plus<>());
    scan(robot_moves, std::back_inserter(visited), Vec2{0,0}, std::plus<>());

    remove_duplicates(visited);
    std::print("Part 2: {}\n", visited.size());

}

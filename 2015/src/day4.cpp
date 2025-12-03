#include <print>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <vector>
#include <array>

namespace stdr=std::ranges;
namespace stdv=std::views;


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

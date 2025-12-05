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

int main(int, char* argv[]) {
    std::ifstream file(argv[1]);
    const auto rows = stdv::istream<std::string>(file) 
        | stdr::to<std::vector>();
    const auto W = rows.front().size();
    const auto L = rows.size();

    std::vector<int> data((W+2)*(L+2), 0);
    auto grid = [&](size_t i, size_t j) -> int& { return data[(W+2)*i + j]; };
    for (size_t i = 1; i <= L; ++i) {
        for (size_t j = 1; j <= W; ++j) {
            grid(i, j) = rows[i-1][j-1] == '@';
        }
    }

    auto accessible = [&](size_t i, size_t j) {
        return grid(i-1, j) + grid(i+1, j) + grid(i, j+1) + grid(i, j-1)
                    + grid(i-1, j-1) + grid(i-1, j+1) 
                    + grid(i+1, j-1) + grid(i+1, j+1) < 4;
    };

    size_t part1 = 0;
    for (size_t i = 1; i <= L; ++i) {
        for (size_t j = 1; j <= W; ++j) {
            part1 += accessible(i, j) && grid(i, j);
        }
    }


    bool made_update;
    size_t part2 = 0;
    do {
        made_update = false;
        for (size_t i = 1; i <= L; ++i) {
            for (size_t j = 1; j <= W; ++j) {
                if (accessible(i, j) && grid(i, j)) {
                    grid(i, j) = 0;
                    ++part2;
                    made_update = true;
                }
            }
        }
    } while (made_update);

    std::print("Part 1: {}\nPart 2: {}\n", part1, part2);

}

#include <print>
#include <ranges>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <string>
#include <optional>

namespace stdr=std::ranges;
namespace stdv=std::views;


int main(int, char* argv[]) {
    std::ifstream file(argv[1]);
    std::vector<size_t> current, next;
    size_t split_count = 0;
    for (auto line : stdv::istream<std::string>(file)) {
        const auto N = line.size();
        current.resize(N);
        next.clear();
        next.resize(N);

        for (auto i : stdv::iota(0ull, N)) {
            if (line[i] == 'S') {
                next[i] += 1;
            } else if (line[i] == '^') {
                if (current[i]) {
                    ++split_count;
                    next[i-1] += current[i];
                    next[i+1] += current[i];
                }
            } else {
                next[i] += current[i];
            }
        }
        std::swap(current, next);
    }
    std::print("Part 1: {}\nPart 2: {}\n", split_count,
            stdr::fold_left(current, 0ull, std::plus<>()));
}

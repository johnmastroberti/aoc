#include <print>
#include <ranges>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>

namespace stdr=std::ranges;
namespace stdv=std::views;

int to_move(char p) {
    switch (p) {
        case '(': return +1;
        case ')': return -1;
        default: return 0;
    }
}

int main(int, char* argv[]) {
    std::ifstream file(argv[1]);
    auto moves = stdv::istream<char>(file)
        | stdv::transform(to_move)
        | stdr::to<std::vector>();
    std::inclusive_scan(moves.begin(), moves.end(), moves.begin());
    auto in_basement = stdr::find(moves, -1);

    std::print("Part 1: {}\nPart 2: {}\n",
            *moves.rbegin(),
            in_basement - moves.begin() + 1);

}

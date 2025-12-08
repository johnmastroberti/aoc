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

void part1(const char* filename) {
    std::ifstream file(filename);
    std::string line;
    auto numbers = stdv::istream<size_t>(file) | stdr::to<std::vector>();
    file.clear();
    auto ops = stdv::istream<char>(file) | stdr::to<std::vector>();
    
    const auto N = ops.size();
    auto group_i = [N, &numbers](auto ix) {
        return numbers | stdv::drop(ix) | stdv::stride(N);
    };

    auto groups = stdv::iota(0ull, N) | stdv::transform(group_i);

    auto evaluate_group = [](auto item) {
        auto [op, group] = item;
        if (op == '+') 
            return stdr::fold_left(group, 0ull, std::plus<>());
        else
            return stdr::fold_left(group, 1ull, std::multiplies<>());
    };

    auto ans = stdr::fold_left(stdv::zip(ops, groups) 
            | stdv::transform(evaluate_group), 0ull, std::plus<>());
    std::print("Part 1: {}\n", ans);
}

// std::pair<size_t, std::optional<char>>
auto parse_column(std::vector<std::string> const& lines, size_t index) {
    size_t value = 0;
    std::optional<char> op;
    for (auto const& line : lines) {
        switch (line[index]) {
            case '*':
            case '+':
                op.emplace(line[index]);
                break;
            case ' ':
                continue;
            default: // 0-9
                value = value*10 + (line[index]-'0');
        }
    }
    return std::pair(value, op);
}

void part2(const char* filename) {
    std::ifstream file(filename);
    std::vector<std::string> lines;
    while (file.peek() != -1) {
        lines.emplace_back();
        std::getline(file, lines.back());
    }
    size_t total = 0;
    std::vector<size_t> stack;
    const size_t N = lines[0].size();
    for (size_t i = 0; i<N; ++i) {
        auto [value, op] = parse_column(lines, N-1-i);
        stack.push_back(value);
        if (op) {
            if (*op == '+')
                total += stdr::fold_left(stack, 0ull, std::plus<>());
            else
                total += stdr::fold_left(stack, 1ull, std::multiplies<>());
            stack.clear();
            ++i; // skip blank line
        }
    }
    std::print("Part 2: {}\n", total);
}

int main(int, char* argv[]) {
    part1(argv[1]);
    part2(argv[1]);
}

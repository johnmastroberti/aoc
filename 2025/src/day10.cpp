#include <print>
#include <ranges>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <string>
#include <optional>
#include <unordered_map>
#include <cmath>
#include <functional>
#include <string_view>
#include <bitset>

#include "lp_lib.h"

namespace stdr=std::ranges;
namespace stdv=std::views;
using namespace std::literals;

struct Machine {
    uint16_t target;
    std::vector<uint16_t> buttons;
    std::vector<int> joltage;
};

std::vector<int> comma_separated_ints(std::string_view input) {
    static constexpr auto sep = ","sv;
    return input.substr(1, input.size()-2)
        | stdv::split(sep)
        | stdv::transform([](auto s) { return std::string(std::string_view(s)); })
        | stdv::transform([](std::string const& s) { return std::stoi(s); })
        | stdr::to<std::vector>();
}

using It = std::vector<uint16_t>::const_iterator;

bool solution_exists(uint16_t target, It b_buttons, It e_buttons, size_t presses);

size_t min_presses(uint16_t target, std::vector<uint16_t> const& buttons) {
    for (size_t n = 0; n <= buttons.size(); ++n) {
        if (solution_exists(target, buttons.begin(), buttons.end(), n))
            return n;
    }
    return size_t(-1);
}

bool solution_exists(uint16_t target, It b_buttons, It e_buttons, size_t presses) {
    if (presses == 0)
        return target == 0;
    else
        for (auto it = b_buttons; it != e_buttons; ++it)
            if (solution_exists(target ^ *it, it+1, e_buttons, presses-1))
                return true;
    return false;
}

size_t part2(Machine const& machine) {
    const auto Ncols = machine.buttons.size(); // One variable per button
    auto lp = make_lp(0, Ncols);
    assert(lp);
    std::vector<REAL> row(Ncols, 0.0);
    std::vector<int> col(Ncols);
    std::iota(col.begin(), col.end(), 1);
    // Make all variables integer
    for (auto i : col)
        assert(set_int(lp, i, TRUE));
    // One row (constraint) per joltage
    set_add_rowmode(lp, TRUE);
    auto Nrows = machine.joltage.size();
    for (size_t j = 0; j < Nrows; ++j) {
        for (size_t i = 0; i < Ncols; ++i) {
            row[i] = std::bitset<16>(machine.buttons[i])[j];
        }
        assert(add_constraintex(lp, Ncols, row.data(), col.data(), EQ,
                    machine.joltage[j]));
    }
    set_add_rowmode(lp, FALSE);
    // Minimize button presses
    std::fill(row.begin(), row.end(), 1);
    assert(set_obj_fnex(lp, Ncols, row.data(), col.data()));
    set_minim(lp);
    // Solve
    set_verbose(lp, IMPORTANT);
    assert(solve(lp) == OPTIMAL);
    get_variables(lp, row.data());
    delete_lp(lp);

    return std::reduce(row.begin(), row.end());
}

int main(int, char* argv[]) {
    std::ifstream file(argv[1]);
    std::vector<Machine> machines;
    Machine *last = nullptr;
    for (auto word : stdv::istream<std::string>(file)) {
        switch (word[0]) {
            case '[': // Target
                machines.emplace_back();
                last = &machines.back();
                for (uint16_t i = 1; i < word.size()-1; ++i)
                    last->target |= (word[i] == '#') << (i-1);
                break;
            case '(': // Button
                last->buttons.emplace_back();
                for (auto bit : comma_separated_ints(word))
                    last->buttons.back() |= 1u << bit;
                break;
            case '{': // Joltage
                last->joltage = comma_separated_ints(word);
                break;
        }
    }

    auto presses = machines 
        | stdv::transform([](auto const& m) { return min_presses(m.target, m.buttons); });
    std::print("Part 1: {}\n",
            stdr::fold_left(presses, size_t(0), std::plus<>()));
    std::print("Part 2: {}\n",
            stdr::fold_left(machines | stdv::transform(part2), size_t(0), std::plus<>()));
    return 0;
}

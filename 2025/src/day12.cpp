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
#include <cstring>
#include <array>
#include <deque>
#include <flat_set>
#include <bitset>

namespace stdr=std::ranges;
namespace stdv=std::views;

using Shape = std::bitset<9>;

struct Scenario {
    uint8_t length, width;
    std::array<uint8_t, 6> counts;
    size_t area() const {
        return size_t(length) * size_t(width);
    }
};

enum class WordType {
    ShapeIndex, ShapeSlice, SpaceSize, Count
};

uint8_t chars_to_num(char tens, char ones) {
    return 10 * (tens - '0') + (ones - '0');
}

int main(int, char* argv[]) {
    std::ifstream file(argv[1]);
    auto words = stdv::istream<std::string>(file);
    std::array<Shape, 6> shapes;
    std::vector<Scenario> scenarios;
    using enum WordType;
    WordType expected = ShapeIndex;
    int shape_index = 0;
    int shape_bit = 0;
    Scenario* current_scenario = nullptr;
    int count_index = 0;
    for (auto word : words) {
        switch (expected) {
            case ShapeIndex:
                shape_index = std::stoi(word);
                shape_bit = 0;
                expected = ShapeSlice;
                break;
            case ShapeSlice:
                for (int i = 0; i < 3; ++i)
                    shapes[shape_index][shape_bit + i] = word[i] == '#';
                shape_bit += 3;
                if (shape_bit == 9)
                    expected = shape_index == 5 ? SpaceSize : ShapeIndex;
                break;
            case SpaceSize:
                scenarios.emplace_back();
                current_scenario = &scenarios.back();
                current_scenario->length = chars_to_num(word[0], word[1]);
                current_scenario->width = chars_to_num(word[3], word[4]);
                expected = Count;
                count_index = 0;
                break;
            case Count:
                current_scenario->counts[count_index] = 
                    chars_to_num(word[0], word[1]);
                ++count_index;
                if (count_index == 6)
                    expected = SpaceSize;
                break;
        }
    }
    int ruled_out = 0;
    for (auto s : scenarios) {
        auto op = [](auto shape, auto count) { return shape.count() * count; };
        int tile_area = stdr::fold_left(
                stdv::zip_transform(op, shapes, s.counts), 0, std::plus<>());
        ruled_out += int(s.area()) < tile_area;
    }
    std::print("Part 1: {}\n", scenarios.size() - ruled_out);
}

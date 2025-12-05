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

struct Segment {
    size_t lower;
    size_t upper;
    size_t size() const { return upper - lower + 1; }
    size_t contains(size_t item) const {
        return item >= lower && item <= upper;
    }
    size_t valid() const { return lower <= upper; }
    std::string str() const { 
        return std::to_string(lower) + "-" + std::to_string(upper); 
    }
};

auto parse_input(const char* filename) {
    std::ifstream file(filename);
    const auto lines = stdv::istream<std::string>(file) 
        | stdr::to<std::vector>();

    auto segment_end = std::partition_point(lines.begin(), lines.end(),
            [](auto const& s) { return s.contains('-'); });

    std::vector<Segment> segments(segment_end - lines.begin());
    std::transform(lines.begin(), segment_end, segments.begin(),
            [](auto const& s) { 
            const auto dash = s.find('-');
            return Segment{ std::stoull(s.substr(0, dash)),
                            std::stoull(s.substr(dash+1)) };
                            });

    std::vector<size_t> items(lines.end() - segment_end);
    std::transform(segment_end, lines.end(), items.begin(),
            [](auto const& s) { return std::stoull(s); });
    return std::pair<std::vector<Segment>, std::vector<size_t>>(
            std::move(segments), std::move(items));
}

int main(int, char* argv[]) {
    size_t part1 = 0, part2 = 0;
    auto [segments, items] = parse_input(argv[1]);

    stdr::sort(segments, [](auto s1, auto s2) { return s1.lower < s2.lower; });
    auto fresh = [&segments](auto item) { 
        auto potentials_end = std::partition_point(
                segments.begin(), segments.end(),
                [item](auto seg) { return seg.lower <= item; });
        return potentials_end != std::find_if(segments.begin(), segments.end(),
                [item](auto seg) { return item <= seg.upper; });
    };

    part1 = std::count_if(items.begin(), items.end(), fresh);

    std::vector<Segment> unique_segments;
    for (auto potential : segments) {
        for (auto existing : unique_segments) {
            if (existing.contains(potential.lower)) {
                potential.lower = existing.upper + 1;
            }
            if (!potential.valid()) break;
        }
        if (!potential.valid()) continue;
        unique_segments.push_back(potential);
        // auto correct_position = stdr::find_if(unique_segments,
                // [potential](auto s) { return s.lower >= potential.lower; });
        // std::rotate(correct_position, unique_segments.end()-1, 
                // unique_segments.end());
    }
    // std::print("{}\n", unique_segments | stdv::transform(&Segment::str));

    part2 = stdr::fold_left(unique_segments | stdv::transform(&Segment::size),
            size_t{0}, std::plus<>());

    std::print("Part 1: {}\nPart 2: {}\n", part1, part2);
}

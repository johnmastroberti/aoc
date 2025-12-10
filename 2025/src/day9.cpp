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

namespace stdr=std::ranges;
namespace stdv=std::views;

struct Vec2 {
    ssize_t x, y;
};

Vec2 operator+(Vec2 v1, Vec2 v2) {
    return Vec2{v1.x + v2.x, v1.y + v2.y};
}

Vec2 operator*(ssize_t A, Vec2 v) {
    return Vec2{ A*v.x, A*v.y };
}

size_t rect(Vec2 v1, Vec2 v2) {
    return (std::abs(v1.x - v2.x) + 1) * (std::abs(v1.y - v2.y) + 1);
}

std::istream& operator>>(std::istream& in, Vec2& v) {
    char comma;
    in >> v.x;
    in >> comma;
    in >> v.y;
    return in;
}

struct Pair {
    int index1, index2;
    size_t area;
};

bool between(auto a, auto b, auto x) {
    return (x > a && x < b) || (x > b && x < a);
}

bool rect_contains(Vec2 p1, Vec2 p2, Vec2 v) {
    return between(p1.x, p2.x, v.x) && between(p1.y, p2.y, v.y);
}

auto points_between(Vec2 p1, Vec2 p2) {
    assert(p1.x == p2.x || p1.y == p2.y);
    Vec2 start, step;
    ssize_t A, B;
    if (p1.x == p2.x) {
        start = Vec2{p1.x, 0};
        step = Vec2{0, 1};
        auto mm = std::minmax(p1.y, p2.y);
        A = mm.first;
        B = mm.second;
    } else {
        start = Vec2{0, p1.y};
        step = Vec2{1, 0};
        auto mm = std::minmax(p1.x, p2.x);
        A = mm.first;
        B = mm.second;
    }
    return stdv::iota(A, B) 
        | stdv::transform([=](auto t) { return start + t * step; });
}

int main(int, char* argv[]) {
    std::ifstream file(argv[1]);
    auto points = stdv::istream<Vec2>(file) | stdr::to<std::vector>();
    const auto N = points.size();

    auto edge_points = points 
        | stdv::adjacent_transform<2>(points_between)
        | stdv::join;

    std::vector<Pair> pairs;
    pairs.reserve(N*(N-1));
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i+1; j < N; ++j) {
            assert(i < j);
            pairs.push_back(Pair(i, j, rect(points[i], points[j])));
        }
    }
    stdr::sort(pairs, std::greater<>(), [](auto const& p) { return p.area; });

    auto edge_free = [&](auto p) {
        auto p1 = points[p.index1];
        auto p2 = points[p.index2];
        return stdr::find_if(edge_points, std::bind_front(rect_contains, p1, p2)) 
            == edge_points.end();
    };

    auto valid = pairs | stdv::filter(edge_free);

    std::print("{} {}\n", pairs.front().index1, pairs.front().index2);
    std::print("Part 1: {}\n", pairs.front().area);
    auto part2 = stdr::begin(valid);
    std::print("{} {}\n", part2->index1, part2->index2);
    std::print("Part 2: {}\n", part2->area);
}

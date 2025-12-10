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

namespace stdr=std::ranges;
namespace stdv=std::views;

struct Vec3 {
    int x, y, z, circuit;
    size_t norm2() const {
        return size_t(x)*size_t(x) + size_t(y)*size_t(y) + size_t(z)*size_t(z);
    }
};

Vec3 operator-(Vec3 v1, Vec3 v2) { 
    return Vec3{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, 0};
}

size_t dist2(Vec3 v1, Vec3 v2) {
    return (v1 - v2).norm2();
}

std::istream& operator>>(std::istream& in, Vec3& v) {
    char comma;
    in >> v.x;
    in >> comma;
    in >> v.y;
    in >> comma;
    in >> v.z;
    v.circuit = 0;
    return in;
}

struct Pair {
    int index1, index2;
    size_t distance;
};

int main(int, char* argv[]) {
    std::ifstream file(argv[1]);
    auto points = stdv::istream<Vec3>(file) | stdr::to<std::vector>();
    const auto N = points.size();

    size_t part1_count = 1000;
    if (points[0].x == 162) // test input
        part1_count = 10;

    std::vector<Pair> pairs;
    pairs.reserve(N*(N-1));
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i+1; j < N; ++j) {
            assert(i < j);
            pairs.push_back(Pair(i, j, dist2(points[i], points[j])));
        }
    }
    stdr::sort(pairs, std::less<>(), [](auto const& p) { return p.distance; });

    int next_circuit = 1;
    std::unordered_map<int, std::vector<int>> circuits;
    auto make_connection = [&](auto p) {
        auto& p1 = points[p.index1];
        auto& p2 = points[p.index2];
        if (p1.circuit == 0 && p2.circuit == 0) {
            // Neither point in a circuit, create a new one
            p1.circuit = next_circuit;
            p2.circuit = next_circuit;
            circuits[next_circuit] = {p.index1, p.index2};
            ++next_circuit;
        } else if (p1.circuit == 0 && p2.circuit != 0) {
            // Add p1 to p2's circuit
            circuits[p2.circuit].push_back(p.index1);
            p1.circuit = p2.circuit;
        } else if (p1.circuit != 0 && p2.circuit == 0) {
            // Add p2 to p1's circuit
            circuits[p1.circuit].push_back(p.index2);
            p2.circuit = p1.circuit;
        } else if (p1.circuit == p2.circuit) {
            // Both already in the same circuit, skip
        } else {
            // Both already in different circuits; move p2's circuit to p1's
            const auto p2_circuit = p2.circuit;
            for (auto ix : circuits[p2_circuit]) {
                points[ix].circuit = p1.circuit;
                circuits[p1.circuit].push_back(ix);
            }
            circuits.erase(p2_circuit);
        }
    };
    for (auto p : pairs | stdv::take(part1_count)) {
        make_connection(p);
    }

    std::vector<size_t> circuit_sizes;
    std::transform(circuits.begin(), circuits.end(),
            std::back_inserter(circuit_sizes),
            [](auto const& c) { return c.second.size(); });
    stdr::sort(circuit_sizes, std::greater<>());
    size_t part1 = circuit_sizes[0] * circuit_sizes[1] * circuit_sizes[2];
    std::print("Part 1: {}\n", part1);

    Pair last_pair;
    for (auto p : pairs | stdv::drop(part1_count)) {
        if (stdr::none_of(points, [](auto point) { return point.circuit == 0; }))
            break;
        else 
            make_connection(p);
        last_pair = p;
    }

    auto p1 = points[last_pair.index1];
    auto p2 = points[last_pair.index2];
    std::print("Part 2: {}\n", 
            size_t(p1.x) * size_t(p2.x));
}

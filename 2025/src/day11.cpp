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

namespace stdr=std::ranges;
namespace stdv=std::views;

using Str = std::array<char, 4>;

struct StrCmp {
    bool operator()(Str a, Str b) const {
        uint32_t x, y;
        std::memcpy(&x, a.data(), 4);
        std::memcpy(&y, b.data(), 4);
        return x < y;
    }
};

struct StrHash {
    auto operator()(Str s) const {
        uint32_t x;
        std::memcpy(&x, s.data(), 4);
        return std::hash<uint32_t>()(x);
    }
};

struct Node {
    size_t paths_in;
    std::flat_set<Str, StrCmp> outputs;
};

using Graph = std::unordered_map<Str, Node, StrHash>;

void reset_graph(Graph& graph) {
    for (auto& [junk, node] : graph)
        node.paths_in = 0;
}

size_t paths_between(Graph& graph, Str start, Str end) {
    std::deque<Str> visit_queue;
    visit_queue.push_back(start);
    graph[start].paths_in = 1;
    graph.emplace(end, Node{}); // make sure end node exists
    graph[end].paths_in = 0;
    while (visit_queue.size()) {
        auto next = visit_queue.front();
        visit_queue.pop_front();
        // Check if we're ready to process next
        if (visit_queue.end() != stdr::find_if(visit_queue, 
                    [&](auto node) { return graph[node].outputs.contains(next); })) {
            // We can only process next if there's nothing in the queue that
            // has next as an output
            visit_queue.push_back(next);
            continue;
        }
        for (auto output : graph[next].outputs) {
            graph[output].paths_in += graph[next].paths_in;
            // Only need to add outputs to the queue if they're not already
            // in the queue
            if (stdr::find(visit_queue, output) == visit_queue.end())
                visit_queue.push_back(output);
        }
        // Can terminate early if we've processed the end node
        if (next == end) break;
    }
    auto ret = graph[end].paths_in;
    reset_graph(graph);
    return ret;
}

Graph parse_input(const char* filename) {
    std::ifstream file(filename);
    Graph graph;
    auto current = graph.end();
    Str s;
    s[3] = 0;
    for (auto word : stdv::istream<std::string>(file)) {
        std::memcpy(s.data(), word.data(), 3);
        if (word.back() == ':') { // first word on line
            auto [new_it, inserted] = graph.emplace(s, Node{});
            assert(inserted);
            current = new_it;
        } else { // an output
            current->second.outputs.emplace(s);
        }
    }
    return graph;
}

int main(int, char* argv[]) {
    auto graph = parse_input(argv[1]);

    Str you{"you"};
    Str out{"out"};

    std::print("Part 1: {}\n", paths_between(graph, you, out));

    Str svr{"svr"}, dac{"dac"}, fft{"fft"};

    auto svr_to_dac = paths_between(graph, svr, dac);
    auto svr_to_fft = paths_between(graph, svr, fft);
    auto dac_to_fft = paths_between(graph, dac, fft);
    auto fft_to_dac = paths_between(graph, fft, dac);
    auto dac_to_out = paths_between(graph, dac, out);
    auto fft_to_out = paths_between(graph, fft, out);

    std::print("Part 2: {}\n",
            svr_to_dac * dac_to_fft * fft_to_out
            + svr_to_fft * fft_to_dac * dac_to_out);
}

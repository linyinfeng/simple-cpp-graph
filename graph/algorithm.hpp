#ifndef GRAPH_ALGORITHM_HPP
#define GRAPH_ALGORITHM_HPP

#include <type_traits>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <tuple>
#include <initializer_list>

#include "graph.hpp"
#include "detail/algorithm.hpp"

#include "../adapter/queue.hpp"
#include "../adapter/stack.hpp"

namespace graph {

template <typename G>
typename G::size_type get_vertex_number(G &g) {
    return g.vertexNumber();
};

template <typename G>
std::make_signed_t<typename G::size_type> get_vertex_index(G &g, const typename G::vertex_info_type& vertex) {
    return g.indexOfVertex(vertex);
}

template <typename G>
const typename G::vertex_info_type &get_vertex(G &g, typename G::size_type index) {
    return g.getVertex(index);
}

template <typename G>
void add_vertex(G &g, const typename G::vertex_info_type& v) {
    g.addVertex(v);
};

template <typename G, typename... Args>
void add_vertex(G &g, const typename G::vertex_info_type& v, const Args&... args) {
    add_vertex(g, v);
    add_vertex(g, args...);
};

template <typename G, typename V1, typename V2>
void set_edge(G &g,
        const V1& v1,
        const V2& v2,
        const typename G::edge_info_type& e) {
    g.setEdge(v1, v2, e);
};

template <typename G, typename V1, typename V2>
void add_edge(G &g, const V1& v1, const V2& v2) {
    set_edge(g, v1, v2, static_cast<typename G::edge_info_type>(true));
};

template <typename G, typename V1, typename V2>
void remove_edge(G &g, const V1& v1, const V2& v2) {
    set_edge(g, v1, v2, g.defaultEdgeInfo());
};

template <typename G, typename V1, typename V2>
typename G::edge_info_type get_edge_info(G &g, const V1& v1, const V2& v2) {
    return g.getEdge(v1, v2);
};

template <typename G, typename Visit>
void breadth_first_traverse(G &g, Visit&& visit) {
    std::vector<bool> visited(get_vertex_number(g), false);
    using size_type = typename G::size_type;
    Queue<std::pair<std::make_signed_t<size_type>, size_type>> queue;
    for (size_type i = 0; i < get_vertex_number(g); ++i) {
        if (!visited[i]) {
            queue.enqueue(std::make_pair(-1, i));
            visited[i] = true;
            while (!queue.empty()) {
                auto pair = queue.front(); queue.dequeue();
                std::forward<Visit>(visit)(g, pair.first, pair.second);
                for (auto beg = g.adjacencyVertexBegin(pair.second), end = g.adjacencyVertexEnd(pair.second); beg != end; ++beg) {
                    auto adjacency_info = *beg;
                    if (!visited[adjacency_info.to]) {
                        queue.enqueue(std::make_pair(pair.second, adjacency_info.to));
                        visited[adjacency_info.to] = true;
                    }
                }
            }
        }
    }
}

namespace detail {

template<typename G, typename Visit>
void depth_first_traverse(G &g, Visit&& visit, long long from, long long i, std::vector<bool> &visited) {
    visited[i] = true;
    std::forward<Visit>(visit)(g, from, i);
    for (auto beg = g.adjacencyVertexBegin(i), end = g.adjacencyVertexEnd(i);
         beg != end; ++beg) {
        if (!visited[(*beg).to])
            depth_first_traverse(g, visit, i, (*beg).to, visited);
    }
};

}

template <typename G, typename Visit>
void depth_first_traverse(G &g, Visit&& visit) {
    std::vector<bool> visited(get_vertex_number(g), false);
    for (std::size_t i = 0; i < get_vertex_number(g); ++i) {
        if (!visited[i])
            detail::depth_first_traverse(g, visit, -1, static_cast<long long>(i), visited);
    }
};

template <typename G, typename VisitVertex>
void depth_first_traverse_non_recursive(G &g, VisitVertex&& visit_vertex) {
    std::vector<bool> visited(get_vertex_number(g), false);
    using size_type = typename G::size_type;
    Stack<size_type> stack;
    for (size_type i = 0; i < get_vertex_number(g); ++i) {
        if (!visited[i]) {
            visited[i] = true;
            stack.push(i);
            while (!stack.empty()) {
                auto v = stack.top(); stack.pop();
                std::forward<VisitVertex>(visit_vertex)(g, v);
                for (auto beg = g.adjacencyVertexBegin(v), end = g.adjacencyVertexEnd(v);
                     beg != end; ++beg) {
                    auto adjacency_info = *beg;
                    if (!visited[adjacency_info.to]) {
                        stack.push(adjacency_info.to);
                        visited[adjacency_info.to] = true;
                    }
                }
            }
        }
    }
}

template <typename G,
        typename = std::enable_if_t<
                std::is_base_of_v<GraphTag<G::is_directed, typename G::vertex_info_type, typename G::edge_info_type>, G>
        >
>
std::ostream& operator<<(std::ostream& os, const G &g) {
    int first_column_hint;
    std::vector<int> hints;
    std::tie(first_column_hint, hints) = detail::print_hint(g);

    using size_type = typename G::size_type;
    auto size = get_vertex_number(g);

    os << std::setw(first_column_hint) << ' ' << ' ';
    for (size_type i = 0; i < size; ++i) {
        os << std::setw(hints[i]) << get_vertex(g, i) << ' ';
    }
    os << '\n';

    for (size_type i = 0; i < size; ++i) {
        os << std::setw(first_column_hint) << get_vertex(g, i) << ' ';
        for (size_type j = 0; j < size; ++j) {
            os << std::setw(hints[j]) << get_edge_info(g, i, j) << ' ';
        }
        os << '\n';
    }
    os << std::flush;
    return os;
}

} // ! namespace graph

#endif //GRAPH_ALGORITHM_HPP

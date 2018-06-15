#ifndef GRAPH_DETAIL_ADJACENCY_LIST_GRAPH_HPP
#define GRAPH_DETAIL_ADJACENCY_LIST_GRAPH_HPP

#include <list>

namespace graph::detail {

template <typename EdgeInfo>
struct AdjacencyVertex {
    AdjacencyVertex(std::size_t to, const EdgeInfo& e) : to(to), edge_info(e) { }

    std::size_t to;
    EdgeInfo edge_info;
};

} // ! namespace graph::detail

#endif //GRAPH_DETAIL_ADJACENCY_LIST_GRAPH_HPP

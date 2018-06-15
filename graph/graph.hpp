#ifndef GRAPH_GRAPH_HPP_INCLUDED
#define GRAPH_GRAPH_HPP_INCLUDED

#include <tuple>

namespace graph {

// An Aggregate class to define a edge
template <typename VertexInfo, typename EdgeInfo = bool>
using Edge = std::tuple<VertexInfo, VertexInfo, EdgeInfo>;

template <bool IsDirected, typename VertexInfo, typename EdgeInfo = bool>
struct GraphTag {
    static constexpr bool is_directed = IsDirected;
    using vertex_info_type = VertexInfo;
    using edge_info_type = EdgeInfo;
};
/* Graph method */

} // ! namespace graph

#endif // GRAPH_GRAPH_HPP_INCLUDED

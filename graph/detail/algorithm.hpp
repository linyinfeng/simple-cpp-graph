#ifndef GRAPH_DETAIL_ALGORITHM_HPP
#define GRAPH_DETAIL_ALGORITHM_HPP

#include <type_traits>
#include <limits>
#include <vector>
#include <sstream>
#include <tuple>
#include "../algorithm.hpp"

namespace graph::detail {

template <typename EdgeInfo, typename Enabled = std::true_type>
struct DefaultEdgeInfo {
    static constexpr EdgeInfo value{};
};

template <typename EdgeInfo>
struct DefaultEdgeInfo<EdgeInfo, std::bool_constant<std::is_integral_v<EdgeInfo>>> {
    static constexpr EdgeInfo value = 0;
};

template <typename EdgeInfo>
struct DefaultEdgeInfo<EdgeInfo, std::bool_constant<std::is_floating_point_v<EdgeInfo>>> {
    static constexpr EdgeInfo value = std::numeric_limits<EdgeInfo>::infinity();
};

template <typename EdgeInfo>
inline constexpr EdgeInfo default_edge_info = DefaultEdgeInfo<EdgeInfo>::value;

template <typename G>
std::tuple<int, std::vector<int>> print_hint(const G &g) {
    std::vector<int> res;

    using size_type = typename G::size_type;
    auto size = get_vertex_number(g);

    int hint = 0; std::ostringstream temp;
    auto update = [&]() {
        if (temp.str().length() > hint)
            hint = static_cast<decltype(hint)>(temp.str().length());
        temp.str(""); // clear
    };

    for (size_type i = 0; i < size; ++i) {
        temp << get_vertex(g, i); update();
        for (size_type j = 0; j < size; ++j) {
            temp << get_edge_info(g, i, j); update();
        }
        res.push_back(hint);
        hint = 0;
    }

    for (size_type i = 0; i < size; ++i) {
        temp << get_vertex(g, i); update();
    }

    return std::make_tuple(hint, res);
}

} // ! namespace graph::detail

#endif //GRAPH_DETAIL_ALGORITHM_HPP

#ifndef GRAPH_ADJACENCY_LIST_GRAPH_HPP
#define GRAPH_ADJACENCY_LIST_GRAPH_HPP

#include "graph.hpp"
#include "detail/adjacency.hpp"
#include "algorithm.hpp"
#include <list>
#include <vector>
#include <algorithm>
#include <stdexcept>

namespace graph {

template <typename EdgeInfo> class AdjacencyListAdjacencyIterator;

template <typename VertexInfo, typename EdgeInfo = bool>
struct AdjacencyListVertex {
    explicit AdjacencyListVertex(const VertexInfo &vertex) : vertex(vertex) { }
    VertexInfo vertex;
    std::list<detail::AdjacencyVertex<EdgeInfo>> edges;
};

// An Aggregate class to define a graph represented by a adjacency list
template<bool IsDirected, typename VertexInfo, typename EdgeInfo = bool>
class AdjacencyList : public GraphTag<IsDirected, VertexInfo, EdgeInfo> {
public:
    using size_type = std::size_t;
    using vertex_reference = VertexInfo &;
    using vertex_const_reference = const VertexInfo &;
    using edge_info_reference = EdgeInfo &;
    using edge_info_const_reference = const EdgeInfo &;
    using iterator = AdjacencyListAdjacencyIterator<EdgeInfo>;

    explicit AdjacencyList(const EdgeInfo &default_edge_info = detail::default_edge_info<EdgeInfo>)
        : default_edge_info(default_edge_info), vertices() { }

    const EdgeInfo& defaultEdgeInfo() const {
        return default_edge_info;
    }

    size_type vertexNumber() const {
        return vertices.size();
    }

    std::make_signed_t<size_type> indexOfVertex(const VertexInfo& v) const {
        for (size_type i = 0; i < vertices.size(); ++i) {
            if (vertices[i].vertex == v)
                return i;
        }
        return -1;
    }

    // add a vertex with info v, fill all edge about the vertex with fill
    size_type addVertex(const VertexInfo& v) {
        auto index = indexOfVertex(v);
        if (index != -1)
            return static_cast<size_type>(index);
        vertices.emplace_back(v);
        return vertices.size() - 1;
    }

    const VertexInfo& getVertex(size_type index) const {
        return vertices[index].vertex;
    }

    iterator adjacencyVertexBegin(const VertexInfo& from) {
        auto index_from = indexOfVertex(from);
        if (index_from == -1)
            throw std::out_of_range("Vertex does not exist");
        return iterator(vertices[from].edges.begin());
    }

    iterator adjacencyVertexEnd(const VertexInfo& from) {
        auto index_from = indexOfVertex(from);
        if (index_from == -1)
            throw std::out_of_range("Vertex does not exist");
        return iterator(vertices[from].edges.end());
    }

    iterator adjacencyVertexBegin(size_type from) {
        if (from >= vertexNumber())
            throw std::out_of_range("Vertex does not exist");
        return iterator(vertices[from].edges.begin());
    }

    iterator adjacencyVertexEnd(size_type from) {
        if (from >= vertexNumber())
            throw std::out_of_range("Vertex does not exist");
        return iterator(vertices[from].edges.end());
    }

    // access and insert
    void setEdge(const VertexInfo& from, const VertexInfo& to, const EdgeInfo& e) {
        auto index_from = addVertex(from);
        auto index_to = addVertex(to);
        setEdge(index_from, index_to, e);
    }

    void setEdge(std::size_t from, std::size_t to, const EdgeInfo& e) {
        if (from >= vertexNumber() || to >= vertexNumber())
            throw std::out_of_range("Vertex does not exist");
        auto &list = vertices[from].edges;
        auto iter = list.begin(), end = list.end();
        for (; iter != end; ++iter) {
            if (iter->to == to) {
                iter->edge_info = e;
                return;
            }
        }
        list.emplace_back(to, e);
        if constexpr (!IsDirected)
            setEdge(to, from, e);
    }

    edge_info_const_reference getEdge(const VertexInfo& from, const VertexInfo& to) const {
        auto index_from = indexOfVertex(from);
        auto index_to = indexOfVertex(to);
        if (index_from != -1 || index_to != -1) throw std::out_of_range("Vertex does not exist");
        return getEdge(static_cast<std::size_t>(index_from), static_cast<std::size_t>(index_to));
    }

    edge_info_const_reference getEdge(std::size_t from, std::size_t to) const {
        if (from >= vertexNumber() || to >= vertexNumber())
            throw std::out_of_range("Vertex does not exist");
        auto &list = vertices[from].edges;
        auto iter = list.begin(), end = list.end();
        for (; iter != end; ++iter) {
            if (iter->to == to) {
                return iter->edge_info;
            }
        }
        return default_edge_info;
    }

private:
    std::vector<AdjacencyListVertex<VertexInfo, EdgeInfo>> vertices;
    const EdgeInfo default_edge_info;
};

template<bool IsDirected, typename EdgeInfo>
class AdjacencyList<IsDirected, std::size_t, EdgeInfo> {
};

template <typename EdgeInfo>
class AdjacencyListAdjacencyIterator {
public:
    explicit AdjacencyListAdjacencyIterator(
            typename std::list<detail::AdjacencyVertex<EdgeInfo>>::iterator iterator
    ) : iterator(iterator) { }

    AdjacencyListAdjacencyIterator(const AdjacencyListAdjacencyIterator &other) {
        iterator = other.iterator;
    }

    AdjacencyListAdjacencyIterator& operator++ () {
        ++iterator;
        return *this;
    }

    AdjacencyListAdjacencyIterator operator++ (int) {
        AdjacencyListAdjacencyIterator res = *this;
        ++ iterator;
        return res;
    }

    const detail::AdjacencyVertex<EdgeInfo>& operator* () {
        return *iterator;
    }

    bool operator== (const AdjacencyListAdjacencyIterator &other) {
        return iterator == other.iterator;
    }

    bool operator!= (const AdjacencyListAdjacencyIterator &other) {
        return iterator != other.iterator;
    }

protected:
    typename std::list<detail::AdjacencyVertex<EdgeInfo>>::iterator iterator;
};

} // namespace graph

#endif // GRAPH_ADJACENCY_LIST_GRAPH_HPP

#ifndef GRAPH_ADJACENCY_MATRIX_GRAPH_HPP
#define GRAPH_ADJACENCY_MATRIX_GRAPH_HPP

#include "graph.hpp"
#include "../matrix/matrix.hpp"
#include "algorithm.hpp"
#include "detail/adjacency.hpp"
#include <type_traits>
#include <stdexcept>

namespace graph {

template <typename EdgeInfo> class AdjacencyMatrixAdjacencyIterator;

// An Aggregate class to define a graph represented by a adjacency matrix
template<bool IsDirected, typename VertexInfo, typename EdgeInfo = bool>
class AdjacencyMatrix : public GraphTag<IsDirected, VertexInfo, EdgeInfo> {
public:
    using size_type = std::size_t;
    using vertex_reference = typename std::vector<VertexInfo>::reference;
    using vertex_const_reference = typename std::vector<VertexInfo>::const_reference;
    using edge_info_reference = typename Matrix<EdgeInfo>::reference;
    using edge_info_const_reference = typename Matrix<EdgeInfo>::const_reference;
    using iterator = AdjacencyMatrixAdjacencyIterator<EdgeInfo>;

    explicit AdjacencyMatrix(const EdgeInfo &default_edge_info = detail::default_edge_info<EdgeInfo>)
        : default_edge_info(default_edge_info), vertices(), matrix() { }

    const EdgeInfo& defaultEdgeInfo() const {
        return default_edge_info;
    }

    size_type vertexNumber() const {
        return vertices.size();
    }

    std::make_signed_t<size_type> indexOfVertex(const VertexInfo& v) const {
        for (size_type i = 0; i < vertices.size(); ++i) {
            if (vertices[i] == v)
                return i;
        }
        return -1;
    }

    // add a vertex with info v, fill all edge about the vertex with fill
    size_type addVertex(const VertexInfo& v) {
        auto index = indexOfVertex(v);
        if (index != -1)
            return static_cast<size_type>(index);
        vertices.push_back(v);
        matrix.resize(vertices.size(), vertices.size(), default_edge_info);
        return vertices.size() - 1;
    }

    const VertexInfo& getVertex(size_type index) const {
        return vertices[index];
    }

    iterator adjacencyVertexBegin(const VertexInfo& from) {
        auto index_from = indexOfVertex(from);
        if (index_from == -1)
            throw std::out_of_range("Vertex does not exist");
        return iterator(matrix, index_from, default_edge_info, 0);
    }

    iterator adjacencyVertexEnd(const VertexInfo& from) {
        auto index_from = indexOfVertex(from);
        if (index_from == -1)
            throw std::out_of_range("Vertex does not exist");
        return iterator(matrix, index_from, default_edge_info, matrix.columns());
    }

    iterator adjacencyVertexBegin(size_type from) {
        if (from >= vertexNumber())
            throw std::out_of_range("Vertex does not exist");
        return iterator(matrix, from, default_edge_info, 0);
    }

    iterator adjacencyVertexEnd(size_type from) {
        if (from >= vertexNumber())
            throw std::out_of_range("Vertex does not exist");
        return iterator(matrix, from, default_edge_info, matrix.columns());
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
        matrix(from, to) = e;
        if constexpr (!IsDirected)
            matrix(to, from) = e;
    }

    edge_info_const_reference getEdge(const VertexInfo& from, const VertexInfo& to) const {
        auto index_from = indexOfVertex(from);
        auto index_to = indexOfVertex(to);
        if (index_from == -1 || index_to == -1) throw std::out_of_range("Vertex does not exist");
        return getEdge(index_from, index_to);
    }

    edge_info_const_reference getEdge(std::size_t from, std::size_t to) const {
        if (from >= vertexNumber() || to >= vertexNumber())
            throw std::out_of_range("Vertex does not exist");
        return matrix(from, to);
    }

private:
    std::vector<VertexInfo> vertices;
    Matrix<EdgeInfo> matrix;
    const EdgeInfo default_edge_info;
};

template<bool IsDirected, typename EdgeInfo>
class AdjacencyMatrix<IsDirected, std::size_t, EdgeInfo> {
};

template <typename EdgeInfo>
class AdjacencyMatrixAdjacencyIterator {
public:
    explicit AdjacencyMatrixAdjacencyIterator(
            Matrix<EdgeInfo>& matrix,
            std::size_t row,
            const EdgeInfo& default_edge_info,
            std::size_t init = 0)
        : matrix(matrix), default_edge_info(default_edge_info) {
        if (row >= matrix.rows())
            throw std::out_of_range("Matrix row out of range");
        if (init > matrix.columns())
            throw std::out_of_range("Initial index out of range");
        this->row = row;
        this->index = init;
        if (index != matrix.columns() && matrix.at(row, index) == default_edge_info)
            increase();
    }

    AdjacencyMatrixAdjacencyIterator(const AdjacencyMatrixAdjacencyIterator &other)
        : matrix(other.matrix), row(row),
          default_edge_info(other.default_edge_info), index(other.index) { }

    AdjacencyMatrixAdjacencyIterator& operator++ () {
        increase();
        return *this;
    }

    AdjacencyMatrixAdjacencyIterator operator++ (int) {
        AdjacencyMatrixAdjacencyIterator res{*this};
        increase();
        return res;
    }

    detail::AdjacencyVertex<EdgeInfo> operator* () {
        return { index, matrix.at(row, index) };
    }

    bool operator== (const AdjacencyMatrixAdjacencyIterator &other) {
        return &matrix == &other.matrix &&
                row == other.row &&
                default_edge_info == other.default_edge_info &&
                index == other.index;
    }

    bool operator!= (const AdjacencyMatrixAdjacencyIterator &other) {
        return !(*this == other);
    }

private:
    void increase() {
        if (index >= matrix.columns() - 1) {
            index = matrix.columns();
            return;
        }
        do {
            ++index;
        } while (index != matrix.columns() && matrix.at(row, index) == default_edge_info);
    }

    Matrix<EdgeInfo>& matrix;
    std::size_t row;
    const EdgeInfo& default_edge_info; // reference to default edge info of AdjacencyMatrix

    std::size_t index;
};

} // ! namespace graph

#endif // GRAPH_ADJACENCY_MATRIX_GRAPH_HPP

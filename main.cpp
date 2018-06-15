#include <iostream>
#include <string>
#include <functional>
#include "graph/graph.hpp"
#include "graph/algorithm.hpp"
#include "graph/adjacency_matrix.hpp"
#include "graph/adjacency_list.hpp"

using namespace std::placeholders;

template <typename Graph>
void test(Graph &g) {
    auto print_vertex = [](auto &g, auto from, auto to) {
        std::cout << graph::get_vertex(g, to) << ' ';
    };
    auto print_tree_edge = [](auto &g, auto from, auto to) {
        if (from == -1)
            std::cout << "(" << graph::get_vertex(g, to) << ") ";
        if (from != -1)
            std::cout << graph::get_vertex(g, from) << "->" << graph::get_vertex(g, to) << ' ';
    };
    auto print_vertex_non_recursive = std::bind(print_vertex, _1, -1, _2);

    std::cout << g;

    std::cout << "Depth first traverse:" << '\n';
    std::cout << "Recursive:\t";
    graph::depth_first_traverse(g, print_vertex);
    std::cout << '\n';
    std::cout << "Non-recursive:\t";
    graph::depth_first_traverse_non_recursive(g, print_vertex_non_recursive);
    std::cout << '\n';
    graph::depth_first_traverse(g, print_tree_edge);
    std::cout << '\n';

    std::cout << "Breadth first traverse:"<< '\n';
    std::cout << "Non-recursive:\t";
    graph::breadth_first_traverse(g, print_vertex);
    std::cout << '\n';
    graph::breadth_first_traverse(g, print_tree_edge);
    std::cout << '\n';
}

template <typename Graph>
void construct_test_graph(Graph &g) {
    graph::add_vertex(g, "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8");
    graph::add_edge(g, "v1", "v2");
    graph::add_edge(g, "v1", "v3");
    graph::add_edge(g, "v2", "v4");
    graph::add_edge(g, "v2", "v5");
    graph::add_edge(g, "v8", "v4");
    graph::add_edge(g, "v8", "v5");
    graph::add_edge(g, "v3", "v6");
    graph::add_edge(g, "v3", "v7");
    graph::add_edge(g, "v6", "v7");
}

void banner(const std::string& str) {
    std::cout << "----- " << str << " -----" << std::endl;
}

int main() {
    banner("Test undirected graph with adjacency matrix");
    graph::AdjacencyMatrix<false, std::string, bool> g1;
    construct_test_graph(g1); test(g1);
    std::cout << std::endl;

    banner("Test directed graph with adjacency matrix");
    graph::AdjacencyMatrix<true, std::string, bool> g2;
    construct_test_graph(g2); test(g2);
    std::cout << std::endl;

    banner("Test undirected graph with adjacency list");
    graph::AdjacencyList<false, std::string, bool> g3;
    construct_test_graph(g3); test(g3);
    std::cout << std::endl;

    banner("Test directed graph with adjacency list");
    graph::AdjacencyList<true, std::string, bool> g4;
    construct_test_graph(g4); test(g4);
    std::cout << std::endl;

    return 0;
}

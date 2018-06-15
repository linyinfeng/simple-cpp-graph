cmake_minimum_required(VERSION 3.0)
project(graph)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        test_graph
        main.cpp
        graph/graph.hpp
        matrix/matrix.hpp
        graph/algorithm.hpp
        graph/detail/algorithm.hpp
        graph/adjacency_list.hpp
        graph/adjacency_matrix.hpp
        graph/detail/adjacency.hpp
        adapter/stack.hpp
        adapter/queue.hpp
        list/circular_linked_list.hpp
)

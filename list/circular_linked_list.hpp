#ifndef CIRCULAR_LINKED_LIST_HPP_INCLUDED
#define CIRCULAR_LINKED_LIST_HPP_INCLUDED

#include <cstddef>
#include <stdexcept>

namespace circular_linked_list {

    /* Interface */

    template <typename T> struct Node;

    template <typename T>
    using List = Node<T> *;
    
    template <typename T>
    struct Node {
        template <typename U>
        Node(U&& data, List<T> l) : data(std::forward<U>(data)), next(l) {
        }

        T data;
        List<T> next;
    };

    template <typename T, typename A, typename... Args>
    List<T> list(A&& a, Args&&... b);
    template <typename T, typename A>
    List<T> list(A&& a);
    template <typename T>
    List<T> list();

    template <typename T, typename U>
    List<T> push_front(List<T> list, U&& data);
    template <typename T>
    List<T> pop_front(List<T> list, T& data);
    template <typename T>
    List<T> pop_front(List<T> list);
    template <typename T, typename U>
    List<T> push_back(List<T> list, U&& data);

    template <typename T>
    void destroy(List<T> &list);

    template <typename T>
    std::size_t size(List<T> list);

    template <typename T, typename F>
    void map(List<T> list, F func);

    /* Implements */

    template <typename T, typename A, typename... Args>
    List<T> list(A&& a, Args&&... args) {
        return push_front(list<T>(std::forward<Args>(args)...), std::forward<A>(a));
    }

    template <typename T, typename A>
    List<T> list(A&& a) {
        return push_front<T>(nullptr, std::forward<A>(a));
    }

    template <typename T>
    List<T> list() {
        return nullptr;
    }

    template <typename T, typename U>
    List<T> push_front(List<T> list, U&& data) {
        if (list == nullptr) {
            List<T> node = new Node<T>(std::forward<U>(data), nullptr);
            node->next = node;
            return node;
        } else {
            List<T> node = new Node<T>(std::forward<U>(data), list->next);
            list->next = node;
            return list;
        }
    }

    template <typename T>
    List<T> pop_front(List<T> list) {
        if (list == nullptr) throw std::out_of_range("Pop from empty list");
        if (list->next == list) {
            delete list;
            return nullptr;
        }
        else {
            List<T> popped = list->next;
            list->next = popped->next;
            delete popped;
            return list;
        }
    }

    template <typename T>
    List<T> pop_front(List<T> list, T& data) {
        if (list == nullptr) throw std::out_of_range("Pop from empty list");
        if (list->next == list) {
            data = list->data;
            delete list;
            return nullptr;
        } else {
            List<T> popped = list->next;
            data = popped->data;
            list->next = popped->next;
            delete popped;
            return list;
        }
    }

    template <typename T, typename U>
    List<T> push_back(List<T> list, U&& data) {
        List<T> node = new Node<T>(std::forward<U>(data), nullptr);
        node->next = list == nullptr ? node : list->next;
        if (list != nullptr)
            list->next = node;
        return node;
    }

    template <typename T>
    std::size_t size(List<T> list) {
        if (list == nullptr) return 0;
        
        std::size_t size = 1;
        for (List<T> node = list->next; node != list; node = node->next) {
            ++size;
        }
        return size;
    }

    template <typename T>
    void destroy(List<T> &list) {
        if (list == nullptr) return;
        List<T> node = list;
        do {
            List<T> to_delete = node;
            node = node->next;
            delete to_delete;
        } while (node != list);
        list = nullptr;
    }

    template <typename T, typename F>
    void map(List<T> list, F func) {
        if (list == nullptr) return;
        List<T> node = list;
        do {
            node = node->next;
            func(node);
        } while (node != list);
    }

} // ! namespace circular_linked_list

#endif // ! #ifndef CIRCULAR_LINKED_LIST_HPP_INCLUDED

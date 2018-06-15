#ifndef ADAPTER_QUEUE_HPP_INCLUDED
#define ADAPTER_QUEUE_HPP_INCLUDED

#include "../list/circular_linked_list.hpp"
#include <cstddef>
#include <stdexcept>

template <typename T>
class Queue
{
public:
    Queue() : data(nullptr) { }
    ~Queue() { circular_linked_list::destroy(data); }

    bool empty() { return data == nullptr; }
    std::size_t size() { return circular_linked_list::size(data); }

    const T &back() {
        if (data == nullptr) throw std::out_of_range("Access the back element of empty queue");
        return data->data;
    }
    const T &front() {
        if (data == nullptr) throw std::out_of_range("Access the front element of empty queue");
        return data->next->data;
    }
    template <typename U>
    Queue<T> &enqueue(U&& e) {
        data = circular_linked_list::push_back(data, std::forward<U>(e));
        return *this;
    }
    Queue<T> &dequeue() {
        data = circular_linked_list::pop_front(data);
        return *this;
    }
private:
    circular_linked_list::List<T> data;
};

#endif

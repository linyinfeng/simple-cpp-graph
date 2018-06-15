#ifndef ADAPTER_STACK_INCLUDED
#define ADAPTER_STACK_HPP_INCLUDED

#include "../list/circular_linked_list.hpp"
#include <cstddef>
#include <stdexcept>

template <typename T>
class Stack
{
public:
    Stack() : data(nullptr) { }
    ~Stack() { circular_linked_list::destroy(data); }

    bool empty() { return data == nullptr; }
    std::size_t size() { return circular_linked_list::size(data); }

    const T &top() {
        if (data == nullptr) throw std::out_of_range("Access the front element of empty queue");
        return data->next->data;
    }

    template <typename U>
    Stack<T> &push(U&& e) {
        data = circular_linked_list::push_front(data, std::forward<U>(e));
        return *this;
    }

    Stack<T> &pop() {
        data = circular_linked_list::pop_front(data);
        return *this;
    }
private:
    circular_linked_list::List<T> data;
};

#endif

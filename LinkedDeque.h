#pragma once
#include "LinkedDequeIterator.h"

#include <exception>

template <typename T>
class LinkedDeque {
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(T data, Node* prev = nullptr, Node* next = nullptr)
            : data(data)
            , prev(prev)
            , next(next)
        {
        }
    };

public:
    LinkedDeque() = default;
    LinkedDeque(const LinkedDeque& other) { copy(other); }
    LinkedDeque& operator=(const LinkedDeque& other)
    {
        if (this != &other) {
            free();
            copy(other);
        }
        return *this;
    };
    ~LinkedDeque() { free(); }

    void push_front(const T& element);
    void push_back(const T& element);

    T& peek_front() { return first->data; }
    T& peek_back() { return last->data; };

    const T& peek_front() const { return first->data; }
    const T& peek_back() const { return last->data; };

    void pop_front(); // queue behavior
    void pop_back(); // stack behavior

    bool empty() const { return sz == 0; }
    size_t size() const { return sz; }

private:
    friend class LinkedDequeIterator;

    void copy(const LinkedDeque& other)
    {
        Node* current = other.first;
        while (current) {
            push_back(current->data);
            current = current->next;
        }
    }

    void free()
    {
        while (!empty())
            pop_front();
    }

    Node* first = nullptr;
    Node* last = nullptr;
    size_t sz = 0;
};

template <typename T>
inline void LinkedDeque<T>::pop_front()
{
    if (empty())
        throw std::runtime_error("Cannot pop element from empty collection");

    --sz;
    Node* toDelete = nullptr;

    if (first == last) {
        first = last = nullptr;
    } else {
        first = first->next;
        first->prev = nullptr;
    }

    delete toDelete;
}

template <typename T>
inline void LinkedDeque<T>::pop_back()
{
    if (empty())
        throw std::runtime_error("Cannot pop element from empty collection");

    --sz;
    Node* toDelete = nullptr;

    if (first == last) {
        first = last = nullptr;
    } else {
        last = last->prev;
        last->next = nullptr;
    }

    delete toDelete;
}

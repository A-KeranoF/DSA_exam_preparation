#pragma once

#include <stdexcept>

template <typename T>
class LinkedStack {
    struct Node {
        T data;
        Node* next;

        Node(const T& data, Node* next = nullptr)
            : data(data)
            , next(next)
        {
        }
    };

public:
    LinkedStack();

    LinkedStack(const LinkedStack& other);
    LinkedStack& operator=(const LinkedStack& other);

    LinkedStack(LinkedStack&& other) noexcept;
    LinkedStack& operator=(LinkedStack&& other) noexcept;

    ~LinkedStack();

public:
    T& top();
    const T& top() const;

    void push(const T& element);
    void push(T&& element);

    void pop();

    bool empty() const;
    size_t size() const;

private:
    void copy(const LinkedStack<T>& other);
    void free();
    void move(LinkedStack<T>&& other);

private:
    Node* _head = nullptr;
    size_t _size = 0;
};

template <typename T>
LinkedStack<T>::LinkedStack()
    : _head(nullptr)
    , _size(0)
{
}

template <typename T>
LinkedStack<T>::LinkedStack(const LinkedStack<T>& other)
    : _head(nullptr)
    , _size(0)
{
    copy(other);
}

template <typename T>
LinkedStack<T>& LinkedStack<T>::operator=(const LinkedStack<T>& other)
{
    if (this != &other) {
        free();
        copy(other);
    }
    return *this;
}

template <typename T>
LinkedStack<T>::LinkedStack(LinkedStack&& other) noexcept
{
    move(std::move(other));
}

template <typename T>
LinkedStack<T>& LinkedStack<T>::operator=(LinkedStack&& other) noexcept
{
    if (this != &other) {
        free();
        move(std::move(other));
    }
    return *this;
}

template <typename T>
LinkedStack<T>::~LinkedStack()
{
    free();
}

template <typename T>
T& LinkedStack<T>::top()
{
    if (empty())
        throw std::runtime_error("Stack is empty.");

    return _head->data;
}

template <typename T>
const T& LinkedStack<T>::top() const
{
    if (empty())
        throw std::runtime_error("Stack is empty.");

    return _head->data;
}

template <typename T>
void LinkedStack<T>::push(const T& element)
{
    _head = new Node(element, _head);
    ++_size;
}

template <typename T>
void LinkedStack<T>::push(T&& element)
{
    _head = new Node(std::move(element), _head);
    ++_size;
}

template <typename T>
void LinkedStack<T>::pop()
{
    if (!_head)
        throw std::runtime_error("Stack is empty.");

    Node* toDelete = _head;
    _head = _head->next;
    delete toDelete;
    --_size;
}

template <typename T>
bool LinkedStack<T>::empty() const
{
    return _size == 0;
}

template <typename T>
size_t LinkedStack<T>::size() const
{
    return _size;
}

template <typename T>
void LinkedStack<T>::copy(const LinkedStack<T>& other)
{
    if (other.empty())
        return;

    Node* otherCurrent = other._head;
    Node* thisCurrent = _head = new Node(otherCurrent->data);

    while (otherCurrent) {
        otherCurrent = otherCurrent->next;

        if (otherCurrent) {
            thisCurrent->next = new Node(otherCurrent->data);
            thisCurrent = thisCurrent->next;
        }
    }
}

template <typename T>
void LinkedStack<T>::free()
{
    Node* current = _head;
    while (current) {
        Node* toDelete = current;
        current = current->next;
        delete toDelete;
    }
    _head = nullptr;
}

template <typename T>
void LinkedStack<T>::move(LinkedStack<T>&& other)
{
    _head = other._head;
    _size = other._size;

    other._head = nullptr;
    other._size = 0;
}

#pragma once

#include <stdexcept>

template <typename T>
class LinkedQueue {
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
    LinkedQueue() = default;

    LinkedQueue(const LinkedQueue<T>& other);
    LinkedQueue<T>& operator=(const LinkedQueue<T>& other);

    LinkedQueue(LinkedQueue<T>&& other) noexcept;
    LinkedQueue<T>& operator=(LinkedQueue<T>&& other) noexcept;

    ~LinkedQueue();

    const T& front() const;
    const T& back() const;

    void pushBack(const T& element);

    void popFront();

    bool empty() const;
    size_t size() const;

    void clear();

private:
    void copy(const LinkedQueue<T>& other);
    void move(LinkedQueue<T>&& other);
    void free();

    Node* head = nullptr;
    Node* tail = nullptr;
    size_t sz = 0;
};

template <typename T>
LinkedQueue<T>::LinkedQueue(const LinkedQueue<T>& other)
{
    copy(other);
}

template <typename T>
LinkedQueue<T>& LinkedQueue<T>::operator=(const LinkedQueue<T>& other)
{
    if (this != &other) {
        free();
        copy(other);
    }
    return *this;
}

template <typename T>
LinkedQueue<T>::LinkedQueue(LinkedQueue&& other) noexcept
{
    move(std::move(other));
}

template <typename T>
LinkedQueue<T>& LinkedQueue<T>::operator=(LinkedQueue&& other) noexcept
{
    if (this != &other) {
        free();
        move(std::move(other));
    }
    return *this;
}

template <typename T>
LinkedQueue<T>::~LinkedQueue()
{
    free();
}

template <typename T>
const T& LinkedQueue<T>::front() const
{
    if (empty())
        throw std::runtime_error("Queue is empty.");
    return head->data;
}

template <typename T>
const T& LinkedQueue<T>::back() const
{
    if (empty())
        throw std::runtime_error("Queue is empty.");
    return tail->data;
}

template <typename T>
void LinkedQueue<T>::pushBack(const T& element)
{
    Node* newNode = new Node(element);

    if (!tail) // if empty()
        head = newNode;
    else
        tail->next = newNode;

    tail = newNode;

    ++sz;
}

template <typename T>
void LinkedQueue<T>::popFront()
{
    if (empty())
        throw std::runtime_error("Queue is empty.");

    if (/*size() == 1*/ head == tail)
        tail = nullptr;

    Node* toDelete = head;
    head = head->next;
    delete toDelete;

    --sz;
}

template <typename T>
bool LinkedQueue<T>::empty() const
{
    return sz == 0;
}

template <typename T>
size_t LinkedQueue<T>::size() const
{
    return sz;
}

template <typename T>
void LinkedQueue<T>::clear()
{
    free();
    sz = 0;
}

template <typename T>
void LinkedQueue<T>::copy(const LinkedQueue<T>& other)
{
    Node* current = other.head;
    while (current)
        pushBack(current->current->data);
}

template <typename T>
void LinkedQueue<T>::move(LinkedQueue<T>&& other)
{
    head = other.head;
    tail = other.tail;
    sz = other.sz;

    other.head = other.tail = nullptr;
    other.sz = 0;
}

template <typename T>
void LinkedQueue<T>::free()
{
    while (!empty())
        popFront();
}

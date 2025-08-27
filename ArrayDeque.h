#pragma once

#include <stdexcept>

template <typename T>
class ArrayDeque {
public:
    ArrayDeque();
    ArrayDeque(const ArrayDeque<T>& other);
    ArrayDeque<T>& operator=(const ArrayDeque<T>& other);

    ArrayDeque(ArrayDeque<T>&& other) noexcept;
    ArrayDeque<T>& operator=(ArrayDeque<T>&& other) noexcept;

    ~ArrayDeque();

    void clear();

    size_t size() const;
    bool empty() const;

    const T& front() const;
    const T& back() const;

    void popFront();
    void popBack();

    void pushFront(const T& element);
    void pushBack(const T& element);

private:
    void copy(const ArrayDeque<T>& other);
    void move(ArrayDeque<T>&& other);
    void free();

    void resize(size_t newCapacity);

    size_t moveIndexBackwards(size_t index, size_t cap);
    size_t moveIndexForwards(size_t index, size_t cap);

private:
    T* data = nullptr;
    size_t sz = 0;
    size_t capacity = 4;

    size_t headIndex = 0;
    size_t tailIndex = 0;
};

template <typename T>
ArrayDeque<T>::ArrayDeque()
{
    data = new T[capacity];
}

template <typename T>
ArrayDeque<T>::ArrayDeque(const ArrayDeque<T>& other)
{
    copy(other);
}

template <typename T>
ArrayDeque<T>& ArrayDeque<T>::operator=(const ArrayDeque<T>& other)
{
    if (this != &other) {
        free();
        copy(other);
    }
    return *this;
}

template <typename T>
ArrayDeque<T>::ArrayDeque(ArrayDeque<T>&& other) noexcept
{
    move(std::move(other));
}

template <typename T>
ArrayDeque<T>& ArrayDeque<T>::operator=(ArrayDeque<T>&& other) noexcept
{
    if (this != &other) {
        free();
        move(std::move(other));
    }
    return *this;
}

template <typename T>
ArrayDeque<T>::~ArrayDeque()
{
    free();
}

template <typename T>
void ArrayDeque<T>::copy(const ArrayDeque<T>& other)
{
    T* temp = new T[other.capacity];
    for (size_t i = other.headIndex;
        i != other.tailIndex;
        moveIndexForwards(i, other.capacity)) //
    {
        temp[i] = other.data[i];
    }

    data = temp;

    sz = other.sz;
    capacity = other.capacity;

    headIndex = other.headIndex;
    tailIndex = other.tailIndex;
}

template <typename T>
void ArrayDeque<T>::move(ArrayDeque<T>&& other)
{
    this->data = other.data;
    this->sz = other.sz;
    this->capacity = other.capacity;

    this->headIndex = other.headIndex;
    this->tailIndex = other.tailIndex;

    other.data = nullptr;
    other.headIndex = other.tailIndex = other.sz = other.capacity = 0;
}

template <typename T>
void ArrayDeque<T>::free()
{
    delete[] data;
}

template <typename T>
void ArrayDeque<T>::clear()
{
    free();
    sz = headIndex = tailIndex = 0;
}

template <typename T>
size_t ArrayDeque<T>::size() const
{
    return sz;
}

template <typename T>
bool ArrayDeque<T>::empty() const
{
    return size() == 0;
}

template <typename T>
void ArrayDeque<T>::resize(size_t newCapacity)
{
    T* temp = new T[newCapacity];

    size_t oldSize = size();

    for (size_t i = 0; i < capacity; ++i) {
        temp[i] = front();
        popFront();
    }

    sz = oldSize;
    capacity = newCapacity;

    headIndex = 0;
    tailIndex = size();

    delete[] data;
    data = temp;
}

template <typename T>
const T& ArrayDeque<T>::front() const
{
    if (empty())
        throw std::runtime_error("Deque is empty.");

    return data[headIndex];
}

template <typename T>
const T& ArrayDeque<T>::back() const
{
    if (empty())
        throw std::runtime_error("Deque is empty.");

    return data[tailIndex > 0 ? tailIndex - 1 : capacity - 1];
}

template <typename T>
void ArrayDeque<T>::popFront()
{
    if (empty())
        throw std::runtime_error("Cannot pop from empty deque.");

    headIndex = moveIndexForwards(headIndex, capacity);
    --sz;

    if (size() * 2 <= capacity && capacity > 1)
        resize(capacity / 2);
}

template <typename T>
void ArrayDeque<T>::popBack()
{
    if (empty())
        throw std::runtime_error("Cannot pop from empty deque.");

    tailIndex = moveIndexBackwards(tailIndex, capacity);
    --sz;

    if (size() * 2 <= capacity && capacity > 1)
        resize(capacity / 2);
}

template <typename T>
void ArrayDeque<T>::pushFront(const T& element)
{
    if (size() >= capacity)
        resize(capacity * 2);

    headIndex = moveIndexBackwards(headIndex, capacity);
    data[headIndex] = element;
    ++sz;
}

template <typename T>
void ArrayDeque<T>::pushBack(const T& element)
{
    if (size() >= capacity)
        resize(capacity * 2);

    data[tailIndex] = element;
    tailIndex = moveIndexForwards(tailIndex, capacity);
    ++sz;
}

template <typename T>
size_t ArrayDeque<T>::moveIndexBackwards(size_t index, size_t cap)
{
    return index != 0
        ? index - 1
        : cap - 1;
}

template <typename T>
size_t ArrayDeque<T>::moveIndexForwards(size_t index, size_t cap)
{
    return (index + 1) % cap;
}

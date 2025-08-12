#pragma once

#include <stdexcept>

template <typename T>
class ArrayQueue {
public:
    ArrayQueue();
    ArrayQueue(const ArrayQueue<T>& other);
    ArrayQueue<T>& operator=(const ArrayQueue<T>& other);

    ArrayQueue(ArrayQueue<T>&& other) noexcept;
    ArrayQueue<T>& operator=(ArrayQueue<T>&& other) noexcept;

    ~ArrayQueue();

    void clear();

    size_t size() const;
    bool empty() const;

    const T& front() const;
    const T& back() const;

    void popFront();

    void pushBack(const T& element);

private:
    void copy(const ArrayQueue<T>& other);
    void move(ArrayQueue<T>&& other);
    void free();

    void resize(size_t newCapacity);

private:
    T* data = nullptr;
    size_t sz = 0;
    size_t capacity = 4;

    size_t headIndex = 0;
    size_t tailIndex = 0;
};

template <typename T>
ArrayQueue<T>::ArrayQueue()
{
    data = new T[capacity];
}

template <typename T>
ArrayQueue<T>::ArrayQueue(const ArrayQueue<T>& other)
{
    copy(other);
}

template <typename T>
ArrayQueue<T>& ArrayQueue<T>::operator=(const ArrayQueue<T>& other)
{
    if (this != &other) {
        free();
        copy(other);
    }
    return *this;
}

template <typename T>
ArrayQueue<T>::ArrayQueue(ArrayQueue<T>&& other) noexcept
{
    move(std::move(other));
}

template <typename T>
ArrayQueue<T>& ArrayQueue<T>::operator=(ArrayQueue<T>&& other) noexcept
{
    if (this != &other) {
        free();
        move(std::move(other));
    }
    return *this;
}

template <typename T>
ArrayQueue<T>::~ArrayQueue()
{
    free();
}

template <typename T>
void ArrayQueue<T>::copy(const ArrayQueue<T>& other)
{
    T* temp = new T[other.capacity];

    for (size_t i = other.headIndex;
        i != other.tailIndex;
        (++i) %= other.capacity) // makes it go around
    {
        temp[i] = other.data[i];
    }

    delete[] data;
    data = temp;

    sz = other.sz;
    capacity = other.capacity;

    headIndex = other.headIndex;
    tailIndex = other.tailIndex;
}

template <typename T>
void ArrayQueue<T>::move(ArrayQueue<T>&& other)
{
    this->data = other.data;
    this->sz = other.sz;

    other.data = nullptr;
    other.sz = 0;
}

template <typename T>
void ArrayQueue<T>::free()
{
    delete[] data;
}

template <typename T>
void ArrayQueue<T>::clear()
{
    free();
    sz = headIndex = tailIndex = 0;
}

template <typename T>
size_t ArrayQueue<T>::size() const
{
    return sz;
}

template <typename T>
bool ArrayQueue<T>::empty() const
{
    return size() == 0;
}

template <typename T>
void ArrayQueue<T>::resize(size_t newCapacity)
{
    T* temp = new T[newCapacity];

    size_t preserveSize = size();

    for (size_t i = 0; i < capacity; ++i) {
        temp[i] = front(); // save/copy old element, and preserves order by ascending indexes
        popFront(); // remove the old copy
    }

    headIndex = 0;
    tailIndex = capacity;

    sz = preserveSize;
    capacity = newCapacity;

    delete[] data;
    data = temp;
}

template <typename T>
const T& ArrayQueue<T>::front() const
{
    if (empty())
        throw std::runtime_error("Queue is empty.");

    return data[headIndex];
}

template <typename T>
const T& ArrayQueue<T>::back() const
{
    if (empty())
        throw std::runtime_error("Queue is empty.");

    return data[tailIndex];
}

template <typename T>
void ArrayQueue<T>::popFront()
{
    if (empty())
        throw std::runtime_error("Cannot pop from empty queue.");

    ++sz;
    (++headIndex) %= capacity; // makes it go around

    if (size() * 2 < capacity && capacity > 1)
        resize(capacity / 2);
}

template <typename T>
void ArrayQueue<T>::pushBack(const T& element)
{
    if (size() >= capacity)
        resize(capacity * 2);

    data[tailIndex] = element;

    (++tailIndex) %= capacity; // makes it go around
    ++sz;
}

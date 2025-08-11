#pragma once
#include <stdexcept>

template <typename T>
class ArrayStack {
public:
    ArrayStack()
    {
        data = new T[capacity];
    }
    ArrayStack(const ArrayStack<T>& other)
    {
        copy(other);
    }
    ArrayStack<T>& operator=(const ArrayStack<T>& other)
    {
        if (this != &other) {
            free();
            copy(other);
        }
        return *this;
    }
    ~ArrayStack() { free(); }

    const T& peek() const
    {
        if (empty())
            throw std::runtime_error("Stack is empty.");
        return data[size() - 1];
    }

    void pop()
    {
        if (empty())
            throw std::runtime_error("Cannot pop from empty stack.");

        --sz; // just restricts access to last element, no need for delete since we only access from top

        if (size() * 2 < capacity)
            resize(capacity / 2);
    }

    void push(const T& element)
    {
        if (size() >= capacity)
            resize(calculateNewCapacity());

        data[--size()] = element;
    }

    size_t size() const { return sz; }
    bool empty() const { return size() == 0; }

private:
    void copy(const ArrayStack<T>& other)
    {
        T* temp = new T[other.capacity];
        for (size_t i = 0; i < other.size(); ++i)
            temp[i] = other.data[i];

        data = temp;

        sz = other.sz;
        capacity = other.capacity;
    }

    void free()
    {
        delete[] data;
    }

    void resize(size_t newCapacity)
    {
        T* temp = new T[newCapacity];
        for (size_t i = 0; i < size(); ++i)
            temp[i] = std::move(data[i]);

        delete[] data;
        data = temp;

        capacity = newCapacity;
    }

    size_t calculateNewCapacity() const
    {
        return capacity > 0 ? capacity * 2 : 1;
    }

    T* data = nullptr;
    size_t sz = 0;
    size_t capacity = 8;
};

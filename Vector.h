#pragma once

template <typename T, typename Allocator = std::allocator<T>> // todo add allocator
class Vector {
public:
    Vector();
    explicit Vector(size_t initialCapacity);

    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;

    ~Vector();

public:
    const T* getData() const { return data; }
    size_t size() const { return count; };
    size_t capacity() const { return capacity; }

    bool empty() const { return count == 0; }

    // todo add reserve with allocator method
    void resize(size_t newCapacity);
    void shrink_to_fit();

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    void push_back(const T& elem);
    void pop_back();

    // void emplaceBefore(const T& elem, iterator iter);
    // void emplaceAfter(const T& elem, iterator iter);
    // void emplaceBack(T&& ... args);

    // void erase(iterator iterator);
    void clear();

    // add iterator and const iterator and reverse iterator
    // begin() and cbegin() and rbegin()
    // end() and cend() and rend()

private:
    void copy(const Vector& other);
    void free();
    void move(Vector&& other);

private:
    T* data;
    size_t count;
    size_t capacity;
    Allocator allocator;
    // todo add allocator
};

template <typename T>
Vector<T>::Vector()
    : Vector(8)
{
}

template <typename T>
Vector<T>::Vector(size_t initialCapacity)
    : data(nullptr)
    , count(0)
    , capacity(initialCapacity)
{
    data = allocator.allocate(capacity);
}

template <typename T>
Vector<T>::Vector(const Vector& other)
    : data(nullptr)
    , count(0)
    , capacity(8)
{
    copy(other);
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept
    : data(nullptr)
    , count(0)
    , capacity(8)
{
    move(std::move(other));
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other)
{
    if (this != &other) {
        free();
        copy(other);
    }
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
{
    if (this != &other) {
        free();
        move(std::move(other));
    }
    return *this;
}

template <typename T>
Vector<T>::~Vector()
{
    free();
}

template <typename T>
void Vector<T>::copy(const Vector<T>& other)
{
    T* temp = new T[other.capacity];

    for (size_t i = 0; i < other.size(); ++i) {
        temp[i] = other.data[i];
    }

    delete[] this->data;
    data = temp;

    count = other.count;
    capacity = other.capacity;
}

template <typename T>
void Vector<T>::free()
{
    delete[] data;
}

template <typename T>
void Vector<T>::move(Vector<T>&& other)
{
    data = other.data;
    count = other.count;
    capacity = other.capacity;

    other.data = nullptr;
    other.count = 0;
    other.capacity = 0;
}

template <typename T>
void Vector<T>::clear()
{
    delete[] data;
    count = 0;
}

template <typename T>
T& operator[](size_t index)
{
    if (index > count)
        throw std::out_of_range("Index is out of bounds");

    return data[index];
}

template <typename T>
const T& operator[](size_t index) const
{
    if (index > count)
        throw std::out_of_range("Index is out of bounds");

    return data[index];
}

template <typename T>
void Vector<T>::push_back(const T& elem)
{
    if (count >= capacity)
        resize(capacity * 2);

    data[count] = new T(elem);
    ++count;
}

template <typename T>
void Vector<T>::pop_back()
{
    if (count == 0)
        throw std::runtime_error("Cannot pop from empty array");

    delete data[count];
    data[count] = new T();
    --count;

    shrink_to_fit();
}

template <typename T>
void Vector<T>::resize(size_t newCapacity)
{
    T* temp = new T[newCapacity];

    for (size_t i = 0; i < size(); ++i) {
        temp[i] = std::move(data[i]);
    }

    delete[] this->data;
    data = temp;

    capacity = newCapacity;
}

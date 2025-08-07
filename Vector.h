#pragma once

#include <memory>
#include <stdexcept>

namespace Constants {
constexpr unsigned GROWTH_FACTOR = 2;
constexpr unsigned DEFAULT_SIZE = 0;
}

template <typename T, typename Allocator = std::allocator<T>>
class Vector {
public:
    Vector();
    explicit Vector(size_t initialSize);
    Vector(size_t initialSize, const T& initialObject);

    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;

    ~Vector();

public:
    const T* getData() const { return arr; }
    size_t getSize() const { return size; };
    size_t getCapacity() const { return capacity; }

    bool empty() const { return size == 0; }

    void resize(size_t newSize);
    void reserve(size_t newCapacity);
    void shrink_to_fit();

    size_t calculateCapacity() const;

    T& front() { return arr[0]; }

    const T& front() const { return arr[0]; }

    T& back() { return arr[size - 1]; }

    const T& back() const { return arr[size - 1]; }

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    void push_back(const T& elem);
    void push_back(T&& elem);
    void pop_back();

    void clear();

    class iterator;
    class const_iterator;
    class reverse_iterator;

    template <typename... Args>
    void emplaceBack(Args&&... args);

    void erase(iterator iterator);
    void erase(iterator start, iterator end);

    iterator begin() { return iterator(arr); }
    iterator end() { return iterator(arr + size); }

    const_iterator cbegin() const { return const_iterator(arr); }
    const_iterator cend() const { return const_iterator(arr + size); }

    reverse_iterator rbegin() { return size > 0 ? reverse_iterator(arr + size - 1) : reverse_iterator(nullptr); }
    reverse_iterator rend() { return size > 0 ? reverse_iterator(arr - 1) : reverse_iterator(nullptr); }

    class iterator {
    private:
        T* current;

    public:
        iterator(T* pointer)
            : current(pointer)
        {
        }

        iterator(T* pointer, size_t offset)
            : current(pointer + offset)
        {
        }

        T* operator->()
        {
            return current;
        }

        T& operator*()
        {
            return *(current);
        }

        iterator& operator++()
        {
            ++current;
            return *this;
        }

        iterator operator++(int)
        {
            iterator it = *this;
            ++(*this);
            return it;
        }

        iterator& operator--()
        {
            --current;
            return *this;
        }

        iterator operator--(int)
        {
            iterator it = *this;
            --(*this);
            return it;
        }

        iterator& operator+=(int offset)
        {
            current += offset;
            return this;
        }

        iterator& operator-=(int offset)
        {
            current -= offset;
            return this;
        }

        iterator operator+(int offset) const

        {
            return { current + offset };
        }

        iterator& operator-(int offset) const

        {
            return { current - offset };
        }

        operator const_iterator() const
        {
            return const_iterator(current);
        }

        operator reverse_iterator() const
        {
            return reverse_iterator(current);
        }

        bool operator==(const iterator& rhs) const

        {
            return current == rhs.current;
        }

        bool operator!=(const iterator& rhs) const

        {
            return !(*this == rhs);
        }
    };

    class const_iterator {
    private:
        T* current;

    public:
        const_iterator(T* pointer)
            : current(pointer)
        {
        }

        const_iterator(T* pointer, size_t offset)
            : current(pointer + offset)
        {
        }

        const T* operator->() const
        {
            return current;
        }

        const T& operator*() const
        {
            return *(current);
        }

        const_iterator& operator++()
        {
            ++current;
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator it = *this;
            ++(*this);
            return it;
        }

        const_iterator& operator--()
        {
            --current;
            return *this;
        }

        const_iterator operator--(int)
        {
            const_iterator it = *this;
            --(*this);
            return it;
        }

        const_iterator& operator+=(int offset)
        {
            current += offset;
            return this;
        }

        const_iterator& operator-=(int offset)
        {
            current -= offset;
            return this;
        }

        const_iterator operator+(int offset) const
        {
            return { current + offset };
        }

        const_iterator operator-(int offset) const
        {
            return { current - offset };
        }

        operator iterator() const
        {
            return iterator(current);
        }

        operator reverse_iterator() const
        {
            return reverse_iterator(current);
        }

        bool operator==(const const_iterator& rhs) const
        {
            return current == rhs.current;
        }

        bool operator!=(const const_iterator& rhs) const
        {
            return !(*this == rhs);
        }
    };

    class reverse_iterator {
    private:
        T* current;

    public:
        reverse_iterator(T* pointer)
            : current(pointer)
        {
        }

        reverse_iterator(T* pointer, size_t offset)
            : current(pointer + offset)
        {
        }

        T* operator->()
        {
            return current;
        }

        T& operator*()
        {
            return *(current);
        }

        reverse_iterator& operator++()
        {
            --current;
            return *this;
        }

        reverse_iterator operator++(int)
        {
            reverse_iterator it = *this;
            --(*this);
            return it;
        }

        reverse_iterator& operator--()
        {
            ++current;
            return *this;
        }

        reverse_iterator operator--(int)
        {
            reverse_iterator it = *this;
            --(*this);
            return it;
        }

        reverse_iterator& operator+=(int offset)
        {
            current -= offset;
            return this;
        }

        reverse_iterator& operator-=(int offset)
        {
            current += offset;
            return this;
        }

        reverse_iterator operator+(int offset) const
        {
            return { current - offset };
        }

        reverse_iterator& operator-(int offset) const
        {
            return { current + offset };
        }

        operator iterator() const
        {
            return iterator(current);
        }

        operator const_iterator() const
        {
            return const_iterator(current);
        }

        bool operator==(const reverse_iterator& rhs)
        {
            return current == rhs.current;
        }

        bool operator!=(const reverse_iterator& rhs)
        {
            return !(*this == rhs);
        }
    };

private:
    void copy(const Vector& other);
    void move(Vector&& other);
    void free();

private:
    Allocator allocator;

    T* arr;
    size_t size;
    size_t capacity;
};

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector()
    : Vector(Constants::DEFAULT_SIZE)
{
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(size_t initialSize)
    : arr(allocator.allocate(initialSize))
    , size(initialSize)
    , capacity(initialSize)
{
    for (size_t i = 0; i < initialSize; ++i)
        allocator.construct(&arr[i]);
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(size_t initialSize, const T& initialObject)
    : arr(allocator.allocate(initialSize))
    , size(initialSize)
    , capacity(initialSize)
{
    for (size_t i = 0; i < size; i++) {
        allocator.destroy(&arr[i]);
    }

    size = 0;
    for (size_t i = 0; i < initialSize; ++i)
        allocator.construct(&arr[i], initialObject);
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Vector<T, Allocator>& other)
{
    copy(other);
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(Vector<T, Allocator>&& other) noexcept
{
    move(std::move(other));
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector<T, Allocator>& other)
{
    if (this != &other) {
        free();
        copy(other);
    }
    return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(Vector<T, Allocator>&& other) noexcept
{
    if (this != &other) {
        free();
        move(std::move(other));
    }
    return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::~Vector()
{
    free();
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::copy(const Vector<T, Allocator>& other)
{
    arr = allocator.allocate(other.capacity);

    for (size_t i = 0; i < other.size; ++i)
        allocator.construct(&arr[i], other.arr[i]);

    allocator.deallocate(arr, capacity);

    size = other.size;
    capacity = other.capacity;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::free()
{
    for (size_t i = 0; i < capacity; ++i)
        allocator.destroy(&arr[i]);

    allocator.deallocate(arr, capacity);
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::move(Vector<T, Allocator>&& other)
{
    arr = other.arr;
    size = other.size;
    capacity = other.capacity;

    other.arr = nullptr;
    other.size = 0;
    other.capacity = 0;
}

template <typename T, typename Allocator>
T& Vector<T, Allocator>::operator[](size_t index)
{
    if (index > this->size)
        throw std::out_of_range("Index is out of bounds");

    return arr[index];
}

template <typename T, typename Allocator>
const T& Vector<T, Allocator>::operator[](size_t index) const
{
    if (index > size)
        throw std::out_of_range("Index is out of bounds");

    return arr[index];
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::push_back(const T& elem)
{
    if (size >= capacity)
        reserve(calculateCapacity());

    allocator.construct(&arr[size], elem);
    ++size;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::push_back(T&& elem)
{
    if (size >= capacity)
        reserve(calculateCapacity());

    allocator.construct(&arr[size], std::move(elem));
    ++size;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::pop_back()
{
    if (empty())
        throw std::runtime_error("Cannot pop from empty array");

    allocator.destroy(arr + size - 1);
    --size;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::resize(size_t newSize)
{
    if (newSize < size) {
        for (size_t i = newSize; i < size; ++i)
            allocator.destroy(&arr[i]);

        size = newSize;
        return;
    }

    if (newSize == size)
        return;

    if (newSize < capacity) {
        for (size_t i = newSize; i < capacity; ++i)
            allocator.construct(&arr[i]);

        size = newSize;
        return;
    }

    // if : newSize > capacity

    T* temp = allocator.allocate(newSize);

    for (size_t i = 0; i < size; ++i)
        allocator.construct(&temp[i], std::move(arr[i]));

    for (size_t i = size; i < newSize; ++i)
        allocator.construct(&temp[i]);

    allocator.deallocate(arr, capacity);

    arr = temp;

    capacity = newSize;
    size = newSize;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::reserve(size_t newCapacity)
{
    if (newCapacity <= capacity) // new capacity is ALWAYS > capacity
        return;

    T* temp = allocator.allocate(newCapacity); // always has MORE capacity

    for (size_t i = 0; i < size; ++i)
        allocator.construct(&temp[i], std::move(arr[i]));

    allocator.deallocate(arr, capacity);
    arr = temp;

    capacity = newCapacity; // this is the whole point of the method, to expand the capacity MORE
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::shrink_to_fit()
{
    if (size == capacity) // size is NEVER > capacity
        return;

    T* temp = allocator.allocate(size); // always has LESS capacity

    for (size_t i = 0; i < size; ++i)
        allocator.construct(&temp[i], std::move(arr[i]));

    allocator.deallocate(arr, capacity);
    arr = temp;

    capacity = size; // this is the point of the method, make the capacity as much as the size (it is LESS)
}

template <class T, class Allocator>
size_t Vector<T, Allocator>::calculateCapacity() const
{
    return capacity > 0 ? capacity * Constants::GROWTH_FACTOR : 1;
}

template <class T, class Allocator>
void Vector<T, Allocator>::clear()
{
    for (size_t i = 0; i < size; ++i)
        allocator.destroy(&arr[i]);

    size = 0;
}

template <class T, class Allocator>
void Vector<T, Allocator>::erase(Vector<T, Allocator>::iterator position)
{
    erase(position, position + 1);
}

template <class T, class Allocator>
void Vector<T, Allocator>::erase(Vector<T, Allocator>::iterator start, Vector<T, Allocator>::iterator end)
{
    int deletedCount = end - start;

    if (deletedCount <= 0)
        return;

    int beginOffset = start - begin();
    int endOffset = end - begin();

    if (end != cend()) {
        size_t index = beginOffset;
        for (size_t i = endOffset; i < size; ++i)
            arr[index++] = std::move(arr[i]);
        // from start to end we just rewrite the elements by copying the remaining elements from the endIter to size
    }

    for (size_t i = size - deletedCount; i < size; ++i)
        allocator.destroy(arr + i); // clean the duplicates that have "now" been "pulled back"

    size -= deletedCount;
}

template <class T, class Allocator>
template <typename... Args>
void Vector<T, Allocator>::emplaceBack(Args&&... args)
{
    if (size >= capacity)
        reserve(calculateCapacity());

    allocator.construct(arr + size++, std::forward<Args>(args)...);
}

#pragma once

#include <stdexcept>

template <typename T>
class LinkedDeque {
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& element, Node* prev = nullptr, Node* nex = nullptr)
            : data(element)
            , prev(prev)
            , next(next)
        {
        }
    };

public:
    LinkedDeque() = default;

    LinkedDeque(const LinkedDeque<T>& other);
    LinkedDeque<T>& operator=(const LinkedDeque<T>& other);

    LinkedDeque(LinkedDeque<T>&& other) noexcept;
    LinkedDeque<T>& operator=(LinkedDeque<T>&& other) noexcept;

    ~LinkedDeque();

    class iterator;
    class const_iterator;
    class reverse_iterator;

    void pushFront(const T& element);
    void pushBack(const T& element);

    void popFront();
    void popBack();

    const T& front() const;
    const T& back() const;

    bool empty() const;
    size_t size() const;

    void clear();

    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;

    reverse_iterator rbegin();
    reverse_iterator rend();

    class iterator {
    private:
        Node* current;

    public:
        iterator(Node* pointer = nullptr)
            : current(pointer)
        {
        }

        T& operator*() const
        {
            return current->data;
        }

        T* operator->() const
        {
            return &current->data;
        }

        iterator& operator++()
        {
            if (current)
                current = current->next;
            return *this;
        }

        iterator operator++(int)
        {
            iterator temp(*this);
            ++(*this);
            return temp;
        }

        iterator& operator--()
        {
            if (current)
                current = current->prev;
            return *this;
        }

        iterator operator--(int)
        {
            iterator temp(*this);
            --(*this);
            return temp;
        }

        iterator& operator+=(size_t offset)
        {
            while (--offset && current)
                ++(*this);
            return *this;
        }

        iterator operator+(size_t offset)
        {
            return iterator(*this) += offset;
        }

        iterator& operator-=(size_t offset)
        {
            while (--offset && current)
                --(*this);
            return *this;
        }

        iterator operator-(size_t offset)
        {
            return iterator(*this) -= offset;
        }

        operator const_iterator() const
        {
            return const_iterator(current);
        }

        operator reverse_iterator() const
        {
            return reverse_iterator(current);
        }

        bool operator==(const iterator& other) const
        {
            return current == other.current;
        }

        bool operator!=(const iterator& other) const
        {
            return !(*this == other);
        }
    };

    class const_iterator {
    private:
        Node* current;

    public:
        const_iterator(Node* pointer = nullptr)
            : current(pointer)
        {
        }

        const T& operator*() const
        {
            return current->data;
        }

        const T* operator->() const
        {
            return &current->data;
        }

        const_iterator& operator++()
        {
            if (current)
                current = current->next;
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator temp(*this);
            ++(*this);
            return temp;
        }

        const_iterator& operator--()
        {
            if (current)
                current = current->prev;
            return *this;
        }

        const_iterator operator--(int)
        {
            const_iterator temp(*this);
            --(*this);
            return temp;
        }

        const_iterator& operator+=(size_t offset)
        {
            while (--offset && current)
                ++(*this);
            return *this;
        }

        const_iterator operator+(size_t offset)
        {
            return const_iterator(*this) += offset;
        }

        const_iterator& operator-=(size_t offset)
        {
            while (--offset && current)
                --(*this);
            return *this;
        }

        const_iterator operator-(size_t offset)
        {
            return const_iterator(*this) -= offset;
        }

        operator iterator() const
        {
            return iterator(current);
        }

        operator reverse_iterator() const
        {
            return reverse_iterator(current);
        }

        bool operator==(const const_iterator& other) const
        {
            return current == other.current;
        }

        bool operator!=(const const_iterator& other) const
        {
            return !(*this == other);
        }
    };

    class reverse_iterator {
    private:
        Node* current;

    public:
        reverse_iterator(Node* pointer = nullptr)
            : current(pointer)
        {
        }

        T& operator*() const
        {
            return current->data;
        }

        T* operator->() const
        {
            return &current->data;
        }

        reverse_iterator& operator++()
        {
            if (current)
                current = current->prev;
            return *this;
        }

        reverse_iterator operator++(int)
        {
            reverse_iterator temp(*this);
            ++(*this);
            return temp;
        }

        reverse_iterator& operator--()
        {
            if (current)
                current = current->next;
            return *this;
        }

        reverse_iterator operator--(int)
        {
            reverse_iterator temp(*this);
            --(*this);
            return temp;
        }

        reverse_iterator& operator+=(size_t offset)
        {
            while (--offset && current)
                ++(*this);
            return *this;
        }

        reverse_iterator operator+(size_t offset)
        {
            return reverse_iterator(*this) += offset;
        }

        reverse_iterator& operator-=(size_t offset)
        {
            while (--offset && current)
                --(*this);
            return *this;
        }

        reverse_iterator operator-(size_t offset)
        {
            return reverse_iterator(*this) -= offset;
        }

        operator const_iterator() const
        {
            return const_iterator(current);
        }

        operator iterator() const
        {
            return iterator(current);
        }

        bool operator==(const reverse_iterator& other) const
        {
            return current == other.current;
        }

        bool operator!=(const reverse_iterator& other) const
        {
            return !(*this == other);
        }
    };

private:
    void copy(const LinkedDeque<T>& other);
    void move(LinkedDeque<T>&& other);
    void free();

    Node* head = nullptr;
    Node* tail = nullptr;
    size_t sz = 0;
};

template <typename T>
LinkedDeque<T>::LinkedDeque(const LinkedDeque<T>& other)
{
    copy(other);
}

template <typename T>
LinkedDeque<T>& LinkedDeque<T>::operator=(const LinkedDeque<T>& other)
{
    if (this != &other) {
        free();
        copy(other);
    }
    return *this;
}

template <typename T>
LinkedDeque<T>::LinkedDeque(LinkedDeque<T>&& other) noexcept
{
    move(std::move(other));
}

template <typename T>
LinkedDeque<T>& LinkedDeque<T>::operator=(LinkedDeque<T>&& other) noexcept
{
    if (this != &other) {
        free();
        move(std::move(other));
    }
    return *this;
}

template <typename T>
LinkedDeque<T>::~LinkedDeque()
{
    free();
}

template <typename T>
void LinkedDeque<T>::copy(const LinkedDeque<T>& other)
{
    Node* current = other.head;
    while (current) {
        pushBack(current);
        current = current->next;
    }
    sz = other.sz;
}

template <typename T>
void LinkedDeque<T>::move(LinkedDeque<T>&& other)
{
    head = other.head;
    tail = other.tail;
    sz = other.sz;

    other.head = other.tail = nullptr;
    other.sz = 0;
}

template <typename T>
void LinkedDeque<T>::free()
{
    while (head)
        popFront();
}

template <typename T>
void LinkedDeque<T>::pushFront(const T& element)
{
    Node* newNode = new Node(element, nullptr, head);

    if (!tail) // empty()
        tail = newNode;
    else if (head == tail) // size() == 1
        tail->prev = newNode;

    if (head)
        head->prev = newNode;

    head = newNode;

    ++sz;
}

template <typename T>
void LinkedDeque<T>::pushBack(const T& element)
{
    Node* newNode = new Node(element, tail, nullptr);

    if (!head) // empty()
        head = newNode;
    else if (head == tail) // size() == 1
        head->next = newNode;

    if (tail)
        tail->next = newNode;

    tail = newNode;

    ++sz;
}

template <typename T>
void LinkedDeque<T>::popFront()
{
    if (empty())
        throw std::runtime_error("Cannot pop from empty deque.");

    Node* toDelete = head;

    head = head->next;
    delete toDelete;

    if (!head)
        tail = nullptr;
    else
        head->prev = nullptr;

    --sz;
}

template <typename T>
void LinkedDeque<T>::popBack()
{
    if (empty())
        throw std::runtime_error("Cannot pop from empty deque.");

    Node* toDelete = tail;

    tail = tail->prev;
    delete toDelete;

    if (!tail)
        head = nullptr;
    else
        tail->next = nullptr;

    --sz;
}

template <typename T>
const T& LinkedDeque<T>::front() const
{
    if (empty())
        throw std::runtime_error("Deque is empty.");
    return head->data;
}

template <typename T>
const T& LinkedDeque<T>::back() const
{
    if (empty())
        throw std::runtime_error("Deque is empty.");
    return tail->data;
}

template <typename T>
bool LinkedDeque<T>::empty() const
{
    return size() == 0;
}

template <typename T>
size_t LinkedDeque<T>::size() const
{
    return sz;
}

template <typename T>
void LinkedDeque<T>::clear()
{
    free(); // if free() weren't manage the size, it would have been nullified later explicitly
}

template <typename T>
typename LinkedDeque<T>::iterator LinkedDeque<T>::begin()
{
    return iterator(head);
}

template <typename T>
typename LinkedDeque<T>::iterator LinkedDeque<T>::end()
{
    return iterator(nullptr);
}

template <typename T>
typename LinkedDeque<T>::const_iterator LinkedDeque<T>::cbegin() const
{
    return const_iterator(head);
}

template <typename T>
typename LinkedDeque<T>::const_iterator LinkedDeque<T>::cend() const
{
    return const_iterator(nullptr);
}

template <typename T>
typename LinkedDeque<T>::reverse_iterator LinkedDeque<T>::rbegin()
{
    return reverse_iterator(tail);
}

template <typename T>
typename LinkedDeque<T>::reverse_iterator LinkedDeque<T>::rend()
{
    return reverse_iterator(nullptr);
}

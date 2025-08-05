#pragma once
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <utility>

template <typename T>
class DLL {
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
    DLL() { };

    DLL(const DLL& other) { copy(other); }
    DLL& operator=(const DLL& other)
    {
        if (this != &other) {
            free();
            copy(other);
        }
        return *this;
    }

    DLL(DLL&& other) noexcept { move(std::move(other)); }
    DLL& operator=(DLL&& other) noexcept
    {
        if (this != &other) {
            free();
            move(std::move(other));
        }
        return *this;
    }

    ~DLL() { free(); }

    class iterator;
    class const_iterator;
    class reverse_iterator;

    class iterator {
    private:
        Node* current = nullptr;
        friend class DLL;
        friend class const_iterator;
        friend class reverse_iterator;

    public:
        iterator(Node* node = nullptr)
            : current(node)
        {
        }

        iterator(const const_iterator& const_iter)
            : current(const_iter.current)
        {
        }

        iterator(const reverse_iterator& reverse_iter)
            : current(reverse_iter.current)
        {
        }

        T& operator*() { return current->data; }
        T* operator->() { return &current->data; }

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

        iterator& operator+=(int arg)
        {
            for (size_t i = 0; i < arg && current; ++i)
                current = current->next;
            return *this;
        }

        iterator operator+(int arg)
        {
            iterator temp(*this);
            return temp + arg;
        }

        iterator& operator-=(int arg)
        {
            for (size_t i = 0; i < arg && current; ++i)
                current = current->prev;
            return *this;
        }

        iterator operator-(int arg)
        {
            iterator temp(*this);
            return temp - arg;
        }

        bool operator==(const iterator& other)
        {
            return current == other.current;
        }

        bool operator!=(const iterator& other)
        {
            return !(*this == other);
        }
    };

    class const_iterator {
    private:
        Node* current = nullptr;
        friend class DLL;
        friend class iterator;
        friend class reverse_iterator;

    public:
        const_iterator(Node* node = nullptr)
            : current(node)
        {
        }

        const_iterator(const iterator& iter)
            : current(iter.current)
        {
        }

        const_iterator(const reverse_iterator& reverse_iter)
            : current(reverse_iter.current)
        {
        }

        const T& operator*() { return current->data; }
        const T* operator->() { return &current->data; }

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

        const_iterator& operator+=(int arg)
        {
            for (size_t i = 0; i < arg && current; ++i)
                current = current->next;
            return *this;
        }

        const_iterator operator+(int arg)
        {
            const_iterator temp(*this);
            return temp + arg;
        }

        const_iterator& operator-=(int arg)
        {
            for (size_t i = 0; i < arg && current; ++i)
                current = current->prev;
            return *this;
        }

        const_iterator operator-(int arg)
        {
            const_iterator temp(*this);
            return temp - arg;
        }

        bool operator==(const const_iterator& other)
        {
            return current == other.current;
        }

        bool operator!=(const const_iterator& other)
        {
            return !(*this == other);
        }
    };

    class reverse_iterator {
    private:
        Node* current = nullptr;
        friend class DLL;
        friend class iterator;
        friend class const_iterator;

    public:
        reverse_iterator(Node* node = nullptr)
            : current(node)
        {
        }

        reverse_iterator(const const_iterator& const_iter)
            : current(const_iter.current)
        {
        }

        reverse_iterator(const iterator& iter)
            : current(iter.current)
        {
        }

        T& operator*()
        {
            return current->data;
        }
        T* operator->() { return &current->data; }

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

        reverse_iterator& operator+=(int arg)
        {
            for (size_t i = 0; i < arg && current; ++i)
                current = current->prev;
            return *this;
        }

        reverse_iterator operator+(int arg)
        {
            reverse_iterator temp(*this);
            return temp + arg;
        }

        reverse_iterator& operator-=(int arg)
        {
            for (size_t i = 0; i < arg && current; ++i)
                current = current->next;
            return *this;
        }

        reverse_iterator operator-(int arg)
        {
            iterator temp(*this);
            return temp - arg;
        }

        bool operator==(const reverse_iterator& other)
        {
            return current == other.current;
        }

        bool operator!=(const reverse_iterator& other)
        {
            return !(*this == other);
        }
    };

    size_t getSize() const { return size; }
    bool empty() const { return size == 0; }

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }

    const_iterator cbegin() const { return const_iterator(head); }
    const_iterator cend() const { return const_iterator(nullptr); }

    reverse_iterator rbegin() { return reverse_iterator(tail); }
    reverse_iterator rend() { return reverse_iterator(nullptr); }

    T& first()
    {
        if (empty())
            throw std::runtime_error("List is empty.");
        return head->data;
    }

    const T& first() const
    {
        if (empty())
            throw std::runtime_error("List is empty.");
        return head->data;
    }

    T& last()
    {
        if (empty())
            throw std::runtime_error("List is empty.");
        return tail->data;
    }

    const T& last() const
    {
        if (empty())
            throw std::runtime_error("List is empty.");
        return tail->data;
    }

    iterator insertBefore(const T& data, iterator iter)
    {
        if (iter == end())
            return end();

        Node* newNode = new Node(data, iter.current->prev, iter.current);
        iter.current->prev = newNode;

        ++size;

        if (iter.current == head)
            head = newNode;
        else
            iter.current->prev->next = newNode;

        return iterator(newNode);
    }

    iterator insertAfter(const T& data, iterator iter)
    {
        if (iter == end())
            return end();

        Node* newNode = new Node(data, iter.current, iter.current->next);
        iter.current->next = newNode;

        ++size;

        if (iter.current == tail)
            tail = newNode;
        else
            iter.current->next->prev = newNode;

        return iterator(newNode);
    }

    iterator removeBefore(iterator iter)
    {
        if (iter == end() || size == 1)
            return end();

        Node* current = iter.current;
        Node* toDelete = current->prev;
        Node* newPrev = toDelete->prev;

        current->prev = newPrev;

        if (newPrev)
            newPrev->next = current;

        if (toDelete == head)
            head = current;

        delete toDelete;
        --size;

        return iterator(newPrev);
    }

    iterator removeAfter(iterator iter)
    {
        if (iter == end() || size == 1)
            return end();

        Node* current = iter.current;
        Node* toDelete = current->next;
        Node* newNext = toDelete->next;

        current->next = newNext;

        if (newNext)
            newNext->prev = current;

        if (toDelete == tail)
            tail = current;

        delete toDelete;
        --size;

        return iterator(newNext);
    }

    void pushFront(const T& data) { insertBefore(data, begin()); }
    void pushBack(const T& data) { insertAfter(data, iterator(tail)); }

    void popFront()
    {
        if (empty())
            throw std::runtime_error("Cannot pop from empty list");

        if (size > 1) {
            removeBefore(begin() + 1);
            return;
        }

        delete head;
        head = tail = nullptr;
        --size;
    }

    void popBack()
    {
        if (empty())
            throw std::runtime_error("Cannot pop from empty list");

        if (size > 1) {
            removeAfter(rbegin() + 1);
            return;
        }

        delete tail;
        head = tail = nullptr;
        --size;
    }

    void clear()
    {
        free();
        size = 0;
    }

    void print()
    {
        const_iterator iter = cbegin();
        while (iter != rbegin())
            std::cout << *(iter++) << " <-> ";
        std::cout << *(static_cast<const_iterator>(rbegin())) << std::endl;
    }

    void reverse()
    {
        Node* current = head;
        while (current) {
            std::swap(current->prev, current->next);
            current = current->prev;
        }
        std::swap(head, tail);
    }

    template <typename U>
    friend DLL<U> concat(const DLL<U>& lhs, const DLL<U> rhs);

private:
    void copy(const DLL& other)
    {
        const_iterator iter = other.cbegin();
        while (iter != other.cend())
            pushBack(*iter);
    }

    void free()
    {
        Node* current = head;
        while (current) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }

    void move(DLL&& other)
    {
        head = other.head;
        tail = other.tail;
        size = other.size;

        other.head = other.tail = nullptr;
        other.size = 0;
    }

    Node* head = nullptr;
    Node* tail = nullptr;
    size_t size = 0;
};

template <typename T>
DLL<T> concat(const DLL<T>& lhs, const DLL<T> rhs)
{
    DLL<T> result(lhs);

    typename DLL<T>::const_iterator iter = rhs.cbegin();
    while (iter != rhs.cend())
        result.pushBack(*(iter++));

    return result;
}

#pragma once
#include <iostream>
#include <stdexcept>

template <typename T>
class SinglyLinkedList {
private:
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
    SinglyLinkedList()
        : head(nullptr)
        , tail(nullptr)
        , size(0)
    {
    }

    SinglyLinkedList(const SinglyLinkedList<T>& other)
        : head(nullptr)
        , tail(nullptr)
        , size(0)
    {
        copy(other);
    }

    SinglyLinkedList<T>& operator=(const SinglyLinkedList<T>& other)
    {
        if (this != &other) {
            free();
            copy(other);
        }
        return this;
    }

    ~SinglyLinkedList() { free(); }

    SinglyLinkedList(SinglyLinkedList<T>&& other) noexcept
        : head(nullptr)
        , tail(nullptr)
        , size(0)
    {
        move(std::move(other));
    }

    SinglyLinkedList<T>& operator=(SinglyLinkedList<T>&& other) noexcept
    {
        if (this != &other) {
            free();
            move(std::move(other));
        }
        return this;
    }

    class iterator {
        Node* current;
        friend class SinglyLinkedList;

        template <typename U>
        friend SinglyLinkedList<U> concat(const SinglyLinkedList<U>& lhs, const SinglyLinkedList<U>& rhs);

    public:
        iterator(Node* node)
            : current(node)
        {
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
            ++(*this); // this->operator++();
            return temp;
        }

        iterator& operator+=(size_t it)
        {
            for (size_t i = 0; i < it; ++i)
                ++(*this);
            return *this;
        }

        iterator operator+(size_t it) const
        {
            iterator temp(*this);
            return temp += it;
        }

        T& operator*()
        {
            return current->data;
        }

        T* operator->()
        {
            return &current->data;
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
        Node* current;
        friend class SinglyLinkedList;

        template <typename U>
        friend SinglyLinkedList<U> concat(const SinglyLinkedList<U>& lhs, const SinglyLinkedList<U>& rhs);

    public:
        const_iterator(Node* node)
            : current(node)
        {
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
            ++(*this); // this->operator++();
            return temp;
        }

        const_iterator& operator+=(size_t it)
        {
            for (size_t i = 0; i < it; ++i)
                ++(*this);
            return *this;
        }

        const_iterator operator+(size_t it) const
        {
            const_iterator temp(*this);
            return temp += it;
        }

        const T& operator*() const
        {
            return current->data;
        }

        const T* operator->() const
        {
            return &current->data;
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

    iterator begin()
    {
        return iterator(head);
    }
    iterator end()
    {
        return iterator(nullptr);
    }

    const_iterator cbegin()
    {
        return const_iterator(head);
    }

    const_iterator cend()
    {
        return const_iterator(nullptr);
    }

    iterator insertAfter(const T& data, const_iterator& iter)
    {
        if (iter == cend())
            return end();

        Node* newNode = new Node(data);
        Node* current = iter.current;

        newNode->next = current->next;
        current->next = newNode;
        ++size;

        return iterator(newNode);
    }

    iterator removeAfter(const_iterator& iter)
    {
        if (iter == cend() || size == 1)
            return end();

        Node* current = iter.current;
        Node* toDelete = current->next;
        Node* newNext = toDelete->next;

        current->next = newNext;

        if (toDelete == tail)
            tail = current;

        delete toDelete;
        --size;

        return iterator(newNext);
    }

    template <typename U>
    friend SinglyLinkedList<U> concat(const SinglyLinkedList<U>& lhs, const SinglyLinkedList<U>& rhs);

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

    void print() const
    {
        Node* current = head;
        while (current)
            std::cout << current->data;
    }

    size_t getSize() const { return size; }
    bool empty() const { return size == 0; }

    void pushFront(const T& data)
    {
        Node* newNode = new Node(data, head);
        head = newNode;

        size++;

        if (!tail)
            tail = newNode;
    }

    void pushBack(const T& data)
    {
        Node* newNode = new Node(data);

        size++;

        if (!tail) {
            head = tail = newNode;
            return;
        }

        tail->next = newNode;
        tail = newNode;
    }

    void popFront()
    {
        if (empty())
            throw std::runtime_error("Cannot pop element from empty list.");

        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            Node* toDelete = head;
            head = head->next;
            delete toDelete;
        }

        --size;
    }

    void reverse()
    {
        Node* prev = nullptr;
        Node* current = head;
        Node* next = nullptr;

        while (current) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }

        std::swap(head, tail);
    }

private:
    void copy(const SinglyLinkedList<T>& other)
    {
        Node* current = other.head;
        while (current)
            pushBack(current->data);
    }

    void move(SinglyLinkedList<T>&& other)
    {
        head = other.head;
        tail = other.tail;
        size = other.size;

        other.head = other.tail = nullptr;
        other.size = 0;
    }

    void free()
    {
        Node* current = head;
        while (current) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }

        head = tail = nullptr;
        size = 0;
    }

    Node* head;
    Node* tail;
    size_t size;
};

template <typename T>
SinglyLinkedList<T> concat(const SinglyLinkedList<T>& first, const SinglyLinkedList<T>& second)
{
    SinglyLinkedList<T> result(first);

    typename SinglyLinkedList<T>::const_iterator secondIter = second.cbegin();

    while (secondIter != second.cend())
        result.pushBack(*(secondIter++));

    return result;
}

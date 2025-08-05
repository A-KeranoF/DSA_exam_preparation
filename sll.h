#pragma once
#include <exception>

template <typename T>
class SinglyLinkedList {
public:
    using iterator = sll_iterator;
    using const_iterator = sll_const_iterator;

    SinglyLinkedList();
    SinglyLinkedList(const SinglyLinkedList& other);
    SinglyLinkedList& operator=(const SinglyLinkedList& other);
    ~SinglyLinkedList();

    void pushFront(const T& data);
    void pushBack(const T& data);

    T popFront();
    T popBack(); // needs to be deprecated in sll

    size_t size() const { return _size; }

    bool empty() const { return _size == 0; }

    iterator begin() { return iterator(_first); }
    const_iterator cbegin() const { return const_iterator(_first); }

    T& front() { return _first->_data; }
    T& back() { return _last->_data; }
    const T& front() const { return _first->_data; }
    const T& back() const { return _last->_data; }

    void reverseIterative();
    void reverseRecursive();

    friend SinglyLinkedList<T> concat(const SinglyLinkedList& lhs, const SinglyLinkedList& rhs) const;

private:
    struct Node {
        T _data;
        Node* _next;

        Node(const T& data, Node* next = nullptr)
            : _data(data)
            , _next(next)
        {
        }

        Node(T&& _data_, Node* next = nullptr)
            : _data(std::move(data))
            , _next(next)
        {
        }
    };

    class sll_iterator {
    public:
        sll_iterator(Node* node)
            : _current(node)
        {
        }
        sll_iterator(sll_iterator& iter)
            : _current(iter._current)
        {
        }
        sll_iterator(sll_const_iterator& const_iter)
            : _current(iter._current)
        {
        }

        sll_iterator operator++()
        {
            if (_current) {
                _current = _current->_next;
            }
            return *this;
        }

        sll_iterator operator++(int)
        {
            sll_iterator it(*this);
            ++(*this);
            return it;
        }

        T& operator*()
        {
            return _current->_data;
        }

        T* operator->()
        {
            return &(_current->_data);
        }

        sll_iterator& operator+=(const size_t _i)
        {
            for (size_t i = 0; i < _i; ++i) {
                ++(*this);
            }
            return *this;
        }

        sll_iterator operator+(const size_t i) const
        {
            sll_iterator it(*this);
            it += i;
            return it;
        }

        bool operator==(const sll_iterator& iter) const
        {
            return _current == iter._current;
        }

        bool operator!=(const sll_iterator& iter) const
        {
            return !(*this == iter);
        }

        // bool operator==(const sll_const_iterator& iter) const; // doesn't it implicitly cast anyway?
        // bool operator!=(const sll_const_iterator& iter) const;

    private:
        Node* _current;
        // SinglyLinkedList<T> ownerList;

        friend class SinglyLinkedList<T>;
    }

    class sll_const_iterator {
    public:
        sll_const_iterator(Node* node)
            : _current(node)
        {
        }
        sll_const_iterator(sll_iterator& iter)
            : _current(iter._current)
        {
        }
        sll_const_iterator(sll_const_iterator& const_iter)
            : _current(iter._current)
        {
        }

        sll_const_iterator operator++()
        {
            if (_current) {
                _current = current->_next;
            }
            return *this;
        }

        sll_const_iterator operator++(int)
        {
            sll_iterator it(*this);
            ++(*this);
            return it;
        }

        const T& operator*() const
        {
            return _current->_data;
        }

        const T* operator->() const
        {
            return &(_current->_data);
        }

        sll_const_iterator& operator+=(const size_t _i)
        {
            for (size_t i = 0; i < _i; ++i) {
                ++(*this);
            }
            return *this;
        }

        sll_const_iterator operator+(const size_t i) const
        {
            sll_const_iterator it(*this);
            it += i;
            return it;
        }

        bool operator==(const sll_const_iterator& iter) const
        {
            return _current == iter._current;
        }

        bool operator!=(const sll_const_iterator& iter) const
        {
            return !(*this == iter);
        }

        // bool operator==(const sll_iterator& iter) const; // doesn't it implicitly cast anyway?
        // bool operator!=(const sll_iterator& iter) const;

    private:
        const Node* _current;
        // SinglyLinkedList<T> ownerList;

        friend class SinglyLinkedList<T>;
    }

    Node* _first;
    Node* _last;
    size_t _size;

    void copy(const SinglyLinkedList& other);
    void free();
    void move(SinglyLinkedList&& other);
    Node* reverseRecursiveHelper(Node* head);
};

template <typename T>
inline SinglyLinkedList<T>::SinglyLinkedList()
    : _first(nullptr)
    , _last(nullptr)
    , _size(0)
{
}

template <typename T>
inline SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList<T>& other)
    : _first(nullptr)
    , _last(nullptr)
    , _size(0)
{
    copy(other)
}

template <typename T>
inline SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(const SinglyLinkedList<T>& other)
{
    if (this != &other) {
        SinglyLinkedList temp(other);
        move(std::move(temp));
    }
    return *this;
}

template <typename T>
inline SinglyLinkedList<T>::~SinglyLinkedList()
{
    free();
}

template <typename T>
inline void SinglyLinkedList<T>::move(SinglyLinkedList<T>&& other)
{
    this->_first = other._first;
    this->_last = other._last;
    this->_size = other._size;

    other._first = other._last = nullptr;
    other._size = 0;
}

template <typename T>
inline void SinglyLinkedList<T>::free()
{
    Node* current = _first;
    while (current) {
        Node* toDelete = current;
        current = current->_next;
        delete toDelete;
    }
    _first = _last = nullptr;
    _size = 0;
}

template <typename T>
inline void SinglyLinkedList<T>::copy(const SinglyLinkedList& other)
{
    Node* current = other._first;
    while (current) {
        pushBack(current->_data);
        current = current->_data;
    }
}

template <typename T>
inline void SinglyLinkedList<T>::pushFront(const T& data)
{
    Node* newNode = new Node(data, _first);
    if (empty()) {
        _first = _last = newNode;
    } else {
        _first = newNode;
    }
    ++_size;
}

template <typename T>
inline void SinglyLinkedList<T>::pushBack(const T& data)
{
    Node* newNode = new Node(data);
    if (empty()) {
        _first = _last = newNode;
    } else {
        _last->_next = newNode;
        _last = newNode;
    }
    ++_size;
}

template <typename T>
inline T SinglyLinkedList<T>::popFront()
{
    if (empty()) {
        throw std::runtime_error("Cannot delete from empty list");
    }

    --_size;

    T _data = _first->_data;

    if (_first == _last) {
        delete _first;
        _first = _last = nullptr;
    } else {
        Node* toDelete = _first;
        _first = _first->_next;
        delete toDelete;
    }

    return _data;
}

template <typename T>
inline T SinglyLinkedList<T>::popBack()
{
    if (empty()) {
        throw std::runtime_error("Cannot delete from empty list");
    }

    --_size;

    T _data = _last->_data;

    if (_first == _last) {
        delete _last;
        _first = _last = nullptr;
    } else {
        Node* toDelete = _last;

        Node* current = _first;
        while (current->_next != _last) // O(n), this method doesnt need to be implemented in SLL but i still did
        {
            current = current->_next;
        }
        _last = current;

        delete toDelete;
    }

    return _data;
}

template <typename T>
inline SinglyLinkedList<T> SinglyLinkedList<T>::concat(const SinglyLinkedList& lhs, const SinglyLinkedList& rhs) const
{
    SinglyLinkedList<T> result(lhs);

    Node* iter = rhs._first;

    while (iter) {
        result.pushBack(iter->_data);
        iter = iter->_next;
    }

    return result;
}

template <typename T>
inline void SinglyLinkedList<T>::reverseIterative()
{
    Node* prev = nullptr;
    Node* current = _first;
    Node* next = nullptr;

    while (current) {
        next = current->_next;
        current->_next = prev;
        prev = current;
        current = next;
    }
}

template <typename T>
inline void SinglyLinkedList<T>::reverseRecursive()
{
    reverseRecursiveHelper(_first);
}

template <typename T>
inline Node* SinglyLinkedList<T>::reverseRecursiveHelper()
{
    if (!head || !head->_next) {
        return head;
    }

    Node* tail = reverseRecursiveHelper(head->_next);
    head->_next->_next = head;
    head->_next = nullptr;
    return tail;
}

#pragma once
#include "LinkedDeque.h"
#include <exception>

#pragma region ITERATOR

template <typename T>
class LinkedDequeIterator {
public:
    LinkedDequeIterator(LinkedDeque<T>* owner, LinkedDeque<T>::Node* node)
        : owner(owner)
        , current(node)
    {
        if (!owner || node)
            throw std::invalid_argument("Node is not related to null list");
    }

    T& operator*() const;
    T* operator->() const;

    LinkedDequeIterator& operator++();
    LinkedDequeIterator operator++(int);
    LinkedDequeIterator& operator--();
    LinkedDequeIterator operator--(int);

    bool operator==(const LinkedDequeIterator& rhs) const;
    bool operator!=(const LinkedDequeIterator& rhs) const;

private:
    friend class LinkedDeque;
    LinkedDeque<T>* owner = nullptr;
    LinkedDeque<T>::Node* current = nullptr;
};

template <typename T>
inline T& LinkedDequeIterator<T>::operator*() const
{
    return current->data;
}

template <typename T>
inline T* LinkedDequeIterator<T>::operator->() const
{
    return &current->data;
}

template <typename T>
inline LinkedDequeIterator<T>& LinkedDequeIterator<T>::operator++()
{
    if (!current)
        ;
    else if (current == owner->last)
        current = owner->first;
    else
        current = current->next;

    return *this;
}

template <typename T>
inline LinkedDequeIterator<T>& LinkedDequeIterator<T>::operator++(int)
{
    LinkedDequeIterator<T> temp(*this);
    ++(*this);
    return temp;
}

template <typename T>
inline LinkedDequeIterator<T>& LinkedDequeIterator<T>::operator--()
{
    if (!current)
        ;
    else if (current == owner->first)
        current = owner->last;
    else
        current = current->prev;

    return *this;
}

template <typename T>
inline LinkedDequeIterator<T>& LinkedDequeIterator<T>::operator--(int)
{
    LinkedDequeIterator<T> temp(*this);
    --(*this);
    return temp;
}

template <typename T>
inline bool LinkedDequeIterator<T>::operator==(const LinkedDequeIterator& rhs) const
{
    return current == rhs.current;
}

template <typename T>
inline bool LinkedDequeIterator<T>::operator!=(const LinkedDequeIterator& rhs) const
{
    return !(*this == rhs)
}

#pragma endregion

#pragma region CONST_ITERATOR

template <typename T>
class ConstLinkedDequeIterator {
public:
    ConstLinkedDequeIterator(LinkedDeque<T>* owner, LinkedDeque<T>::Node* node)
        : owner(owner)
        , current(node)
    {
        if (!owner || node)
            throw std::invalid_argument("Node is not related to null list");
    }

    const T& operator*() const;
    const T* operator->() const;

    ConstLinkedDequeIterator& operator++();
    ConstLinkedDequeIterator operator++(int);
    ConstLinkedDequeIterator& operator--();
    ConstLinkedDequeIterator operator--(int);

    bool operator==(const ConstLinkedDequeIterator& rhs) const;
    bool operator!=(const ConstLinkedDequeIterator& rhs) const;

private:
    friend class LinkedDeque;
    const LinkedDeque<T>* owner = nullptr;
    const LinkedDeque<T>::Node* current = nullptr;
};

template <typename T>
inline const T& ConstLinkedDequeIterator<T>::operator*() const
{
    return current->data;
}

template <typename T>
inline const T* ConstLinkedDequeIterator<T>::operator->() const
{
    return &current->data;
}

template <typename T>
inline ConstLinkedDequeIterator<T>& ConstLinkedDequeIterator<T>::operator++()
{
    if (!current)
        ;
    else if (current == owner->last)
        current = owner->first;
    else
        current = current->next;

    return *this;
}

template <typename T>
inline ConstLinkedDequeIterator<T>& ConstLinkedDequeIterator<T>::operator++(int)
{
    ConstLinkedDequeIterator<T> temp(*this);
    ++(*this);
    return temp;
}

template <typename T>
inline ConstLinkedDequeIterator<T>& ConstLinkedDequeIterator<T>::operator--()
{
    if (!current)
        ;
    else if (current == owner->first)
        current = owner->last;
    else
        current = current->prev;

    return *this;
}

template <typename T>
inline ConstLinkedDequeIterator<T>& ConstLinkedDequeIterator<T>::operator--(int)
{
    ConstLinkedDequeIterator<T> temp(*this);
    --(*this);
    return temp;
}

template <typename T>
inline bool ConstLinkedDequeIterator<T>::operator==(const ConstLinkedDequeIterator& rhs) const
{
    return current == rhs.current;
}

template <typename T>
inline bool ConstLinkedDequeIterator<T>::operator!=(const ConstLinkedDequeIterator& rhs) const
{
    return !(*this == rhs)
}

#pragma endregion

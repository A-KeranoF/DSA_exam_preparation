#pragma once

#include <utility>

template <typename T, typename Comparator = std::less<T>>
class TreeSet {
    struct Node {
        T data;
        Node* left;
        Node* right;

        Node(const T& data, Node* left = nullptr, Node* right = nullptr)
            : data(data)
            , left(left)
            , right(right)
        {
        }

        Node(T&& data, Node* left = nullptr, Node* right = nullptr)
            : data(std::move(data))
            , left(left)
            , right(right)
        {
        }
    };

public:
    TreeSet();

    TreeSet(const TreeSet& other);
    TreeSet& operator=(const TreeSet& other);

    TreeSet(TreeSet&& other) noexcept;
    TreeSet& operator=(TreeSet&& other) noexcept;

    ~TreeSet();

    bool insert(const T& element);
    bool insert(T&& element);

    bool contains(const T& element) const;

    bool remove(const T& element);

    void clear();

    size_t size() const;
    bool empty() const;

private:
    Node* copy(Node* node);
    void free(Node* node);
    void move(const TreeSet& other);

    Node** getMinNode(Node** root);

    Node** findTargetToDelete(const T& element);

    Comparator compare;

    Node* root;
    size_t sz;
};

template <typename T, typename Comparator>
TreeSet<T, Comparator>::TreeSet()
    : root(nullptr)
    , sz(0)
{
}

template <typename T>
TreeSet<T, Comparator>::TreeSet(const TreeSet<T, Comparator>& other)
    : root(copy(other.root))
    , sz(other.sz)
{
}

template <typename T, typename Comparator>
TreeSet<T, Comparator>& TreeSet<T, Comparator>::operator=(const TreeSet<T, Comparator>& other)
{
    if (this != &other) {
        free(root);
        root = copy(other.root);
        sz = other.sz;
    }
    return *this;
}

template <typename T>
TreeSet<T, Comparator>::TreeSet(TreeSet<T, Comparator>&& other)
    : root(nullptr)
    , sz(0)
{
    move(std::move(other));
}

template <typename T>
TreeSet<T, Comparator>& TreeSet<T, Comparator>::operator=(TreeSet<T, Comparator>&& other) noexcept
{
    if (this != &other) {
        free();
        move(std::move(other));
    }
    return *this;
}

template <typename T>
TreeSet<T, Comparator>::~TreeSet()
{
    free(root);
}

template <typename T>
typename TreeSet<T, Comparator>::Node* TreeSet<T, Comparator>::copy(Node* node)
{
    if (!node)
        return nullptr;

    Node* newNode = new Node(node->data);
    newNode->left = copy(node->left);
    newNode->right = copy(node->right);
    return newNode;
}

template <typename T>
void TreeSet<T, Comparator>::move(TreeSet<T, Comparator>&& other) noexcept
{
    this->root = other.root;
    this->sz = other.sz;

    other.root = nullptr;
    other.sz = 0;
}

template <typename T>
void TreeSet<T, Comparator>::free(Node* node)
{
    if (!node)
        return;

    free(node->left);
    free(node->right);
    delete node;
}

template <typename T>
void TreeSet<T, Comparator>::clear()
{
    free(root);
    root = nullptr;
    sz = 0;
}

template <typename T>
size_t TreeSet<T, Comparator>::size() const
{
    return sz;
}

template <typename T>
bool TreeSet<T, Comparator>::empty() const
{
    return size() == 0;
}

template <typename T>
bool TreeSet<T, Comparator>::insert(const T& element)
{
    Node** current = findTarget(element);

    if (*current != nullptr)
        return false;

    *current = new Node(element);
    ++sz;
    return true;
}

template <typename T>
bool TreeSet<T, Comparator>::insert(T&& element)
{
    Node** current = findTarget(element);

    if (*current != nullptr)
        return false;

    *current = new Node(std::move(element));
    ++sz;
    return true;
}

template <typename T>
bool TreeSet<T, Comparator>::contains(const T& element) const
{
    return findTarget(element) != nullptr;
}

template <typename T>
bool TreeSet<T, Comparator>::remove(const T& element)
{
    Node** current = findTarget(element);

    if (!*current)
        return false;

    Node* toDelete = *current;

    if (!*current->left == !*current->right)
        *current = nullptr;
    else if (!*current->left)
        *current = *current->right;
    else if (!*current->right)
        *current = *current->left;
    else {
        Node** replaceNode = getMinNode(*current->right);

        *current = *replaceNode;
        *replaceNode = *replaceNode->right;

        *current->left = *toDelete->left;
        *current->right = *toDelete->right;
    }

    delete toDelete;
    --sz;
    return true;
}

template <typename T>
typename TreeSet<T, Comparator>::Node** TreeSet<T, Comparator>::getMinNode(Node* root);
{
    if (!root)
        return nullptr;

    Node** current = &root;
    while (*current->left)
        current = *current->left;

    return current;
}

template <typename T>
typename TreeSet<T, Comparator>::Node** TreeSet<T, Comparator>::findTarget(const T& element)
{
    Node** current = &root;

    while (*current) {
        if (compare(element, *current->data))
            current = *current->left;

        else if (compare(*current->data, element))
            current = *current->right;

        else
            break;
    }

    return current;
}

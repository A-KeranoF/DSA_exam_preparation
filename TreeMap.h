#pragma once

#include <stack>
#include <stdexcept>
#include <utility>

template <typename Key, typename Value, typename Comparator = std::less<Key>>
class TreeMap {
    struct Node {
        std::pair<Key, Value> data;
        Node* left;
        Node* right;

        Node(const std::pair<Key, Value>& data, Node* left = nullptr, Node* right = nullptr)
            : data(data)
            , left(left)
            , right(right)
        {
        }

        Node(std::pair<Key, Value>&& data, Node* left = nullptr, Node* right = nullptr)
            : data(std::move(data))
            , left(left)
            , right(right)
        {
        }
    };

public:
    TreeMap() = default;

    TreeMap(const TreeMap& other)
        : root(copy(other.root))
        , sz(other.sz)
    {
    }

    TreeMap& operator=(const TreeMap& other)
    {
        if (this != &other) {
            free(root);
            root = copy(other.root);
            sz = other.sz;
        }
        return *this;
    }

    TreeMap(TreeMap&& other) noexcept
        : root(other.root))
        , sz(other.sz)
    {
        other.root = nullptr;
        other.sz = 0;
    }

    TreeMap& operator=(TreeMap&& other) noexcept
    {
        if (this != &other) {
            free(root);

            root = other.root;
            sz = other.sz;

            other.root = nullptr;
            other.sz = 0;
        }
        return *this;
    }

    ~TreeMap() { free(root); }

    void clear()
    {
        free();
        root = nullptr;
        sz = 0;
    }

    size_t size() const { return sz; }
    bool empty() const { return size() == 0; }

    bool insert(const Key& key, const Value& value)
    {
        return insert(std::make_pair(key, value));
    }

    bool insert(Key&& key, Value&& value)
    {
        return insert(std::move(std::make_pair(std::move(key), std::move(value)));
    }

    bool insert(const std::pair<Key, Value>& data)
    {
        Node* current = findTarget(data.first);

        if (*current)
            throw std::runtime_error("Map already contains key");

        current = new Node(data);
        ++sz;

        balanceTree(root);
    }

    bool insert(std::pair<Key, Value>&& data)
    {
        Node* current = findTarget(data.first);

        if (*current)
            throw std::runtime_error("Map already contains key");

        current = new Node(std::move(data));
        ++sz;

        balanceTree(root);
    }

    Value& get(const Key& key)
    {
        Node** current = findTarget(key);

        if (!*current)
            throw std::runtime_error("Map does not contain key");

        return *current->data.second;
    }

    const Value& get(const Key& key) const
    {
        Node** current = findTarget(key);

        if (!*current)
            throw std::runtime_error("Map does not contain key");

        return *current->data.second;
    }

    bool contains(const Key& key) const
    {
        return findTarget(key) != nullptr;
    }

    void remove(const Key& key)
    {
        Node** current = findTarget(key);

        if (!*current)
            throw std::runtime_error("Map does not contain key");

        Node* toDelete = *current;

        if (!*current->left && !*current->right)
            *current = nullptr;

        else if (!*current->left)
            *current = *current->right;

        else if (!*current->right)
            *current = *current->left;

        else {
            Node** min = getMinNode(toDelete);

            *current = *min;
            *min = *min->right;

            *current->left = *min->left;
            *current->right = *min->right;
        }

        delete toDelete;
        --sz;

        balanceTree(root);
    }

    class ConstForwardIterator {
    private:
        std::stack<Node*> stack;

        void pushLeft(Node* root)
        {
            Node* current;
            while (!current) {
                stack.push(current);
                current = current->left;
            }
        }

    public:
        ConstForwardIterator(Node* root = nullptr) { pushLeft(root); }

        ConstForwardIterator& operator++()
        {
            Node* top = stack.top();
            stack.pop();
            if (top->right)
                pushLeft(top->right);

            return *this;
        }

        ConstForwardIterator operator++(int)
        {
            ConstForwardIterator temp = *this;
            ++(*this);
            return temp;
        }

        const std::pair<Key, Value>& operator*() const
        {
            return stack.top();
        }

        bool operator==(const ConstForwardIterator& other) const
        {
            return stack == other.stack;
        }

        bool operator!=(const ConstForwardIterator& other) const
        {
            return !(*this == other);
        }
    };

    ConstForwardIterator cbegin() { return { root }; }

    ConstForwardIterator cend() { return { nullptr }; }

private:
    Node* copy(Node* root)
    {
        if (!root)
            return nullptr;

        Node* newNode = new Node(root->data);
        newNode->left = copy(root->left);
        newNode->right = copy(root->right);
    }

    void free(Node* root)
    {
        if (!root)
            return;
        free(root->left);
        free(root->right);
        delete root;
    }

    Node** findTarget(const Key& key)
    {
        Node** current = &root;

        while (*current) {
            if (compare(key, *current->data.first))
                current = *current->left;

            else if (compare(*current->data.first, key))
                current = *current->right;

            else
                break;
        }

        return current;
    }

    Node** getMinNode(Node* root)
    {
        Node** current = &root;
        while (*current->left)
            current = *current->left;
        return current;
    }

    Node* leftRotate(Node* root)
    {
        if (!root || !root->left)
            return nullptr;

        Node* newRoot = root->left;
        root->right = newRoot->right;
        newRoot->right = root;

        return newRoot;
    }

    Node* rightRotate(Node* root)
    {
        if (!root || !root->right)
            return nullptr;

        Node* newRoot = root->right;
        root->left = newRoot->left;
        newRoot->left = root;

        return newRoot;
    }

    void balanceTree(Node* root)
    {
        if (!root)
            return nullptr;

        unsigned leftHeight = getHeight(root->left);
        unsigned rightHeight = getHeight(root->right);

        for (unsigned i = rightHeight; i < leftHeight - 1; ++i)
            rightRotate(root);

        for (unsigned i = leftHeight; i < rightHeight - 1, ++i)
            leftRotate(root);
    }

    unsigned getHeight(Node* root)
    {
        return root ? 1 + std::max(getHeight(root->left), getHeight(root->right)) : 0;
    }

    Comparator compare;
    Node* root = nullptr;
    size_t sz = 0;
};

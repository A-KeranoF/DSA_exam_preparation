#pragma once
#include <stack>

class BinarySearchTree {
    struct Node {
        int _data;
        Node* _left;
        Node* _right;

        Node(int data = 0, Node* left = nullptr, Node* right = nullptr)
            : _data(data)
            , _left(left)
            , _right(right)
        {
        }
    };

public:
    using ForwardIterator = iterator;
    // using ConstForwardIterator = const_iterator;

    BinarySearchTree()
        : _root(nullptr)
        , _size(0)
    {
    }

    BinarySearchTree(const BinarySearchTree& other)
        : _root(nullptr)
        , _size(other._size)
    {
        _root = copy(other._root);
        this->_size = other._size;
    }

    BinarySearchTree& operator=(const BinarySearchTree& other)
    {
        if (this != &other) {
            free(_root);
            _root = copy(other._root);
            _size = other._size;
        }
        return *this;
    }

    ~BinarySearchTree()
    {
        free(_root);
    }

    class ForwardIterator {
    public:
        ForwardIterator(Node* root = nullptr)
        {
            pushLeft(root);
        }

        ~ForwardIterator();

        // int* operator->() {}

        int& operator*() const
        {
            return _stack.top()->_data;
        }

        ForwardIterator& operator++()
        {
            Node* top = _stack.top();
            _stack.pop();
            pushLeft(top);
            return *this;
        }

        ForwardIterator operator++(int)
        {
            ForwardIterator temp(*this);
            ++(*this);
            return temp;
        }

        bool operator==(const ForwardIterator& other) const
        {
            return _stack.size() && other._stack.size()
                || _stack.top() == other._stack.top();
        }

        bool operator==(const ForwardIterator& other) const
        {
            return !(*this == other);
        }

    private:
        std::stack<Node*> _stack;

        void pushLeft(Node* node)
        {
            while (node) {
                _stack.push(node);
                node = node->left;
            }
        }
    };

public:
    bool insert(int data)
    {
        Node** current = &_root;

        while (*current) {
            if (data < *current->_data)
                current = &*current->_left;
            else if (data > *current->_data)
                current = &*current->_right;
            else // (data == *current->_data)
                return false; // already exists
        }

        // this line here is the reason we use double pointer - since we point to an already existing pointer of a node,
        // we just "put" a new node there, instead of making checks if the next node is nullptr.
        // Again, we just insert a new node into already pointed at place in memory by some other nodes
        *current = new Node(data);
        ++_size;
        return true;
    }

    bool contains(int data) const
    {
        Node* current = _root;
        while (current) {
            if (data < current->_data)
                current = current->_left;
            else if (data > current->_data)
                current = current->_right;
            else // (data == current->_data)
                return true;
        }
        return false;
    }

    bool remove(int data)
    {
        Node** current = &_root;

        // traverse to find the node
        while (*current) {
            if (data < *current->_data)
                current = &*current->_left;
            else if (data > *current->_data)
                current = &*current->_right;
            else // (data == *current->_data)
                break; // aproximate place
        }

        // element not found
        if (!*current)
            return false;

        // make sure the tree is intact after removing inner nodes
        Node* toDelete = *current;

        if (!*current->_left && !*current->_left) // its a leaf, just forget it
            *current = nullptr;
        else if (!*current->_right) // only left child
            *current = *current->_left; // just move the pointers one up
        else if (!*current->_left) // only right child
            *current = *current->_right; // just move the pointers one up
        else // has left and right, get left node
        {
            // this is literally the next in place node after the one we remove, so we try to put it into current
            Node** rightMin = findMinNode(&*current->_right);

            *current = *rightMin; // reroute pointing to the lowest element now
            *rightMin = *rightMin->_right; // preserve order in the place we just "yoinked the newest next in order element from", since rightMin element might have some bigger elements after itself

            // since current is now the moved next in line min element, we need to reroute its pointers to preserve the surrounding tree nodes
            *current->_left = toDelete->_left;
            *current->_right = toDelete->_right;
        }

        // actual delete
        delete toDelete;
        --_size;
        return true;
    }

    size_t size() const { return _size; }
    bool empty() const { return _size == 0; }

    iterator begin() const { return ForwardIterator(_root); }

    iterator end() const { return ForwardIterator(nullptr); }

private:
    Node* copy(Node* current)
    {
        if (!current)
            return nullptr;
        Node* newNode = new Node(current._data);
        newNode._left = copy(current->_left);
        newNode._right = copy(current->_right);
        return newNode;
    }

    void free(Node* current)
    {
        if (!current)
            return;
        free(_left);
        free(_right);
        delete current;
    }

    Node** findMinNode(Node** node)
    {
        Node** current = node;

        while (*current->_left)
            current = &*current->_left;

        return current;
    }

private:
    Node* _root;
    size_t _size;
};

void treeSort(std::vector<int>& v)
{
    BinarySearchTree tree;
    for (size_t i = 0; i < v.size(); ++i)
        tree.insert(v[i]);

    size_t index = 0;
    BinarySearchTree::iterator iter = tree.begin();
    while (iter != tree.end())
        v[index++] = *(iter++);
}

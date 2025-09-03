#include <exception>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>

template <typename T>
struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int data = 0, Node* left = nullptr, Node* right = nullptr)
        : data(data)
        , left(left)
        , right(right)
    {
    }
};

template <typename T>
void printTree(Node* root)
{
    if (!root)
        return;

    // in-order
    printTree(root->left);
    std::cout << root->data;
    printTree(root->right);
}

template <typename T>
void printTreeBranchHelper(Node* root, std::vector<T>& branch)
{
    if (!root)
        return;

    branch.push_back(root->data);

    if (!root->left && !root->right) // => root is leaf
    {
        for (auto element : branch) // print branch
            std::cout << element << std::endl;
    } //
    else // not a leaf
    {
        if (root->left)
            printTreeBranchHelper(root->left);

        if (root->right)
            printTreeBranchHelper(root->right);
    }

    branch.pop_back();
}

template <typename T>
void printTreeBranch(Node* root)
{
    if (!root)
        return;

    std::vector<Node*> branch;
    printTreeBranchHelper(root, branch);
}

template <typename T>
T& getMaxInTree(Node* root)
{
    if (!root)
        throw std::logic_error("Cannot get max element from empty tree.");

    T& element = root->data;

    if (root->left) {
        T& left = getMaxInTree(root->left);
        element = std::max(element, left);
    }

    if (root->right) {
        T& right = getMaxInTree(root->right);
        element = std::max(element, right);
    }

    return element;
}

template <typename T>
size_t getTreeHeight(Node* root)
{
    if (!root)
        return 0;

    size_t leftHeight = getTreeHeight(root->left);
    size_t rightHeight = getTreeHeight(root->right);

    return 1 + std::max(leftHeight, rightHeight);
}

template <typename T>
void traverseTreeInOrder(Node* root)
{
    if (!root)
        return;

    traverseTreeInOrder(root->left);
    std::cout << root->data << ", ";
    traverseTreeInOrder(root->right);
}

template <typename T>
bool contains(Node* root, const T& element)
{
    if (!root)
        return false;

    return root->data == element
        || contains(root->left)
        || contains(root->right);
}

template <typename T>
T sumTotalOfTree(Node* root)
{
    if (!root)
        throw std::logic_error("Cannot sum empty tree");

    T sum = root->data;

    if (root->left)
        sum += sumTotalOfTree(root->left);

    if (root->right)
        sum += sumTotalOfTree(root->left);

    return sum;
}

template <typename T>
size_t getTreeSize(Node* root)
{
    if (!root)
        return 0;
    return 1 + getTreeSize(root->left) + getTreeSize(root->right);
}

template <>
bool checkIfTreeIsSetFromOneToNHelper(Node<size_t>* root, std::vector<bool>& visited)
{
    if (!root)
        return true;

    // it is not within bounds
    if (root->data < 1 || root->data > visited.size())
        return false;

    // check for duplicates
    if (visited[root->data - 1])
        return false;

    visited[root->data - 1] = true;

    return checkIfTreeIsSetFromOneToNHelper(root->left, visited) && checkIfTreeIsSetFromOneToNHelper(root->right, visited);
}

template <>
bool checkIfTreeIsSetFromOneToN(Node<size_t>* root)
{
    if (!root)
        return true;

    std::vector<bool> visited(getTreeSize(root));

    return checkIfTreeIsSetFromOneToNHelper(root, visited);
}

template <>
void printWordsFromRootToEveryLeafHelper(Node<char>* root, std::string& branch)
{
    if (!root)
        return;

    branch.push_back(root->data);

    if (!root->left && !root->right)
        std::cout << branch << std::endl;

    if (!root->left)
        printWordsFromRootToEveryLeafHelper(root->right, branch);

    if (!root->right)
        printWordsFromRootToEveryLeafHelper(root->left, branch);

    branch.pop_back();
}

template <>
void printWordsFromRootToEveryLeaf(Node<char>* root)
{
    if (!root)
        return;

    std::string branch;
    printWordsFromRootToEveryLeafHelper(root, branch);
}

template <>
void printWordInKLevelOfTrie(Node<char>* root, size_t k)
{
    if (!root)
        return;

    if (k == 0)
        throw std::logic_error("Cannot print leaves from level 0");

    if (k == 1) {
        std::cout << root->data;
        return;
    }

    printWordInKLevelOfTrie(root->left, k - 1);
    printWordInKLevelOfTrie(root->right, k - 1);

    std::cout << std::flush;
}

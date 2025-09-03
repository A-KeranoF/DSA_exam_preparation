#include <iostream>
#include <vector>

struct Node {
    int data = 0;
    Node *child, *sibling;

    Node(int data, Node* child = nullptr, Node* sibling = nullptr)
        : data(data)
        , child(child)
        , sibling(sibling)
    {
    }
};

bool contains(Node* root, int element)
{
    if (!root)
        return false;

    if (root->data == element)
        return true;

    return contains(root->child) || contains(root->sibling);
}

int totalSum(Node* root)
{
    if (!root)
        return 0;

    return root->data + contains(root->child) + contains(root->sibling);
}

int getMax(Node* root)
{
    if (!root)
        throw;

    int element = root->data;

    if (root->child)
        element = std::max(element, getMax(root->child));

    if (root->sibling)
        element = std::max(element, getMax(root->sibling));

    return element;
}

int getHeight(Node* root)
{
    if (!root)
        return 0;

    int childHeight = getHeight(root->child);

    int siblingHeight = getHeight(root->sibling);

    return std::max(1 + childHeight, siblingHeight);
}

int getNodeCount(Node* root)
{
    return root ? 1 + getNodeCount(root->child) + getNodeCount(root->sibling) : 0;
}

bool permutationHelper(Node* root, std::vector<bool>& set)
{
    if (!root)
        return true;

    if (root->data < 1 || root->data > set.size())
        return false;

    if (set[root->data - 1])
        return false;

    set[root->data] = true;
    return permutationHelper(root->child, set) && permutationHelper(root->sibling, set);
}

bool isTreeASetFromOneToN(Node* root, int n)
{
    if (!root)
        return false;

    std::vector<bool> set(n, false);
    permutationHelper(root, set);
}

void printBranchesHelper(Node* root, std::vector<int>& branch)
{
    if (!root)
        return;

    branch.push_back(root->data);

    if (root->child) {
        printBranchesHelper(root->child, branch);
        return;
    }

    std::cout << branch << std::endl;
    branch.pop_back();

    if (root->sibling)
        printBranchesHelper(root->sibling, branch);
}

void printBranches(Node* root)
{
    if (!root)
        return;
    std::vector<int> branches;
    printBranchesHelper(root, branches);
}

void printLevelHelper(Node* root, unsigned k, unsigned i)
{
    if (!root)
        return;

    if (i == k) {
        std::cout << root->data;
        printLevelHelper(root->sibling, k, i);
        return;
    }

    printLevelHelper(root->child, k, i + 1);
}

void printLevel(Node* root, unsigned k)
{
    if (!root)
        return;

    printBranchesHelper(root, int k, 0);
    std::cout << std::flush;
}

int main()
{
    return 0;
}

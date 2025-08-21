
struct BST_Node {
    int data;
    BST_Node* left;
    BST_Node* right;

    BST_Node(const T& data, BST_Node* left = nullptr, BST_Node* right = nullptr)
        : data(data)
        , left(left)
        , right(right)
    {
    }
};

size_t getTreeHeight(BST_Node* root)
{
    return root ? 1 + std::max(getTreeHeight(root->left), getTreeHeight(root->right)) : 0;
}

BST_Node* rightRotate(BST_Node* root)
{
    if (!root->left)
        return nullptr;

    Node* newRoot = root->left; // remember the Y (it is on the left)
    root->left = newRoot->right; // x->left gets the T2 subtree
    newRoot->right = root; // y->right grabs the x => y becomes the new root

    return newRoot;
}

BST_Node* rightRotate(BST_Node* root)
{
    if (!root->right)
        return nullptr;

    Node* newRoot = root->right; // remember the Y (it is on the right)
    root->right = newRoot->left; // x->right gets the T2 subtree
    newRoot->left = root; // y->left grabs the x => y becomes the new root

    return newRoot;
}

BST_Node* balanceBST(BST_Node* root)
{
    if (!root)
        return;

    size_t leftHeight = getTreeHeight(root->left);
    size_t rightHeight = getTreeHeight(root->right);

    BST_Node* newRoot = nullptr;

    for (size_t i = leftHeight; i > rightHeight + 1; --i)
        newNode = rightRotate(root);

    for (size_t i = rightHeight; i > leftHeight + 1; --i)
        newNode = leftRotate(root);

    return newNode;
}

BST_Node* insertIntoBST(Node* root, int element)
{
    if (!root) {
        root = new BST_Node(element);
        return root;
    } else {
        BST_Node** current = &root;

        while (*current) {
            if (element < *current->data)
                current = &*current->left;
            else
                current = &*current->right;
        }

        *current = new BST_Node(element);
        return balanceBST(*current); // ??
    }
}

BST_Node* level_sort(BST_Node* root)
{
    if (!root)
        return nullptr;

    BST_Node* newTree = insertIntoBST(root->data);
    newTree = balanceBST(newTree);

    level_sort(root->left);
    level_sort(root->right);

    return newRoot;
}

BST_Node* build_BST_from_sorted_array_helper(const vector<int>& arr, size_t first, size_t second)
{
    if (first > second)
        return nullptr;

    size_t middle = first + (second - first) / 2;

    BST_Node* newNode = new BST_Node(arr[middle]);

    newNode->left = build_BST_from_sorted_array_helper(arr, first, middleIndex - 1);
    newNode->right = build_BST_from_sorted_array_helper(arr + first, middle + 1, second);

    return newNode;
}

BST_Node* build_BST_from_sorted_array(const vector<int>& arr)
{
    return build_BST_from_sorted_array_helper(arr, 0, arr.size() - 1);
}

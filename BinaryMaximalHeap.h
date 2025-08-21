#pragma once

#include <stdexcept>
#include <vector>

template <typename T>
class BinaryMaximalHeap {
public:
    void clear();

    size_t size() const;
    bool empty() const;

    const T& top() const { return data[0]; }

    void pop()
    {
        if (empty())
            throw std::runtime_error("Cannot pop from empty heap.");

        data[0] = data[size() - 1];
        data.pop_back();

        // the if clause is just useless optimization to not calculate
        // indexes for nothing when vector-tree is empty
        // the heapify is what matters
        // if (!empty())
        heapify(0);
    }

    void insert(const T& element)
    {
        data.push_back(element);

        int index = data.size() - 1;
        int parentIndex = parent(index);
        while (index > 0
            && data[index] > data[parentIndex]) // sift up
        {
            std::swap(data[index], data[parentIndex]); // actual sift

            // start sifting elements furture atop of the branch
            index = parentIndex; // go up
            parentIndex = parent(parentIndex); // go up
        }
    }

private:
    static size_t leftChild(size_t rootIndex) { return rootIndex * 2 + 1; }
    static size_t rightChild(size_t rootIndex) { return rootIndex * 2 + 2; }
    static size_t parent(size_t childIndex) { return (childIndex - 1) / 2; }

    void heapify(size_t startingIndex)
    {
        size_t current = startingIndex;

        size_t leftIndex = leftChild(current);
        size_t rightIndex = rightChild(current);

        bool should_go_left = leftIndex < size()
            && data[leftIndex] > data[current];

        bool should_go_right = rightIndex < size()
            && data[rightIndex] > data[current];

        // swap(...) is actual sift

        // current = ...-Index along with the while loop is making
        // the process of sifting go down the branch

        while (true) {
            if (!should_go_left && !should_go_right)
                break;

            if (should_go_left && should_go_right) {
                // get the bigger of the two children, to get the max
                if (data[leftIndex] < data[rightIndex]) {
                    std::swap(data[current], data[leftIndex]);
                    current = leftIndex;
                } else {
                    std::swap(data[current], data[rightIndex]);
                    current = rightIndex;
                }
            } else if (should_go_left) {
                std::swap(data[current], data[leftIndex]);
                current = leftIndex;
            } else /* if (should_go_right) */ {
                std::swap(data[current], data[rightIndex]);
                current = rightIndex;
            }
        }
    }

    std::vector<T> data;
};

template <typename T>
void BinaryHeap<T>::clear()
{
    data.clear();
}

template <typename T>
size_t BinaryHeap<T>::size() const
{
    return data.size();
}

template <typename T>
bool BinaryHeap<T>::empty() const
{
    return size() == 0;
}

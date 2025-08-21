#pragma once

#include <stdexcept>
#include <utility>
#include <vector>

// order amongst class T is treated as priority
template <typename T, typename Comparator = std::greater<T>>
class PriorityQueueComparableElements {
public:
    void clear();

    size_t size() const;
    bool empty() const;

    const T& top() const { return data->first; }

    void pop()
    {
        if (empty())
            throw std::runtime_error("Cannot pop from empty heap.");

        data[0] = data[size() - 1];
        data.pop_back();

        if (!empty())
            heapify(0);
        else
            insertion_counter = 0;
    }

    void insert(const T& element)
    {
        data.push_back(std::make_pair(element, insertion_counter++));

        int index = data.size() - 1;
        int parentIndex = parent(index);
        while (index > 0
            && compare(data[index].first, data[parentIndex].first)) // sift up
        {
            std::swap(data[index], data[parentIndex]); // actual sift

            // start sifting elements further atop of the branch
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
        while (true) {
            size_t current = startingIndex;

            size_t leftIndex = leftChild(current);
            size_t rightIndex = rightChild(current);

            bool should_go_left = leftIndex < size()
                && compare(data[leftIndex].first, data[current].first);

            bool should_go_right = rightIndex < size()
                && compare(data[rightIndex].first, data[current].first);

            if (!should_go_left && !should_go_right)
                break;

            if (should_go_left && should_go_right) {
                // get the bigger of the two children, to get the max
                if (compare(data[rightIndex].first, data[leftIndex].first) {
                    std::swap(data[current], data[leftIndex]);
                    current = leftIndex;
                } else if (compare(data[leftIndex].first, data[rightIndex].first)){
                    std::swap(data[current], data[rightIndex]);
                    current = rightIndex;
                }
                else // equal priority invokes FIFO amongst such elements
                {
                    if (data[leftIndex].second < data[rightIndex].second) {
                        std::swap(data[current], data[leftIndex]);
                        current = leftIndex;
                    } else {
                        std::swap(data[current], data[rightIndex]);
                        current = rightIndex;
                    }
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

    Comparator compare;
    std::vector<std::pair<T, size_t>> data;
    size_t insertion_counter = 0;
};

template <typename T>
void BinaryHeap<T>::clear()
{
    data.clear();
    insertion_counter = 0;
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

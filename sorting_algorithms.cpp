#include <algorithm>
#include <vector>

using std::swap;

namespace simple_sorting_algorithms {

template <typename T>
void bubbleSortVector(std::vector<T>& vect)
{
    size_t lastSortedElement = vect.size() - 1; // no need to sort elements (at the end of the array) that are already sorted

    for (size_t i = 0; i < vect.size() - 1; ++i) //
    {
        size_t currentLastSorted = lastSortedElement;

        for (size_t j = 0; j < lastSortedElement; ++j) //
        {
            if (vect[j] > vect[j + 1])
                swap(vect[j], vect[j + 1]);

            currentLastSorted = j; // if it doesnt change it means everything is sorted
        }

        if (lastSortedElement == currentLastSorted) // this checks if nothing has changed, nothing has been swapped
            break;

        lastSortedElement = currentLastSorted; // usually the current index will be one or more behind the lastSortedElement index
    }
} // bubbleSortVector

template <typename T>
void insertionSortVector(std::vector<T>& vect)
{

    for (size_t i = 1; i < vect.size(); ++i) //
    {
        T elementToInsert = vect[i];
        size_t indexToInsert = i - 1;

        while (indexToInsert >= 0 // it is supposed to overshoot
            && elementToInsert < vect[indexToInsert]) // skipping/iterating to the relative place before the bigger elements
        {
            vect[indexToInsert + 1] = vect[indexToInsert];
            --indexToInsert;
        }

        // it is j + 1 since it overshoots THEN it checks if it is supposed to be pushed forward (a.k.a. bigger)
        vect[indexToInsert + 1] = elementToInsert;
    }
} // insertionSortVector

template <typename T>
void selectionSortVector(std::vector<T>& vect)
{

    for (size_t i = 1; i < vect.size() - 1; ++i) //
    {
        size_t minIndex = i;

        for (size_t j = 0; j < vect.size(); ++j)
            if (vect[j] < vect[minIndex])
                minIndex = j;

        if (i != minIndex)
            swap(vect[i], vect[minIndex]);
    }
} // selectionSortVector

} // simple_sorting_algorithms

namespace divide_conquer_sorting_algorithms {

namespace merge_sort {
    template <typename T>
    void mergeSortedArrays(T* firstArr, size_t firstSize, T* secondArr, size_t secondSize, T* result)
    {
        size_t firstIter = 0;
        size_t secondIter = 0;
        size_t resultIter = 0;

        while (firstIter < firstSize && secondIter < secondSize) //
        {
            bool get_left_or_right = firstArr[firstIter] < secondArr[secondIter];

            result[resultIter++] = get_left_or_right
                ? firstArr[firstIter++]
                : secondArr[secondIter++];
        }

        // merge the last remaining elements from ONE of the two arrays
        // therefore only one of the following while loops is going to execute

        while (firstIter < firstSize)
            result[resultIter++] = firstArr[firstIter++];

        while (secondIter < secondSize)
            result[resultIter++] = secondArr[secondIter++];
    }

    template <typename T>
    void mergeSortStepWithBuffer(T* arr, size_t size, T* result)
    {
        if (size < 2)
            return;

        size_t middle = size / 2;

        mergeSortStepWithBuffer(arr, middle, result);
        mergeSortStepWithBuffer(arr + middle, size - middle, result);

        mergeSortedArrays(arr, middle, arr + middle, size - middle, result);

        // reflect the merged result to origin
        for (size_t i = 0; i < size; ++i)
            arr[i] = result[i];
    }

    template <typename T>
    void mergeSort(T* arr, size_t size)
    {
        if (!arr || size < 2)
            return;

        T* buffer = new T[size];
        mergeSortStepWithBuffer(arr, size, buffer);
        delete[] buffer;
    }
}

namespace quick_sort {

    template <typename T>
    size_t quickSortPartition(T* arr, size_t size)
    {
        // this if clause exists because without it later the while loops will MISS them
        if (arr[0] > arr[size - 1])
            swap(arr[0], arr[size - 1]);

        // different people decide on different pivots, in this case - the pivot is the last element - a popular choice apparently
        T& pivotValue = arr[size - 1];

        // indexes that help find and put elements on the according sides compared to pivot
        // (left - smaller elements, right - bigger elements)
        size_t left = 0;
        size_t right = size - 1;

        while (true) {
            // find the first bigger element than the pivot on the left of the pivot where the pivot will go later
            while (arr[++left] < pivotValue)
                ;

            // find the first smaller element than the pivot on the right of the pivot where the pivot will go later
            while (arr[--right] > pivotValue)
                if (left == right) // if they point to the same place, there is no need "right" index to procceed
                    break;

            if (left >= right) // if they happen to pass by each other, we have found where the pivot is supposed to be, thus stop traversing
                break;

            // put bigger element on the right of future pivot, put smaller element on the left of future pivot
            swap(arr[left], arr[right]);
        }

        swap(arr[left], pivotValue); // put pivot where it belongs

        return left; // this is now the position of the pivot;
    }

    template <typename T>
    void quickSort(T* arr, size_t size)
    {
        if (size < 2)
            return;

        size_t pivotIndex = quickSortPartition(arr, size);

        quickSort(arr, pivotIndex);
        quickSort(arr + pivotIndex + 1, size - pivotIndex - 1);
    }

}

} // divide_conquer_sorting_algorithms

namespace other_sorting_algorithms {

void countingSort(std::vector<int>& vect)
{
    if (vect.empty())
        return;

    int min = *std::min_element(vect.begin(), vect.end());
    int max = *std::max_element(vect.begin(), vect.end());

    size_t countArrSize = max - min + 1; // elements range size

    if (min == max)
        return; // all elements are the same, no need to sort

    std::vector<int> countArr(countArrSize, 0);

    // counting the elements "and grouping them"
    for (int number : vect)
        ++countArr[number - min];

    // calculates where the end of a group of same integers is going to be
    // (i.e. the rightmost available index for such element)
    for (size_t i = 1; i < countArr.size(); ++i)
        countArr[i] += countArr[i - 1];

    std::vector<int> result(vect.size());

    // traverse in reverse order, insert grouped elements in descending order
    for (size_t i = vect.size() - 1; i >= 0; --i) //
    {
        size_t countIndex = vect[i] - min;

        size_t resultIndex = countArr[countIndex] - 1; // find associated max index.
        // subtracting the "ONE" because it has been added in the max - min equation for the size of counting array

        // put current element from origin to result in corresponding place from the end to beginning
        result[resultIndex] = vect[i];

        // decrease (move to the left) the rightmost available index for a group of ints
        --countArr[countIndex];
    }

    vect = std::move(result);
} // countingSort

namespace heap_sort {
    void heapify(int* arr, int size, int index)
    {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            bool goLeft = left < size && arr[index] < arr[left];
            bool goRight = right < size && arr[index] < arr[right];

            if (!goLeft && !goRight)
                break;

            if (goLeft && goRight) {
                if (arr[left] < arr[right]) {
                    swap(arr[left], arr[index]);
                    index = left;
                }

                else {
                    swap(arr[right], arr[index]);
                    index = left;
                }
            }

            else if (goLeft) {
                swap(arr[left], arr[index]);
                index = left;
            }

            else {
                swap(arr[right], arr[index]);
                index = right;
            }
        }
    }

    void heapSort(int* arr, int size)
    {
        if (!arr || size < 2)
            return;

        // makes the array a max heap
        for (int i = size / 2 - 1; i >= 0; i--)
            heapify(arr, size, i);

        int min = size - 1;

        // put the begginning at the end, then let the beginning sink, prioritizing going to the left
        // and thus it will SOMEHOW make this shit a perfectly balanced binary search tree
        for (int i = 0; i < size; ++i) {
            swap(arr[0], arr[min]);
            heapify(arr, min--, 0);
        }
        // it is becoming sorted because since the heap is perfectly balanced, we get the min from the last element,
        // let it sink from the top to the leftmost possible position it belongs to
    }
} // heap_sort

namespace topological_sort {

    void topoSortStep(const std::vector<std::vector<int>> adjList, std::vector<bool>& visited, std::vector<int>& result, int current)
    {
        visited[current] = true;

        for (auto neighbor : adjList[current])
            if (!visited[neighbor])
                topoSortStep(adjList, visited, result, neighbor);

        result.push_back(current);
    }

    std::vector<int> topologicalSort(const std::vector<std::vector<int>> adjList)
    {
        std::vector<int> result(adjList.size());
        std::vector<bool> visited(adjList.size(), false);

        for (int i = 0; i < adjList.size(); ++i)
            if (!visited[i])
                topoSortStep(adjList, visited, result, i);

        std::reverse(result.begin(), result.end());
        return result;
    }

} // topological_sort

} // other_sorting_algorithms

int main()
{
    return 0;
}

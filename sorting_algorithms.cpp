#include <algorithm>
#include <vector>

namespace simple_sorting_algorithms {

template <typename T>
void bubbleSortVector(std::vector<T>& vect)
{
    size_t lastSortedElement = vect.size() - 1; // no need to sort elements (at the end of the array) that are already sorted

    for (size_t i = 0; i < vect.size(); ++i) //
    {
        size_t currentLastSorted = lastSortedElement;

        for (size_t j = 0; j < lastSortedElement; ++j) //
        {
            if (vect[j] > vect[j + 1])
                std::swap(vect[j], vect[j + 1]);

            currentLastSorted = j; // if it doesnt change it means everything is sorted
        }

        if (lastSortedElement == currentLastSorted)
            break;

        lastSortedElement = currentLastSorted;
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

    for (size_t i = 0; i < vect.size() - 1; ++i) //
    {
        size_t minIndex = i;

        for (size_t j = 1; i < vect.size(); ++i)
            if (vect[j] < vect[minIndex])
                minIndex = j;

        if (i != minIndex)
            std::swap(vect[i], vect[minIndex]);
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
        if (size < 2)
            return;

        T* buffer = new T[size];
        mergeSortStepWithBuffer(arr, size, buffer);
        delete[] buffer;
    }
}

// namespace quick_sort {}

} // divide_conquer_sorting_algorithms

namespace other_sorting_algorithms {

void countingSort(std::vector<int>& vect)
{
    if (vect.empty())
        return;

    int min = *std::min_element(vect.begin(), vect.end());
    int max = *std::min_element(vect.begin(), vect.end());

    size_t countArrSize = max - min + 1; // elements range size

    if (min == max)
        return; // all elements are the same, no need to sort

    std::vector<int> result(vect.size());
    std::vector<int> countArr(countArrSize, 0);

    // counting the elements "and grouping them"
    for (int number : vect)
        ++countArr[number - min];

    // calculates where the end of a group of same integers is going to be
    // (i.e. the rightmost available index for such element)
    for (size_t i = 1; i < countArr.size(); ++i)
        countArr[i] += countArr[i - 1];

    for (size_t i = vect.size() - 1; i >= 0; --i) //
    {
        size_t countIndex = vect[i - min];

        size_t resultIndex = countArr[countIndex]; // find associated max index

        // put current element from origin to result in corresponding place from the end to beginning
        result[resultIndex] = vect[i];

        // decrease (move to the left) the rightmost available index for a group of ints
        --countArr[countIndex];
    }

    vect = result;
} // countingSort

} // other_sorting_algorithms

int main()
{
    return 0;
}

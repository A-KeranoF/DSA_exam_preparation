#include <vector>

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
}

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
}

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
}

template <typename T>
void mergePartitions(T* firstArray, size_t firstSize, T* secondArray, size_t secondSize)
{
}

template <typename T>
void mergeSortVectorStepWithBuffer(T* arr, size_t size)
{
}

template <typename T>
void mergeSortArray(T* arr, size_t size)
{
}

int main()
{
    return 0;
}

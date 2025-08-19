#pragma once

#include <list>
#include <stdexcept>
#include <utility>
#include <vector>

namespace HashSetConstants {
constexpr double INIT_LOAD_FACTOR = 0.7;
constexpr size_t INIT_SIZE = 8;
constexpr size_t GROWTH_FACTOR = 2;
};

template <typename T, typename Hasher = std::hash<T>>
class UnorderedSetInsertionOrder {
    typedef typename std::list<T>::iterator DataIterator;
    typedef std::list<DataIterator> BucketOfDataIterators;

public:
    UnorderedSetInsertionOrder()
        : UnorderedSetInsertionOrder(HashSetConstants::INIT_SIZE, HashSetConstants::INIT_LOAD_FACTOR)
    {
    }

    UnorderedSetInsertionOrder(size_t initialSize, double loadFactor)
        : collisionBuckets(initialSize)
        , loadFactor(loadFactor)
    {
        if (loadFactor <= 0.0)
            throw std::logic_error("Cannot initialize hash set with non-positive load factor");
    }

    void clear()
    {
        data.clear();
        collisionBuckets.clear();
        sz = 0;
    }

    double load_factor() const { return loadFactor; }

    size_t size() const { return sz; }
    size_t capacity() const { return cap; }

    bool empty() const { return sz == 0; }

    bool insert(const T& element)
    {
        size_t bucketIndex = getBucketIndex(element);
        BucketOfDataIterators& bucket = collisionBuckets[bucketIndex];

        auto bucketIterator = getBucketIterator(element, bucket);

        if (bucketIterator != bucket.end())
            return false;

        data.push_back(element);
        bucket.push_front(--data.end()); // since the new element is pushed in the end, we just get exactly that place as an iterator
        ++sz;

        if ((double)size() >= loadFactor * collisionBuckets.capacity())
            resize(collisionBuckets.size() > 0
                    ? collisionBuckets.capacity() * HashSetConstants::GROWTH_FACTOR
                    : HashSetConstants::INIT_SIZE);

        return true;
    }

    bool insert(T&& element)
    {
        size_t bucketIndex = getBucketIndex(element);
        BucketOfDataIterators& bucket = collisionBuckets[bucketIndex];

        auto bucketIterator = getBucketIterator(element, bucket);

        if (bucketIterator != bucket.end())
            return false;

        data.push_back(std::move(element));
        bucket.push_front(--data.end());
        ++sz;

        if ((double)size() >= loadFactor * collisionBuckets.capacity())
            resize(collisionBuckets.size() > 0
                    ? collisionBuckets.capacity() * HashSetConstants::GROWTH_FACTOR
                    : HashSetConstants::INIT_SIZE);

        return true;
    }

    bool contains(const T& element) const
    {
        if (empty())
            return false;

        size_t bucketIndex = getBucketIndex(element);
        BucketOfDataIterators& bucket = collisionBuckets[bucketIndex];

        auto bucketIterator = getBucketIterator(element, bucket);
        return bucketIterator != bucket.end());
    }

    bool remove(const T& element)
    {
        if (empty())
            return false;

        size_t bucketIndex = getBucketIndex(element);
        BucketOfDataIterators& bucket = collisionBuckets[bucketIndex];

        auto bucketIterator = getBucketIterator(element, bucket);

        // Since bucket.end() (and data.end()) is (are) basically "nothing"/"non-existing element",
        // semantically we can say they are the same and this can check if we found the element
        // without using contains() method or getDataIterator() method, which can slow the program by half otherwise.
        if (bucketIterator == bucket.end())
            return false;

        // This is how we can use the very iterator to remove the element from the data list and its iterator from bucket
        data.erase(*bucketIterator); // basically DataIterator, which is what this method requires
        bucket.erase(bucketIterator);
        --sz;

        return true;
    }

    void resize(size_t newBucketCount)
    {
        collisionBuckets.clear();
        collisionBuckets.resize(newBucketCount);

        // changed from insert(eachElement) to directly copying an iterator
        // because my previous method with inserting is duplicating data.
        // i could have just copied the data and cleared it after, but this copying method is redundant
        for (DataIterator dataIter = data.cbegin(); dataIter != data.cend(); ++dataIter) {
            size_t bucketIndex = getBucketIndex(data.first);
            Bucket& bucket = collisionBuckets[bucketIndex];
            bucket.push_back(dataIter);
        }
    }

private:
    size_t getBucketIndex(const T& element) const
    {
        return hash(element) % collisionBuckets.size();
    }

    typename BucketOfDataIterators::iterator getBucketIterator(const T& element, const BucketOfDataIterators& bucket)
    {
        auto bucketIterator = bucket.begin();

        while (bucketIterator != bucket.end()) {
            if (**bucketIterator == element)
                break;

            ++bucketIterator;
        }

        // this way we can point to bucket.end() that we can use as a placeholder for "nothing"/"non-existing element"

        return bucketIterator;
    }

    std::list<T> data;
    std::vector<BucketOfDataIterators> collisionBuckets;

    Hasher hash;

    const double loadFactor;
    size_t sz = 0;
    size_t cap;
};

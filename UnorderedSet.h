#pragma once

#include <list>
#include <stdexcept>
#include <utility>
#include <vector>

namespace HashSetConstants {
constexpr double INIT_LOAD_FACTOR = 0.7;
constexpr size_t INIT_CAPACITY = 16;
constexpr size_t GROWTH_FACTOR = 2;
};

template <typename T, typename Hasher = std::hash<T>>
class UnorderedSet {
    typedef typename std::list<T>::iterator DataIterator;
    typedef std::list<DataIterator> BucketOfDataIterators;

public:
    UnorderedSet()
        : UnorderedSet(HashSetConstants::INIT_CAPACITY, HashSetConstants::INIT_LOAD_FACTOR)
    {
    }

    UnorderedSet(size_t capacity, double loadFactor)
        : cap(capacity)
        , loadFactor(loadFactor)
    {
        if (loadFactor <= 0)
            throw std::logic_error("Cannot initialize hash set with non-positive load factor");
    }

    void clear()
    {
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
        bucket.push_back(--data.end()); // since the new element is pushed in the end, we just get exactly that place as an iterator
        ++sz;

        if (size() == HashSetConstants::INIT_LOAD_FACTOR * collisionBuckets.capacity())
            resize(collisionBuckets.capacity() * HashSetConstants::GROWTH_FACTOR);

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
        bucket.push_back(--data.end());
        ++sz;

        if (size() == HashSetConstants::INIT_LOAD_FACTOR * collisionBuckets.capacity())
            resize(collisionBuckets.capacity() * HashSetConstants::GROWTH_FACTOR);

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

        // since bucket.end() and data.end() are basically "nothing",
        // semantically we can say they are the same and this can check if we found the element
        // without using contains() or getIterator() which can slow the program by half otherwise.
        // This is how we can use to remove the element from the data list and its iterator from bucket
        if (bucketIterator == bucket.end())
            return false;

        data.erase(*bucketIterator);
        bucket.erase(bucketIterator);
        --sz;

        return true;
    }

    void resize(size_t newCapacity)
    {
        collisionBuckets.clear();
        collisionBuckets.reserve(newCapacity);

        for (const T& element : data)
            insert(element);
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
            if (*bucketIterator == element)
                break;

            ++bucketIterator;
        }

        return bucketIterator;
    }

    std::list<T> data;
    std::vector<BucketOfDataIterators> collisionBuckets;

    Hasher hash;

    const double loadFactor;
    size_t sz = 0;
    size_t cap;
};

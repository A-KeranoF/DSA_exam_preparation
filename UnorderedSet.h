#pragma once

#include <list>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T, typename Hasher = std::hash<T>>
class UnorderedSet {
public:
    typedef typename std::list<T>::iterator DataListIterator;
    typedef DataListIterator Bucket;

    UnorderedSet()
        : UnorderedSet(INIT_CAPACITY, INIT_LOAD_FACTOR)
    {
    }

    UnorderedSet(size_t initialCapacity, double loadFactor)
        : collisionBuckets(initialCapacity)
        , loadFactor(loadFactor)
    {
        if (loadFactor < 0)
            throw std::invalid_argument("Cannot initialize set with negative load factor.");
    }

    // Th(1), O(n)
    std::pair<DataListIterator, bool> insert(const T& key)
    {
        // reset buckets if needed
        if (collisionBuckets.empty())
            collisionBuckets.resize(INIT_CAPACITY, data.end()); // Th(n), rarely happens

        DataListIterator iter = getCollisionIterator(key); // Th(1), O(n)

        // check if element already exists
        if (iter != data.end()) // O(1)
            return std::make_pair(iter, false);

        // actual insert
        data.push_back(key); // O(1)
        collisionBuckets[hashIndex(key)].push_back(--data.end()); // O(1)

        if (size() >= load_factor() * bucket_count()) // Th(1), O(n) since it rarely happens
        {
            resize(calculateNewCapacity()); // Th(n)
            iter = getCollisionIterator(key); // refresh the iterator // Th(1), O(n)
        }

        return std::make_pair(iter, true); // Th(1)
    }

    // Th(1), O(n)
    bool contains(const T& key) const
    {
        if (data.empty())
            return false;

        DataListIterator iter = getCollisionIterator(key);
        if (iter != data.end())
            return true;

        return false;
    }

    // Th(1), O(n)
    bool remove(const T& key)
    {
        if (data.empty())
            return false;

        DataListIterator iter = getCollisionIterator(key); // Th(1), O(n)

        if (iter == data.end())
            return false;

        data.erase(iter); // O(1)
        collisionBuckets[hashIndex(key)].erase(iter); // Th(1), O(n)

        return true;
    }

    // O(n)
    void clear()
    {
        data.clear();
        collisionBuckets.clear();
    }

    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }
    double load_factor() const { return loadFactor; }
    size_t bucket_count() const { return collisionBuckets.size(); }

    // Th(n)
    void resize(size_t newCapacity)
    {
        collisionBuckets.clear();
        collisionBuckets.reserve(newCapacity);

        for (const T& element : data)
            insert(element);
    }

private:
    size_t hashIndex(const T& elem) const
    {
        return hasher(elem) % collisionBuckets.size();
    }

    size_t calculateNewCapacity() const
    {
        return bucket_count() > 0
            ? bucket_count() * GROWTH_FACTOR
            : INIT_CAPACITY;
    }

    // Th(1), O(n)
    DataListIterator getCollisionIterator(const T& key)
    {
        size_t chainIndex = hashIndex(key); // O(1)

        Bucket& chain = collisionBuckets[chainIndex]; // why the fuck is there a reference? // O(1)

        if (chain.empty()) // O(1)
            return data.end();

        for (DataListIterator iter = collisionBuckets[chainIndex].begin();
            iter != collisionBuckets[chainIndex].end();
            ++iter) // O(n), but should be small ~> O(1)
        {
            if (*iter == key)
                return iter;
        }

        return data.end();
    }

    static const double INIT_LOAD_FACTOR;
    static const size_t INIT_CAPACITY;
    static const size_t GROWTH_FACTOR;

    std::list<T> data;
    std::vector<Bucket> collisionBuckets;
    Hasher hasher;
    double loadFactor;
};

template <typename T, typename Hasher>
const double UnorderedSet<T, Hasher>::INIT_LOAD_FACTOR = 0.7;

template <typename T, typename Hasher>
const size_t UnorderedSet<T, Hasher>::INIT_CAPACITY = 16;

template <typename T, typename Hasher>
const size_t UnorderedSet<T, Hasher>::GROWTH_FACTOR = 2;

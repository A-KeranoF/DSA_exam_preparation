#pragma once

#include <list>
#include <stdexcept>
#include <utility>
#include <vector>

namespace HashMapConstants {
constexpr double INIT_LOAD_FACTOR = 0.7;
constexpr size_t INIT_CAPACITY = 16;
constexpr size_t GROWTH_FACTOR = 2;
};

template <typename Key, typename Value, typename Hasher>
class UnorderedMap {
public:
    typedef std::pair<Key, Value> DataType;
    typedef typename std::list<DataType>::iterator DataListIterator;
    typedef DataListIterator Bucket;

    UnorderedMap()
        : UnorderedMap(HashMapConstants::INIT_CAPACITY, HashMapConstants::INIT_LOAD_FACTOR)
    {
    }

    UnorderedMap(size_t initialCapacity, double loadFactor)
        : collisionBuckets(initialCapacity)
        , loadFactor(loadFactor)
    {
    }

    // amortized: O(const), worst case: O(n)
    std::pair<DataListIterator, bool> insert(Key key, Value value)
    {
        // reset buckets if needed
        if (collisionBuckets.empty())
            collisionBuckets.resize(HashMapConstants::INIT_CAPACITY, data.end()); // O(n), rarely happens // this is not proper initialization of the list

        DataListIterator iter = getCollisionIterator(key); // amortized O(const)

        // check if element already exists
        if (iter != data.end()) // amortized O(const)
            return std::make_pair(iter, false);

        // actual insert
        data.push_back(std::make_pair(key, value)); // O(const)
        collisionBuckets[hashIndex(key)].push_back(--data.end()); // O(const)

        if (data.size() >= loadFactor * collisionBuckets.size()) // amortized O(const) since it rarely happens
        {
            resize(HashMapConstants::GROWTH_FACTOR * collisionBuckets.size()); // O(n)
            iter = getCollisionIterator(key); // refresh the iterator // O(n)
        }

        return std::make_pair(iter, true); // O(const)
    }

    // amortized: O(const), worst case: O(n)
    Value& get(Key key) const
    {
        if (data.empty())
            throw std::runtime_error("Element not found");

        DataListIterator iter = getCollisionIterator(key);
        if (iter != data.end())
            return iter->second;

        throw std::runtime_error("Element not found");
    }

    // amortized: O(const), worst case: O(n)
    Value& modify(Key key)
    {
        if (data.empty())
            throw std::runtime_error("Element not found");

        DataListIterator iter = getCollisionIterator(key);
        if (iter != data.end())
            return iter->second;

        throw std::runtime_error("Element not found");
    }

    // amortized: O(const), worst case: O(n)
    bool contains(Key key) const
    {
        try {
            get(key);
        } catch (std::runtime_error& e) {
            return false;
        }
        return true;
    }

    // O(n)
    bool remove(Key key)
    {
        if (data.empty())
            return false;

        size_t index = hashIndex(key);
        DataListIterator iter = getCollisionIterator(key); // amortized O(const)
        Bucket& bucket = collisionBuckets[index];

        if (iter == data.end())
            return false;

        data.erase(iter); // O(const)
        bucket.erase(iter); // O(n)

        return true;
    }

    void clear()
    {
        data.clear();
        collisionBuckets.clear();
    }

    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }
    double load_factor() const { return loadFactor; }
    size_t bucket_count() const { return collisionBuckets.size(); }

    // O(n)
    void resize(size_t newCapacity)
    {
        collisionBuckets.clear();
        collisionBuckets.reserve(newCapacity);

        for (DataType& element : data)
            insert(element.first, element.second);
    }

private:
    // O(const)
    size_t hashIndex(Key key) const
    {
        return hasher(key) % collisionBuckets.size();
    }

    // amortized O(const);
    // worst case: O(n) but it should be VERY small anyway
    DataListIterator getCollisionIterator(Key key)
    {
        size_t bucketIndex = hashIndex(key); // O(1)

        Bucket& chain = collisionBuckets[bucketIndex]; // we use reference because we want the chain itself, not a perishable copy //O(1)

        if (chain.empty()) // O(1)
            return data.end();

        for (DataListIterator iter = chain.begin(); iter != chain.end(); ++iter) // O(n), but should be small
            if (iter->first == key)
                return iter;

        return data.end();
    }

    std::list<DataType> data;
    std::vector<Bucket> collisionBuckets;
    Hasher hasher;
    double loadFactor;
};

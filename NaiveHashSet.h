#pragma once

#include <list>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T, typename Hasher = std::hash<T>>
class NaiveHashSet {
public:
    void clear()
    {
        collisionBuckets.clear();
        sz = 0;
    }

    size_t size() const { return sz; }

    bool empty() const { return sz == 0; }

    bool insert(const T& element)
    {
        size_t index = getBucketIndex(element);
        auto& chain = collisionBuckets[index];

        for (auto iter = chain.begin(); iter != chain.end(); ++iter)
            if (*iter == element)
                return false;

        chain.push_back(element);
        ++sz;

        if (size() == INIT_LOAD_FACTOR * collisionBuckets.capacity())
            resize(collisionBuckets.capacity() * GROWTH_FACTOR);

        return true;
    }

    bool insert(T&& element)
    {
        size_t index = getBucketIndex(element);
        auto& chain = collisionBuckets[index];

        for (auto iter = chain.begin(); iter != chain.end(); ++iter)
            if (*iter == element)
                return false;

        chain.push_back(std::move(element));

        ++sz;

        if (size() == INIT_LOAD_FACTOR * collisionBuckets.capacity())
            resize(collisionBuckets.capacity() * GROWTH_FACTOR);

        return true;
    }

    bool contains(const T& element) const
    {
        if (empty())
            return false;

        size_t index = getBucketIndex(element);
        auto& chain = collisionBuckets[index];

        for (auto iter = chain.begin(); iter != chain.end(); ++iter)
            if (*iter == element)
                return true;

        return false;
    }

    bool remove(const T& element)
    {
        if (empty())
            return false;

        size_t index = getBucketIndex(element);
        auto& chain = collisionBuckets[index];

        for (auto iter = chain.begin(); iter != chain.end(); ++iter) {
            if (*iter == element) {
                chain.erase(iter);
                --sz;
                return true;
            }
        }

        return false;
    }

private:
    size_t getBucketIndex(const T& element) const
    {
        return hash(element) % collisionBuckets.size();
    }

    void resize(size_t newCapacity)
    {
        std::vector<std::list<T>> oldDataBuckets = collisionBuckets;

        collisionBuckets.clear();
        collisionBuckets.reserve(newCapacity);

        for (auto bucket : oldDataBuckets)
            for (auto element : bucket)
                insert(element);
    }

    const double INIT_LOAD_FACTOR = 0.7;
    const size_t GROWTH_FACTOR = 2;

    std::vector<std::list<T>> collisionBuckets;
    Hasher hash;

    size_t sz = 0;
};

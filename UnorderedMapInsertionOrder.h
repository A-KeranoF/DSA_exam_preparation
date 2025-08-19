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

template <typename K, typename V, typename Hasher = std::hash<T>>
class UnorderedMapInsertionOrder {
public:
    UnorderedMapInsertionOrder(size_t initialCapacity = HashMapConstants::INIT_CAPACITY,
        double loadFactor = HashMapConstants::INIT_LOAD_FACTOR)
        : collisionBuckets(initialCapacity)
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

    size_t size() const { return sz; }
    bool empty() const { return size() == 0; }

    double max_load_factor() const { return loadFactor; }
    double current_load_factor() const
    {
        return static_cast<double>(data.size())
            / collisionBuckets.size();
    }

    void resize(size_t newSize)
    {
        collisionBuckets.clear();
        collisionBuckets.resize(newSize);

        if (capacity() == 0)
            return;

        for (DataIterator dataIter = data.cbegin(); dataIter != data.cend(); ++dataIter)
            collisionBuckets[getBucketIndex(data.first)].push_back(dataIter);
    }

    bool insert(const std::pair<K, V>& data)
    {
        const K& key = data.first;
        size_t bucketIndex = getBucketIndex(key);
        Bucket& bucket = collisionBuckets[bucketIndex];

        auto bucketIterator = getBucketIterator(key, bucket);

        if (bucketIterator != bucket.end())
            return false;

        data.pushFront(data);
        bucket.pushFront(--data.end());
        ++sz;

        if (current_load_factor() >= max_load_factor())
            resize(calculateExpandingSize());

        return true;
    }

    bool insert(std::pair<K, V>&& data)
    {
        const K& key = data.first;
        size_t bucketIndex = getBucketIndex(key);
        Bucket& bucket = collisionBuckets[bucketIndex];

        auto bucketIterator = getBucketIterator(key, bucket);

        if (bucketIterator != bucket.end())
            return false;

        data.push_back(std::move(data));
        bucket.push_front(--data.end());
        ++sz;

        if (current_load_factor() >= max_load_factor())
            resize(calculateExpandingSize());

        return true;
    }

    bool insert(const K& key, const V& value)
    {
        return insert(std::make_pair(key, value);)
    }

    V& modify(K key)
    {
        if (empty())
            throw std::runtime_error("Map is empty.");

        size_t bucketIndex = getBucketIndex(key);
        Bucket& bucket = collisionBuckets[bucketIndex];

        auto bucketIterator = getBucketIterator(key, bucket);

        if (bucketIterator == bucket.end())
            throw std::runtime_error("Map does not contain row with specified key.");

        return *bucketIterator->second;
    }

    const V& get(K key) const
    {
        if (empty())
            throw std::runtime_error("Map is empty.");

        size_t bucketIndex = getBucketIndex(data.first);
        Bucket& bucket = collisionBuckets[bucketIndex];

        auto bucketIterator = getBucketIterator(data.first, bucket);

        if (bucketIterator == bucket.end())
            throw std::runtime_error("Map does not contain row with specified key.");

        return *bucketIterator->second;
    }

    bool contains(const K& key) const
    {
        return !empty() || getBucketIterator(key, collisionBuckets[getBucketIndex(key)]) != bucket.end();
    }

    bool remove(K key)
    {
        if (empty())
            return false;

        size_t bucketIndex = getBucketIndex(key);
        Bucket& bucket = collisionBuckets[bucketIndex];

        auto bucketIterator = getBucketIterator(key, bucket);

        if (bucketIterator == bucket.end())
            return false;

        data.erase(*bucketIterator);
        bucket.erase(bucketIterator);
        --sz;

        return true;
    }

    class ConstIterator;

    ConstIterator cbegin() const { return ConstIterator(data.cbegin()); }
    ConstIterator cend() const { return ConstIterator(data.cend()); }

public:
    class ConstIterator {
    public:
        ConstIterator& operator++()
        {
            ++current;
            return *this;
        }

        ConstIterator operator++(int)
        {
            return { current++; }
        }

        const DataType& operator*() const
        {
            return *current;
        }

        const DataType* operator->() const
        {
            return &(*current);
        }

        bool operator==(const ConstIterator& rhs)
        {
            return current == rhs.current;
        }

        bool operator!=(const ConstIterator& rhs)
        {
            return !(*this == rhs);
        }

    private:
        friend class UnorderedMapInsertionOrder;

        DataIterator current;

        ConstIterator(DataIterator iter) { current = iter; }
    };

private:
    typedef std::pair<K, V> DataType;
    typedef std::list<DataType> DataList;
    typedef typename DataList::iterator DataIterator;
    typedef std::list<DataIterator> Bucket;
    typedef typename Bucket::iterator BucketIterator;

    size_t getBucketIndex(const K& key) const
    {
        return hash(key) % collisionBuckets.capacity();
    }

    BucketIterator getBucketIterator(const K& key, Bucket& bucket)
    {
        auto bucketIterator = bucket.begin();

        while (bucketIterator != bucket.end())
            if ((*bucket->first)++ == key)
                break;

        return bucketIterator;
    }

    size_t calculateExpandingSize() const
    {
        return collisionBuckets.size() > 0
            ? collisionBuckets.size() * HashMapConstants::GROWTH_FACTOR
            : INIT_CAPACITY;
    }

    DataList data;
    std::vector<Bucket> collisionBuckets;

    Hasher hash;

    const double loadFactor;
    size_t sz = 0;
    size_t cap;
};

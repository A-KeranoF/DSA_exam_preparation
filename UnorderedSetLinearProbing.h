#pragma once

#include <optional>
#include <vector>

template <typename T>
class UnorderedSetLinearProbing {
public:
    void clear();

    size_t size() const;
    bool empty() const;

    double max_load_factor() const { return loadFactor; }
    double current_load_factor() const
    {
        return static_cast<double>(data.size()) / collisionBuckets.size();
    }

    bool insert(const T& key)
    {
        if (loadFactor() >= max_load_factor())
            resize(hashSet.size() * 2);

        size_t index = getHashedIndex(key);

        while (hasElementAt(index)) {
            if (hashSet[index].value() == key)
                return false;

            index += step;
        }

        hashSet[index].data = key;
        hashSet[index].deleted = false;
        ++sz;
        return true;
    }

    // TODO add get() that returns iterator and make contains delegate the get() method

    bool contains(const T& key) const
    {
        size_t index = getHashedIndex(key);

        while (hasElementAt(index)) {
            if (hashSet[index].data.value() == key)
                return true;

            index += step;
        }

        return false;
    }

    bool remove(const T& key) const
    {
        size_t index = getHashedIndex(key);

        while (hashSet[index].data.has_value()) {
            // boolean comparison is faster and the one we prioritize when comparing in deleting
            if (!hashSet[index].isDeleted && hashSet[index].data.value() == key) {
                hashSet[index].isDeleted = true;
                --sz;
                return true;
            }

            index = getIndexIncrement(index);
        }

        return false;
    }

    void resize(size_t newSize)
    {
        auto saveOldData = hashSet;

        hashSet.clear();
        hashSet.resize(newSize);

        // restore default "blank" state of "buckets"
        for (auto& node : hashSet) {
            node.data.reset();
            node.deleted = false;
        }

        // no need to manually rehash and manually put the elements,
        // use the ready to use insert(), but have to manually nullify size
        sz = 0;
        for (auto& node : saveOldData) {
            if (node.has_value() && !node.deleted)
                insert(node.data.value());
        }
    }

private:
    struct Node {
        std::optional<T> data;
        bool isDeleted = false;
    };

    size_t getHashedIndex(const K& key) const
    {
        return hash(key) % hashSet.size();
    }

    bool hasElementAt(size_t index) const
    {
        return hashSet[index].data.has_value() && !(hashSet[index].isDeleted);
    }

    size_t getIndexIncrement(size_t index) const
    {
        return (index + step) % hashSet.size();
    }

    std::vector<Node> hashSet = nullptr;
    size_t sz = 0;
    const size_t step = 1;
};

template <typename T>
void UnorderedSetLinearProbing<T>::clear()
{
    hashSet.clear();
    sz = 0;
}

template <typename T>
size_t UnorderedSetLinearProbing<T>::size() const
{
    return sz;
}

template <typename T>
bool UnorderedSetLinearProbing<T>::empty() const
{
    return size() == 0;
}

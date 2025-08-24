#pragma once

#include <list>
#include <vector>

template <typename T, typename Hasher = std::hash<T>>
class UnorderedSet {
public:
    void clear();

    size_t size() const;
    bool empty() const;

    double max_load_factor() const { return loadFactor; }
    double current_load_factor() const
    {
        return static_cast<double>(data.size()) / chainBeginnings.size();
    }

    std::pair<DataIterator, bool> insert(const T& key)
    {
        size_t startOfChainIndex = getHash(key);
        DataIterator iter = getElementFromChainAtIndex(key, startOfChainIndex);

        if (iter != data.end())
            return std::make_pair<DataIterator, bool>(iter, false);

        std::pair<DataIterator, size_t>& currentChainData = chainBeginnings[startOfChainIndex];

        if (currentChainData.second == 0) {
            data.push_front(key);
            // since we inserted the NEW element at the front, we take that elements
            // as the beginning of a NEW chain (with its size being one, incremented later)
            currentChainData.first = data.begin();
        } else {
            // list.insert(iterator, elementToPut) is basically insert_before(iter, element)
            // and returns iter to newly added element
            std::pair<DataIterator, size_t> newBeginningOfChain = data.insert(currentChainData.first, element);
            // update the new beginning of chain
            currentChainData.first = newBeginningOfChain;
        }

        // always update the count of elements in the current chain
        currentChainData.second++;

        if (current_load_factor() >= max_load_factor()) {
            resize(chainBeginnings.size() * 2);
            // element might get new place, so we need to refresh the iterator
            DataIterator newIterator = getElementFromChainAtIndex(key, getHash(key));
            return std::make_pair(newIterator, true);
        }

        return std::make_pair(currentChainData.first, true);
    }

    bool contains(const T& key) const
    {
        return get(key) != data.end();
    }

    DataIterator get(const T& key)
    {
        if (empty())
            return data.end();

        return getElementFromChainAtIndex(key, getHash(key));
    }

    bool remove(const T& key) const
    {
        if (empty())
            return false;

        size_t startOfChainIndex = getHash(key);
        DataIterator iter = getElementFromChainAtIndex(key, startOfChainIndex);

        if (iter == data.end())
            return false;

        std::pair<DataIterator, size_t>& currentChainData = chainBeginnings[startOfChainIndex];

        // if element is the start of the chain, we must update to the next
        if (iter == currentChainData.first)
            currentChainData.first = iter + 1;

        data.erase(iter);
        currentChainData.second--;
        return true;
    }

    void resize(size_t newSize)
    {
        std::list<T> oldData(std::move(data));

        data.clear();
        chainBeginnings.clear();

        chainBeginnings.resize(newSize, std::make_pair(data.end(), 0));

        for (auto& el : oldData)
            insert(el);
    }

private:
    typedef typename std::list<T>::iterator DataIterator;
    typedef std::pair<DataIterator, size_t> ChainMetaData;

    size_t getHash(const T& key) const
    {
        return hash(key) % chainBeginnings.size();
    }

    DataIterator getElementFromChainAtIndex(const T& key, size_t index)
    {
        size_t chainSize = chainBeginnings[index].second;

        if (chainSize == 0) // if data was empty, this would still hold true, since it points nothing of use
            return data.end();

        DataIterator iter = chainBeginnings[index].first;

        // iter is guaranteed to point to a chain, since the chainSize now says this chain has elements
        for (size_t i = 0; i < chainSize; ++i) {
            if (*iter == key)
                return iter;
            iter++; // continue if not found
        }

        return data.end();
    }

    Hasher hash;
    std::list<T> data;
    std::vector<ChainMetaData> chainBeginnings;

    double loadFactor;
};

template <typename T>
void UnorderedSet<T, Hasher>::clear()
{
    data.clear();
    chainBeginnings.clear();
}

template <typename T>
size_t UnorderedSet<T, Hasher>::size() const
{
    return data.size();
}

template <typename T>
bool UnorderedSet<T, Hasher>::empty() const
{
    return data.empty();
}

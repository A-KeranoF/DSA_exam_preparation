#pragma once
#include <deque>

template <typename T, typename Container = std::deque<T>>
class TemplateQueue {
public:
    T& front() { return container.front(); }
    const T& front() const { return container.front(); }

    T& back() { return container.back(); }
    const T& back() const { return container.back(); }

    void push(const T& element) { container.push_back(element); }
    void push(T&& element) { container.push_back(element); }

    void pop() { container.pop_front(); }

    bool empty() const { return container.empty(); }
    size_t size() const { return container.size(); }

private:
    Container container;
};

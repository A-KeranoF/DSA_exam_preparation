#pragma once
#include <deque>

template <typename T, typename Container = std::deque<T>>
class TemplateStack {
public:
    T& top() { return container.back(); }
    const T& top() const { return container.back(); }

    void push(const T& element) { container.push_back(element); }
    void push(T&& element) { container.push_back(element); }

    void pop() { container.pop_back(); }

    bool empty() const { return container.empty(); }
    size_t size() const { return container.size(); }

private:
    Container container;
};

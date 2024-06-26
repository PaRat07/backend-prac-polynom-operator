#pragma once

#include <algorithm>
#include <memory>
#include <list>


template<typename Value_t>
class LinkedList {
private:
    struct Node {
        template<typename... Args>
        Node(Args&&... v)
                : val(std::forward<Args>(v)...)
        {
        }

        Node *next = this;
        Node *prev = this;
        Value_t val;
    };

public:
    using value_type = Value_t;

    class ConstListIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = const Value_t;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        ConstListIterator(const Node *ptr)
                : base_(ptr)
        {}

        ConstListIterator() = default;

        reference operator*() const {
            return base_->val;
        }

        pointer operator->() const {
            return &base_->val;
        }

        ConstListIterator operator++() {
            return { base_ = base_->next };
        }

        ConstListIterator operator--() {
            return { base_ = base_->prev };
        }

        ConstListIterator operator++(int /*fucking_unused*/) {
            const Node *buf = base_;
            base_ = base_->next;
            return buf;
        }

        ConstListIterator operator--(int /*fucking_unused*/) {
            const Node *buf = base_;
            base_ = base_->prev;
            return buf;
        }

        bool operator==(const ConstListIterator &rhs) const = default;
        bool operator!=(const ConstListIterator &rhs) const = default;


        const Node *Base() const {
            return base_;
        }

    private:
        const Node *base_;
    };

    class ListIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = Value_t;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        ListIterator() = default;

        ListIterator(Node *ptr)
                : base_(ptr)
        {}

        reference operator*() {
            return base_->val;
        }

        reference operator*() const {
            return base_->val;
        }

        pointer operator->() {
            return base_->val;
        }

        const Node *operator->() const {
            return base_;
        }

        ListIterator operator++() {
            return base_ = base_->next;
        }

        ListIterator operator--() {
            return base_ = base_->prev;
        }

        ListIterator operator++(int /*fucking_unused*/) {
            Node *buf = base_;
            base_ = base_->next;
            return buf;
        }

        ListIterator operator--(int /*fucking_unused*/) {
            Node *buf = base_;
            base_ = base_->prev;
            return buf;
        }

        Node* Base() const {
            return base_;
        }

        bool operator==(const ListIterator &rhs) const = default;
        bool operator!=(const ListIterator &rhs) const = default;

        operator ConstListIterator() {
            return const_cast<const Node*>(base_);
        }

    private:
        Node *base_;
    };

    LinkedList()
        : sentinel_(new Node)
    {
    }

    LinkedList(const LinkedList<Value_t> &other)
            : sentinel_(new Node)
    {
        std::for_each(other.begin(), other.end(), [&] (const Value_t &val) {
            push_back(val);
        });
    }

    LinkedList(LinkedList &&other)
        : sentinel_(other.sentinel_)
        , size_(other.size_)
    {
        other.sentinel_ = new Node;
        other.size_ = 0;
    }

    template<typename Iterator>
    LinkedList(Iterator beg, Iterator end)
            : sentinel_(new Node)
    {
        std::for_each(beg, end, [&] (auto x) {
            push_back(x);
        });
    }

    ~LinkedList() {
        sentinel_->prev->next = nullptr;
        while(sentinel_) {
            Node *buf = sentinel_->next;
            delete sentinel_;
            sentinel_ = buf;
        }
    }

    LinkedList<Value_t> &operator=(const LinkedList<Value_t> &other) {
        if (other.sentinel_ == sentinel_) return *this;
        while(size_ > 0) {
            Node *buf = sentinel_->next;
            delete sentinel_;
            sentinel_ = buf;
            --size_;
        }
        sentinel_ = new Node;

        std::for_each(other.begin(), other.end(), [&] (auto x) {
            PushBack(x);
        });
        return *this;
    }

    LinkedList<Value_t> &operator=(LinkedList<Value_t> &&other) noexcept {
        std::swap(sentinel_, other.sentinel_);
        std::swap(size_, other.size_);
        return *this;
    }

    template<typename... Args>
    void EmplaceBack(Args&&... val) {
        EmplaceBefore(sentinel_, std::forward<Args>(val)...);
    }

    template<typename T>
    void push_back(T&& val) {
        EmplaceBack(std::forward<T>(val));
    }

    template<typename T>
    void PushBack(T&& val) {
        EmplaceBack(std::forward<T>(val));
    }

    ConstListIterator begin() const {
        return {sentinel_->next };
    }

    ConstListIterator end() const {
        return {sentinel_ };
    }

    ListIterator begin() {
        return {sentinel_->next};
    }

    ListIterator end() {
        return {sentinel_ };
    }

    ConstListIterator cbegin() const {
        return {sentinel_->next };
    }

    ConstListIterator cend() const {
        return {sentinel_ };
    }

    void Sort() {
        *this = Sort(*this);
    }

    Value_t &Front() {
        return sentinel_->next->val;
    }

    const Value_t &Front() const {
        return sentinel_->next->val;
    }

    Value_t &Back() {
        return sentinel_->prev->val;
    }

    const Value_t &Back() const {
        return sentinel_->prev->val;
    }

    bool Empty() const {
        return size_ == 0;
    }

    template<typename... Args>
    void EmplaceBefore(ConstListIterator it, Args&&... val) {
        Node *new_node = new Node(std::forward<Args>(val)...);
        ListIterator non_const_it = const_cast<Node*>(it.Base());
        new_node->prev = non_const_it.Base()->prev;
        new_node->next = non_const_it.Base();
        non_const_it.Base()->prev->next = new_node;
        non_const_it.Base()->prev = new_node;
        ++size_;
    }

    template<typename T>
    void InsertBefore(ConstListIterator it, T val) {
        EmplaceBefore(it, std::forward<T>(val));
    }

    void Erase(ConstListIterator it) {
        if (it.Base() == sentinel_) {
            throw std::runtime_error("Erase end");
        }
        ListIterator non_const_it = const_cast<Node*>(it.Base());
        non_const_it.Base()->next->prev = non_const_it.Base()->prev;
        non_const_it.Base()->prev->next = non_const_it.Base()->next;
        --size_;
    }

    size_t Size() const {
        return size_;
    }

private:
    Node *sentinel_;
    size_t size_ = 0;

    static LinkedList Sort(LinkedList l) {
        if (l.Size() <= 1) {
            return l;
        }

        LinkedList left, right;
        bool to_l = true;
        for (const auto &i : l) {
            if (to_l) {
                left.push_back(i);
            } else {
                right.push_back(i);
            }
            to_l = !to_l;
        }
        left = Sort(left);
        right = Sort(right);

        auto itl = left.begin();
        auto itr = right.begin();
        LinkedList ans;
        while (itl != left.end() && itr != right.end()) {
            if (*itl < *itr) {
                ans.push_back(*itl++);
            } else {
                ans.push_back(*itr++);
            }
        }
        while (itl != left.end()) {
            ans.push_back(*itl++);
        }
        while (itr != right.end()) {
            ans.push_back(*itr++);
        }
        return ans;
    }
};
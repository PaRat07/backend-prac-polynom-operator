#pragma once

#include <algorithm>
#include <memory>
#include <list>

// FIXME
#include <iostream>

//template<typename T>
//static std::weak_ptr<T> WeakConstCast(std::weak_ptr<const T> ptr) {
//    return std::weak_ptr<T>(const_cast<T*>(&*ptr.lock()));
//}

template<typename Value>
class SingleLinkedList {
private:
    struct Node {
        template<typename... Args>
        Node(Args&&... v)
                : val(std::forward<Args>(v)...)
        {
        }

        Node *next = this;
        Node *prev = this;
        Value val;
    };

public:
    using value_type = Value;

    class ConstListIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = const Value;
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
        using value_type = Value;
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

    SingleLinkedList()
        : pre_sentinel_(new Node)
    {
    }

    SingleLinkedList(const SingleLinkedList<Value> &other)
            : pre_sentinel_(new Node)
    {
        std::for_each(other.begin(), other.end(), [&] (const Value &val) {
            PushBack(val);
        });
    }

    SingleLinkedList(SingleLinkedList &&other)
        : pre_sentinel_(other.pre_sentinel_)
        , size_(other.size_)
    {
        other.pre_sentinel_ = new Node;
        other.size_ = 0;
    }

    template<typename Iterator>
    SingleLinkedList(Iterator beg, Iterator end)
            : pre_sentinel_(new Node)
    {
        std::for_each(beg, end, [&] (auto x) {
            PushBack(x);
        });
    }

    ~SingleLinkedList() {
        pre_sentinel_->prev->next = nullptr;
        while(pre_sentinel_) {
            Node *buf = pre_sentinel_->next;
            delete pre_sentinel_;
            pre_sentinel_ = buf;
        }
    }

    SingleLinkedList<Value> &operator=(const SingleLinkedList<Value> &other) {
        if (other.pre_sentinel_ == pre_sentinel_) return *this;
        while(pre_sentinel_) {
            Node *buf = pre_sentinel_->next;
            delete pre_sentinel_;
            pre_sentinel_ = buf;
        }
        pre_sentinel_ = new Node;

        std::for_each(other.begin(), other.end(), [&] (auto x) {
            PushBack(x);
        });
        return *this;
    }

    SingleLinkedList<Value> &operator=(SingleLinkedList<Value> &&other) noexcept {
        std::swap(pre_sentinel_, other.pre_sentinel_);
        return *this;
    }

    template<typename... Args>
    void EmplaceBack(Args&&... val) {
        EmplaceBefore(pre_sentinel_, std::forward<Args>(val)...);
    }

    template<typename T>
    void PushBack(T&& val) {
        EmplaceBack(std::forward<T>(val));
    }

    ConstListIterator begin() const {
        return { pre_sentinel_->next };
    }

    ConstListIterator end() const {
        return { pre_sentinel_ };
    }

    ListIterator begin() {
        return {pre_sentinel_->next};
    }

    ListIterator end() {
        return { pre_sentinel_ };
    }

    ConstListIterator cbegin() const {
        return { pre_sentinel_->next };
    }

    ConstListIterator cend() const {
        return { pre_sentinel_ };
    }

    void Sort() {
        *this = Sort(*this);
    }

    Value &Front() {
        return pre_sentinel_->val;
    }

    const Value &Front() const {
        return pre_sentinel_->val;
    }

    Value &Back() {
        return pre_sentinel_->prev->val;
    }

    const Value &Back() const {
        return pre_sentinel_->val;
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
        if (it.Base() == pre_sentinel_) {
            throw std::runtime_error("Erase end");
        }
        ListIterator non_const_it = const_cast<Node*>(it.Base());
        non_const_it.Base()->next->prev = non_const_it.Base()->prev;
        non_const_it.Base()->prev->next = non_const_it.Base()->next;
    }

    size_t Size() const {
        return size_;
    }

private:
    Node *pre_sentinel_;
    size_t size_ = 0;

    static SingleLinkedList Sort(SingleLinkedList l) {
        if (l.Size() <= 1) {
            return l;
        }

        SingleLinkedList left, right;
        bool to_l = true;
        for (const auto &i : l) {
            if (to_l) {
                left.PushBack(i);
            } else {
                right.PushBack(i);
            }
            to_l = !to_l;
        }
        left = Sort(left);
        right = Sort(right);

        auto itl = left.begin();
        auto itr = right.begin();
        SingleLinkedList ans;
        while (itl != left.end() && itr != right.end()) {
            if (*itl < *itr) {
                ans.PushBack(*itl++);
            } else {
                ans.PushBack(*itr++);
            }
        }
        while (itl != left.end()) {
            ans.PushBack(*itl++);
        }
        while (itr != right.end()) {
            ans.PushBack(*itr++);
        }
        return ans;
    }
};
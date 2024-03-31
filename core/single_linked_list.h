#pragma once

#include <bits/stdc++.h>

template<typename Value>
class SingleLinkedList {
private:
    struct Node {
        template<typename... Args>
        Node(Args&&... v)
                : next(nullptr)
                , val(std::forward<Args>(v)...)
        {}

        std::shared_ptr<Node> next;
        Value val;
    };

public:

    class ListIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Value;
        using difference_type = std::ptrdiff_t;
        using pointer = std::shared_ptr<value_type>;
        using reference = value_type&;

        ListIterator() = default;

        ListIterator(std::shared_ptr<Node> ptr)
                : base_(std::move(ptr))
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

        std::shared_ptr<const Node> operator->() const {
            return base_;
        }

        ListIterator operator++() {
            if (base_ == nullptr) {
                throw std::runtime_error("Attempt to dereference null ListIterator");
            }
            return base_ = base_->next;
        }

        ListIterator operator++(int /*fucking_unused*/) {
            if (base_ == nullptr) {
                throw std::runtime_error("Attempt to dereference null ListIterator");
            }
            Node *buf = base_;
            base_ = base_->next;
            return buf;
        }

        bool operator==(const ListIterator&) const = default;
        bool operator!=(const ListIterator&) const = default;

    private:
        std::shared_ptr<Node> base_;
    };

    class ConstListIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Value;
        using difference_type = std::ptrdiff_t;
        using pointer = const std::shared_ptr<value_type>;
        using reference = const value_type&;

        ConstListIterator(std::shared_ptr<const Node> ptr)
                : base_(std::move(ptr))
        {}

        ConstListIterator() = default;

        reference operator*() const {
            return base_->val;
        }

        pointer operator->() {
            return base_;
        }

        const pointer operator->() const {
            return base_;
        }

        ConstListIterator operator++() {
            if (base_ == nullptr) {
                throw std::runtime_error("Attempt to dereference null ConstListIterator");
            }
            return { base_ = std::const_pointer_cast<const Node>(base_->next) };
        }

        ConstListIterator operator++(int /*fucking_unused*/) {
            if (base_ == nullptr) {
                throw std::runtime_error("Attempt to dereference null ConstListIterator");
            }
            std::shared_ptr<const Node> *buf = base_;
            base_ = base_->next;
            return buf;
        }

        bool operator==(const ConstListIterator&) const = default;
        bool operator!=(const ConstListIterator&) const = default;

    private:
        std::shared_ptr<const Node> base_;
    };

    SingleLinkedList() = default;

    SingleLinkedList(const SingleLinkedList<Value> &other) {
        std::for_each(other.begin(), other.end(), [&] (const Value &val) {
            PushBack(val);
        });
    }

    SingleLinkedList(SingleLinkedList &&other) {
        first_ = other.first_;
        last_ = other.last_;
        other.first_.reset();
        other.last_.reset();
    }

    template<typename Iterator>
    SingleLinkedList(Iterator beg, Iterator end) {
        std::for_each(beg, end, [&] (auto x) {
            PushBack(x);
        });
    }

    SingleLinkedList<Value> &operator=(const SingleLinkedList<Value> &other) {
        std::for_each(other.begin(), other.end(), [&] (auto x) {
            PushBack(x);
        });
        return *this;
    }

    SingleLinkedList<Value> &operator=(SingleLinkedList<Value> &&other) noexcept {
        first_ = other.first_;
        last_ = other.last_;
        other.first_.reset();
        other.last_.reset();
        return *this;
    }


    template<typename T>
    void PushBack(T&& val) {
        if (last_ != nullptr) {
            last_->next = std::make_shared<Node>(std::forward<T>(val));
            last_ = last_->next;
        } else {
            last_ = first_ = std::make_shared<Node>(std::forward<T>(val));
        }
    }

    template<typename... Args>
    void EmplaceBack(Args&&... val) {
        last_->next = std::make_shared<Node>(std::forward(val)...);
        last_ = last_->next;
    }

    ConstListIterator begin() const {
        return { first_ };
    }

    ConstListIterator end() const {
        return ConstListIterator();
    }

    ListIterator begin() {
        return first_;
    }

    ListIterator end() {
        return ListIterator();
    }

    ConstListIterator cbegin() const {
        return first_;
    }

    ConstListIterator cend() const {
        return ConstListIterator();
    }

    void Sort() {
        std::tie(first_, last_) = Sort(first_);
    }

    Value &Front() {
        return first_->val;
    }

    const Value &Front() const {
        return first_->val;
    }

    Value &Back() {
        return last_->val;
    }

    const Value &Back() const {
        return last_->val;
    }

    bool Empty() const {
        return first_ == nullptr;
    }

private:
    std::shared_ptr<Node> first_, last_;



    static std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>> Sort(std::shared_ptr<Node> beg) {
        std::shared_ptr<Node> less_beg = nullptr, less_back = nullptr,
                            eq_beg = beg, eq_back = beg,
                            gr_beg = nullptr, gr_back = nullptr;

        beg = beg->next;
        while (beg != nullptr) {
            if (beg->val < eq_back->val) {
                if (less_beg == nullptr) {
                    less_beg = beg;
                } else {
                    less_back->next = beg;
                }
                less_back = beg;
            } else if (eq_back->val < beg->val) {
                if (gr_beg == nullptr) {
                    gr_beg = beg;
                } else {
                    gr_back->next = beg;
                }
                gr_back = beg;
            } else {
                eq_back->next = beg;
                eq_back = beg;
            }
            beg = beg->next;
        }
        if (less_beg != nullptr) {
            less_back->next = nullptr;
            std::tie(less_beg, less_back) = Sort(less_beg);
        }
        if (gr_beg != nullptr) {
            gr_back->next = nullptr;
            std::tie(gr_beg, gr_back) = Sort(gr_beg);
        }
        if (gr_beg != nullptr) {
            gr_back->next = nullptr;
        } else {
            gr_back = eq_back;
        }
        if (less_beg != nullptr) {
            less_back->next = eq_beg;
        } else {
            less_beg = eq_beg;
        }
        if (gr_beg != nullptr) {
            eq_back->next = gr_beg;
        } else {
            eq_back->next = nullptr;
        }

        return std::pair(less_beg, gr_back);
    }
};
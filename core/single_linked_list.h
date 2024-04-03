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

        ~Node() {
            next.reset();
        }

        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;
        Value val;
    };

public:

    class ConstListIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = const Value;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        ConstListIterator(std::weak_ptr<const Node> ptr)
                : base_(std::move(ptr))
        {}

        ConstListIterator() = default;

        reference operator*() const {
            return base_.lock()->val;
        }

        pointer operator->() const {
            return &base_.lock()->val;
        }

        ConstListIterator operator++() {
            if (base_.expired()) {
                throw std::runtime_error("Attempt to dereference null ConstListIterator");
            }
            return { base_ = std::const_pointer_cast<const Node>(base_.lock()->next) };
        }

        ConstListIterator operator--() {
            if (base_.expired()) {
                throw std::runtime_error("Attempt to dereference null ConstListIterator");
            }
            return { base_ = std::const_pointer_cast<const Node>(base_->prev) };
        }

        ConstListIterator operator++(int /*fucking_unused*/) {
            if (base_.expired()) {
                throw std::runtime_error("Attempt to dereference null ConstListIterator");
            }
            std::shared_ptr<const Node> buf = base_;
            base_ = std::const_pointer_cast<const Node>(base_->next);
            return buf;
        }

        ConstListIterator operator--(int /*fucking_unused*/) {
            if (base_ == nullptr) {
                throw std::runtime_error("Attempt to dereference null ConstListIterator");
            }
            std::shared_ptr<const Node> buf = base_;
            base_ = std::const_pointer_cast<const Node>(base_->prev);
            return buf;
        }

        bool operator==(const ConstListIterator &rhs) const {
            return &*base_.lock() == &*rhs.base_.lock();
        }
        bool operator!=(const ConstListIterator &rhs) const {
            return &*base_.lock() != &*rhs.base_.lock();
        }


        std::weak_ptr<const Node> Base() const {
            return base_;
        }

    private:
        std::weak_ptr<const Node> base_;
    };

    class ListIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = Value;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        ListIterator() = default;

        ListIterator(std::weak_ptr<Node> ptr)
                : base_(std::move(ptr))
        {}

        reference operator*() {
            return base_.lock()->val;
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
            if (base_.expired()) {
                throw std::runtime_error("Attempt to dereference null ListIterator");
            }
            return base_ = base_.lock()->next;
        }

        ListIterator operator--() {
            if (base_.expired()) {
                throw std::runtime_error("Attempt to dereference null ListIterator");
            }
            return base_ = base_->prev;
        }

        ListIterator operator++(int /*fucking_unused*/) {
            if (base_.expired()) {
                throw std::runtime_error("Attempt to dereference null ListIterator");
            }
            std::shared_ptr<Node> buf = base_;
            base_ = base_->next;
            return buf;
        }

        ListIterator operator--(int /*fucking_unused*/) {
            if (base_ == nullptr) {
                throw std::runtime_error("Attempt to dereference null ListIterator");
            }
            std::shared_ptr<Node> buf = base_;
            base_ = base_->prev;
            return buf;
        }

        std::weak_ptr<Node> Base() const {
            return base_;
        }

        bool operator==(const ListIterator &rhs) const {
            return &*base_.lock() == &*rhs.base_.lock();
        }
        bool operator!=(const ListIterator &rhs) const {
            return &*base_.lock() != &*rhs.base_.lock();
        }

        operator ConstListIterator() {
            return std::weak_ptr<const Node>(std::const_pointer_cast<const Node>(base_.lock()));
        }

    private:
        std::weak_ptr<Node> base_;
    };

    SingleLinkedList()
        : pre_sentinel_(ConstructSentinel())
    {
    }

    SingleLinkedList(const SingleLinkedList<Value> &other)
            : pre_sentinel_(ConstructSentinel())
    {
        std::for_each(other.begin(), other.end(), [&] (const Value &val) {
            PushBack(val);
        });
    }

    SingleLinkedList(SingleLinkedList &&other) {
        pre_sentinel_ = other.pre_sentinel_;
        other.pre_sentinel_ = ConstructSentinel();
    }

    template<typename Iterator>
    SingleLinkedList(Iterator beg, Iterator end)
            : pre_sentinel_(ConstructSentinel())
    {
        std::for_each(beg, end, [&] (auto x) {
            PushBack(x);
        });
    }

    ~SingleLinkedList() {
        pre_sentinel_->next.reset();
    }

    SingleLinkedList<Value> &operator=(const SingleLinkedList<Value> &other) {
        std::for_each(other.begin(), other.end(), [&] (auto x) {
            PushBack(x);
        });
        return *this;
    }

    SingleLinkedList<Value> &operator=(SingleLinkedList<Value> &&other) noexcept {
        pre_sentinel_ = other.pre_sentinel_;
        other.pre_sentinel_ = ConstructSentinel();
        return *this;
    }

    template<typename... Args>
    void EmplaceBack(Args&&... val) {
        pre_sentinel_->prev.lock()->next = std::make_shared<Node>(std::forward<Args>(val)...);
        pre_sentinel_->prev = pre_sentinel_->prev.lock()->next;
        pre_sentinel_->prev.lock()->next->prev = pre_sentinel_->prev.lock()->next;
        pre_sentinel_->prev.lock()->next->next = pre_sentinel_;
    }

    template<typename T>
    void PushBack(T&& val) {
        EmplaceBack(std::forward<T>(val));
    }

    ConstListIterator begin() const {
        return { pre_sentinel_->next };
    }

    ConstListIterator end() const {
        return ConstListIterator();
    }

    ListIterator begin() {
        return {pre_sentinel_->next};
    }

    ListIterator end() {
        return ListIterator();
    }

    ConstListIterator cbegin() const {
        return { pre_sentinel_->next };
    }

    ConstListIterator cend() const {
        return ConstListIterator();
    }

    void Sort() {
        std::tie(pre_sentinel_->next, pre_sentinel_->prev) = Sort(pre_sentinel_->next);
    }

    Value &Front() {
        return pre_sentinel_->val;
    }

    const Value &Front() const {
        return pre_sentinel_->val;
    }

    Value &Back() {
        return pre_sentinel_->prev.lock()->val;
    }

    const Value &Back() const {
        return pre_sentinel_.lock()->val;
    }

    bool Empty() const {
        return pre_sentinel_ == nullptr;
    }


    void PopFront() {
        if (pre_sentinel_ == nullptr) {
            throw std::runtime_error("Tried PopFront for empty SingleLinkedList");
        }
        pre_sentinel_ = pre_sentinel_->next;
        pre_sentinel_->prev.reset();
    }

    template<typename T>
    void InsertBefore(ConstListIterator it, T &&val) {
        std::shared_ptr<Node> new_node = std::make_shared<Node>(std::forward<T>(val));
        ListIterator non_const_it = std::weak_ptr<Node>(std::const_pointer_cast<Node>(it.Base().lock()));
        new_node->prev = non_const_it.Base().lock()->prev;
        new_node->next = non_const_it.Base().lock();
        non_const_it.Base().lock()->prev.lock()->next = new_node;
        non_const_it.Base().lock()->prev = new_node;
    }

    void Erase(ConstListIterator it) {
//        ListIterator non_const_it = SharedConstCast(it.base_);
//        non_const_it.Base()->next->prev = non_const_it.Base()->prev;
//        non_const_it.Base()->prev->next = non_const_it.Base()->next;
    }

private:
    std::shared_ptr<Node> pre_sentinel_;

    static std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>> Sort(std::shared_ptr<Node> beg) {
        if (beg->next == nullptr) return std::pair(beg, beg);

        std::shared_ptr<Node> turtle = beg, rabbit = beg;
        while (rabbit != nullptr && rabbit->next != nullptr) {
            rabbit = rabbit->next->next;
            turtle = turtle->next;
        }
        turtle->prev.lock()->next = nullptr;

        std::shared_ptr<Node> l, ml, mr, r;
        std::tie(l, ml) = Sort(beg);
        std::tie(mr, r) = Sort(turtle);
        return Merge(l, mr);
    }

    static std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>> Merge(std::shared_ptr<Node> beg1, std::shared_ptr<Node> beg2) {
        std::shared_ptr<Node> ans_back = nullptr, ans_beg = nullptr;
        while (beg1 != nullptr && beg2 != nullptr) {
            if (beg1->val < beg2->val) {
                if (ans_back == nullptr) {
                    ans_back = beg1;
                    ans_beg = ans_back;
                } else {
                    ans_back->next = beg1;
                    beg1->prev = ans_back;
                    ans_back = beg1;
                }
                beg1 = beg1->next;
            } else {
                if (ans_back == nullptr) {
                    ans_back = beg2;
                    ans_beg = ans_back;
                } else {
                    ans_back->next = beg2;
                    beg2->prev = ans_back;
                    ans_back = beg2;
                }
                beg2 = beg2->next;
            }
        }
        while (beg1 != nullptr) {
            if (ans_back == nullptr) {
                ans_back = beg1;
                ans_beg = ans_back;
            } else {
                ans_back->next = beg1;
                beg1->prev = ans_back;
                ans_back = beg1;
            }
            beg1 = beg1->next;
        }
        while (beg2 != nullptr) {
            if (ans_back == nullptr) {
                ans_back = beg2;
                ans_beg = ans_back;
            } else {
                ans_back->next = beg2;
                beg2->prev = ans_back;
                ans_back = beg2;
            }
            beg2 = beg2->next;
        }
        ans_beg->prev.reset();
        ans_back->next.reset();
        return std::pair(ans_beg, ans_back);
    }

    static std::shared_ptr<Node> ConstructSentinel() {
        std::shared_ptr<Node> ptr = std::make_shared<Node>();
        ptr->next = ptr;
        ptr->prev = ptr;
        return ptr;
    }
};
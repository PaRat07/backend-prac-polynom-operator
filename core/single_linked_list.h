#pragma once

#include <algorithm>
#include <memory>

template<typename Value>
class SingleLinkedList {
private:
    struct Node {
        template<typename... Args>
        Node(Args&&... v)
                : next(nullptr)
                , prev(nullptr)
                , val(std::forward<Args>(v)...)
        {}

        std::shared_ptr<Node> next, prev;
        Value val;
    };

public:

    class ListIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = Value;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
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

        ListIterator operator--() {
            if (base_ == nullptr) {
                throw std::runtime_error("Attempt to dereference null ListIterator");
            }
            return base_ = base_->prev;
        }

        ListIterator operator++(int /*fucking_unused*/) {
            if (base_ == nullptr) {
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

        std::shared_ptr<Node> Base() const {
            return base_;
        }

        bool operator==(const ListIterator&) const = default;
        bool operator!=(const ListIterator&) const = default;

    private:
        std::shared_ptr<Node> base_;
    };

    class ConstListIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = const Value;
        using difference_type = std::ptrdiff_t;
        using pointer = std::shared_ptr<value_type>;
        using reference = value_type&;

        ConstListIterator(std::shared_ptr<const Node> ptr)
                : base_(std::move(ptr))
        {}

        ConstListIterator() = default;

        reference operator*() const {
            return base_->val;
        }

        pointer operator->() const {
            return base_;
        }

        ConstListIterator operator++() {
            if (base_ == nullptr) {
                throw std::runtime_error("Attempt to dereference null ConstListIterator");
            }
            return { base_ = std::const_pointer_cast<const Node>(base_->next) };
        }

        ConstListIterator operator--() {
            if (base_ == nullptr) {
                throw std::runtime_error("Attempt to dereference null ConstListIterator");
            }
            return { base_ = std::const_pointer_cast<const Node>(base_->prev) };
        }

        ConstListIterator operator++(int /*fucking_unused*/) {
            if (base_ == nullptr) {
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

        bool operator==(const ConstListIterator&) const = default;
        bool operator!=(const ConstListIterator&) const = default;

        std::shared_ptr<const Node> Base() const {
            return base_;
        }

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
        last_->next = std::make_shared<Node>(std::forward<Args>(val)...);
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


    void PopFront() {
        if (first_ == nullptr) {
            throw std::runtime_error("Tried PopFront for empty SingleLinkedList");
        }
        first_ = first_->next;
    }

    template<typename T>
    void Insert(ConstListIterator it, T &&val) {
        std::shared_ptr<Node> new_node = std::make_shared<Node>(std::forward<T>(val));
        if (it.base_ != nullptr) {

        }
    }

private:
    std::shared_ptr<Node> first_, last_;


    static bool Cmp(ConstListIterator lhs, ConstListIterator rhs, bool(*)(Value,Value)) {

    }

    static std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>> Sort(std::shared_ptr<Node> beg) {
        if (beg->next == nullptr) return std::pair(beg, beg);

        std::shared_ptr<Node> turtle = beg, rabbit = beg;
        while (rabbit != nullptr && rabbit->next != nullptr) {
            rabbit = rabbit->next->next;
            turtle = turtle->next;
        }
        turtle->prev->next = nullptr;

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
        ans_beg->prev = nullptr;
        ans_back->next = nullptr;
        return std::pair(ans_beg, ans_back);
    }
};
#include <bits/stdc++.h>

template<typename Value>
class SingleLinkedList {
private:
    struct Node {
        template<typename... Args>
        Node(Args&&... v)
            : next(nullptr)
            , val(std::forward(v...))
        {}

        Node *next;
        Value val;
    };

public:
    SingleLinkedList(const SingleLinkedList<Value> &other) {

    }

    template<typename Iterator>
    SingleLinkedList(Iterator beg, Iterator end) {
        std::for_each(beg, end, [&] ( n  / x) {
            PushBack(x);
        });
    }


    template<typename T>
    void PushBack(T&& val) {
        last_->next = new Node(std::forward(val));
        last_ = last_->next;
    }

    template<typename... Args>
    void EmplaceBack(Args&&... val) {
        last_->next = new Node(std::forward(val...));
        last_ = last_->next;
    }



private:
    Node *first_ = nullptr, *last_ = nullptr;
};
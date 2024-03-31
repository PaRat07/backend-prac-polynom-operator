#pragma once

#include "bits/stdc++.h"

#include "monomial.h"

class Parser {
public:
    Parser(std::string data)
        : data_((!data.empty() && data[0] != '-' ? '+' + data : data))
        , to_pares_(data_)
    {}


    Monomial GetMonomial() {
        State state;
        while (!to_pares_.empty()) {
            switch (to_pares_[0]) {
                case '^': {
                    switch (state.prev) {
                        case Prev::CARET:
                        case Prev::NOTHING:
                        case Prev::SIGN:
                            throw std::invalid_argument("Unexpected caret at position " + std::to_string(data_.size() - to_pares_.size()));
                    }
                    state.prev = Prev::CARET;
                    break;
                }
                case '-': {
                    if (state.prev == Prev::NOTHING) {
                        state.is_gr0 = false;
                    }
                }
                case '+': {
                    if (state.prev != Prev::NOTHING) {
                        if (state.cur_letter != '#') {
                            state.cur_ans.powers[state.cur_letter - 'a'] = (state.cur_num.has_value() ? *state.cur_num : 1);
                        } else {
                            state.cur_ans.factor = (state.cur_num.has_value() ? *state.cur_num : 1);
                        }
                        goto end;
                    }
                    state.prev = Prev::SIGN;
                    break;
                }
                default: {
                    if (to_pares_[0] >= '0' && to_pares_[0] <= '9') {
                        switch (state.prev) {
                            case Prev::LETTER:
                            case Prev::NOTHING:
                                throw std::invalid_argument("Unexpected number at position " + std::to_string(data_.size() - to_pares_.size()));
                        }
                        if (!state.cur_num.has_value()) {
                            state.cur_num = 0;
                        }
                        *state.cur_num *= 10;
                        *state.cur_num += to_pares_[0] - '0';
                    } else if (to_pares_[0] >= 'a' && to_pares_[0] <= 'z') {
                        switch (state.prev) {
                            case Prev::CARET:
                                if (state.cur_num.has_value()) {
                                    state.cur_ans.powers[state.cur_letter - 'a'] = (state.cur_num.has_value() ? *state.cur_num : 1);
                                    state.cur_num.reset();
                                    break;
                                }
                            case Prev::NOTHING:
                                throw std::invalid_argument("Unexpected letter at position " + std::to_string(data_.size() - to_pares_.size()));
                            case Prev::LETTER:
                                state.cur_ans.powers[state.cur_letter - 'a'] = 1;
                                break;
                            case Prev::SIGN:
                                state.cur_ans.factor = *state.cur_num;
                                if (!state.is_gr0) {
                                    state.cur_ans.factor *= -1;
                                }
                                state.cur_num.reset();
                        }
                        state.cur_letter = to_pares_[0];
                        state.prev = Prev::LETTER;
                    } else {
                        throw std::invalid_argument("Unexpected character" + std::string(to_pares_.substr(0, 1)) + " at pos " + std::to_string(data_.size() - to_pares_.size()));
                    }
                    break;
                }
            }
            to_pares_.remove_prefix(1);

        }
        if (state.cur_letter != '#') {
            state.cur_ans.powers[state.cur_letter - 'a'] = (state.cur_num.has_value() ? *state.cur_num : 1);
        }
        end:;
        return state.cur_ans;
    }

    bool Empty() const {
        return to_pares_.empty();
    }

private:
    std::string data_;
    std::string_view to_pares_;

    enum class Prev {
        LETTER,
        SIGN,
        CARET,
        NOTHING
    };

    struct State {
        Prev prev = Prev::NOTHING;
        Monomial cur_ans;
        char cur_letter= '#';
        bool is_gr0 = true;
        std::optional<int> cur_num;
    };
};
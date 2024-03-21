#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <future>
#include <optional>
#include <cmath>

#include "window.h"
#include "general_data.h"
#include "colors.h"



class WindowManager {
    struct Animation {
        size_t from, to;
        std::chrono::time_point<std::chrono::steady_clock> beg, end;
    };

public:
    void AddWindow(Window win) {
        windows_.push_back(std::move(win));
    }

    void Start();

private:
    size_t active_ = 0;
    std::optional<Animation> anim_;
    std::vector<Window> windows_;

    static double GetPercantage(double time_gone);
};

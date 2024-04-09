//
// Created by pablo on 20.02.24.
//

#include "../app/window.h"

void Window::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // for (auto &i : elements_) {
    //     target.draw(*i);
    // }

    std::for_each(elements_.begin(), elements_.end(), [&target] (const std::unique_ptr<AbstractElement> &ptr) {
        target.draw(*ptr);
    });
}

void Window::ProcessEvent(sf::Event event) {
    // for (auto &i : elements_) {
    //     i->ProcessEvent(event, target);
    // }

    std::for_each(elements_.begin(), elements_.end(), [event] (const std::unique_ptr<AbstractElement> &ptr) {
        ptr->ProcessEvent(event);
    });
}

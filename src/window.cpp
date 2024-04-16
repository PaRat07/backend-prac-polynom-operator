//
// Created by pablo on 20.02.24.
//

#include "../app/tab.h"

void Tab::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // for (auto &i : elements_) {
    //     target.draw(*i);
    // }

    std::for_each(elements_.begin(), elements_.end(), [&target] (const std::unique_ptr<AbstractDrawer> &ptr) {
        target.draw(*ptr);
    });
}

void Tab::ProcessEvent(sf::Event event) {
    // for (auto &i : elements_) {
    //     i->ProcessEvent(event, target);
    // }

    std::for_each(elements_.begin(), elements_.end(), [event] (const std::unique_ptr<AbstractDrawer> &ptr) {
        ptr->ProcessEvent(event);
    });
}

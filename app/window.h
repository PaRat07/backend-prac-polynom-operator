#pragma once

#include <memory>
#include <vector>

#include "SFML/Graphics.hpp"


class AbstractElement : public sf::Drawable {
public:
    virtual void ProcessEvent(sf::Event event) = 0;
    virtual ~AbstractElement() = default;
};

class Window : public sf::Drawable {
public:
    Window(const Window &other) = delete;
    Window(Window &&other) = default;
    Window() = default;

    template<class Elem>
    void AddElement(std::unique_ptr<Elem> new_elem) {
        elements_.emplace_back(std::move(new_elem));
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void ProcessEvent(sf::Event event);

private:
    std::vector<std::unique_ptr<AbstractElement>> elements_;
};

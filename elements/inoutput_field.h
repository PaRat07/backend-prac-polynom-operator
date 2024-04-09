#pragma once

#include <functional>

#include "SFML/Graphics.hpp"

#include "rounded_rectangle.h"
#include "../app/general_data.h"
#include "../app/window.h"
#include "center_positioned_string.h"


class InputField : public AbstractElement {
public:
    InputField(sf::Vector2f pos, sf::Vector2f size, std::string label);

    void ProcessEvent(sf::Event event) override;

    void SetPosition(sf::Vector2f pos) {
        pos_ = {pos.x / win_size.x, pos.y / win_size.y};
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void Write(sf::Uint32 event);

    void Activate() {
        active_ = true;
    }

    const std::string &GetText() const {
        return data_;
    }

private:
    sf::Vector2f pos_, size_;
    std::string data_;
    std::string label_;
    bool active_ = false;
};



class OutputField : public AbstractElement {
public:
    OutputField(sf::Vector2f pos, sf::Vector2f size);

    void ProcessEvent(sf::Event event) override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void SetText(std::string s);

private:
    sf::Vector2f pos_, size_;
    std::string data_;
    bool active_ = false;
};
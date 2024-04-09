#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>

template<int round_radius>
class RoundedRectangleShape : public sf::Drawable {
 public:
    explicit RoundedRectangleShape(sf::Vector2f size);

    void setSize(sf::Vector2f new_size);

    const sf::Vector2f &getSize() const {
        return size_;
    }

    void setPosition(sf::Vector2f new_pos);

    const sf::Vector2f &getPosition() const {
        return pos_;
    }

    void setFillColor(sf::Color new_color);

    void setOutlineColor(sf::Color new_color);

    const sf::Color &getFillColor() const {
        return fill_color_;
    }

    const sf::Color &getOulineColor() const {
        return outline_color_;
    }

    void setOutlineThickness(float thick);

    float getOutlineThickness() const {
        return outline_thickness_;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
    float outline_thickness_ = 1;
    sf::Vector2f pos_{ 0.f, 0.f}, size_;
    sf::Color outline_color_ = sf::Color::Black, fill_color_ = sf::Color::Transparent;
};

template<int round_radius>
RoundedRectangleShape<round_radius>::RoundedRectangleShape(sf::Vector2f size)
        : size_(size)
{
}

template<int round_radius>
void RoundedRectangleShape<round_radius>::setSize(sf::Vector2f new_size) {
    size_ = new_size;
}

template<int round_radius>
void RoundedRectangleShape<round_radius>::setPosition(sf::Vector2f new_pos) {
    pos_ = new_pos;
}

template<int round_radius>
void RoundedRectangleShape<round_radius>::setFillColor(sf::Color new_color) {
    if (new_color == sf::Color::Transparent) throw std::runtime_error("RoundedRectangleShape doesn't support transparent fill color");
    fill_color_ = new_color;
}

template<int round_radius>
void RoundedRectangleShape<round_radius>::setOutlineColor(sf::Color new_color) {
    outline_color_ = new_color;
}

template<int round_radius>
void RoundedRectangleShape<round_radius>::setOutlineThickness(float thick) {
    outline_thickness_ = thick;
}

template<int round_radius>
void RoundedRectangleShape<round_radius>::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::CircleShape lu, ld, ru, rd;

    lu.setPointCount(50);
    ld.setPointCount(50);
    ru.setPointCount(50);
    rd.setPointCount(50);

    lu.setRadius(round_radius);
    ld.setRadius(round_radius);
    ru.setRadius(round_radius);
    rd.setRadius(round_radius);

    lu.setPosition(pos_.x + outline_thickness_, pos_.y + outline_thickness_);
    ld.setPosition(pos_.x + outline_thickness_, pos_.y + size_.y - round_radius * 2 - outline_thickness_);
    ru.setPosition(pos_.x + size_.x - round_radius * 2 - outline_thickness_, pos_.y + outline_thickness_);
    rd.setPosition(pos_.x + size_.x - round_radius * 2 - outline_thickness_, pos_.y + size_.y - round_radius * 2 - outline_thickness_);

    lu.setOutlineColor(outline_color_);
    ld.setOutlineColor(outline_color_);
    ru.setOutlineColor(outline_color_);
    rd.setOutlineColor(outline_color_);

    lu.setOutlineThickness(outline_thickness_);
    ld.setOutlineThickness(outline_thickness_);
    ru.setOutlineThickness(outline_thickness_);
    rd.setOutlineThickness(outline_thickness_);

    lu.setFillColor(fill_color_);
    ld.setFillColor(fill_color_);
    ru.setFillColor(fill_color_);
    rd.setFillColor(fill_color_);

    target.draw(lu);
    target.draw(ld);
    target.draw(ru);
    target.draw(rd);

    sf::RectangleShape horizontal(sf::Vector2f(size_.x - outline_thickness_ * 2, size_.y - round_radius * 2 - outline_thickness_ * 2));
    horizontal.setPosition(pos_.x + outline_thickness_, pos_.y + round_radius + outline_thickness_);
    horizontal.setFillColor(fill_color_);
    target.draw(horizontal);

    sf::RectangleShape vertical(sf::Vector2f(size_.x - round_radius * 2 - outline_thickness_ * 2, size_.y - outline_thickness_ * 2));
    vertical.setPosition(pos_.x + round_radius + outline_thickness_, pos_.y + outline_thickness_);
    vertical.setFillColor(fill_color_);
    target.draw(vertical);

    sf::RectangleShape u(sf::Vector2f(size_.x - round_radius * 2, outline_thickness_));
    u.setFillColor(outline_color_);
    u.setPosition(pos_.x + round_radius, pos_.y);
    target.draw(u);

    sf::RectangleShape d(sf::Vector2f(size_.x - round_radius * 2, outline_thickness_));
    d.setFillColor(outline_color_);
    d.setPosition(pos_.x + round_radius, pos_.y + size_.y - outline_thickness_);
    target.draw(d);

    sf::RectangleShape l(sf::Vector2f(outline_thickness_, size_.y - round_radius * 2));
    l.setFillColor(outline_color_);
    l.setPosition(pos_.x, pos_.y + round_radius);
    target.draw(l);

    sf::RectangleShape r(sf::Vector2f(outline_thickness_, size_.y - round_radius * 2));
    r.setFillColor(outline_color_);
    r.setPosition(pos_.x + size_.x - outline_thickness_, pos_.y + round_radius);
    target.draw(r);
}

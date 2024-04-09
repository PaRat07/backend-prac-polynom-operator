#include "../elements/inoutput_field.h"


InputField::InputField(sf::Vector2f pos, sf::Vector2f size, std::string label)
        : pos_(pos.x / win_size.x, pos.y / win_size.y)
        , size_(size.x / win_size.x, size.y / win_size.y)
        , label_(std::move(label))
{
}



void InputField::ProcessEvent(sf::Event event) {
    switch (event.type) {
        case sf::Event::MouseButtonPressed: {
            sf::Vector2f pos(event.touch.x / win_size.x, event.touch.y / win_size.y);
            active_ = std::abs(pos.x - (pos_.x + size_.x / 2)) <= size_.x / 2 &&
                      std::abs(pos.y - (pos_.y + size_.y / 2)) <= size_.y / 2;
            break;
        }
        case sf::Event::TextEntered:
            Write(event.text.unicode);
            break;
    }
}


void InputField::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Vector2f real_size(size_.x * win_size.x, size_.y * win_size.y), real_pos(pos_.x * win_size.x,
                                                                                 pos_.y * win_size.y);
    RoundedRectangleShape<3> rect(real_size);
    rect.setPosition(real_pos);
    rect.setOutlineThickness(0);
    rect.setFillColor(surface_container_highest);
    target.draw(rect);

    if (active_) {
        sf::RectangleShape under(sf::Vector2f(real_size.x, 3));
        under.setPosition(real_pos.x, real_pos.y + real_size.y - 3);;
        under.setFillColor(on_surface_variant);
        target.draw(under);

        sf::Text data;
        data.setString(data_);
        data.setFont(font);
        data.setCharacterSize(letter_size);
        data.setFillColor(on_surface);
        data.setPosition(real_pos.x + 10, real_pos.y + real_size.y * 0.5 - data.getLocalBounds().height * 0.5);
        target.draw(data);

        sf::Text label;
        data.setString(label_);
        data.setFont(font);
        data.setCharacterSize(letter_size / 2);
        data.setFillColor(on_surface);
        data.setPosition(real_pos.x + 10, real_pos.y + real_size.y * 0.25 - data.getLocalBounds().height * 0.5);
        target.draw(data);
    } else {
        sf::RectangleShape under(sf::Vector2f(real_size.x, 3));
        under.setPosition(real_pos.x, real_pos.y + real_size.y - 3);
        under.setFillColor(on_surface);
        target.draw(under);

        CenterPositionedString label;
        label.setString(label_);
        label.setTextColor(on_surface);
        label.setPosition(real_pos.x + real_size.x / 2, real_pos.y + real_size.y / 2);
        target.draw(label);
    }
}


void InputField::Write(sf::Uint32 event) {
    if (active_) {
        switch (event) {
            // Backspace
            case 8:
                if (!data_.empty()) {
                    data_.pop_back();
                }
                break;
                // Enter
            case 42:
                return;
            default:
                data_.push_back(static_cast<char>(event));
        }
    }
}


OutputField::OutputField(sf::Vector2f pos, sf::Vector2f size)
        : pos_(pos.x / win_size.x, pos.y / win_size.y)
        , size_(size.x / win_size.x, size.y / win_size.y)
{}



void OutputField::ProcessEvent(sf::Event event) {
    sf::Vector2f pos(event.touch.x / win_size.x, event.touch.y / win_size.y);
    active_ = std::abs(pos.x - (pos_.x + size_.x / 2)) <= size_.x / 2 &&
              std::abs(pos.y - (pos_.y + size_.y / 2)) <= size_.y / 2;
}


void OutputField::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::RectangleShape rect(sf::Vector2f(size_.x * win_size.x, size_.y * win_size.y));
    rect.setPosition(sf::Vector2f(pos_.x * win_size.x, pos_.y * win_size.y));
    rect.setOutlineThickness(0);
    rect.setFillColor(primary);
    rect.setOutlineThickness(2);
    target.draw(rect);

    sf::Text text;
    text.setFont(font);
    text.setString(data_);
    text.setCharacterSize(letter_size);
    text.setPosition(sf::Vector2f(pos_.x, pos_.y + size_.y / 2));
    text.setFillColor(on_primary);
    target.draw(text);
}


void OutputField::SetText(std::string s) {
    data_ = std::move(s);
}

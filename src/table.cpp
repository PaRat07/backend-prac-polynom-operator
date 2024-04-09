#include "../elements/table.h"

sf::Vector2i TableElement::cell_size = {90, 40 };

TableElement::TableElement(sf::Vector2f pos, sf::Vector2f size, Table &machine)
        : machine_(machine)
        , pos_(pos.x / win_size.x, pos.y / win_size.y)
        , size_(size.x / win_size.x, size.y / win_size.y)
{}

void TableElement::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Vector2i real_size(size_.x * win_size.x, size_.y * win_size.y);

    {
        RoundedRectangleShape<10> frame{sf::Vector2f(real_size.x - 4, real_size.y - 4)};
        frame.setPosition(sf::Vector2f(pos_.x * win_size.x + 2, pos_.y * win_size.y + 2));
        frame.setFillColor(surface_container);
        frame.setOutlineThickness(0);
        target.draw(frame);
    }

    // net + vals
    {
        sf::Sprite net_sprite;
        sf::RenderTexture net_texture;
        net_texture.setActive();
        net_texture.create(real_size.x - cell_size.x  * 2 - 24, real_size.y - cell_size.y * 2 - 24);
        net_texture.clear(surface_container);
        sf::RectangleShape line(sf::Vector2f(std::min(real_size.x - 24, machine_.Size().x * cell_size.x), 2));
        line.setFillColor(outline);
        for (int i = pos_in_.y % cell_size.y; i < std::min(real_size.y, machine_.Size().y * cell_size.y); i += cell_size.y) {
            line.setPosition(0, i);
            net_texture.draw(line);
        }
        net_texture.display();
        net_sprite.setPosition(12 + cell_size.x, cell_size.y + 12);

        line.setSize(sf::Vector2f(2, std::min(real_size.y - 24, machine_.Size().y * cell_size.y)));
        for (int i = pos_in_.x % cell_size.x; i <= std::min(real_size.x, machine_.Size().x * cell_size.x); i += cell_size.x) {
            line.setPosition(i, 0);
            net_texture.draw(line);
        }

        {
            CenterPositionedString str;
            for (int i = -pos_in_.x / cell_size.x; i * cell_size.x + pos_in_.x < real_size.x && i < machine_.Size().x; ++i) {
                for (int j = -pos_in_.y / cell_size.y; j * cell_size.y + pos_in_.y < real_size.y && j < machine_.Size().y; ++j) {
                    str.setPosition(i * cell_size.x + cell_size.x / 2 + pos_in_.x,
                                    j * cell_size.y + cell_size.y / 2 + pos_in_.y);
                    str.setString(machine_.GetValue(sf::Vector2i(i, j)));
                    net_texture.draw(str);
                }
            }
        }
        if (field.has_value()) {
            net_texture.draw(*field);
        }
        net_texture.display();
        net_sprite.setTexture(net_texture.getTexture());
        net_sprite.setPosition(cell_size.x * 2 + 12 + pos_.x * win_size.x, cell_size.y * 2 + 12 + pos_.y * win_size.y);
        target.draw(net_sprite);
    }

    buttons_qs_mutex_.lock();
    buttons_qs_.clear();
    for (int i = 0; i < machine_.Size().y; ++i) {
        ButtonWithTextAbsPos erase_q(sf::Vector2f(12 + cell_size.x * 0.5, (i + 0.5) * cell_size.y + pos_in_.y),
                                     sf::Vector2f(40, 30),
                                     "del",
                                     std::bind(&Table::EraseLine, &machine_, i));
        buttons_qs_.push_back(erase_q);
    }
    buttons_qs_mutex_.unlock();
    // qs
    {
        sf::RenderTexture qs;
        qs.create(cell_size.x * 2, real_size.y - cell_size.y * 2 - 24);
        qs.clear(sf::Color::Transparent);
        CenterPositionedString str;
        for (int i = -pos_in_.y / cell_size.y; i < machine_.Size().y && i * cell_size.y + pos_in_.y < std::min(real_size.y, machine_.Size().y * cell_size.y); ++i) {
            str.setString(machine_.GetLineName(i));
            str.setPosition(sf::Vector2f(cell_size.x * 1.5, i * cell_size.y + cell_size.y / 2 + pos_in_.y));
            qs.draw(str);
            qs.draw(buttons_qs_[i]);
        }
        sf::RectangleShape line(sf::Vector2f(cell_size.x * 2, 2));
        line.setFillColor(outline);

        for (int i = pos_in_.y % cell_size.y; i < std::min(real_size.y, machine_.Size().y * cell_size.y); i += cell_size.y) {
            line.setPosition(0, i);
            qs.draw(line);
        }
        qs.display();
        sf::Sprite sprite;
        sprite.setPosition(12 + pos_.x * win_size.x,  cell_size.y * 2 + 12 + pos_.y * win_size.y);
        sprite.setTexture(qs.getTexture());
        target.draw(sprite);
    }

    buttons_syms_mutex_.lock();
    buttons_syms_.clear();
    for (int i = 0; i < machine_.Size().x; ++i) {
        ButtonWithTextAbsPos erase_sym(sf::Vector2f((i + 0.5) * cell_size.x + pos_in_.x, cell_size.y / 2),
                                       sf::Vector2f(40, 30),
                                       "del",
                                       std::bind(&Table::EraseColumn, &machine_, i));
        buttons_syms_.push_back(erase_sym);
    }
    buttons_syms_mutex_.unlock();
    // syms
    {
        sf::RenderTexture syms;
        syms.create(real_size.x - cell_size.x * 2 - 24, cell_size.y * 2);
        syms.clear(sf::Color::Transparent);
        CenterPositionedString str;
        for (int i = -pos_in_.x / cell_size.x; i < machine_.Size().x && i * cell_size.x + pos_in_.x < std::min(real_size.x, machine_.Size().x * cell_size.x) && i < buttons_syms_.size(); ++i) {
            str.setString(machine_.GetColumnName(i));
            str.setPosition(sf::Vector2f(i * cell_size.x + cell_size.x / 2 + pos_in_.x, cell_size.y * 1.5));
            syms.draw(str);
            syms.draw(buttons_syms_[i]);
        }
        sf::RectangleShape line(sf::Vector2f(2, cell_size.y * 2));
        line.setFillColor(outline);

        for (int i = pos_in_.x % cell_size.x; i < std::min(real_size.x, machine_.Size().x * cell_size.x); i += cell_size.x) {
            line.setPosition(i, 0);
            syms.draw(line);
        }
        syms.display();
        sf::Sprite sprite;
        sprite.setPosition(cell_size.x * 2 + 12 + pos_.x * win_size.x, 12 + pos_.y * win_size.y);
        sprite.setTexture(syms.getTexture());
        target.draw(sprite);
    }

    // ver line(not texture)
    {
        sf::RectangleShape legend_border(sf::Vector2f(std::min(real_size.x - 24, machine_.Size().x * cell_size.x), 2));
        legend_border.setFillColor(outline);
        target.draw(legend_border);
        legend_border.setPosition(sf::Vector2f(pos_.x * win_size.x + 12, pos_.y * win_size.y + 12 + cell_size.y * 2));
        target.draw(legend_border);
    }

    // hor line(not texture)
    {
        sf::RectangleShape legend_border(sf::Vector2f(2, std::min(real_size.y - 24, machine_.Size().y * cell_size.y)));
        legend_border.setFillColor(outline);
        target.draw(legend_border);
        legend_border.setPosition(sf::Vector2f(pos_.x * win_size.x + 12 + cell_size.x * 2, pos_.y * win_size.y + 12));
        target.draw(legend_border);
    }

    // frame
    {
        sf::RectangleShape table_border(sf::Vector2f(std::min(real_size.x - 24, (machine_.Size().x + 2) * cell_size.x - 2), std::min(real_size.y - 24, (machine_.Size().y + 2) * cell_size.y - 2)));
        table_border.setPosition(sf::Vector2f(pos_.x * win_size.x + 14, pos_.y * win_size.y + 14));
        table_border.setFillColor(sf::Color::Transparent);
        table_border.setOutlineThickness(2);
        table_border.setOutlineColor(outline);
        target.draw(table_border);
    }
}

void TableElement::ProcessEvent(sf::Event event) {
    switch (event.type) {
        case sf::Event::EventType::MouseWheelScrolled: {
            if (event.mouseWheelScroll.wheel == sf::Mouse::Wheel::VerticalWheel) {
                pos_in_.y += event.mouseWheelScroll.delta * 3;
                pos_in_.y = std::max(pos_in_.y, (int) (-GetTableSize().y + size_.y * win_size.y - 24));
                pos_in_.y = std::min(pos_in_.y, 0);
            } else {
                pos_in_.x += event.mouseWheelScroll.delta * 3;
                pos_in_.x = std::max(pos_in_.x, (int) (-GetTableSize().x + size_.x * win_size.x - 24));
                pos_in_.x = std::min(pos_in_.x, 0);
            }
            field->SetPosition(sf::Vector2f(active_pos_->x * cell_size.x + pos_in_.x + 2,
                                                active_pos_->y * cell_size.y + pos_in_.y + 2));
            break;
        }
        case sf::Event::EventType::MouseButtonPressed: {
            {
                sf::Event ev_for_buts = event;
                ev_for_buts.mouseButton.y -= 12;
                ev_for_buts.mouseButton.x -= cell_size.x * 2 + 12;
                buttons_syms_mutex_.lock();
                std::for_each(buttons_syms_.begin(), buttons_syms_.end(),
                              [&ev_for_buts](ButtonWithTextAbsPos &but) {
                                  but.ProcessEvent(ev_for_buts);
                              });
                buttons_syms_mutex_.unlock();
                ev_for_buts = event;
                ev_for_buts.mouseButton.y -= cell_size.y * 2 + 12;
                ev_for_buts.mouseButton.x -= 12;
                buttons_qs_mutex_.lock();
                std::for_each(buttons_qs_.begin(), buttons_qs_.end(),
                              [&ev_for_buts](ButtonWithTextAbsPos &but) {
                                  but.ProcessEvent(ev_for_buts);
                              });
                buttons_qs_mutex_.unlock();
            }
            pos_in_.x = std::max(pos_in_.x, (int) (-GetTableSize().x + size_.x * win_size.x - 24));
            pos_in_.x = std::min(pos_in_.x, 0);
            pos_in_.y = std::max(pos_in_.y, (int) (-GetTableSize().y + size_.y * win_size.y - 24));
            pos_in_.y = std::min(pos_in_.y, 0);

            sf::Vector2f pos(event.touch.x / win_size.x, event.touch.y / win_size.y);
            if (std::abs(pos_.x + size_.x / 2 - pos.x) <= size_.x / 2 &&
                std::abs(pos_.x + size_.x / 2 - pos.x) <= size_.x / 2 &&
                (event.mouseButton.x - pos_in_.x - 14 - cell_size.x) / cell_size.x - 1 < machine_.Size().x &&
                (event.mouseButton.y - pos_in_.y - 14 - cell_size.y) / cell_size.y - 1 < machine_.Size().y) {
                active_pos_.emplace((event.mouseButton.x - pos_in_.x - 14 - cell_size.x) / cell_size.x - 1,
                                    (event.mouseButton.y - pos_in_.y - 14 - cell_size.y) / cell_size.y - 1);
                field.emplace(sf::Vector2f(active_pos_->x * cell_size.x + pos_in_.x + 2,
                                                active_pos_->y * cell_size.y + pos_in_.y + 2),
                              sf::Vector2f(cell_size.x - 2, cell_size.y - 2),
                              "Value");
                field->Activate();
            } else {
                field.reset();
            }
            break;
        }
        case sf::Event::TextEntered:
            if (field.has_value()) {
                if (event.text.unicode == 13) {
                    try {
                        machine_.SetValue(*active_pos_, field->GetText());
                    } catch (...) { }
                }
                else {
                    field->ProcessEvent(event);
                }
            }
            break;
    }
}

sf::Vector2i TableElement::GetTableSize() const {
    return { cell_size.x * (machine_.Size().x + 2),cell_size.y * (machine_.Size().y + 1) };
}

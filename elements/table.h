#pragma once

#include <iostream>

#include "SFML/Graphics.hpp"
#include <future>
#include <optional>
#include <cmath>

#include "center_positioned_string.h"
#include "../app/window.h"
#include "../app/general_data.h"
#include "rounded_rectangle.h"
#include "inoutput_field.h"
#include "button.h"

class Table {
public:
    virtual sf::String GetValue(sf::Vector2i pos) const = 0;
    virtual sf::Vector2i Size() const = 0;
    virtual void SetValue(sf::Vector2i pos, sf::String value) = 0;
    virtual sf::String GetColumnName(int pos) const = 0;
    virtual sf::String GetLineName(int pos) const = 0;
    virtual void EraseColumn(int pos) = 0;
    virtual void EraseLine(int pos) = 0;
};

class TableElement : public AbstractElement {
public:
    static sf::Vector2i cell_size;

    TableElement(sf::Vector2f pos, sf::Vector2f size, Table &machine);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void ProcessEvent(sf::Event event) override;

private:
    Table &machine_;
    sf::Vector2f pos_, size_;
    sf::Vector2i pos_in_ = {0,0};
    std::optional<sf::Vector2i> active_pos_;
    mutable std::vector<ButtonWithTextAbsPos> buttons_syms_;
    mutable std::mutex buttons_syms_mutex_;
    mutable std::vector<ButtonWithTextAbsPos> buttons_qs_;
    mutable std::mutex buttons_qs_mutex_;
    std::optional<InputField> field;

    sf::Vector2i GetTableSize() const;
};

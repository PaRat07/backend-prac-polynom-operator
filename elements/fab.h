#include "SFML/Graphics.hpp"




class ButtonWithTextRelativePos : public AbstractElement {
public:
    ButtonWithTextRelativePos(sf::Vector2f pos, sf::Vector2f sz, std::string text, const std::function<void()> &cb);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void ProcessEvent(sf::Event event) override;

private:
    std::function<void()> callback_;
    std::string text_;
    sf::Vector2f size_, pos_;
};

class ButtonWithTextAbsPos : public AbstractElement {
public:
    ButtonWithTextAbsPos(sf::Vector2f pos, sf::Vector2f sz, std::string text, std::function<void()> cb);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void ProcessEvent(sf::Event event) override;

private:
    std::function<void()> callback_;
    std::string text_;
    sf::Vector2f size_, pos_;
};

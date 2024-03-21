#include "../app/window_manager.h"

void WindowManager::Start() {
    sf::RenderWindow win(sf::VideoMode(win_size.x, win_size.y), "TuringMachineSimulator", sf::Style::Default);

    win.setActive(false);
    bool close = false;
    auto displayer = std::async([&] () {
        win.setVerticalSyncEnabled(true);
        while (true) {
            if (close) {
                win.close();
                return;
            }
            win.clear(background);
            if (anim_) {
                auto cur_time = std::chrono::steady_clock::now();
                if (anim_.has_value() && cur_time > anim_->end) {
                    anim_.reset();
                    win.draw(windows_[active_]);
                } else {
                    double percentage = 1. - GetPercantage(1. * (cur_time - anim_->beg).count() / (anim_->end - anim_->beg).count());
                    int border = win_size.x * percentage;
                    {
                        sf::RenderTexture from;
                        from.create(win_size.x, win_size.y);
                        from.clear(background);
                        from.draw(windows_[anim_->from]);
                        from.display();
                        sf::Sprite sprite;
                        sprite.setTexture(from.getTexture());
                        sprite.setPosition(border - win_size.x - 5, 0);
                        win.draw(sprite);
                    }

                    {
                        sf::RenderTexture to;
                        to.create(win_size.x, win_size.y);
                        to.clear(background);
                        to.draw(windows_[anim_->to]);
                        to.display();
                        sf::Sprite sprite;
                        sprite.setTexture(to.getTexture());
                        sprite.setPosition(border, 0);
                        win.draw(sprite);
                    }
                }
            } else {
                win.draw(windows_[active_]);
            }
            win.display();
        }
    });

    sf::Event event;
    while (win.isOpen() && win.waitEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed: {
                close = true;
                displayer.wait();
                break;
            }
            case sf::Event::Resized: {
                win_size = {(float) event.size.width, (float) event.size.height};
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                win.setView(sf::View(visibleArea));
                break;
            }
            case sf::Event::TextEntered: {
                // ctrl + tab
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) &&
                    event.text.unicode == 9) {
                    anim_ = Animation   {
                                            .from = active_,
                                            .to = (active_ + 1) % windows_.size(),
                                            .beg = std::chrono::steady_clock::now(),
                                            .end = std::chrono::steady_clock::now() + std::chrono::milliseconds(350)
                                        };
                    ++active_;
                    active_ %= windows_.size();
                    break;
                }
            }
            default: {
                windows_[active_].ProcessEvent(event);
                break;
            }
        }
    }
}

double WindowManager::GetPercantage(double time_gone) {
    return std::sin(time_gone * M_PI_2);
}

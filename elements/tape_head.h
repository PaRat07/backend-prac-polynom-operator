#include <chrono>
#include <optional>
#include <cmath>

#include "../app/window.h"
#include "../app/general_data.h"
#include "../app/colors.h"

class TapeHead : public AbstractElement {
    struct Animation {
        int direction;
        std::chrono::time_point<std::chrono::steady_clock> beg_time, end_time;
    };

public:
    TapeHead(float y_pos);

    void ProcessEvent(sf::Event event) override;

    void MoveRight();

    void MoveLeft();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    static float move_for;
private:
    float y_pos_;
    mutable std::optional<Animation> animation_;


    static double CalcPos(double time);
};

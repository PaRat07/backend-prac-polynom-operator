#include "bits/stdc++.h"

#include "app/window_manager.h"
#include "app/window.h"
#include "elements/table.h"

#include "core/polynomial.h"

using namespace std;

sf::Vector2f win_size = {1000, 1000};
sf::Uint32 lambda(L'\u03BB');
long animation_time = 1500;

class DataBase : public Table {
public:
    DataBase() {
        std::string line;
        std::ifstream fin(path);
        while (std::getline(fin, line)) {
            if (line.empty()) continue;
            data_.push_back(std::move(line));
        }
    }

    ~DataBase() {
        std::ofstream fout(path, std::ios::trunc);
        for (std::string_view sv : data_) {
            fout << sv << '\n';
        }
    }

    virtual sf::String GetValue(sf::Vector2i pos) const override {
        return data_[pos.y];
    }

    virtual sf::Vector2i Size() const override {
        return { 1, static_cast<int>(data_.size()) };
    }

    virtual sf::String GetLineName(int pos) const override {
        return std::to_string(pos);
    }

    virtual void EraseLine(int pos) override {
        data_.erase(data_.begin() + pos);
    }

    virtual sf::String GetColumnName(int pos) const override { return {}; }
    virtual void SetValue(sf::Vector2i pos, sf::String value) override {}
    virtual void EraseColumn(int pos) override {}

    void AddLine(std::string s) {
        data_.push_back(std::move(s));
    }

    constexpr static auto path = "../db.txt";

private:
    std::vector<std::string> data_;
};

int main() {
    DataBase db;

    Window win;
    {
        win.AddElement(std::make_unique<TableElement>(sf::Vector2f(10, 10), sf::Vector2f(700, 980), db));
        auto new_pol = std::make_unique<InputField>(sf::Vector2f(710, 10), sf::Vector2f(280, 40), "New polynom");
        win.AddElement(std::make_unique<ButtonWithTextRelativePos>(sf::Vector2f(710, 60), sf::Vector2f(100, 45),
                                                                            "Add", [&db, data = &*new_pol] () {
            db.AddLine(data->GetText());
        }));
        win.AddElement(std::move(new_pol));
    }
    WindowManager wm;
    wm.AddWindow(std::move(win));
    wm.Start();
}
#include "bits/stdc++.h"

#include "app/window_manager.h"
#include "app/window.h"
#include "drawers/table_drawer.h"

#include "core/polynomial.h"
#include "drawers/inoutput_field.h"
#include "drawers/button.h"
#include "drawers/table_drawer.h"
#include "drawers/table_header_drawers.h"

using namespace std;

sf::Vector2f win_size = {1000, 1000};
sf::Uint32 lambda(L'\u03BB');
long animation_time = 1500;

//class UntouchableTale : public TableElement {
//public:
//    using TableElement::TableElement;
//
//    virtual void ProcessEvent(sf::Event event) override {
//        if (event.type != sf::Event::MouseButtonPressed) {
//            TableElement::ProcessEvent(event);
//        }
//    }
//};

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

class VarsAsker : public Table {
public:
    VarsAsker(const DataBase &db)
        : db_(db)
    {
    }

    void SetCallback(std::function<std::string()> get_ind) {
        get_ind_ = std::move(get_ind);
    }

    virtual sf::String GetValue(sf::Vector2i pos) const override {
        assert(pos.x == 0);
        auto need = GetNeed();
        auto it = need.begin();
        advance(it, pos.y);
        return std::to_string(vals_.count(*it) ? vals_.at(*it) : 0);
    }

    virtual sf::Vector2i Size() const {
        return sf::Vector2i(1, GetNeed().size());
    }

    virtual void SetValue(sf::Vector2i pos, sf::String value) {
        auto need = GetNeed();
        vals_[*std::next(need.begin(), pos.y)] = std::stoi(std::string(value));
    }

    virtual sf::String GetColumnName(int pos) const  {
        return "";
    }

    virtual sf::String GetLineName(int pos) const override {
        auto need = GetNeed();
        return *std::next(need.begin(), pos);
    }

    virtual void EraseColumn(int pos) {}
    virtual void EraseLine(int pos) {}

private:
    const DataBase &db_;
    std::function<std::string()> get_ind_;
    std::map<char, int> vals_;

    std::set<char> GetNeed() const {
        try {
            std::set<char> ans;
            int ind = std::min(std::stoi(get_ind_()), db_.Size().y - 1);
            std::string s = db_.GetValue(sf::Vector2i(0, ind));
            for (char i: s) {
                if (i >= 'a' && i <= 'z') {
                    ans.insert(i);
                }
            }
            return ans;
        } catch (...) {
            return {};
        }
    }
};

int main() {
    DataBase db;
    VarsAsker asker(db);


    Window win;
    {
        win.AddElement(std::make_unique<TableDrawer>(sf::Vector2f(10, 10),
                                                              sf::Vector2f(700, 980),
                                                              db,
                                                              false,
                                                              std::make_unique<EmptyHeader>(),
                                                              std::make_unique<VerticalHeader>(100, db)));
        auto err = std::make_unique<OutputField>(sf::Vector2f(710, 360), sf::Vector2f(280, 40), "Error");
        {
            auto new_pol = std::make_unique<InputField>(sf::Vector2f(710, 10), sf::Vector2f(280, 40), "New polynom");
            win.AddElement(std::make_unique<ButtonWithTextRelativePos>(sf::Vector2f(710, 60), sf::Vector2f(280, 45),
                                                                       "Add Polynom", [&db, data = &*new_pol, err = &*err]() {
                try {
                    Polynomial p(data->GetText());
                    db.AddLine(p.ToString());
                    err->SetText("");
                } catch (const std::exception &exc) {
                    err->SetText(exc.what());
                }
            }));

            win.AddElement(std::move(new_pol));
        }
        {
            auto lhs = std::make_unique<InputField>(sf::Vector2f(710, 115), sf::Vector2f(135, 40), "Lhs");
            asker.SetCallback([lhs = &*lhs] () {
                return lhs->GetText();
            });
            auto rhs = std::make_unique<InputField>(sf::Vector2f(990 - 135, 115), sf::Vector2f(135, 40), "Rhs");
            auto out = std::make_unique<OutputField>(sf::Vector2f(710, 260), sf::Vector2f(280, 40), "Result");


            win.AddElement(std::make_unique<ButtonWithTextRelativePos>(sf::Vector2f(710, 160), sf::Vector2f(135, 45),
                                                                       "Add", [&db, l = &*lhs, r = &*rhs, out = &*out, err = &*err]() {
                try {
                    out->SetText((Polynomial(db.GetValue(sf::Vector2i(0, std::stoi(l->GetText())))) +
                                  Polynomial(db.GetValue(sf::Vector2i(0, std::stoi(r->GetText()))))).ToString());
                    err->SetText("");
                } catch (const std::exception &exc) {
                    err->SetText(exc.what());
                }
                }));
            win.AddElement(std::make_unique<ButtonWithTextRelativePos>(sf::Vector2f(990 - 135, 160), sf::Vector2f(135, 45),
                                                                       "Sub", [&db, l = &*lhs, r = &*rhs, out = &*out, err = &*err]() {
                try {
                    out->SetText((Polynomial(db.GetValue(sf::Vector2i(0, std::stoi(l->GetText())))) - Polynomial(db.GetValue(sf::Vector2i(0, std::stoi(r->GetText()))))).ToString());
                    err->SetText("");
                } catch (const std::exception &exc) {
                    err->SetText(exc.what());
                }
                    }));
            win.AddElement(std::make_unique<ButtonWithTextRelativePos>(sf::Vector2f(710, 210), sf::Vector2f(135, 45),
                                                                       "Mul", [&db, l = &*lhs, r = &*rhs, out = &*out, err = &*err]() {
                try {
                    out->SetText((Polynomial(db.GetValue(sf::Vector2i(0, std::stoi(l->GetText())))) * Polynomial(db.GetValue(sf::Vector2i(0, std::stoi(r->GetText()))))).ToString());
                    err->SetText("");
                } catch (const std::exception &exc) {
                    err->SetText(exc.what());
                }
                }));
            win.AddElement(std::make_unique<ButtonWithTextRelativePos>(sf::Vector2f(990 - 135, 210), sf::Vector2f(135, 45),
                                                                       "Div", [&db, l = &*lhs, r = &*rhs, out = &*out, err = &*err]() {
                try {
                    out->SetText((Polynomial(db.GetValue(sf::Vector2i(0, std::stoi(l->GetText())))) / Polynomial(db.GetValue(sf::Vector2i(0, std::stoi(r->GetText()))))).ToString());
                    err->SetText("");
                } catch (const std::exception &exc) {
                    err->SetText(exc.what());
                }
                }));

            auto n = std::make_unique<InputField>(sf::Vector2f(710 + 200 + 10, 400), sf::Vector2f(1000 - 710 - 200 - 20, 40), "N");
            win.AddElement(std::make_unique<ButtonWithTextRelativePos>(sf::Vector2f(710, 400), sf::Vector2f(200, 45),
                                                                       "Get Nth derivative", [&db, l = &*lhs, n = &*n, out = &*out, err = &*err]() {
                        try {
                            Polynomial p(db.GetValue(sf::Vector2i(0, std::stoi(l->GetText()))));
                            for (int i = 0; i < std::stoi(n->GetText()); ++i) {
                                p = p.GetDerivative();
                            }
                            out->SetText(p.ToString());
                            err->SetText("");
                        } catch (const std::exception &exc) {
                            err->SetText(exc.what());
                        }
                    }));

            win.AddElement(std::make_unique<TableDrawer>(sf::Vector2f(710, 550),
                                                                  sf::Vector2f(1000 - 710 - 10, 1000 - 500 - 10),
                                                                  asker,
                                                                  false,
                                                                  std::make_unique<EmptyHeader>(),
                                                                  std::make_unique<EmptyHeader>()));
            win.AddElement(std::make_unique<ButtonWithTextRelativePos>(sf::Vector2f(710, 450), sf::Vector2f(1000 - 710 - 10, 45),
                                                                       "Get int roots", [&db, l = &*lhs, n = &*n, out = &*out, err = &*err]() {
                        try {
                            std::cerr << "In Get int roots" << std::endl;
                            Polynomial p(db.GetValue(sf::Vector2i(0, std::stoi(l->GetText()))));
                            auto roots = p.GetIntegerRoots();
                            std::string ans;
                            bool first = true;
                            for (int i : roots) {
                                if (!first) {
                                    ans += ", ";
                                }
                                first = false;
                                ans += std::to_string(i);
                            }
                            std::cerr << ans << std::endl;
                            out->SetText(ans);
                            err->SetText("");
                        } catch (const std::exception &exc) {
                            err->SetText(exc.what());
                        }
                    }));

            win.AddElement(std::make_unique<ButtonWithTextRelativePos>(sf::Vector2f(710, 500), sf::Vector2f(1000 - 710 - 10, 45),
                                                                       "Get val", [&db, l = &*lhs, &asker, out = &*out, err = &*err]() {
                        try {
                            Polynomial p(db.GetValue(sf::Vector2i(0, std::stoi(l->GetText()))));
                            std::array<int, 26> arr;
                            for (int i = 0; i < 26; ++i) {
                                arr[i] = std::stoi(std::string(asker.GetValue(sf::Vector2i(0, i))));
                            }
                            out->SetText(std::to_string(p.GetValueAt(arr)));
                            err->SetText("");
                        } catch (const std::exception &exc) {
                            err->SetText(exc.what());
                        }
                    }));

            win.AddElement(std::move(lhs));
            win.AddElement(std::move(rhs));
            win.AddElement(std::move(n));
            win.AddElement(std::make_unique<ButtonWithTextRelativePos>(sf::Vector2f(710, 310), sf::Vector2f(280, 45),
                                                                       "Save", [&db, out = &*out, err = std::ref(*err)]() {
                if (out->GetText().empty()) return;
                try {
                    db.AddLine(out->GetText());
                    out->SetText("");
                }  catch (const std::exception &exc) {
                    err.get().SetText(exc.what());
                }
            }));
            win.AddElement(std::move(out));
        }
        win.AddElement(std::move(err));
    }
    WindowManager wm;
    wm.AddWindow(std::move(win));
    wm.Start();
}
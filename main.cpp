#include "bits/stdc++.h"

#include "app/window_manager.h"
#include "app/tab.h"
#include "drawers/table_drawer.h"

#include "core/polynomial.h"
#include "drawers/inoutput_field.h"
#include "drawers/button.h"
#include "drawers/table_drawer.h"
#include "drawers/table_header_drawers.h"
#include "app/tabs_drawer.h"

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
//    std::cout << "x+7"_p * "x-7"_p << std::endl;
//    return 0;

    DataBase db;
    VarsAsker asker(db);

    Tab win;
    {
        win.AddElement(std::make_unique<TableDrawer>(
                          sf::Vector2f(10, 10),
                          sf::Vector2f(700, 980),
                          db, false,
                          std::make_unique<EmptyHeader>(),
                          std::make_unique<VerticalHeader>(70, db)));

        {
            std::unique_ptr<TabsManager> tm(std::make_unique<TabsManager>(
                                        sf::Vector2f(710, 10),
                                        sf::Vector2f(280, 980)));
            {
                Tab new_polynom;
                std::unique_ptr<OutputField> err(std::make_unique<OutputField>(
                        sf::Vector2f(10, 980 - 5 - 45),
                        sf::Vector2f(260, 45),
                        "Error"
                ));


                std::unique_ptr<InputField> input(std::make_unique<InputField>(
                        sf::Vector2f(10, 10),
                        sf::Vector2f(260, 45),
                        "New polynom"));


                new_polynom.AddElement(std::make_unique<ButtonWithTextRelativePos>(
                        sf::Vector2f(10, 60),
                        sf::Vector2f(260, 45),
                        "Add",
                        [&err = *err, &in = *input, &db] () {
                            try {
                                Polynomial p(in.GetText());
                                db.AddLine(p.ToString());
                                err.SetText("");
                            } catch (const std::exception &exc) {
                                err.SetText(exc.what());
                            }
                        }
                ));

                new_polynom.AddElement(std::move(input));
                new_polynom.AddElement(std::move(err));
                tm->AddTab(std::move(new_polynom));
            }

            {
                Tab add;
                std::unique_ptr<OutputField> err(std::make_unique<OutputField>(
                        sf::Vector2f(10, 980 - 10 - 45),
                        sf::Vector2f(260, 45),
                        "Error"
                ));


                std::unique_ptr<InputField> lhs(std::make_unique<InputField>(
                        sf::Vector2f(10, 10),
                        sf::Vector2f(125, 45),
                        "Lhs"));
                std::unique_ptr<InputField> rhs(std::make_unique<InputField>(
                        sf::Vector2f(270 - 125, 10),
                        sf::Vector2f(125, 45),
                        "Rhs"));
                std::unique_ptr<OutputField> res(std::make_unique<OutputField>(
                        sf::Vector2f(10, 110),
                        sf::Vector2f(260, 45),
                        "Result"));


                add.AddElement(std::make_unique<ButtonWithTextRelativePos>(
                        sf::Vector2f(10, 60),
                        sf::Vector2f(260, 45),
                        "Add",
                        [&err = *err, &lhs = *lhs, &rhs = *rhs, &res = *res, &db] () {
                            try {
                                Polynomial l(db.GetValue(sf::Vector2i(0, std::stoi(lhs.GetText()))));
                                Polynomial r(db.GetValue(sf::Vector2i(0, std::stoi(rhs.GetText()))));
                                res.SetText((l + r).ToString());
                                err.SetText("");
                            } catch (const std::exception &exc) {
                                err.SetText(exc.what());
                            }
                        }
                ));

                add.AddElement(std::make_unique<ButtonWithTextRelativePos>(
                        sf::Vector2f(10, 160),
                        sf::Vector2f(260, 45),
                        "Save",
                        [&res = *res, &db] () {
                            Polynomial(res.GetText());
                            db.AddLine(res.GetText());
                        }
                ));

                add.AddElement(std::move(lhs));
                add.AddElement(std::move(rhs));
                add.AddElement(std::move(err));
                add.AddElement(std::move(res));
                tm->AddTab(std::move(add));
            }

            {
                Tab multiply;
                std::unique_ptr<OutputField> err(std::make_unique<OutputField>(
                        sf::Vector2f(10, 980 - 10 - 45),
                        sf::Vector2f(260, 45),
                        "Error"
                ));


                std::unique_ptr<InputField> lhs(std::make_unique<InputField>(
                        sf::Vector2f(10, 10),
                        sf::Vector2f(125, 45),
                        "Lhs"));

                std::unique_ptr<InputField> rhs(std::make_unique<InputField>(
                        sf::Vector2f(270 - 125, 10),
                        sf::Vector2f(125, 45),
                        "Rhs"));

                std::unique_ptr<OutputField> res(std::make_unique<OutputField>(
                        sf::Vector2f(10, 110),
                        sf::Vector2f(260, 45),
                        "Result"));

                multiply.AddElement(std::make_unique<ButtonWithTextRelativePos>(
                        sf::Vector2f(10, 60),
                        sf::Vector2f(260, 45),
                        "Mul",
                        [&err = *err, &lhs = *lhs, &rhs = *rhs, &res = *res, &db] () {
                            try {
                                Polynomial l(db.GetValue(sf::Vector2i(0, std::stoi(lhs.GetText()))));
                                Polynomial r(db.GetValue(sf::Vector2i(0, std::stoi(rhs.GetText()))));
                                res.SetText((l * r).ToString());
                                err.SetText("");
                            } catch (const std::exception &exc) {
                                err.SetText(exc.what());
                            }
                        }
                ));

                multiply.AddElement(std::make_unique<ButtonWithTextRelativePos>(
                        sf::Vector2f(10, 160),
                        sf::Vector2f(260, 45),
                        "Save",
                        [&res = *res, &db] () {
                            Polynomial(res.GetText());
                            db.AddLine(res.GetText());
                        }
                ));

                multiply.AddElement(std::move(lhs));
                multiply.AddElement(std::move(rhs));
                multiply.AddElement(std::move(res));
                multiply.AddElement(std::move(err));
                tm->AddTab(std::move(multiply));
            }

            {
                Tab divide;
                std::unique_ptr<OutputField> err(std::make_unique<OutputField>(
                        sf::Vector2f(10, 980 - 10 - 45),
                        sf::Vector2f(260, 45),
                        "Error"
                ));


                std::unique_ptr<InputField> lhs(std::make_unique<InputField>(
                        sf::Vector2f(10, 10),
                        sf::Vector2f(125, 45),
                        "Lhs"));
                std::unique_ptr<InputField> rhs(std::make_unique<InputField>(
                        sf::Vector2f(270 - 125, 10),
                        sf::Vector2f(125, 45),
                        "Rhs"));
                std::unique_ptr<OutputField> quot(std::make_unique<OutputField>(
                        sf::Vector2f(10, 110),
                        sf::Vector2f(260, 45),
                        "Quotient"));
                std::unique_ptr<OutputField> mod(std::make_unique<OutputField>(
                        sf::Vector2f(10, 210),
                        sf::Vector2f(260, 45),
                        "Mod"));


                divide.AddElement(std::make_unique<ButtonWithTextRelativePos>(
                        sf::Vector2f(10, 60),
                        sf::Vector2f(260, 45),
                        "Divide",
                        [&err = *err, &lhs = *lhs, &rhs = *rhs, &mod = *mod, &quot = *quot, &db] () {
                            try {
                                Polynomial l(db.GetValue(sf::Vector2i(0, std::stoi(lhs.GetText()))));
                                Polynomial r(db.GetValue(sf::Vector2i(0, std::stoi(rhs.GetText()))));
                                auto res = l / r;
                                quot.SetText(res.whole_num.ToString());
                                mod.SetText(res.num.ToString());
                                err.SetText("");
                            } catch (const std::exception &exc) {
                                err.SetText(exc.what());
                            }
                        }
                ));

                divide.AddElement(std::make_unique<ButtonWithTextRelativePos>(
                        sf::Vector2f(10, 160),
                        sf::Vector2f(260, 45),
                        "Save",
                        [&res = *quot, &db]() {
                            Polynomial(res.GetText());
                            db.AddLine(res.GetText());
                        }
                ));

                divide.AddElement(std::make_unique<ButtonWithTextRelativePos>(
                        sf::Vector2f(10, 260),
                        sf::Vector2f(260, 45),
                        "Save",
                        [&res = *mod, &db]() {
                            Polynomial(res.GetText());
                            db.AddLine(res.GetText());
                        }
                ));

                divide.AddElement(std::move(lhs));
                divide.AddElement(std::move(rhs));
                divide.AddElement(std::move(err));
                divide.AddElement(std::move(quot));
                divide.AddElement(std::move(mod));
                tm->AddTab(std::move(divide));
            }

            {
                Tab get_roots;
                std::unique_ptr<OutputField> err(std::make_unique<OutputField>(
                        sf::Vector2f(10, 980 - 10 - 45),
                        sf::Vector2f(260, 45),
                        "Error"
                ));

                std::unique_ptr<InputField> polynomial(std::make_unique<InputField>(
                        sf::Vector2f(10, 10),
                        sf::Vector2f(260, 45),
                        "Polynomial"));

                std::unique_ptr<OutputField> res(std::make_unique<OutputField>(
                        sf::Vector2f(10, 110),
                        sf::Vector2f(260, 45),
                        "Result"));


                get_roots.AddElement(std::make_unique<ButtonWithTextRelativePos>(
                        sf::Vector2f(10, 60),
                        sf::Vector2f(260, 45),
                        "Get integer roots",
                        [&err = *err, &polynomial = *polynomial, &res = *res, &db] () {
                            try {
                                Polynomial l(db.GetValue(sf::Vector2i(0, std::stoi(polynomial.GetText()))));
                                auto roots = l.GetIntegerRoots();
                                std::string ans;
                                for (const auto i : roots) {
                                    ans += std::to_string(i);
                                }
                                res.SetText(ans.empty() ? "No roots" : ans);
                                err.SetText("");
                            } catch (const std::exception &exc) {
                                err.SetText(exc.what());
                            }
                        }
                ));

                get_roots.AddElement(std::move(polynomial));
                get_roots.AddElement(std::move(err));
                get_roots.AddElement(std::move(res));
                tm->AddTab(std::move(get_roots));
            }

            {
                Tab get_derivative;
                std::unique_ptr<OutputField> err(std::make_unique<OutputField>(
                        sf::Vector2f(10, 980 - 10 - 45),
                        sf::Vector2f(260, 45),
                        "Error"
                ));

                std::unique_ptr<InputField> polynomial(std::make_unique<InputField>(
                        sf::Vector2f(10, 10),
                        sf::Vector2f(260, 45),
                        "Polynomial"));

                std::unique_ptr<OutputField> res(std::make_unique<OutputField>(
                        sf::Vector2f(10, 160),
                        sf::Vector2f(260, 45),
                        "Result"));

                std::unique_ptr<InputField> var(std::make_unique<InputField>(
                        sf::Vector2f(10, 60),
                        sf::Vector2f(125, 45),
                        "Var"));

                std::unique_ptr<InputField> n(std::make_unique<InputField>(
                        sf::Vector2f(145, 60),
                        sf::Vector2f(125, 45),
                        "N"));


                get_derivative.AddElement(std::make_unique<ButtonWithTextRelativePos>(
                        sf::Vector2f(10, 110),
                        sf::Vector2f(260, 45),
                        "Get Nth derivative",
                        [&err = *err, &polynomial = *polynomial, &res = *res, &var = *var, &n = *n, &db] () {
                            try {
                                if (var.GetText().size() != 1) {
                                    throw std::invalid_argument("Incorrect var");
                                }
                                Polynomial l(db.GetValue(sf::Vector2i(0, std::stoi(polynomial.GetText()))));
                                int n_val = std::stoi(n.GetText());
                                for (int i = 0; i < n_val; ++i) {
                                    l = l.GetDerivative(var.GetText()[0]);
                                }
                                res.SetText(l.ToString());
                                err.SetText("");
                            } catch (const std::exception &exc) {
                                err.SetText(exc.what());
                            }
                        }
                ));

                get_derivative.AddElement(std::make_unique<ButtonWithTextRelativePos>(
                        sf::Vector2f(10, 210),
                        sf::Vector2f(260, 45),
                        "Save",
                        [&res = *res, &db] () {
                            Polynomial(res.GetText());
                            db.AddLine(res.GetText());
                        }
                ));

                get_derivative.AddElement(std::move(polynomial));
                get_derivative.AddElement(std::move(err));
                get_derivative.AddElement(std::move(res));
                get_derivative.AddElement(std::move(var));
                get_derivative.AddElement(std::move(n));
                tm->AddTab(std::move(get_derivative));
            }

            {
                Tab get_val_at;
                std::unique_ptr<OutputField> err(std::make_unique<OutputField>(
                        sf::Vector2f(10, 980 - 10 - 45),
                        sf::Vector2f(260, 45),
                        "Error"
                ));

                std::unique_ptr<InputField> polynomial(std::make_unique<InputField>(
                        sf::Vector2f(10, 10),
                        sf::Vector2f(260, 45),
                        "Polynomial"));

                asker.SetCallback([&polynomial = *polynomial] () {
                    return polynomial.GetText();
                });

                std::unique_ptr<OutputField> res(std::make_unique<OutputField>(
                        sf::Vector2f(10, 610),
                        sf::Vector2f(260, 45),
                        "Result"));

                get_val_at.AddElement(std::make_unique<TableDrawer>(
                            sf::Vector2f(0, 65 - TableDrawer::PADDING + TableDrawer::LINE_THICKNESS),
                            sf::Vector2f(280, 500),
                            asker,
                            true,
                            std::make_unique<EmptyHeader>(),
                            std::make_unique<VerticalHeader>(70, asker)
                        ));

                get_val_at.AddElement(std::make_unique<ButtonWithTextRelativePos>(
                        sf::Vector2f(10, 555),
                        sf::Vector2f(260, 45),
                        "Get value",
                        [&err = *err, &l = *polynomial, &out = *res, &asker, &db] () {
                            try {
                                Polynomial p(db.GetValue(sf::Vector2i(0, std::stoi(l.GetText()))));
                                std::array<int, 26> arr;
                                arr.fill(0);
                                for (int i = 0; i < asker.Size().y; ++i) {
                                    arr[asker.GetLineName(i)[0] - 'a'] = std::stoi(std::string(asker.GetValue(sf::Vector2i(0, i))));
                                }
                                std::string ans = std::to_string(p.GetValueAt(arr));
                                while (ans.size() > 1 && (ans.back() == '0' || ans.back() == '.')) {
                                    ans.pop_back();
                                }
                                out.SetText(ans);
                                err.SetText("");
                            } catch (const std::exception &exc) {
                                err.SetText(exc.what());
                            }
                        }
                ));

                get_val_at.AddElement(std::move(polynomial));
                get_val_at.AddElement(std::move(err));
                get_val_at.AddElement(std::move(res));
                tm->AddTab(std::move(get_val_at));
            }

            win.AddElement(std::move(tm));
        }
    }


    WindowManager wm(std::move(win));
    wm.Start();
}
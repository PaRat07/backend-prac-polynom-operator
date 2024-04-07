#include <iostream>
#include <map>

#include "extern/cpp-httplib/httplib.h"

#include "core/polynomial.h"
#include "json/json.h"

int main() {
    constexpr auto path = "../db.txt";

    httplib::Server svr;

    svr.Get("/polynomial/add",  [] (const httplib::Request& req, httplib::Response& res) {
        Polynomial lhs(req.get_param_value("lhs")), rhs(req.get_param_value("rhs"));
        Json::Value ans;
        ans["respose"]["res"] = (lhs + rhs).ToString();
        res.set_content(ans.toStyledString(), "application/json");
    });

    svr.Get("/polynomial/multiply",  [] (const httplib::Request& req, httplib::Response& res) {
        Polynomial lhs(req.get_param_value("lhs")), rhs(req.get_param_value("rhs"));
        Json::Value ans;
        ans["respose"]["res"] = (lhs * rhs).ToString();
        res.set_content(ans.toStyledString(), "application/json");
    });

    svr.Get("/polynomial/divide",  [] (const httplib::Request& req, httplib::Response& res) {
        Polynomial lhs(req.get_param_value("lhs")), rhs(req.get_param_value("rhs"));
        Json::Value ans;
        ans["respose"]["res"] = (lhs / rhs).ToString();
        res.set_content(ans.toStyledString(), "application/json");
    });

    svr.Post("/polynomial/add_to_base", [&path] (const httplib::Request& req, httplib::Response& res) {
        Polynomial(req.get_param_value("polynomial"));
        std::ofstream fout(path, std::ios::app);
        fout << req.get_param_value("polynoial") << std::endl;
    });

    svr.Get("/polynomial/get_polynomials", [&path] (const httplib::Request& req, httplib::Response& res) {
        Json::Value ans;
        std::ifstream fin(path);
        int ind = 0;
        while (fin) {
            std::string line;
            std::getline(fin, line);
            ans["respose"][ind++] = line;
        }
        res.set_content(ans.toStyledString(), "application/json");
    });

    svr.Get("/polynomial/get_nth_derivative", [] (const httplib::Request& req, httplib::Response& res) {
        Polynomial p(req.get_param_value("polynomial"));
        int n = std::stoi(req.get_param_value("n"));
        while (n--) {
            p = p.GetDerivative();
        }
        Json::Value ans;
        ans["respose"]["res"] = p.ToString();
        res.set_content(ans.toStyledString(), "application/json");
    });


    svr.Get("/polynomial/get_value_at", [] (const httplib::Request& req, httplib::Response& res) {
        Polynomial p(req.get_param_value("polynomial"));
        std::array<int, 26> val;
        val.fill(1e6);
        for (int i = 0; i < 26; ++i) {
            if (req.has_param(std::string('a' + i, 1))) {
                val[i] = std::stoi(req.get_param_value(std::string('a' + i, 1)));
            }
        }
        Json::Value ans;
        ans["respose"]["res"] = p.GetValueAt(val);
        res.set_content(ans.toStyledString(), "application/json");
    });




    svr.set_exception_handler([](const httplib::Request& req, httplib::Response& res, std::exception_ptr ex) {
        Json::Value json;
        try {
            if (ex) {
                std::rethrow_exception(ex);
            }
        } catch (const std::exception& e) {
            json["respose"]["err"] = e.what();
        }

        res.set_content(json.toStyledString(), "application/json");
        res.status = 500;
    });


    svr.set_post_routing_handler([](const auto& req, auto& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "*");
    });

    svr.listen("0.0.0.0", 4242);


}
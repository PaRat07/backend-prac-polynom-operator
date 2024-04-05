#include <iostream>
#include <map>

//#include "extern/cpp-httplib/httplib.h"
//
#include "core/polynomial.h"
//#include "json/json.h"
//
//#define REQ_HANDLER [](const httplib::Request& req, httplib::Response& res)

int main() {


    std::cout << "x-7"_p * "x-7"_p * "x-7"_p * "x-7"_p * "x-7"_p * "x-7"_p << std::endl;

    return 0;
////    std::map<int, SingleLinkedList<Polynomial>> db;
//
////    Json::Value json;
//
//    httplib::Server svr;
//
//    svr.Get("/ping", REQ_HANDLER{
//        Json::Value ans, body;
//
//        body = req.body;
//
//        ans["response"] = body;
//
////        throw std::invalid_argument("МАРК ЛОХ");
//
//        res.set_content(ans.toStyledString(), "application/json");
//    });
//
//
//    svr.set_exception_handler([](const httplib::Request& req, httplib::Response& res, std::exception_ptr ex) {
//        Json::Value json;
//
//        json["status"] = "Internal server error";
//        try {
//            if (ex) {
//                std::rethrow_exception(ex);
//            }
//        } catch (const std::exception& e) {
//            json["detail"] = e.what();
//        }
//
//        res.set_content(json.toStyledString(), "application/json");
//        res.status = 500;
//    });
//
//    svr.listen("0.0.0.0", 4242);


}
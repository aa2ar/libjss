#define BOOST_TEST_MAIN
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <rapidjson/document.h>
#include "libjss/jss.hpp"

namespace jss = LIBJSS_NAMESPACE;

BOOST_AUTO_TEST_SUITE(jss_tests)

BOOST_AUTO_TEST_CASE(jss_data_test)
{
    std::string s;

    const char *value = "v\na\rl\tu\"e";
    jss::data data("k\n\r\te\"y", std::string(value));
    data.serialize(s);
    std::cout << s << std::endl;
    BOOST_CHECK(s == "\"k\\n\\r\\te\\\"y\":\"v\\na\\rl\\tu\\\"e\"");

    s = "{"+s+"}";

    rapidjson::Document doc;
    doc.Parse(s.c_str(), s.length());
    BOOST_CHECK(!doc.HasParseError());
    BOOST_CHECK(doc.IsObject());
    BOOST_CHECK(doc[data.k().c_str()].IsString());
    BOOST_CHECK(std::strcmp(doc[data.k().c_str()].GetString(), value) == 0);

    s.clear();

    jss::data("null", jss::null).serialize(s);
    std::cout << s << std::endl;
    BOOST_CHECK(s == R"("null":null)");

    s.clear();

    jss::data("true", true).serialize(s);
    std::cout << s << std::endl;
    BOOST_CHECK(s == R"("true":true)");

    s.clear();

    jss::data("false", false).serialize(s);
    std::cout << s << std::endl;
    BOOST_CHECK(s == R"("false":false)");

    s.clear();

    jss::data("string", jss::str("string")).serialize(s);
    std::cout << s << std::endl;
    BOOST_CHECK(s == R"("string":"string")");

    s.clear();

    jss::data("int", 123).serialize(s);
    std::cout << s << std::endl;
    BOOST_CHECK(s == R"("int":123)");

    jss::set_presision(3);

    s.clear();

    jss::data("double", -123.456).serialize(s);
    std::cout << s << std::endl;
    BOOST_CHECK(s == R"("double":-123.456)");

    s.clear();

    jss::data("double", 123.456789).serialize(s);
    std::cout << s << std::endl;
    BOOST_CHECK(s == R"("double":123.457)");

}

BOOST_AUTO_TEST_CASE(jss_object_test)
{
    jss::set_presision(4);

    std::string s;

    jss::object obj;
    rapidjson::Document doc;

    obj["nullptr"] = nullptr;
    obj["null"] = jss::null;
    obj["true"] = true;
    obj["false"] = false;
    obj["int"] = -123;
    obj["double"] = 123.456789;
    obj["string"] = jss::str("string");

    jss::object obj1;
    obj1["int"] = 0;
    obj["object"] = obj1;

    obj.serialize(s);
    std::cout << s << std::endl;

    doc.Parse(s.c_str(), s.length());
    BOOST_CHECK(!doc.HasParseError());
    BOOST_CHECK(doc.IsObject());
    BOOST_CHECK(doc["nullptr"].IsNull());
    BOOST_CHECK(doc["null"].IsNull());
    BOOST_CHECK(doc["true"].IsBool() && doc["true"] == true);
    BOOST_CHECK(doc["false"].IsBool() && doc["false"] == false);
    BOOST_CHECK(doc["int"].IsInt() && doc["int"] == -123);
    BOOST_CHECK(doc["double"].IsDouble() && doc["double"] == 123.4568);
    BOOST_CHECK(doc["string"].IsString() && doc["string"] == "string");
    BOOST_CHECK(doc["object"].IsObject());
}

BOOST_AUTO_TEST_CASE(jss_array_test)
{
    std::string s;
    jss::array ary;
    rapidjson::Document doc;

    jss::set_presision(3);

    ary << nullptr << jss::null << true << false << 123 << 123.456789 << jss::str("string");

    ary.serialize(s);
    std::cout << s << std::endl;

    doc.Parse(s.c_str(), s.length());

    BOOST_CHECK(!doc.HasParseError());
    BOOST_CHECK(doc.IsArray());
    BOOST_CHECK(doc.Size() == 7);
    BOOST_CHECK(doc[0].IsNull());
    BOOST_CHECK(doc[1].IsNull());
    BOOST_CHECK(doc[2].IsBool() && doc[2] == true);
    BOOST_CHECK(doc[3].IsBool() && doc[3] == false);
    BOOST_CHECK(doc[4].IsInt() && doc[4] == 123);
    BOOST_CHECK(doc[5].IsDouble() && doc[5] == 123.457);
    BOOST_CHECK(doc[6].IsString() && doc[6] == "string");
}

BOOST_AUTO_TEST_CASE(jss_object_in_array_test)
{
    std::string s;

    jss::object obj;
    obj["string"] = jss::str("string");

    jss::array ary;
    ary << obj;

    ary.serialize(s);
    std::cout << s << std::endl;

    rapidjson::Document doc;
    doc.Parse(s.c_str(), s.length());

    BOOST_CHECK(!doc.HasParseError());
    BOOST_CHECK(doc.IsArray() && doc.Size() == 1);

    rapidjson::Value v = std::move(doc[0]);
    BOOST_CHECK(v.IsObject());
    BOOST_CHECK(v["string"].IsString() && v["string"] == "string");
}

BOOST_AUTO_TEST_CASE(jss_array_in_object_test)
{
    std::string s;

    jss::array ary;
    ary << 0;

    jss::object obj;
    obj["array"] = ary;

    obj.serialize(s);
    std::cout << s << std::endl;

    rapidjson::Document doc;
    doc.Parse(s.c_str(), s.length());

    BOOST_CHECK(!doc.HasParseError());
    BOOST_CHECK(doc.IsObject());

    rapidjson::Value v;
    v.Swap(doc["array"]);
    BOOST_CHECK(v.IsArray() && v.Size() == 1 && v[0].IsInt() && v[0] == 0);
}

BOOST_AUTO_TEST_SUITE_END()

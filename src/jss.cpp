#include "libjss/jss.hpp"
#include <sstream>
#include <iomanip>
#include <boost/algorithm/string/replace.hpp>

LIBJSS_BEGIN_NAMESPACE

namespace
{
    int const default_precision = 6;
    int precision = default_precision;
}

value::value(bool b)
{
    c_ = b ? "true" : "false";
}

value::value(int i)
{
    c_ = std::to_string(i);
}

value::value(double d, int prec)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(prec == -1 ? precision : prec) << d;
    c_ = ss.str();
}

value::value(str const& s)
{
    c_.reserve(s.data.size() + 2);
    c_.append("\"");
    c_.append(s.data.data(), s.data.size());
    c_.append("\"");
}

value::value(null_t)
{
    c_ = "null";
}

value::value(value&& other)
    : c_(other.c_)
{}

void value::serialize(std::string& s) const
{
    s.append(c_);
}

std::string value::serialize() const
{
    std::string out;
    serialize(out);
    return out;
}


data::data(boost::string_view key, bool b)
{
    c_ = std::make_pair(std::string(key), b ? "true" : "false");
}

data::data(boost::string_view key, int i)
{
    c_ = std::make_pair(std::string(key), std::to_string(i));
}

data::data(boost::string_view key, double d, int prec)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(prec == -1 ? precision : prec) << d;
    c_ = std::make_pair(std::string(key), ss.str());
}

static void replace_specchars(std::string& s)
{
    boost::replace_all(s, "\r", "\\r");
    boost::replace_all(s, "\n", "\\n");
    boost::replace_all(s, "\t", "\\t");
    boost::replace_all(s, "\"", "\\\"");
}

data::data(boost::string_view key, str const& s)
{
    std::string v(s.data.data(), s.data.size());
    replace_specchars(v);
    c_.first = std::string(key);
    c_.second.reserve(s.data.size() + 2);
    c_.second.append("\"");
    c_.second.append(v);
    c_.second.append("\"");
}

data::data(boost::string_view key, value const& v)
{
    std::string s;
    v.serialize(s);
    c_ = std::make_pair(std::string(key), s);
}

data::data(boost::string_view key, null_t)
{
    c_ = std::make_pair(std::string(key), "null");
}

data::data(boost::string_view key, array const& obj)
{
    c_.first = std::string(key);
    obj.serialize(c_.second);
}

data::data(boost::string_view key, object const& obj)
{
    c_.first = std::string(key);
    obj.serialize(c_.second);
}

data::data(boost::string_view key, std::string s)
{
    replace_specchars(s);
    c_.first = std::string(key);
    c_.second.reserve(s.size() + 2);
    c_.second.append("\"");
    c_.second.append(s);
    c_.second.append("\"");
}

void data::serialize(std::string& s) const
{
    std::string k = c_.first;
    std::string v = c_.second;

    replace_specchars(k);

    s.reserve(s.size() + k.size() + v.size() + 1 + 2);
    s.append("\"");
    s.append(k);
    s.append("\"");
    s.append(":");
    s.append(v);
}

std::string data::serialize() const
{
    std::string out;
    serialize(out);
    return out;
}

array& array::operator <<(bool b)
{
    if (!c_.empty()) c_.append(",");
    c_.append(b ? "true" : "false");
    return *this;
}

array& array::operator <<(int i)
{
    if (!c_.empty()) c_.append(",");
    c_.append(std::to_string(i));
    return *this;
}

array& array::operator <<(double d)
{
    if (!c_.empty()) c_.append(",");
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << d;
    c_.append(ss.str());
    return *this;
}

array& array::operator <<(str const& s)
{
    if (!c_.empty()) c_.append(",");
    c_.reserve(c_.size() + s.data.size() + 2);
    c_.append("\"");
    c_.append(s.data.data(), s.data.size());
    c_.append("\"");
    return *this;
}

array& array::operator <<(value const& v)
{
    if (!c_.empty()) c_.append(",");
    v.serialize(c_);
    return *this;
}

array& array::operator <<(object const& o)
{
    if (!c_.empty()) c_.append(",");
    o.serialize(c_);
    return *this;
}

array& array::operator <<(null_t)
{
    if (!c_.empty()) c_.append(",");
    c_.append("null");
    return *this;
}

void array::serialize(std::string& s) const
{
    s.reserve(s.size() + c_.size() + c_.size() + 2);
    s.append("[");
    s.append(c_);
    s.append("]");
}

object::key object::operator[](boost::string_view name)
{
    return key(this, name);
}

object& object::operator <<(data const& d)
{
    if (!c_.empty()) c_.append(",");
    d.serialize(c_);
    return *this;
}

void object::serialize(std::string& s) const
{
    s.reserve(s.size() + c_.size() + c_.size() + 2);
    s.append("{");
    s.append(c_);
    s.append("}");
}

std::string object::serialize() const
{
    std::string out;
    serialize(out);
    return out;
}

bool object::key::operator =(bool b)
{
    if (obj)
    {
        *obj << data(name, b);
        obj = nullptr;
        return true;
    }
    return false;
}

bool object::key::operator =(int i)
{
    if (obj)
    {
        *obj << data(name, i);
        obj = nullptr;
        return true;
    }
    return false;
}

bool object::key::operator =(double d)
{
    if (obj)
    {
        *obj << data(name, d);
        obj = nullptr;
        return true;
    }
    return false;
}

bool object::key::operator =(str const& s)
{
    if (obj)
    {
        *obj << data(name, s);
        obj = nullptr;
        return true;
    }
    return false;
}

bool object::key::operator =(value const& val)
{
    if (obj)
    {
        *obj << data(name, val);
        obj = nullptr;
        return true;
    }
    return false;
}

bool object::key::operator =(null_t)
{
    if (obj)
    {
        *obj << data(name, null);
        obj = nullptr;
        return true;
    }
    return false;
}

bool object::key::operator =(array const& ary)
{
    if (obj)
    {
        *obj << data(name, ary);
        obj = nullptr;
        return true;
    }
    return false;
}

bool object::key::operator =(object const& o)
{
    if (obj)
    {
        *obj << data(name, o);
        obj = nullptr;
        return true;
    }
    return false;
}

object::key::key(object *obj, boost::string_view name)
    : obj(obj)
    , name(name.data(), name.size())
{}

void set_presision(int value)
{
    precision = (value > 0)
            ? value
            : default_precision;
}

LIBJSS_END_NAMESPACE

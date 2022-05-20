// Copyright 2021 Alexander Arefyev
// Distributed under MIT license
// See LICENSE file or http://opensource.org/licenses/MIT

#ifndef JSS__INCLUDED
#define JSS__INCLUDED

#include <string>
#include <map>
#include <vector>
#include <boost/variant.hpp>
#include <boost/utility/string_view.hpp>
#include "libjssglobal.hpp"

LIBJSS_BEGIN_NAMESPACE

struct str
{
    boost::string_view const data;
    str(const char* s) : data(s) {}
    str(std::string const& s) : data(s) {}
};

typedef std::nullptr_t null_t;
static null_t null;

class value;
class array;
class object;

class value
{
public:
    value(bool b);
    value(int i);
    value(double d, int prec = -1);
    value(str const& str);
    value(null_t);

    value(value&& other);

    void serialize(std::string& str) const;
    std::string serialize() const;

private:
    std::string c_;
};

class data
{
public:
    data(boost::string_view key, bool b);
    data(boost::string_view key, int i);
    data(boost::string_view key, double d, int prec = -1);
    data(boost::string_view key, str const& str);
    data(boost::string_view key, value const& v);
    data(boost::string_view key, null_t);
    data(boost::string_view key, array const& obj);
    data(boost::string_view key, object const& obj);
    data(boost::string_view key, std::string s);

    constexpr std::string const& k() const { return c_.first; }
    constexpr std::string const& v() const { return c_.second; }

    void serialize(std::string& str) const;
    std::string serialize() const;

private:
    std::pair<std::string, std::string> c_;
};

class object
{
public:
    class key
    {
        friend class object;
        object* obj;
        std::string name;
    public:
        bool operator =(bool b);
        bool operator =(int i);
        bool operator =(double d);
        bool operator =(str const& str);
        bool operator =(value const& val);
        bool operator =(null_t);
        bool operator =(array const& ary);
        bool operator =(object const& obj);

    private:
        key(object* obj, boost::string_view name);
    };

    key operator[](boost::string_view key);
    object& operator <<(data const& d);

    template <typename T>
    object& operator <<(std::map<std::string, T> const& map)
    {
        for (auto const& m: map) key(this, m.first) = m.second;
        return *this;
    }

    void serialize(std::string& str) const;
    std::string serialize() const;

private:
    std::string c_;
};

class array
{
public:
    array& operator <<(bool b);
    array& operator <<(int i);
    array& operator <<(double d);
    array& operator <<(str const& str);
    array& operator <<(value const& v);
    array& operator <<(object const& o);
    array& operator <<(null_t);

    template <typename T>
    array& operator <<(std::initializer_list<T> const& vec)
    {
        for (auto const& v: vec) *this << v;
        return *this;
    }

    template <typename T>
    array& operator <<(std::vector<T> const& vec)
    {
        for (auto const& v: vec) *this << v;
        return *this;
    }

    void serialize(std::string& str) const;

private:
    std::string c_;
};

void set_presision(int value);

LIBJSS_END_NAMESPACE

#endif // JSS__INCLUDED

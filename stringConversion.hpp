#pragma once
// class for decimal numbers with comma
#include <sstream>
class UseCommaAsSeparator : public std::numpunct<char>
{
protected:
    char do_decimal_point() const
    {
        return ',';
    }
};

template <typename T> T toNumber(const std::string& in)
{
    bool failed = false;
    T val;
    std::istringstream inputStream(in);
    if (inputStream.str().find(",") != std::string::npos)
    {
        std::locale loc = std::locale(std::locale(), new UseCommaAsSeparator);
        inputStream.imbue(loc);
    }
    failed = (inputStream >> val).fail();
    if (true == failed)
        throw std::exception("extraction failed");
    return (val);
};

double toFloat(const std::string& in)
{
    bool failed = false;
    double val;
    
    auto result =  std::from_chars(in.data(), in.data()+in.size(), val);


    return (val);
};
double toInt(const std::string& in)
{
    bool failed = false;
    int val;

    auto result = std::from_chars(in.data(), in.data() + in.size(), val);


    return (val);
};

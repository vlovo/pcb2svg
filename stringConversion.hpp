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

double toDouble(const std::string& in)
{
    bool failed = false;
    double val=0.0;
    
    auto result =  std::from_chars(in.data(), in.data()+in.size(), val);


    return (val);
};

float toFloat(const std::string& in)
{
    bool failed = false;
    float val = 0.0;

    auto result = std::from_chars(in.data(), in.data() + in.size(), val);


    return (val);
};


int32_t toInt(const std::string& in)
{
    bool failed = false;
    int32_t val=0;

    auto result = std::from_chars(in.data(), in.data() + in.size(), val);
 

    return (val);
};

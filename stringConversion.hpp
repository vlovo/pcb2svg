#pragma once

#include <sstream>

template <typename T> T toNumber(const std::string &in)
{
    bool failed = false;
    T val;
    std::istringstream inputStream(in);

    failed = (inputStream >> val).fail();
    if (true == failed)
        throw std::exception("extraction failed");
    return (val);
};

inline double toDouble(const std::string &in)
{
    bool failed = false;
    double val = 0.0;
#if _MSVC_LANG == 202002L
    auto result = std::from_chars(in.data(), in.data() + in.size(), val);
#else
    val = toNumber<double>(in);
#endif

    return (val);
};

inline float toFloat(const std::string &in)
{
    bool failed = false;
    float val = 0.0;

#if _MSVC_LANG == 202002L
    auto result = std::from_chars(in.data(), in.data() + in.size(), val);
#else
    val = toNumber<float>(in);
#endif

    return (val);
};

inline int32_t toInt(const std::string &in)
{
    bool failed = false;
    int32_t val = 0;

#if _MSVC_LANG == 202002L
    auto result = std::from_chars(in.data(), in.data() + in.size(), val);
#else
    val = toNumber<int32_t>(in);
#endif

    return (val);
};

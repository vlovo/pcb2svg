

#include <functional>
#include <regex>
#include <string>
#include <variant>
#include <vector>

#include <filesystem>

#include "basicShapes.h"
#include "regexConstants.h"
#include "stringConversion.hpp"
#include "unitConversion.h"
#include <simple_svg_1.0.0.hpp>

namespace pcb2svg
{

svg::Polyline *polyline;

svg::Point lastPolyPoint;

BoundingBox box;

using Symbols = std::vector<std::variant<Circle, Rect, RoundRect, Oval, RoundDonut, Unknown>>;

std::unordered_map<std::string, int> statistics;

Symbols SymbolList(200, Unknown());

inline int DrawSymbol(svg::Document &doc, const std::smatch &param)
{

    auto x = inchtomm * toDouble(param[1].str());
    auto y = inchtomm * toDouble(param[2].str());

    auto symbol = SymbolList[toInt(param[3])];

    double penWidth = 0;

    int orientation = toInt(param[6]);

    std::visit(
        [&](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, Circle>)
            {
                doc << svg::Circle(svg::Point(x, y), arg.d, svg::Fill(svg::Color::Green),
                                   svg::Stroke(penWidth, svg::Color::Green));
                statistics["Circle"]++;
            }
            else if constexpr (std::is_same_v<T, Rect>)
            {
                x = x - (arg.width / 2.0);
                y = y + (arg.height / 2.0);
                auto w = arg.width;
                auto h = arg.height;
                switch (orientation)
                {
                case 0:
                    break;
                }

                doc << svg::Rectangle(svg::Point(x, y), w, h, svg::Fill(svg::Color::Green),
                                      svg::Stroke(penWidth, svg::Color::Green));

                statistics["Rectangle"]++;
            }
            else if constexpr (std::is_same_v<T, RoundRect>)
            {
                x = x - (arg.width / 2.0);
                y = y + (arg.height / 2.0);
                auto w = arg.width;
                auto h = arg.height;

                doc << svg::RoundRectangle(svg::Point(x, y), w, h, arg.radius, svg::Fill(svg::Color::Green),
                                           svg::Stroke(penWidth, svg::Color::Green));
                statistics["RoundRect"]++;
            }
            else if constexpr (std::is_same_v<T, RoundDonut>)
            {
                doc << svg::Circle(svg::Point(x, y), arg.od, svg::Fill(svg::Color::Transparent),
                                   svg::Stroke(penWidth, svg::Color::Green));
                doc << svg::Circle(svg::Point(x, y), arg.id, svg::Fill(svg::Color::Transparent),
                                   svg::Stroke(penWidth, svg::Color::Green));
                statistics["RoundDonut"]++;
            }
            else if constexpr (std::is_same_v<T, Unknown>)
            {
                statistics["Unknown"]++;
            }
        },
        symbol);

    return 1;
}

inline double LineWidth(size_t index)
{
    auto symbol = SymbolList[index];
    double ret = 0.0;
    std::visit(
        [&](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, Circle>)
            {

                ret = (arg.d);
            }

            else if constexpr (std::is_same_v<T, Rect>)
            {

                ret = (arg.width);
            }

            else if constexpr (std::is_same_v<T, RoundRect>)
            {

                ret = (arg.width);
            }
            else if constexpr (std::is_same_v<T, Oval>)
            {

                ret = (arg.width);
            }
        },
        symbol);

    return ret;
}

inline int DrawLine(svg::Document &doc, const std::smatch &param)
{

    double penWidth = LineWidth(toNumber<int>(param[5].str()));

    auto x1 = inchtomm * toDouble(param[1].str());
    auto y1 = inchtomm * toDouble(param[2].str());

    box.bounds(x1, y1);

    auto x2 = inchtomm * toDouble(param[3].str());
    auto y2 = inchtomm * toDouble(param[4].str());
    box.bounds(x2, y2);

    svg::Line line(svg::Point(x1, y1), svg::Point(x2, y2), svg::Stroke(penWidth, svg::Color::Black));

    doc << line;

    statistics["Line"]++;
    return 1;
}

inline int DrawArc(svg::Document &doc, const std::smatch &param)
{

    double penWidth = LineWidth(toInt(param[7].str()));

    auto x1 = inchtomm * toDouble(param[1].str());
    auto y1 = inchtomm * toDouble(param[2].str());
    auto x2 = inchtomm * toDouble(param[3].str());
    auto y2 = inchtomm * toDouble(param[4].str());

    box.bounds(x1, y1);
    auto rx = inchtomm * toDouble(param[5].str()) - x1;
    auto ry = inchtomm * toDouble(param[6].str()) - y1;

    auto r = std::max(std::abs(rx), std::abs(ry));

    int cw = param[10].str() == "Y" ? 1 : 0;

    doc << svg::Arc(svg::Point(x1, y1), svg::Point(x2, y2), r, cw, svg::Stroke(penWidth, svg::Color::Black));

    statistics["Arc"]++;
    return 1;
}

inline int DrawSegment(svg::Document &doc, const std::smatch &param)
{
    return 1;
}

inline int AddSymbol(svg::Document &doc, const std::smatch &param)
{

    int index = toInt(param[1]);

    if (param[2] == "r")
    {

        SymbolList[index] = (Circle(milTomm * toDouble(param[3].str())));
    }
    else if (param[2] == "rect")
    {
        auto w = milTomm * toDouble(param[3].str());
        auto h = milTomm * toDouble(param[4].str());

        if (param[5].str() == "r")
        {
            auto r = milTomm * toDouble(param[6].str());
            SymbolList[index] = (RoundRect(w, h, r));
        }
        else
        {
            SymbolList[index] = (Rect(w, h));
        }
    }
    else if (param[2] == "oval")
    {
        auto w = milTomm * toDouble(param[3].str());
        auto h = milTomm * toDouble(param[4].str());

        SymbolList[index] = (Oval(w, h));
    }
    else if (param[2] == "donut_r")
    {
        auto w = milTomm * toDouble(param[3].str());
        auto h = milTomm * toDouble(param[4].str());

        SymbolList[index] = (RoundDonut(w, h));
    }
    else
    {
        SymbolList[index] = Unknown();
    }
    return 1;
}

inline int CreatePolyline(svg::Document &doc, const std::smatch &param)
{

    if (polyline != nullptr)
        delete polyline;
    polyline = new svg::Polyline(svg::Fill(svg::Color::Transparent), svg::Stroke(outLineWidth, svg::Color::Red));

    auto x1 = inchtomm * toDouble(param[1].str());
    auto y1 = inchtomm * toDouble(param[2].str());

    *polyline << svg::Point(x1, y1);

    lastPolyPoint = svg::Point(x1, y1);

    box.bounds(x1, y1);
    statistics["Polyline"]++;
    return 0;
}

inline int AddPointToPolyline(svg::Document &doc, const std::smatch &param)
{

    if (polyline == nullptr)
    {
        polyline = new svg::Polyline(svg::Fill(svg::Color::Transparent), svg::Stroke(outLineWidth, svg::Color::Red));
    }

    auto x1 = inchtomm * toDouble(param[1].str());
    auto y1 = inchtomm * toDouble(param[2].str());

    box.bounds(x1, y1);

    if (polyline != nullptr)
    {
        *polyline << svg::Point(x1, y1);
    }
    lastPolyPoint = svg::Point(x1, y1);

    return 0;
}

inline int AddArcToPolyline(svg::Document &doc, const std::smatch &param)
{

    auto x1 = inchtomm * toDouble(param[1].str());
    auto y1 = inchtomm * toDouble(param[2].str());
    box.bounds(x1, y1);
    auto rx = inchtomm * toDouble(param[3].str()) - x1;
    auto ry = inchtomm * toDouble(param[4].str()) - y1;

    int sign = 1;

    if (rx < 0 || ry < 0)
        sign = 1;
    auto r = std::max(std::abs(rx), std::abs(ry));

    int cw = param[5].str() == "Y" ? 1 : 0;

    doc << svg::Arc(svg::Point(lastPolyPoint.x, lastPolyPoint.y), svg::Point(x1, y1), sign * r, cw,
                    svg::Stroke(outLineWidth, svg::Color::Red));

    lastPolyPoint = svg::Point(x1, y1);

    if (polyline != nullptr)
    {

        doc << *polyline;
        delete polyline;
        polyline = nullptr;

        polyline = new svg::Polyline(svg::Fill(svg::Color::Transparent), svg::Stroke(outLineWidth, svg::Color::Red));

        *polyline << svg::Point(x1, y1);
    }

    return 0;
}

inline int AddPolylineToDoc(svg::Document &doc, const std::smatch &param)
{
    if (polyline != nullptr)
    {
        doc << *polyline;
        delete polyline;
        polyline = nullptr;
    }
    return 0;
}
inline int SurfaceEnd(svg::Document &doc, const std::smatch &param)
{
    statistics["Surfaces"]++;
    return 0;
}

std::vector<std::pair<const std::regex, std::function<int(svg::Document &doc, const std::smatch &)>>> actionTable;

inline std::string pcb2svg(const std::string &filename, BoundingBox &boundbox, svg::Document &doc)
{
    std::cout << "\n  " << filename << " \n";
    statistics.clear();

    box = BoundingBox();

    actionTable.push_back({E, AddSymbol});
    actionTable.push_back({P, DrawSymbol});
    actionTable.push_back({A, DrawArc});
    actionTable.push_back({L, DrawLine});

    actionTable.push_back({OB, CreatePolyline});
    actionTable.push_back({OS, AddPointToPolyline});
    actionTable.push_back({OC, AddArcToPolyline});

    actionTable.push_back({OE, AddPolylineToDoc});
    actionTable.push_back({SE, SurfaceEnd});

    std::filesystem::path p(filename);

    std::ifstream file(p);

    std::stringstream ss;
    ss << file.rdbuf();

    auto groupElement = svg::elemStart("g") + svg::attribute("id", filename) + svg::elemClose();
    doc << groupElement;

    std::string data;
    std::smatch match;
    int cnt = 0;
    while (std::getline(ss, data))
    {
        cnt++;
        for (auto element : actionTable)
        {

            if (std::regex_match(data, match, element.first))
            {

                element.second(doc, match);
                break;
            }
        }
    }

    doc << svg::elemEnd("g");

    for (auto e : statistics)
    {
        std::cout << e.first << " = " << e.second << "\n";
    }

    std::cout << "-------------------------------------\n";

    boundbox = box;
    return doc.toString();
}
} // namespace pcb2svg

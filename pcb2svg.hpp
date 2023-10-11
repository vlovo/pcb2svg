
/*
* BSD 3-Clause License

Copyright (c) 2023, Markus Leitz

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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

constexpr svg::Color::Defaults PadColor = svg::Color::Green;
constexpr svg::Color::Defaults LineColor = svg::Color::Black;
constexpr svg::Color::Defaults ArcColor = svg::Color::Black;
constexpr svg::Color::Defaults SurfaceColor = svg::Color::Red;
using Symbols = std::vector<std::variant<Circle, Rect, RoundRect, Oval, RoundDonut, Unknown>>;

struct BoundingBox
{
    double x1, y1, x2, y2;

    BoundingBox()
    {
        x1 = std::numeric_limits<double>::max();
        x2 = -std::numeric_limits<double>::max();
        y1 = std::numeric_limits<double>::max();
        y2 = -std::numeric_limits<double>::max();
    }

    void update(double x, double y)
    {
        x1 = std::min(x, x1);
        x2 = std::max(x, x2);

        y1 = std::min(y, y1);
        y2 = std::max(y, y2);
    };
};
struct pcbdata_t
{

    pcbdata_t::pcbdata_t()
    {
        symbolList = Symbols(200, Unknown());
    }

    svg::Polyline *polyLine = nullptr;

    svg::Point lastPolyPoint;

    BoundingBox box;

    Symbols symbolList;

    std::unordered_map<std::string, int> statistics;
};

inline int AddSymbol(svg::Document &doc, const std::smatch &param, pcbdata_t &pcbdata)
{

    int index = toInt(param[1]);

    if (param[2] == "r")
    {

        pcbdata.symbolList[index] = Circle(milTomm * toDouble(param[3].str()));
    }
    else if (param[2] == "rect")
    {
        auto w = milTomm * toDouble(param[3].str());
        auto h = milTomm * toDouble(param[4].str());

        if (param[5].str() == "r")
        {
            auto r = milTomm * toDouble(param[6].str());
            pcbdata.symbolList[index] = RoundRect(w, h, r);
        }
        else
        {
            pcbdata.symbolList[index] = Rect(w, h);
        }
    }
    else if (param[2] == "oval")
    {
        auto w = milTomm * toDouble(param[3].str());
        auto h = milTomm * toDouble(param[4].str());

        pcbdata.symbolList[index] = Oval(w, h);
    }
    else if (param[2] == "donut_r")
    {
        auto w = milTomm * toDouble(param[3].str());
        auto h = milTomm * toDouble(param[4].str());

        pcbdata.symbolList[index] = RoundDonut(w, h);
    }
    else
    {
        pcbdata.symbolList[index] = Unknown();
        std::cout << param.str() << "\n";
        std::cout << "--------------------------------------------------\n";
    }
    return 1;
}

inline int DrawSymbol(svg::Document &doc, const std::smatch &param, pcbdata_t &pcbdata)
{

    auto x = inchtomm * toDouble(param[1].str());
    auto y = inchtomm * toDouble(param[2].str());

    auto symbol = pcbdata.symbolList[toInt(param[3])];

    double penWidth = 0;

    int orientation = toInt(param[6]);

    std::visit(
        [&](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, Circle>)
            {
                doc << svg::Circle(svg::Point(x, y), arg.d, svg::Fill(PadColor), svg::Stroke(penWidth, PadColor));
                pcbdata.statistics["Circle"]++;
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

                doc << svg::Rectangle(svg::Point(x, y), w, h, svg::Fill(PadColor), svg::Stroke(penWidth, PadColor));

                pcbdata.statistics["Rectangle"]++;
            }
            else if constexpr (std::is_same_v<T, RoundRect>)
            {
                x = x - (arg.width / 2.0);
                y = y + (arg.height / 2.0);
                auto w = arg.width;
                auto h = arg.height;

                doc << svg::RoundRectangle(svg::Point(x, y), w, h, arg.radius, svg::Fill(PadColor),
                                           svg::Stroke(penWidth, PadColor));
                pcbdata.statistics["RoundRect"]++;
            }
            else if constexpr (std::is_same_v<T, RoundDonut>)
            {
                doc << svg::Circle(svg::Point(x, y), arg.od, svg::Fill(svg::Color::Transparent),
                                   svg::Stroke(penWidth, PadColor));
                doc << svg::Circle(svg::Point(x, y), arg.id, svg::Fill(svg::Color::Transparent),
                                   svg::Stroke(penWidth, PadColor));
                pcbdata.statistics["RoundDonut"]++;
            }
            else if constexpr (std::is_same_v<T, Unknown>)
            {
                pcbdata.statistics["Unknown"]++;
            }
        },
        symbol);

    return 1;
}

inline double LineWidth(size_t index, const pcbdata_t &pcbdata)
{
    auto symbol = pcbdata.symbolList[index];
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

inline int DrawLine(svg::Document &doc, const std::smatch &param, pcbdata_t &pcbdata)
{

    double penWidth = LineWidth(toNumber<int>(param[5].str()), pcbdata);

    auto x1 = inchtomm * toDouble(param[1].str());
    auto y1 = inchtomm * toDouble(param[2].str());

    pcbdata.box.update(x1, y1);

    auto x2 = inchtomm * toDouble(param[3].str());
    auto y2 = inchtomm * toDouble(param[4].str());
    pcbdata.box.update(x2, y2);

    svg::Line line(svg::Point(x1, y1), svg::Point(x2, y2), svg::Stroke(penWidth, LineColor));

    doc << line;

    pcbdata.statistics["Line"]++;
    return 1;
}

inline int DrawArc(svg::Document &doc, const std::smatch &param, pcbdata_t &pcbdata)
{

    double penWidth = LineWidth(toInt(param[7].str()), pcbdata);

    auto x1 = inchtomm * toDouble(param[1].str());
    auto y1 = inchtomm * toDouble(param[2].str());
    auto x2 = inchtomm * toDouble(param[3].str());
    auto y2 = inchtomm * toDouble(param[4].str());

    pcbdata.box.update(x1, y1);
    auto rx = inchtomm * toDouble(param[5].str()) - x1;
    auto ry = inchtomm * toDouble(param[6].str()) - y1;

    auto r = std::max(std::abs(rx), std::abs(ry));

    int cw = param[10].str() == "Y" ? 1 : 0;

    doc << svg::Arc(svg::Point(x1, y1), svg::Point(x2, y2), r, cw, svg::Stroke(penWidth, ArcColor));

    pcbdata.statistics["Arc"]++;
    return 0;
}

inline int DrawSegment(svg::Document &doc, const std::smatch &param, pcbdata_t &pcbdata)
{
    return 0;
}

inline int CreatePolyline(svg::Document &doc, const std::smatch &param, pcbdata_t &pcbdata)
{

    if (pcbdata.polyLine != nullptr)
        delete pcbdata.polyLine;
    pcbdata.polyLine = new svg::Polyline(svg::Fill(svg::Color::Transparent), svg::Stroke(outLineWidth, SurfaceColor));

    auto x1 = inchtomm * toDouble(param[1].str());
    auto y1 = inchtomm * toDouble(param[2].str());

    *pcbdata.polyLine << svg::Point(x1, y1);

    pcbdata.lastPolyPoint = svg::Point(x1, y1);

    pcbdata.box.update(x1, y1);
    pcbdata.statistics["Polyline"]++;
    return 0;
}

inline int AddPointToPolyline(svg::Document &doc, const std::smatch &param, pcbdata_t &pcbdata)
{

    if (pcbdata.polyLine == nullptr)
    {
        pcbdata.polyLine =
            new svg::Polyline(svg::Fill(svg::Color::Transparent), svg::Stroke(outLineWidth, SurfaceColor));
    }

    auto x1 = inchtomm * toDouble(param[1].str());
    auto y1 = inchtomm * toDouble(param[2].str());

    pcbdata.box.update(x1, y1);

    if (pcbdata.polyLine != nullptr)
    {
        *pcbdata.polyLine << svg::Point(x1, y1);
    }
    pcbdata.lastPolyPoint = svg::Point(x1, y1);

    return 0;
}

inline int AddArcToPolyline(svg::Document &doc, const std::smatch &param, pcbdata_t &pcbdata)
{

    auto x1 = inchtomm * toDouble(param[1].str());
    auto y1 = inchtomm * toDouble(param[2].str());
    pcbdata.box.update(x1, y1);

    auto rx = inchtomm * toDouble(param[3].str()) - x1;
    auto ry = inchtomm * toDouble(param[4].str()) - y1;

    int sign = 1;

    if (rx < 0 || ry < 0)
    {
        sign = 1;
    }
    auto r = std::max(std::abs(rx), std::abs(ry));

    int cw = param[5].str() == "Y" ? 1 : 0;

    doc << svg::Arc(svg::Point(pcbdata.lastPolyPoint.x, pcbdata.lastPolyPoint.y), svg::Point(x1, y1), sign * r, cw,
                    svg::Stroke(outLineWidth, SurfaceColor));

    pcbdata.lastPolyPoint = svg::Point(x1, y1);

    if (pcbdata.polyLine != nullptr)
    {

        doc << *pcbdata.polyLine;
        delete pcbdata.polyLine;
        pcbdata.polyLine = nullptr;

        pcbdata.polyLine =
            new svg::Polyline(svg::Fill(svg::Color::Transparent), svg::Stroke(outLineWidth, SurfaceColor));

        *pcbdata.polyLine << svg::Point(x1, y1);
    }

    return 0;
}

inline int AddPolylineToDoc(svg::Document &doc, const std::smatch &param, pcbdata_t &pcbdata)
{
    if (pcbdata.polyLine != nullptr)
    {
        doc << *pcbdata.polyLine;
        delete pcbdata.polyLine;
        pcbdata.polyLine = nullptr;
    }
    return 0;
}

inline int SurfaceEnd(svg::Document &doc, const std::smatch &param, pcbdata_t &pcbdata)
{
    pcbdata.statistics["Surfaces"]++;
    return 0;
}

inline std::string pcb2svg(const std::string &filename, pcbdata_t &pcbdata, svg::Document &doc)
{

    std::vector<std::pair<const std::regex, std::function<int(svg::Document & doc, const std::smatch &, pcbdata_t &)>>>
        actionTable;

    std::cout << "\n  " << filename << " \n";
    pcbdata.statistics.clear();

    actionTable.push_back({F, AddSymbol});
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

    while (std::getline(ss, data))
    {

        for (auto element : actionTable)
        {

            if (std::regex_match(data, match, element.first))
            {

                element.second(doc, match, pcbdata);
                break;
            }
        }
    }

    doc << svg::elemEnd("g");

    return doc.toString();
}
} // namespace pcb2svg

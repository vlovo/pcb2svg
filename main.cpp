
#include <functional>
#include <iostream>
#include <regex>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <variant>

#include <filesystem>

#include "simple_svg_1.0.0.hpp"

#include "regexConstants.h"

#include "basicShapes.h"

#include "stringConversion.hpp"

#include "unitConversion.h"


using namespace  pcb2svg;

svg::Document doc;

svg::Polyline *polyline;

svg::Point lastPolyPoint;

using Symbols = std::vector<std::variant<Circle, Rect, RoundRect, Oval>>;


Symbols SymbolList;

int DrawSymbol(const std::smatch &param)
{
     
    auto x = inchtomm * toDouble(param[1].str());
    auto y = inchtomm * toDouble(param[2].str());

    auto symbol = SymbolList[toInt(param[3].str())];

    double penWidth = 0.05;

    std::visit(
        [&](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, Circle>)
            {
                doc << svg::Circle(svg::Point(x, y), arg.radius * 2, svg::Fill( ),svg::Stroke(penWidth,svg::Color::Green));
            }
            else if constexpr (std::is_same_v<T, Rect>)
            {
                doc << svg::Rectangle(svg::Point(x-(arg.width/2.0), y-(arg.height/2.0)), arg.width, arg.height, svg::Fill(), svg::Stroke(penWidth, svg::Color::Yellow));
            }
            else if constexpr (std::is_same_v<T, RoundRect>)
            {
                doc << svg::RoundRectangle(svg::Point(x - (arg.width / 2.0), y - (arg.height / 2.0)), arg.width, arg.height,arg.radius,svg::Fill(), svg::Stroke(penWidth, svg::Color::Yellow));
            }
        },
        symbol);

    return 1;
}

double LineWidth(size_t index)
{
    auto symbol = SymbolList[index];
    double ret = 0.0;
    std::visit(
        [&](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, Circle>)
            {

                ret = (arg.radius);
            }

            else if constexpr (std::is_same_v<T, Rect>)
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

int DrawLine(const std::smatch &param)
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

    return 1;
}

int DrawArc(const std::smatch &param)
{
    // std::cout << "\n  match " << param[0] << "\n";

    auto symbol = SymbolList[toInt(param[7].str())];

    

    double penWidth = LineWidth(toInt(param[7].str()));
   
    auto x1 = inchtomm * toDouble(param[1].str());
    auto y1 = inchtomm * toDouble(param[2].str());
    auto x2 = inchtomm * toDouble(param[3].str());
    auto y2 = inchtomm * toDouble(param[4].str());

    box.bounds(x1, y1);
    auto rx = inchtomm * toDouble(param[5].str()) - x1;
    auto ry = inchtomm * toDouble(param[6].str()) - y1;

    auto r = std::max(std::abs(rx), std::abs(ry));

    int cw = param[10].str() == "Y" ? 0 : 1;

    doc << svg::Arc(svg::Point(x1, y1), svg::Point(x2, y2),   r, cw,
        svg::Stroke(penWidth, svg::Color::Gray));

    return 1;
}

int n(const std::smatch &param)
{
    return 1;
}

int DrawSegment(const std::smatch &param)
{
    return 1;
}

int AddSymbol(const std::smatch &param)
{
    
    
    if (param[2] == "r")
    {

        SymbolList.push_back(Circle(milTomm * toDouble(param[3].str())));
    }
    if (param[2] == "rect")
    {
        auto w = milTomm * toDouble(param[3].str());
        auto h = milTomm * toDouble(param[4].str());

        if (param[5].str() == "r")
        {
            auto r = milTomm * toDouble(param[6].str());
            SymbolList.push_back(RoundRect(w, h,r));
        }
        else
        {
            SymbolList.push_back(Rect(w, h));
        }
    }
    if (param[2] == "oval")
    {
        auto w = milTomm * toDouble(param[3].str());
        auto h = milTomm * toDouble(param[4].str());

        SymbolList.push_back(Oval(w, h));
    }
    return 1;
}



int create = 0;
int CreatePolyline(const std::smatch &param)
{
   

    double penWidth = 0.05;
    if (polyline != nullptr)
        delete polyline;
    polyline = new svg::Polyline(svg::Fill(svg::Color::Red), svg::Stroke(penWidth, svg::Color::Red));
    ++create;
    auto x1 = inchtomm * toDouble(param[1].str());
    auto y1 = inchtomm * toDouble(param[2].str());

    *polyline << svg::Point(x1, y1);

    lastPolyPoint = svg::Point(x1, y1);

    box.bounds(x1, y1);
    return 0;
}

int AddPointToPolyline(const std::smatch &param)
{
  
    double penWidth = 0.03;
    if (polyline == nullptr)
    {
        polyline = new svg::Polyline(svg::Fill(svg::Color::Transparent), svg::Stroke(penWidth, svg::Color::Red));
        ++create;
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

int AddArcToPolyline(const std::smatch &param)
{

  

    double penWidth = 0.03;
    auto x1 = inchtomm * toDouble(param[1].str());
    auto y1 = inchtomm * toDouble(param[2].str());
    box.bounds(x1, y1);
    auto rx = inchtomm * toDouble(param[3].str()) - x1;
    auto ry = inchtomm * toDouble(param[4].str()) - y1;

    int sign = 1;

    if (rx < 0 || ry < 0)
        sign = 1;
    auto r = std::max(std::abs(rx), std::abs(ry));

    int cw = param[5].str() == "Y" ? 0 : 1;

    doc << svg::Arc(svg::Point(lastPolyPoint.x, lastPolyPoint.y), svg::Point(x1, y1), sign * r, cw,
                    svg::Stroke(penWidth, svg::Color::Red));

    lastPolyPoint = svg::Point(x1, y1);

    if (polyline != nullptr)
    {

        doc << *polyline;
        delete polyline;
        polyline = nullptr;

        double penWidth = 0.03;

        polyline = new svg::Polyline(svg::Fill(svg::Color::Transparent), svg::Stroke(penWidth, svg::Color::Red));
        ++create;
        *polyline << svg::Point(x1, y1);
    }

    return 0;
}

int pdelete = 0;
int AddPolylineToDoc(const std::smatch &param)
{
    if (polyline != nullptr)
    {
        doc << *polyline;
        delete polyline;
        polyline = nullptr;
        pdelete++;
    }
    return 0;
}




std::vector<std::pair<const std::regex, std::function<int(const std::smatch &)>>> actionTable;



int main()
{
  

    actionTable.push_back({E, AddSymbol});
    actionTable.push_back({P, DrawSymbol});
    actionTable.push_back({A, DrawArc});
    actionTable.push_back({L, DrawLine});

    actionTable.push_back({S, n});
    actionTable.push_back({OB, CreatePolyline});
    actionTable.push_back({OS, AddPointToPolyline});
    actionTable.push_back({OC, AddArcToPolyline});

    actionTable.push_back({OE, AddPolylineToDoc});
    actionTable.push_back({SE, n});

    std::filesystem::path p( R"(..\data\example1\soldermask_top)");
       

    for (auto const &dir_entry : std::filesystem::directory_iterator{p})
    {
      

        std::string name = dir_entry.path().filename().string();
        if ((name == "features" || name == "profile"))
        {
            std::cout << dir_entry.path() << '\n';
            std::ifstream file(dir_entry.path());

            std::stringstream ss;
            ss << file.rdbuf();
            

            svg::Dimensions dimensions(80, 45);
            doc = svg::Document(dir_entry.path().string() + ".svg", svg::Layout(dimensions, svg::Layout::TopLeft));

            std::string data;
            std::smatch match;
            while (std::getline(ss, data))
            {
               
                for (auto element : actionTable)
                {

                    if (std::regex_match(data, match, element.first))
                    {

                        element.second(match);
                        break;
                    }
                }
            }

           

            doc.setLayout(svg::Layout(dimensions, svg::Layout::TopLeft));

            doc.save();

            std::cout << box.x1 << " " << box.x2;
        }
    }
}

 

#include "pcb2svg.hpp"

int main(int argc, char **argv)
{
    std::string file;
    if (argc > 1)
    {
        file = std::string(argv[1]);
    }
    else
    {
        std::cout << "no file specified on command line. usage: pcb2svg.exe infile \n";
        file = "..\\data\\example1\\soldermask_bottom\\features";

        std::cout << "example file " << file << " is used\n";
    }

    svg::Document doc = svg::Document("", svg::Layout(svg::Dimensions(), svg::Layout::TopLeft));

    pcb2svg::BoundingBox box;

    // 1. read the   outline of  pcb board an determine  bounding box .
    pcb2svg::pcb2svg("..\\data\\example1\\signal_1\\profile", box, doc);

    // 2 .set layout /  viewbox of svg  and read the features file

    auto w = box.x2 - box.x1;
    auto h = box.y2 - box.y1;
    auto dimensions = svg::Dimensions(w, h);

    doc.setLayout(svg::Layout(dimensions, svg::Layout::TopLeft));
    doc.setViewBox(svg::ViewBox(svg::Dimensions(dimensions), svg::Point(box.x1, -(h + box.y1))));

    pcb2svg::pcb2svg("..\\data\\example1\\signal_1\\features", box, doc);

    //  3. write svg content to file
    std::ofstream out("out.svg");

    out << doc.toString();

    out.close();

    return 0;
}

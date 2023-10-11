/*
BSD 3 - Clause License

Copyright(c) 2023, Markus Leitz

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met :

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and /or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "pcb2svg.hpp"

int main(int argc, char **argv)
{
    std::string profile_file;
    std::string features_file;
    std::string output_file;
    if (argc > 3)
    {
        profile_file = std::string(argv[1]);
        features_file = std::string(argv[2]);
        output_file = std::string(argv[3]);
    }
    else
    {
        std::cout << "no file specified on command line. usage: pcb2svg.exe [profile_file] [features_file] \n";
        features_file = "..\\data\\example1\\signal_1\\features";
        profile_file = "..\\data\\example1\\signal_1\\profile";
        output_file = "out.svg";
    }

    svg::Document doc = svg::Document("", svg::Layout(svg::Dimensions(), svg::Layout::TopLeft));

    pcb2svg::pcbdata_t pcbdata;

    // 1. read the   outline of  pcb board an determine  bounding box .
    pcb2svg::pcb2svg(profile_file, pcbdata, doc);

    for (auto e : pcbdata.statistics)
    {
        std::cout << e.first << " = " << e.second << "\n";
    }

    std::cout << "-------------------------------------\n";

    // 2 .set layout /  viewbox of svg  and read the features file

    auto w = pcbdata.box.x2 - pcbdata.box.x1;
    auto h = pcbdata.box.y2 - pcbdata.box.y1;
    auto dimensions = svg::Dimensions(w, h);

    doc.setLayout(svg::Layout(dimensions, svg::Layout::TopLeft));
    doc.setViewBox(svg::ViewBox(svg::Dimensions(dimensions), svg::Point(pcbdata.box.x1, -(h + pcbdata.box.y1))));

    pcb2svg::pcb2svg(features_file, pcbdata, doc);

    for (auto e : pcbdata.statistics)
    {
        std::cout << e.first << " = " << e.second << "\n";
    }

    std::cout << "-------------------------------------\n";

    //  3. write svg content to file
    std::ofstream out(output_file);
    out << doc.toString();

    out.close();

    return 0;
}

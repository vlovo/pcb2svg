
#include "pcb2svg.hpp"


int main(int argc, char** argv)
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

	svg::Dimensions dimensions(80, 45);

	svg::Document  doc = svg::Document(file + ".svg", svg::Layout(dimensions, svg::Layout::TopLeft));
    
	pcb2svg::BoundingBox box;

	pcb2svg::pcb2svg("..\\data\\example1\\profile",box ,doc );


	//----------------------------------------------------------------------------


	dimensions = svg::Dimensions(box.x2 - box.x1, box.y2 - box.y1);
	
	doc = svg::Document(file + ".svg", svg::Layout(dimensions, svg::Layout::TopLeft));
	
	pcb2svg::pcb2svg("..\\data\\example1\\profile", box, doc);
	
	

	doc.setViewBox(svg::ViewBox(svg::Dimensions(dimensions), svg::Point(box.x1, box.y1)));
    
	
	auto svg = pcb2svg::pcb2svg(file,box,doc);

	std::ofstream out("out.svg");

	out << doc.toString();

	out.close();
  
	return 0;
}

 
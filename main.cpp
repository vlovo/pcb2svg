
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
		file = "..\\data\\example1\\soldermask_top\\features";
	//C:\Users\markus\source\repos\pcb2svg\data\example1\soldermask_top
		std::cout << "example file " << file << " is used\n";
	}
	auto svg = pcb2svg::pcb2svg(file);


	std::ofstream out("out.svg");

	out << svg;

	out.close();
  
	return 0;
}

 
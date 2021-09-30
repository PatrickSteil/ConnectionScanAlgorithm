#include "Station.h"
#include "Date.h"
#include "Connection.h"

/*
QUELLEN:
- https://www.systutorials.com/how-to-process-a-file-line-by-line-in-c/
- https://newbedev.com/c-split-string-by-regex
*/

#include <fstream>
#include <iostream>
#include <regex>

int main(int argc, char const *argv[])
{
	// TEST, Station reader
	// Array oder Vector (?)
	Station* stations[122];

	std::ifstream file("../HD_GTFS/stops.txt");
	std::string str;
	std::getline(file, str); // HEADER
	int i = 0;
	while (std::getline(file, str)) {
		std::cout << str << "\n";
		stations[i] = new Station(str);
		i++;
	}
	for (int i = 0; i < 10; ++i)
	{
		stations[i]->pprint();
	}
	return 0;
}
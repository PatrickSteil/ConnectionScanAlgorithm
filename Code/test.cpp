#include "Classes/GTFS_Reader.h"
#include "Classes/Connection.h"

#include <iostream>
#include <vector>
#include <chrono>

int main(int argc, char const *argv[]) {
	GTFS_Reader core("../HD_GTFS");
	core.init();
	for (auto i = core.connections.rbegin(); i != core.connections.rend(); ++i) {
		std::cout << (*(*i)) << std::endl;
	}	
	return 0;
}


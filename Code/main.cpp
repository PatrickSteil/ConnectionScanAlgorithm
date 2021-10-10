#include "Classes/GTFS_Reader.h"
#include "Classes/Connection.h"

#include <iostream>
#include <vector>

int main(int argc, char const *argv[]) {
	
	GTFS_Reader core("../HD_GTFS");
	core.init();

	// 120001 (& 120002) sind Albert-Fritz-Str, 122801 (& 122802) sind Bunsengymnasium (Mathematikon)
	std::vector<Connection*> result = core.csa_lines(core.getTimeAsInt("10:00:00"), 120001, 122801);

	for (std::vector<Connection*>::iterator i = result.begin(); i != result.end(); ++i)
	{
		std::cout << *(*i) << std::endl;
	}
	return 0;
}
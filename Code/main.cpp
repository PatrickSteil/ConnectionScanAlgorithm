#include "Classes/GTFS_Reader.h"
#include "Classes/Connection.h"

#include <iostream>
#include <vector>

int main(int argc, char const *argv[])
{

	GTFS_Reader core("../HD_GTFS");
	core.init();
	// core.csa(core.getTimeAsInt("08:00:00"), 120001, 114601);

	// Beispiel
	std::vector<Connection*> result = core.csa_lines(core.getTimeAsInt("08:00:00"), 120001, 122802);

	for (std::vector<Connection*>::iterator i = result.begin(); i != result.end(); ++i)
	{
		std::cout << *(*i) << std::endl;
	}
	return 0;
}
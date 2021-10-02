#include "Classes_2/GTFS_Reader.h"
#include "Classes_2/Connection.h"

#include <iostream>
#include <vector>

int main(int argc, char const *argv[])
{
	GTFS_Reader core("../HD_GTFS");

	core.readConnections();
	core.sortConnections();

	unsigned int time_dep = core.getTimeAsInt("12:00:00");
	
	std::vector<Connection>::iterator current_itr = core.findFirstDep(time_dep);
	for (int i = 0; i < 20; ++i)
	{
		std::cout << *current_itr++ << "\n";

	}
	return 0;
}
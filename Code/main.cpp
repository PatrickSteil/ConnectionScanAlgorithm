#include "Classes/GTFS_Reader.h"
#include "Classes/Connection.h"

#include <iostream>
#include <vector>

int main(int argc, char const *argv[])
{
	GTFS_Reader core("../HD_GTFS");

	core.init();

	unsigned int time_dep = core.getTimeAsInt("12:00:00");
	core.csa(time_dep, 120001, 114601);

	return 0;
}
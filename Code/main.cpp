#include "Classes_2/GTFS_Reader.h"
#include "Classes_2/Connection.h"

#include <iostream>
#include <vector>

int main(int argc, char const *argv[])
{
	GTFS_Reader core("../HD_GTFS");

	core.readConnections();

	core.sortConnections();

	core.pprint();
	return 0;
}
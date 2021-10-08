#include "Classes/GTFS_Reader.h"
#include "Classes/Connection.h"

#include <iostream>
#include <vector>

int main(int argc, char const *argv[])
{

	GTFS_Reader core("../HD_GTFS");
	core.init();
	// std::reverse(core.connections.begin(), core.connections.end());
	core.csa(core.getTimeAsInt("08:00:00"), 120001, 114601);

	// Beispiel
	/*
	std::vector<Connection*> result = core.csa_lines(core.getTimeAsInt("08:00:00"), 120001, 114601);

	for (std::vector<Connection*>::iterator i = result.begin(); i != result.end(); ++i)
	{
		std::cout << *(*i) << std::endl;
	}
	26-2-2026-28560: 120001 [28860] -> 121801 [28920]
	26-2-2026-28560: 121801 [28920] -> 679501 [28980]
	26-2-2026-28560: 679501 [28980] -> 114921 [29100]
	26-2-2026-28440: 114921 [28980] -> 427201 [29100]
	26-2-2026-28440: 427201 [29100] -> 427403 [29160]
	26-2-2026-28440: 427403 [29160] -> 115121 [29220]
	26-2-2026-28440: 115121 [29220] -> 114421 [29280]
	26-2-2026-28440: 114421 [29340] -> 119921 [29400]
	26-2-2026-28440: 119921 [29400] -> 118921 [29460]
	26-2-2026-28440: 118921 [29460] -> 119621 [29520]
	26-2-2026-28440: 119621 [29520] -> 114601 [29640]
	*/

	return 0;
}
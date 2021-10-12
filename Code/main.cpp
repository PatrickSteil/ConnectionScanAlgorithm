#include "Classes/GTFS_Reader.h"
#include "Classes/Connection.h"

#include <iostream>
#include <vector>

int main(int argc, char const *argv[]) {
	unsigned int dep_id, arr_id;
	std::string timestamp;

	GTFS_Reader core("../HD_GTFS");
	core.init();
	
	std::cout << core.stations.size() << " stations & " << core.connections.size() << " connections loaded" << std::endl;
	// core.csa_lines(core.getTimeAsInt("10:00:00"), 120001, 122801);
	// 120001 (& 120002) sind Albert-Fritz-Str, 122801 (& 122802) sind Bunsengymnasium (Mathematikon)
	std::cout << "From Station ID:\t";
	std::cin >> dep_id;
	std::cout << "\nTo Station ID:\t";
	std::cin >> arr_id;
	std::cout << "\nTimestamp:\t";
	std::cin >> timestamp;
	std::cout << "\n";
	std::vector<Connection*> result = core.csa_lines(core.getTimeAsInt(timestamp), dep_id, arr_id);

	for (std::vector<Connection*>::iterator i = result.begin(); i != result.end(); ++i)
	{
		std::cout << *(*i) << std::endl;
	}
	
	return 0;
}

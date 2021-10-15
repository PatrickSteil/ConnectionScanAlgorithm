#include "Classes/GTFS_Reader.h"
#include "Classes/Connection.h"

#include <iostream>
#include <vector>
#include <chrono>

int main(int argc, char const *argv[]) {
	using std::chrono::high_resolution_clock;
    	using std::chrono::duration_cast;
    	using std::chrono::duration;
    	using std::chrono::milliseconds;	

	unsigned int dep_id, arr_id;
	std::string time;

	GTFS_Reader core("../RNV_gtfs_original");
	core.init();

	std::cout << core.stations.size() << " stations & " << core.connections.size() << " connections loaded!" << std::endl;
	std::cout << "From Station:\t";
	std::cin >> dep_id;
	std::cout << "\nTo Station:\t";
	std::cin >> arr_id;
	std::cout << "\nTimestamp:\t";
	std::cin >> time;
	std::cout << "\n";
	// core.csa(core.getTimeAsInt("10:00:00"), 120001, 122801);
	// 120001 (& 120002) sind Albert-Fritz-Str, 122801 (& 122802) sind Bunsengymnasium (Mathematikon)
	auto t1 = high_resolution_clock::now();
	
	std::vector<Connection*> result = core.csa_lines(core.getTimeAsInt(time), dep_id, arr_id);
	
	auto t2 = high_resolution_clock::now();
	duration<double, std::milli> ms_double = t2 - t1;

	for (std::vector<Connection*>::iterator i = result.begin(); i != result.end(); ++i)
	{
		std::cout << *(*i) << std::endl;
	}
	std::cout << "Query took " << ms_double.count() << " ms!\n";
	return 0;
}

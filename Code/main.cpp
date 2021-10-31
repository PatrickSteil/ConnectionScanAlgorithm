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

	GTFS_Reader core("../RNV_gtfs_original");
	core.init();

	unsigned int dep_id, arr_id;
	std::string start_time, latest_arr;
	std::cout << "From Station:\t";
	std::cin >> dep_id;
	std::cout << "\nTo Station:\t";
	std::cin >> arr_id;
	std::cout << "\nEarliest Departure Timestamp:\t";
	std::cin >> start_time;
	std::cout << "\nLatest Arrival Timestamp:\t";
	std::cin >> latest_arr;
	std::cout << "\n";

	auto t1 = high_resolution_clock::now();

	std::vector<std::array<unsigned int, 2>> result = core.latest_dep_prof(dep_id, arr_id, core.getTimeAsInt(start_time), core.getTimeAsInt(latest_arr), 10);

	auto t2 = high_resolution_clock::now();
	
	for (auto i = result.begin(); i != result.end(); ++i) {
		std::cout << (*i)[0] << "\t" << (*i)[1] << "\n";
	}
	duration<double, std::milli> ms_double = t2 - t1;
	/*
	auto t1 = high_resolution_clock::now();
	
	std::vector<Connection*> result = core.csa_lines(core.getTimeAsInt(time), dep_id, arr_id);
	
	auto t2 = high_resolution_clock::now();
	duration<double, std::milli> ms_double = t2 - t1;

	for (std::vector<Connection*>::iterator i = result.begin(); i != result.end(); ++i)
	{
		std::cout << *(*i) << std::endl;
	}
	*/
	std::cout << "Query took " << ms_double.count() << " ms!\n";
	return 0;
}

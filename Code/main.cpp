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
	
	/*
	unsigned int start_time = core.getTimeAsInt("08:30:00");
	unsigned int latest_arr = core.getTimeAsInt("10:00:00");

	auto t1 = high_resolution_clock::now();

	std::vector<std::array<unsigned int, 2>> result = core.latest_dep_prof(120001, 122802, start_time, latest_arr, 10);

	auto t2 = high_resolution_clock::now();
	
	for (auto i = result.begin(); i != result.end(); ++i) {
		std::cout << (*i)[0] << "\t" << (*i)[1] << "\n";
	}
	duration<double, std::milli> ms_double = t2 - t1;
	*/
	unsigned int dep_id, arr_id;
	std::string time;
	std::cout << "From Station:\t";
	std::cin >> dep_id;
	std::cout << "\nTo Station:\t";
	std::cin >> arr_id;
	std::cout << "\nTimestamp:\t";
	std::cin >> time;
	std::cout << "\n";
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

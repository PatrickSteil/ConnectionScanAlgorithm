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

	std::vector<std::vector<Connection*>> result = core.earliest_arr_profile_journey_extraction(dep_id, arr_id, core.getTimeAsInt(start_time), core.getTimeAsInt(latest_arr));
	// std::vector<std::vector<Connection*>> result = core.earliest_arr_profile_journey_extraction_leg_opt(dep_id, arr_id, core.getTimeAsInt(start_time), core.getTimeAsInt(latest_arr));

	auto t2 = high_resolution_clock::now();
	
	duration<double, std::milli> ms_double = t2 - t1;
	for (unsigned int j = 0; j < result.size(); j++) {
	        std::cout << "Profile " << (j+1) << ":\n";
	        for (std::vector<Connection*>::iterator i = result[j].begin(); i != result[j].end(); ++i)
	        {
	                std::cout << *(*i) << std::endl;
	        }
	        std::cout << "\n\n";
	}
	std::cout << "Query took " << ms_double.count() << " ms!\n";
	return 0;
}

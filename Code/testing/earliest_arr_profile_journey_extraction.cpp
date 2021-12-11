#include "../Classes/GTFS_Reader.h"
#include "../Classes/Connection.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <stdlib.h>
// Parallel-stuff
#include <omp.h>

struct timeAndDistance {
	double time;
	double distance;
};

int main(int argc, char const *argv[]) {
	using std::chrono::high_resolution_clock;
    	using std::chrono::duration_cast;
    	using std::chrono::duration;
    	using std::chrono::milliseconds;	

	GTFS_Reader core("../../RNV_gtfs_original");
	core.init();
	
	srand (time(NULL));

	int counter;
	if (argc > 1) counter = std::atoi(argv[1]);
	else counter = 200;

	std::vector<timeAndDistance> results(counter);

	unsigned int lower_time = core.getTimeAsInt("10:00:00");
	unsigned int upper_time = core.getTimeAsInt("18:00:00");
	unsigned int dep_id, arr_id;

	int index_dep, index_arr;
	#pragma omp for
	for (int i = 0; i < counter; i++) {
		index_dep = rand() % core.stations.size();
		index_arr = rand() % core.stations.size();
		dep_id = core.stations[index_dep]->getID();
		arr_id = core.stations[index_arr]->getID();
		// std::cout << dep_id << " [" << index_dep << "]  -> " << arr_id << " [" << index_arr << "]\n";
		auto t1 = high_resolution_clock::now();
		core.earliest_arr_profile_journey_extraction(dep_id, arr_id, lower_time, upper_time);
		auto t2 = high_resolution_clock::now();
		duration<double, std::milli> ms_double = t2 - t1;
		results[i].time = ms_double.count();
		results[i].distance = core.stations[index_dep]->getDistance(*core.stations[index_arr]);
	}
	for (auto it = results.begin(); it != results.end(); ++it) std::cout << (*it).time << "\t" << (*it).distance << "\n";
	return 0;
}

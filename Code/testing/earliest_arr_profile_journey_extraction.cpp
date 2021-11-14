#include "../Classes/GTFS_Reader.h"
#include "../Classes/Connection.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <stdlib.h>
// Parallel-stuff
#include <omp.h>


int main(int argc, char const *argv[]) {
	using std::chrono::high_resolution_clock;
    	using std::chrono::duration_cast;
    	using std::chrono::duration;
    	using std::chrono::milliseconds;	

	GTFS_Reader core("../../RNV_gtfs_original");
	core.init();
	
	srand((unsigned) time(0));

	int counter;
	if (argc > 1) counter = std::atoi(argv[1]);
	else counter = 200;

	long double sum_all_times = 0;

	unsigned int lower_time = core.getTimeAsInt("10:00:00");
	unsigned int upper_time = core.getTimeAsInt("12:00:00");
	unsigned int dep_id, arr_id;

	int index_dep, index_arr;
	for (int i = 0; i < counter; i++) {
		index_dep = rand() % core.stations.size();
		index_arr = rand() % core.stations.size();
		dep_id = core.stations[index_dep]->getID();
		arr_id = core.stations[index_arr]->getID();
		std::cout << dep_id << " [" << index_dep << "]  -> " << arr_id << " [" << index_arr << "]\n";
		auto t1 = high_resolution_clock::now();
		core.earliest_arr_profile_journey_extraction(dep_id, arr_id, lower_time, upper_time);
		auto t2 = high_resolution_clock::now();
		duration<double, std::milli> ms_double = t2 - t1;
		sum_all_times += ms_double.count();
	}
	std::cout << "Number of iterations: " << counter << " and average running time: " << (sum_all_times / counter) << std::endl;
	return 0;
}

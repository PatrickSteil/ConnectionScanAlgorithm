#include "../Classes/GTFS_Reader.h"
#include "../Classes/Connection.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <stdlib.h> 

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
	
	srand(time(NULL));

	int counter;
	if (argc > 1) counter = std::atoi(argv[1]);
	else counter = 1000;
	
	std::vector<timeAndDistance> results(counter);

	long double sum_all_times = 0;

	unsigned int time = core.getTimeAsInt("00:00:00");
	unsigned int dep_id, arr_id;

	int index_dep, index_arr;
	for (int i = 0; i < counter; i++) {
		index_dep = rand() % core.stations.size();
		index_arr = rand() % core.stations.size();
		dep_id = core.stations[index_dep]->getID();
		arr_id = core.stations[index_arr]->getID();

		auto t1 = high_resolution_clock::now();
		core.csa_lines(time, dep_id, arr_id);
		auto t2 = high_resolution_clock::now();
		duration<double, std::milli> ms_double = t2 - t1;
		results[i].time = ms_double.count();
		results[i].distance = core.stations[index_dep]->getDistance(*core.stations[index_arr]);
	}
	for (auto it = results.begin(); it != results.end(); ++it)
		std::cout << (*it).time << "\t" << (*it).distance << "\n";
	return 0;
}

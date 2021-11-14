#include "../Classes/GTFS_Reader.h"
#include "../Classes/Connection.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <random>

int main(int argc, char const *argv[]) {
	using std::chrono::high_resolution_clock;
    	using std::chrono::duration_cast;
    	using std::chrono::duration;
    	using std::chrono::milliseconds;	

	GTFS_Reader core("../../RNV_gtfs_original");
	core.init();
	
	// https://stackoverflow.com/a/7560564
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, core.stations.size()-1);
	
	int counter;
	if (argc > 1) counter = std::atoi(argv[1]);
	else counter = 1000;

	long double sum_all_times = 0;

	unsigned int time = core.getTimeAsInt("10:00:00");
	unsigned int dep_id, arr_id;
	for (int i = 0; i < counter; i++) {
		dep_id = core.stations[distr(gen)]->getID();
		arr_id = core.stations[distr(gen)]->getID();

		auto t1 = high_resolution_clock::now();
		core.csa_lines(time, dep_id, arr_id);
		auto t2 = high_resolution_clock::now();
		duration<double, std::milli> ms_double = t2 - t1;
		sum_all_times += ms_double.count();
	}
	std::cout << "Number of CSA calls: " << counter << " and average running time: " << (sum_all_times / counter) << std::endl;
	return 0;
}

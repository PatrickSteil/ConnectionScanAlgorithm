#include "GTFS_Reader.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

void GTFS_Reader::readConnections() {
	std::string current_str, current_trip;

	std::ifstream file(this->filename + "/stop_times.txt");

	/*
		REMINDER! File looks like this:

		trip_id,stop_id,arrival_time,departure_time,stop_sequence\n
		21-1-1-32820,113603,09:07:00,09:07:00,1\n
		21-1-1-32820,133301,09:09:00,09:09:00,2\n
		21-1-1-32820,122601,09:11:00,09:11:00,3\n
		21-1-1-32820,122801,09:12:00,09:12:00,4\n
		...

		and we want it converted into this:
	
		Connection(113603, 133301, 09:07:00, 09:09:00)
		Connection(133301, 122601, 09:09:00, 09:11:00)
		Connection(122601, 122801, 09:11:00, 09:12:00)
		...
	*/
	
	std::getline(file, current_str);		// Header, we dont need -> just ignore the first line
	std::vector<std::string> splitter = this->split(current_str, ",");
	for (std::vector<std::string>::iterator i = splitter.begin(); i != splitter.end(); ++i) {
		std::cout << *i << "\n";
	}
	// while (std::getline(file, current_str))
}
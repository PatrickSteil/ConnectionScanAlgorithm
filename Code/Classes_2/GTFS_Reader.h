#ifndef GTFS_R_h
#define GTFS_R_h

#include "Core.h"
#include "Connection.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

class GTFS_Reader : public Core
{
private:
	std::string filename;

public:
	GTFS_Reader(std::string filename) { this->filename = filename; };
	~GTFS_Reader() {};
	
	void readStations() {};

	void readConnections() {
		// init the stuff we need
		std::string current_str, current_trip, current_dep_id, current_arr_id, current_dep_time, current_arr_time;
		std::vector<std::string>::iterator split_iter;
		std::vector<std::string> split;

		// TODO: THIS IS USING TEST FILE

		std::ifstream file(this->filename + "/stop_timesTEST.txt");

		/*
			REMINDER! File looks like this:

			trip_id,stop_id,arrival_time,departure_time,stop_sequence
			21-1-1-32820,113603,09:07:00,09:07:00,1
			21-1-1-32820,133301,09:09:00,09:09:00,2
			21-1-1-32820,122601,09:11:00,09:11:00,3
			21-1-1-32820,122801,09:12:00,09:12:00,4
			21-1-1-32820,122901,09:13:00,09:13:00,5
			21-1-1-32820,119921,09:16:00,09:16:00,6
			21-1-1-32820,118921,09:17:00,09:17:00,7
			21-1-1-32820,119621,09:18:00,09:18:00,8
			21-1-1-32820,116822,09:21:00,09:21:00,9
			21-2-2-25260,116822,07:01:00,07:01:00,1
			21-2-2-25260,119322,07:01:30,07:01:30,2
			21-2-2-25260,119402,07:02:30,07:02:30,3
			...

			and we want it converted into this:
		
			Connection(113603, 133301, 09:07:00, 09:09:00)
			Connection(133301, 122601, 09:09:00, 09:11:00)
			Connection(122601, 122801, 09:11:00, 09:12:00)
			...
		*/
		
		std::getline(file, current_str);		// Header, we dont need -> just ignore the first line

		std::getline(file, current_str);		// read "first" real line, fill first variables
		split_iter = this->split(current_str, ",").begin();
		current_trip = *split_iter++;
		current_dep_id = *split_iter++;
		split_iter++;
		current_dep_time = *split_iter;

		while (std::getline(file, current_str)) {
			split = this->split(current_str, ",");
			split_iter = split.begin();
			// string.compare == 0 if equal, so we can write this as negation (!)
			if (!current_trip.compare(*split_iter)) {
				split_iter++;
				// same trip, so we can create new Connection
				current_arr_id = *split_iter++;
				current_arr_time = *split_iter++;

				this->addConnection(
					Connection(std::stoi(current_dep_id), std::stoi(current_arr_id), this->getTimeAsInt(current_dep_time), this->getTimeAsInt(current_arr_time))
				);

				current_dep_id = current_arr_id;
				current_dep_time = *split_iter;
			} else {
				current_trip = *split_iter++;
				current_dep_id = *split_iter++;
				split_iter++;
				current_dep_time = *split_iter;
			}
		}
	}

	unsigned int getTimeAsInt(const std::string& time_stamp) {
		int hours = std::stoi(time_stamp.substr(0, 2));
		int minutes = std::stoi(time_stamp.substr(3, 2));
		int seconds = std::stoi(time_stamp.substr(6, 2));
		return (unsigned int) (seconds + minutes * 60 + hours * 3600);
	};

	std::vector<std::string> split(const std::string& str, const std::string& regex)
	{
		return {std::sregex_token_iterator(str.begin(), str.end(), std::regex(regex), -1), std::sregex_token_iterator()};
	}
};
#endif
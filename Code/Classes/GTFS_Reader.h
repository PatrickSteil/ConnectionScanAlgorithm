#ifndef GTFS_R_h
#define GTFS_R_h

#include "Core.h"
#include "Connection.h"
#include "Transfer.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <iterator>
#include <string>
#include <vector>

class GTFS_Reader : public Core
{
private:
	std::string filename;

	void readStations() {
		std::string current_str;
		std::vector<std::string>::iterator split_iter;	// "splitter" wäre ein cooler Name :D
		std::vector<std::string> split;

		std::ifstream file(this->filename + "/stops.txt");

		std::getline(file, current_str); 		// HEADER
		while (std::getline(file, current_str)) {
			split = this->split(current_str, ",");
			split_iter = split.begin();
			this->addStation(
				new Station(
					std::stoi(*split_iter++), 
					*split_iter++,
					*split_iter++,
					*split_iter++,
					std::stoi(*split_iter++),
					*split_iter
					)
			);
		}
		file.close();
	};

	void readConnections() {
		std::string current_str, 
			current_trip = "", 
			current_dep_id, 
			current_arr_id, 
			current_dep_time, 
			current_arr_time;
		std::vector<std::string>::iterator split_iter;	// "splitter" wäre ein cooler Name :D
		std::vector<std::string> split;

		std::ifstream file(this->filename + "/stop_times.txt");

		/*
			REMINDER! File looks like this:

			"trip_id,stop_id,arrival_time,departure_time,stop_sequence
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
			...""

			and we want it converted into this:
		
			Connection(pointer -> 113603, 133301, 09:07:00, 09:09:00, 21-1-1-32820)
			Connection(pointer -> 133301, 122601, 09:09:00, 09:11:00, 21-1-1-32820)
			Connection(pointer -> 122601, 122801, 09:11:00, 09:12:00, 21-1-1-32820)
			...
		*/
		
		std::getline(file, current_str);		// Header, we dont need -> just ignore the first line

		while (std::getline(file, current_str)) {
			split = this->split(current_str, ",");
			split_iter = split.begin();
			if (!current_trip.compare(*split_iter)) {
				split_iter++;
				// same trip, so we can create new Connection
				current_arr_id = *split_iter++;
				current_arr_time = *split_iter++;

				this->addConnection(
					new Connection(
						this->station_ptr_map[std::stoi(current_dep_id)],
						this->station_ptr_map[std::stoi(current_arr_id)],
						this->getTimeAsInt(current_dep_time), 
						this->getTimeAsInt(current_arr_time), 
						current_trip)
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

		file.close();
	}

	void readTransferFile() {
		// HEADER 
		// from_stop_id,to_stop_id,transfer_type,min_transfer_time
		std::string current_str;
		unsigned int current_dep_id, 
			current_arr_id, 
			current_duration;
		std::vector<std::string>::iterator split_iter;	// "splitter" wäre ein cooler Name :D
		std::vector<std::string> split;

		std::ifstream file(this->filename + "/transfers.txt");

		std::getline(file, current_str);		// Header, we dont need -> just ignore the first line
		while (std::getline(file, current_str)) {
			split = this->split(current_str, ",");
			split_iter = split.begin();
			current_dep_id = std::stoi(*split_iter++);
			current_arr_id = std::stoi(*split_iter++);
			split_iter++;
			current_duration = std::stoi(*split_iter);
			this->addTransfer(Transfer(this->station_ptr_map[current_dep_id], this->station_ptr_map[current_arr_id], current_duration));
		}
		file.close();
	};

public:
	GTFS_Reader(std::string filename) { this->filename = filename; };
	~GTFS_Reader() {};

	void init() {
		this->readStations();
		this->readConnections();
		this->sortConnections();
		this->readTransferFile();
	};
	
	
	unsigned int getTimeAsInt(const std::string& time_stamp) {
		int hours = std::stoi(time_stamp.substr(0, 2));
		int minutes = std::stoi(time_stamp.substr(3, 2));
		int seconds = std::stoi(time_stamp.substr(6, 2));
		return (unsigned int) (seconds + minutes * 60 + hours * 3600);
	};

	// https://gist.github.com/filsinger/3910580
	std::vector<std::string> split(const std::string& str, const std::string& regex) {
		std::regex r(regex);
		return {std::sregex_token_iterator(str.begin(), str.end(), r, -1), std::sregex_token_iterator()};
	}
};
#endif

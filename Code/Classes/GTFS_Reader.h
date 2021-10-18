#ifndef GTFS_R_h
#define GTFS_R_h

// CSA
#include "Core.h"

// Classes to handle the format
#include "Connection.h"
#include "Transfer.h"

// k-d-Tree of nanoflann
#include "../nanoflann/nanoflann.hpp"
#include "../nanoflann/utils.h"
#include <ctime>
#include <cstdlib>

using namespace nanoflann;

#include <fstream>
#include <iostream>
#include <regex>
#include <iterator>
#include <string>
#include <vector>

// Parallel-stuff
#include <omp.h>

class GTFS_Reader : public Core
{
private:
	PointCloud<float> cloud;
	std::string filename;

	unsigned int getNumberOfLinesInFile(std::string file_name) {
		unsigned int number_of_lines = 0;
		std::string line;
		std::ifstream myfile(this->filename+"/"+file_name);

		while (std::getline(myfile, line)) ++number_of_lines;
		myfile.close();
		// first line is Header
		return --number_of_lines;
	}

	void readStations() {
		unsigned int number_of_stations = this->getNumberOfLinesInFile("stops.txt");
		this->stations.reserve(number_of_stations);
		this->station_ptr_map.reserve(number_of_stations);

		std::string current_str;
		std::vector<std::string>::iterator split_iter;	// "splitter" wäre ein cooler Name :D
		std::vector<std::string> split;

		std::ifstream file(this->filename + "/stops.txt");

		std::getline(file, current_str); 		// HEADER
		while (std::getline(file, current_str)) {
			split = this->split(current_str, ",");
			split_iter = split.begin();
			this->addStation(
			/*	new Station(
					std::stoi(*split_iter++), 
					*split_iter++,
					*split_iter++,
					*split_iter++,
					std::stoi(*split_iter++),
					*split_iter
				)
				*/
				new Station(
					std::stoi(*split_iter++), 
					*split_iter++,
					*split_iter++,
					*split_iter
				)
			);
		}
		file.close();
	};

	void readConnections() {
		this->connections.reserve(this->getNumberOfLinesInFile("stop_times.txt"));
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
	};

	void readTransferFile() {
		// HEADER 
		// from_stop_id,to_stop_id,transfer_type,min_transfer_time
		std::string current_str;
		unsigned int current_dep_id, 
			current_arr_id, 
			current_duration;
		std::vector<std::string>::iterator split_iter;	// "splitter" wäre ein cooler Name :D
		std::vector<std::string> split;
		Station *dep_ptr;

		std::ifstream file(this->filename + "/transfers.txt");

		if (!file.is_open()) {
			std::cout << "No " << this->filename << "/transfers.txt found" << std::endl;
			return;
		}
		std::getline(file, current_str);		// Header, we dont need -> just ignore the first line
		while (std::getline(file, current_str)) {
			split = this->split(current_str, ",");
			split_iter = split.begin();
			current_dep_id = std::stoi(*split_iter++);
			current_arr_id = std::stoi(*split_iter++);
			split_iter++;
			current_duration = std::stoi(*split_iter);
			dep_ptr = this->station_ptr_map[current_dep_id];
			dep_ptr->addTransfer(new Transfer(dep_ptr, this->station_ptr_map[current_arr_id], current_duration));
		}
		file.close();
	};


	void createCloud() {
		this->cloud.pts.resize(this->stations.size());

		#pragma omp parallel
		#pragma omp for
		for (int i = 0; i < this->stations.size(); ++i) {
			this->cloud.pts[i].x = this->stations[i]->getLatAsFloat();
			this->cloud.pts[i].y = this->stations[i]->getLonAsFloat();
		}
	};

public:
	GTFS_Reader(std::string filename) { this->filename = filename; };
	~GTFS_Reader() {};

	void init() {
		this->readStations();
		this->readConnections();
		#pragma omp parallel 
		#pragma omp single
		{
			#pragma omp task
			{
				this->sortConnections();
			}
			#pragma omp task
			{
				this->readTransferFile();
			}
		}
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
	};

	void createTransferFile() {
		/*
		creates transfers.txt file from the stations
		uses this repo [https://github.com/jlblancoc/nanoflann] and a self-written distance adapter
		*/
		this->createCloud();
		std::ofstream trans_file(this->filename+"/transfers.txt");

		// set header
		trans_file << "from_stop_id,to_stop_id,transfer_type,min_transfer_time";

		typedef KDTreeSingleIndexAdaptor<L_Coordinates_2_dim<float, PointCloud<float> > ,PointCloud<float>,2> my_kd_tree_t;

		my_kd_tree_t tree(2, cloud, KDTreeSingleIndexAdaptorParams(10));
		tree.buildIndex();

		// to calc the seconds, speed is given in m/s
		const float avr_walking_speed = 1.4;
		const float search_radius = static_cast<float>(200); 	// meter
		std::vector<std::pair<uint32_t, float>> ret_matches;

		nanoflann::SearchParams params;
		//params.sorted = false;

		size_t nMatches;

		#pragma omp parallel for
		for (std::vector<Station*>::iterator i = this->stations.begin(); i != this->stations.end(); ++i)
		{
			float current_ptr[2] = { (*i)->getLatAsFloat(), (*i)->getLonAsFloat() };
			nMatches = tree.radiusSearch(&current_ptr[0], search_radius, ret_matches, params);
			for (size_t index = 0; index < nMatches; index++) {
				if (this->stations[ret_matches[index].first]->getID() != (*i)->getID()) {
					#pragma omp critical
					{
						trans_file << std::endl << (*i)->getID() << "," << this->stations[ret_matches[index].first]->getID() << ",2," << (int) (ret_matches[index].second / avr_walking_speed);
					}
				}
			}
			ret_matches.clear();
		}
		trans_file.close();
	}
};
#endif

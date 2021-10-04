#ifndef PARSER_h
#define PARSER_h

#include "CSA.h"
#include "Connection.h"
#include "Station.h"
#include "Date.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

class Parser: public CSA {
private:
	// ohne backslash am Ende. Also "../HD_GTFS", nicht "../HD_GTFS/"
	std::string filename;

	void loadGTFS() {
		this->loadStations();
		this->loadConnections();
	}

	void loadStations() {
		std::string str;

		// load stops
		std::ifstream file(this->filename + "/stops.txt");
		
		std::getline(file, str); 	// HEADER, den wollen wir nicht
		while (std::getline(file, str))
			this->addStation(Station(str));
	}

	void loadConnections () {
		std::string str;
		
		// load stop_times
		std::ifstream file(this->filename + "/stop_times.txt");
		
		std::getline(file, str); 	// HEADER, den wollen wir nicht
	}

public:
	Parser(std::string filename) {
		this->filename = filename;
		this->loadGTFS();
	};
	~Parser() {};

	void setFileName(std::string filename) {
		this->filename = filename;
	}

	void pprint() {
		this->stations.front().pprint();
	}
};
#endif
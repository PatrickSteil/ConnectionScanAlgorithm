#ifndef CSA_h
#define CSA_h

#include "Station.h"
#include "Connection.h"
#include "Date.h"

#include <string>
#include <iostream>

class CSA
{
protected:
	std::vector<Connection> connections;
	std::vector<Station> stations;

public:
	CSA() {};
	~CSA() {};

	void addConnection(Connection con) {
		this->connections.push_back(con);
	}

	void addStation(Station station) {
		this->stations.push_back(station);
	}

	void scan(Station dep, Station arr, Date dep_time) {}
};

#endif
#ifndef CORE_h
#define CORE_h

#include "Connection.h"
#include "Station.h"
#include <algorithm>
#include <vector>
#include <unordered_map>

class Core
{
public:
	std::vector<Connection> connections;
	std::vector<Station> stations;
	Core() {};
	~Core() {};
	
	void addConnection(Connection new_connection) {
		// TODO: 
		/* 
			- Maybe using a Binary Tree or BTree as Navigation Structure for better time results
			- How can the complexity of binary_sarch on a vector be log(n) https://www.cplusplus.com/reference/algorithm/binary_search/
		*/

		// this->connections.insert(std::upper_bound(this->connections.begin(), this->connections.end(), new_connection), new_connection);
		this->connections.push_back(new_connection);
	}

	void addStation(Station new_station) { this->stations.push_back(new_station); }

	void sortConnections() { std::sort(this->connections.begin(), this->connections.end()); }

	std::vector<Connection>::iterator findFirstDep(unsigned int dep_time) {
		// lower_bound has log_2(n) Complexity
		return std::lower_bound(this->connections.begin(), this->connections.end(), dep_time);
	}

	void pprint() {
		for (std::vector<Connection>::iterator i = this->connections.begin(); i != this->connections.end(); ++i)
			std::cout << (*i) << '\n';
	}

	// TODO
	void csa(unsigned int time, unsigned int from_id, unsigned int to_id) {
		unsigned int infty = (~0);
		// https://stackoverflow.com/a/3578247
		std::unordered_map<unsigned int, unsigned int> map;

		map.reserve(this->stations.size());

		for (std::vector<Station>::iterator i = this->stations.begin(); i != this->stations.end(); ++i)
		{
			map[(*i).getID()] = infty;
		}

		// init departure id with time
		map[from_id] = time;

		// find first departure at time
		std::vector<Connection>::iterator conn = this->findFirstDep(time);

		bool notFinished = true;

		while (conn != this->connections.end() && notFinished) {
			// can the connection be reached ?
			if ( map[(*conn).getDepartureID()] != infty) {
				// update time in map
				if ( map[(*conn).getArrivalID()] > (*conn).getArrivalTime() ) {
					map[(*conn).getArrivalID()] = (*conn).getArrivalTime();
				}
				if ((*conn).getArrivalID() == to_id) {
					std::cout << "THE Conncetion: " << (*conn) << std::endl;
					notFinished = false;
				}
			}
			conn++;
		}
		// print the map
		for(auto it = map.cbegin(); it != map.cend(); ++it)
		{
		    std::cout << it->first << ": " << it->second << "\n";
		}
	};
};

#endif
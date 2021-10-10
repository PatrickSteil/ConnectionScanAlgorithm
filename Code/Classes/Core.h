#ifndef CORE_h
#define CORE_h

#include "Connection.h"
#include "Station.h"
#include "Transfer.h"
#include <algorithm>
#include <vector>
#include <unordered_map>

class Core
{
public:
	std::vector<Connection> connections;
	std::vector<Station> stations;
	std::unordered_map<unsigned int, std::vector<Transfer>> transfer_map;
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

	void addTransfer(Transfer new_transfer) { this->transfer_map[new_transfer.getDepartureID()].push_back(new_transfer); }

	void sortConnections() { std::sort(this->connections.begin(), this->connections.end()); }

	void sortStationsByLat()
	{ 
		std::sort(this->stations.begin(), this->stations.end(),
			[](Station & a, Station & b) { 
				return a.getLatAsFloat() > b.getLatAsFloat(); 
			}
		); 
	}

	std::vector<Connection>::iterator findFirstDep(unsigned int dep_time) {
		// lower_bound has log_2(n) Complexity
		return std::lower_bound(this->connections.begin(), this->connections.end(), dep_time);
	}

	void pprint() {
		for (std::vector<Connection>::iterator i = this->connections.begin(); i != this->connections.end(); ++i)
			std::cout << (*i) << '\n';
	}

	std::vector<Transfer>* getTransferByStationID(unsigned int id) {
		return &(this->transfer_map[id]);
	}


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

		std::vector<Transfer> *current_transfers;

		bool notFinished = true;

		while (conn != this->connections.end() && notFinished) {
			// can the connection be reached ?
			if (map[(*conn).getDepartureID()] != infty) {
				if (map[to_id] < (*conn).getDepartureTime()) {
					// this connection departs later than a earlier found journey to to_id, so we can stop
					notFinished = false;
				} else {
					// update time in map
					if (map[(*conn).getArrivalID()] > (*conn).getArrivalTime()) {
						map[(*conn).getArrivalID()] = (*conn).getArrivalTime();
					}
					current_transfers = this->getTransferByStationID((*conn).getArrivalID());
					for (std::vector<Transfer>::iterator trans = (*current_transfers).begin(); trans != (*current_transfers).end(); ++trans)
					{
						if (map[(*trans).getArrivalID()] > map[(*conn).getArrivalID()] + (*trans).getDuration()) {
							map[(*trans).getArrivalID()] = map[(*conn).getArrivalID()] + (*trans).getDuration();
						}
					}
				}
			}
			conn++;
		}
		// print the map
		for(auto it = map.cbegin(); it != map.cend(); ++it)
		{
			if (it->second != infty)
				std::cout << it->first << ": " << it->second << "\n";
		}
	};


	std::vector<Connection*> csa_lines(unsigned int time, unsigned int from_id, unsigned int to_id) {
		unsigned int infty = (~0);

		// https://stackoverflow.com/a/3578247
		std::unordered_map<unsigned int, Connection*> map;

		map.reserve(this->stations.size());

		for (std::vector<Station>::iterator i = this->stations.begin(); i != this->stations.end(); ++i)
		{
			map[(*i).getID()] = new Connection();
		}

		map[from_id] = new Connection(from_id, from_id, time, time);

		std::vector<Transfer> *current_transfers;

		// find first departure at time
		std::vector<Connection>::iterator conn = this->findFirstDep(time);

		bool notFinished = true;

		while (conn != this->connections.end() && notFinished) {
			// can the connection be reached ? the right part is to check if we are at the station at the departureTime
			if (map[(*conn).getDepartureID()]->getArrivalID() != infty && map[(*conn).getDepartureID()]->getArrivalTime() <= (*conn).getDepartureTime()) {
				if (map[to_id]->getDepartureTime() < (*conn).getDepartureTime()) {
					// this connection departs later than a earlier found journey to to_id, so we can stop
					notFinished = false;
				} else {
					// update time in map
					if (map[(*conn).getArrivalID()]->getArrivalTime() > (*conn).getArrivalTime()) {
						map[(*conn).getArrivalID()] = &(*conn);
					}
					current_transfers = this->getTransferByStationID((*conn).getArrivalID());
					for (std::vector<Transfer>::iterator trans = (*current_transfers).begin(); trans != (*current_transfers).end(); ++trans)
					{
						if (map[(*trans).getArrivalID()]->getArrivalTime() > map[(*conn).getArrivalID()]->getArrivalTime() + (*trans).getDuration()) {
							map[(*trans).getArrivalID()] = new Connection((*trans).getDepartureID(), (*trans).getArrivalID(), map[(*conn).getArrivalID()]->getArrivalTime(), map[(*conn).getArrivalID()]->getArrivalTime() + (*trans).getDuration(), "Walking");
						}
					}
				}
			}
			conn++;
		}
		
		if (map[to_id]->getArrivalTime() == infty) {
			return {};
		}
		std::vector<Connection*> result;

		unsigned int current_station = to_id;
		while (map[current_station]->getDepartureID() != from_id) {
			result.push_back(map[current_station]);
			current_station = map[current_station]->getDepartureID();
		}
		result.push_back(map[current_station]);
		std::reverse(result.begin(), result.end());
		return result;
	};
};

#endif
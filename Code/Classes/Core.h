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
	std::vector<Connection*> connections;
	std::vector<Station*> stations;
	std::unordered_map<unsigned int, std::vector<Transfer>> transfer_map;
	std::unordered_map<unsigned int, Station*> station_ptr_map;
	Core() {};
	~Core() {};
	
	void addConnection(Connection *new_connection_ptr) { this->connections.push_back(new_connection_ptr);}

	void addStation(Station *new_station_ptr) {
		this->stations.push_back(new_station_ptr);
		this->station_ptr_map[new_station_ptr->getID()] = new_station_ptr;
	}

	void addTransfer(Transfer new_transfer) { this->transfer_map[new_transfer.getDepartureID()].push_back(new_transfer); }


	void sortConnections() { 
		std::sort(this->connections.begin(), this->connections.end(), [](Connection *a, Connection *b) { return (*a < *b);});
	}

	std::vector<Connection*>::iterator findFirstDep(unsigned int dep_time) {
		return std::lower_bound(this->connections.begin(), this->connections.end(), dep_time, [](Connection *a, unsigned int time) { return (*a < time); });
	}

	std::vector<Transfer>* getTransferByStationID(unsigned int id) { return &(this->transfer_map[id]); }


	void csa(unsigned int time, unsigned int from_id, unsigned int to_id) {
		unsigned int infty = (~0);

		// https://stackoverflow.com/a/3578247
		std::unordered_map<unsigned int, unsigned int> map;

		map.reserve(this->stations.size());

		for (std::vector<Station*>::iterator i = this->stations.begin(); i != this->stations.end(); ++i)
		{
			map[(*i)->getID()] = infty;
		}

		// init departure id with time
		map[from_id] = time;

		// find first departure at time
		std::vector<Connection*>::iterator conn_itr = this->findFirstDep(time);

		std::vector<Transfer*> *current_transfers;

		bool notFinished = true;

		while (conn_itr != this->connections.end() && notFinished) {
			// can the connection be reached ?
			if (map[(*conn_itr)->getDepartureID()] != infty) {
				if (map[to_id] < (*conn_itr)->getDepartureTime()) {
					// this connection departs later than a earlier found journey to to_id, so we can stop
					notFinished = false;
				} else {
					// update time in map
					if (map[(*conn_itr)->getArrivalID()] > (*conn_itr)->getArrivalTime()) {
						map[(*conn_itr)->getArrivalID()] = (*conn_itr)->getArrivalTime();
					}
					current_transfers = (*conn_itr)->getArrival()->getTransfers();
					// current_transfers = this->getTransferByStationID((*conn_itr)->getArrivalID());
					for (std::vector<Transfer*>::iterator trans = (*current_transfers).begin(); trans != (*current_transfers).end(); ++trans)
					{
						if (map[(*trans)->getArrivalID()] > map[(*conn_itr)->getArrivalID()] + (*trans)->getDuration()) {
							map[(*trans)->getArrivalID()] = map[(*conn_itr)->getArrivalID()] + (*trans)->getDuration();
						}
					}
				}
			}
			conn_itr++;
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

		for (std::vector<Station*>::iterator i = this->stations.begin(); i != this->stations.end(); ++i)
		{
			map[(*i)->getID()] = new Connection();
		}

		map[from_id] = new Connection(this->station_ptr_map[from_id], this->station_ptr_map[from_id], time, time);

		std::vector<Transfer*> *current_transfers;

		// find first departure at time
		std::vector<Connection*>::iterator conn_itr = this->findFirstDep(time);

		bool notFinished = true;

		while (conn_itr != this->connections.end() && notFinished) {
			// can the connection be reached ? the right part is to check if we are at the station at the departureTime
			if (map[(*conn_itr)->getDepartureID()]->getArrivalTime() != infty && map[(*conn_itr)->getDepartureID()]->getArrivalTime() <= (*conn_itr)->getDepartureTime()) {
				if (map[to_id]->getDepartureTime() < (*conn_itr)->getDepartureTime()) {
					// this connection departs later than a earlier found journey to to_id, so we can stop
					notFinished = false;
				} else {
					// update time in map
					if (map[(*conn_itr)->getArrivalID()]->getArrivalTime() > (*conn_itr)->getArrivalTime()) {
						map[(*conn_itr)->getArrivalID()] = (*conn_itr);
					}
					current_transfers = (*conn_itr)->getArrival()->getTransfers();
					// current_transfers = this->getTransferByStationID((*conn_itr)->getArrivalID());
					for (std::vector<Transfer*>::iterator trans = (*current_transfers).begin(); trans != (*current_transfers).end(); ++trans)
					{
						if (map[(*trans)->getArrivalID()]->getArrivalTime() > map[(*conn_itr)->getArrivalID()]->getArrivalTime() + (*trans)->getDuration()) {
							map[(*trans)->getArrivalID()] = new Connection((*trans)->getDeparture(), (*trans)->getArrival(), map[(*conn_itr)->getArrivalID()]->getArrivalTime(), map[(*conn_itr)->getArrivalID()]->getArrivalTime() + (*trans)->getDuration(), "Platform: "+(*trans)->getDeparture()->getPlatformCode()+" -> "+(*trans)->getArrival()->getPlatformCode());
						}
					}
				}
			}
			conn_itr++;
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

#ifndef CORE_h
#define CORE_h

#include "Connection.h"
#include "Station.h"
#include "Transfer.h"
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <omp.h>
#include <cassert>

class Core
{
public:
	std::vector<Connection*> connections;
	std::vector<Station*> stations;
	std::vector<std::string> trips;

	std::unordered_map<unsigned int, Station*> station_ptr_map;
	Core() {};
	~Core() {};
	
	void addConnection(Connection *new_connection_ptr) {
		this->connections.push_back(new_connection_ptr);
	}

	void addStation(Station *new_station_ptr) {
		this->stations.push_back(new_station_ptr);
		this->station_ptr_map[new_station_ptr->getID()] = new_station_ptr;
	}

	void sortConnections() { 
		std::sort(this->connections.begin(), this->connections.end(),
			       [](Connection *a, Connection *b) {
					return (a->getDepartureTime() < b->getDepartureTime());
		});
	}

	std::vector<Connection*>::iterator findFirstDep(unsigned int dep_time) {
		return std::lower_bound(this->connections.begin(), this->connections.end(), dep_time, [](Connection *a, unsigned int time) { return (*a < time); });
	}

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

		// we can fill in the footpath from start id
		std::vector<Transfer*> *current_transfers;

		current_transfers = this->station_ptr_map[from_id]->getTransfers();
		for (std::vector<Transfer*>::iterator trans = (*current_transfers).begin(); trans != (*current_transfers).end(); ++trans)
		{
			map[(*trans)->getArrivalID()] = time + (*trans)->getDuration();
		}

		// find first departure at time
		std::vector<Connection*>::iterator conn_itr = this->findFirstDep(time);

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

	std::vector<std::array<unsigned int, 2>> earliest_arr_profile(unsigned int from_id, unsigned int to_id, unsigned int timestamp) {
		// csa_overview - Page 15 ff.
		unsigned int infty = (~0);

		std::unordered_map<std::string, unsigned int> T;
		// Profile <=> int [2] = {dep_time, arr_time}
		std::unordered_map<unsigned int, std::vector<std::array<unsigned int, 2>>> S;
		
		// init T & S
		for (std::vector<std::string>::iterator i = this->trips.begin(); i != this->trips.end(); ++i)
		{
			T[(*i)] = infty;
		}
		for (std::vector<Station*>::iterator i = this->stations.begin(); i != this->stations.end(); ++i)
		{
			S[(*i)->getID()].push_back({infty, infty});
		}

		unsigned int t1, t2, t3, t_c, c_arr_time;
		for (std::vector<Connection*>::reverse_iterator i = this->connections.rbegin(); i != this->connections.rend(); ++i ) {
			t1 = infty;
			t3 = infty;
			c_arr_time = (*i)->getArrivalTime();

			if ((*i)->getArrivalID() == to_id) t1 = c_arr_time;
			t2 = T[(*i)->getTripID()];

			std::vector<std::array<unsigned int, 2>>::iterator profile_itr = S[(*i)->getArrivalID()].begin();

			while ((*profile_itr)[0] < c_arr_time) profile_itr++;
			t3 = (*profile_itr)[1];

			t_c = std::min({t1, t2, t3});

			std::array<unsigned int, 2> p = {{(*i)->getDepartureTime(), t_c}};
			std::vector<std::array<unsigned int, 2>>::iterator q = S[(*i)->getDepartureID()].begin();
			if (p[0] <= (*q)[0] && p[1] <= (*q)[1]) {
				if (p[0] != (*q)[0]) {
					S[(*i)->getDepartureID()].insert(q, p);
				} else {
					(*q)[1] = p[1];
				}
			}
			T[(*i)->getTripID()] = t_c;
		}
		
		return S[from_id];	
		/*
		for (std::vector<std::string>::iterator i = this->trips.begin(); i != this->trips.end(); ++i)
                {
			if (T[(*i)] < infty)
				std::cout << (*i) << ": " <<  T[(*i)] << std::endl;
                }
		for (std::vector<Station*>::iterator i = this->stations.begin(); i != this->stations.end(); ++i)
                {
			std::cout << (*i)->getID() << " " << (*i)->getName() << std::endl;
			for (std::vector<std::array<unsigned int, 2>>::iterator j = S[(*i)->getID()].begin(); j != S[(*i)->getID()].end(); ++j)
				std::cout << "(" << (*j)[0] << ", " << (*j)[1] << ") ";
			std::cout << std::endl;
                }*/

	}
};

#endif

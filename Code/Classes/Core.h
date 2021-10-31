#ifndef CORE_h
#define CORE_h

#include "Connection.h"
#include "Station.h"
#include "Transfer.h"
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "../sparsehash/src/sparsehash/dense_hash_map"
using namespace google;
#include <omp.h>
#include <cassert>

class Core
{
public:
	std::vector<Connection*> connections;
	std::vector<Station*> stations;
	std::vector<std::string> trips;
	dense_hash_map<unsigned int, unsigned int> D; 	// used in profile
	
	dense_hash_map<unsigned int, Station*> station_ptr_map;
	// std::unordered_map<unsigned int, Station*> station_ptr_map;
	Core() {
		D.set_empty_key(~0);
	};
	~Core() {};
	
	void addConnection(Connection *new_connection_ptr) {
		this->connections.push_back(new_connection_ptr);
	}

	void addStation(Station *new_station_ptr) {
		this->stations.push_back(new_station_ptr);
		this->station_ptr_map[new_station_ptr->getID()] = new_station_ptr;
		this->D[new_station_ptr->getID()] = (~0);
	}

	void sortConnections() { 
		std::sort(this->connections.begin(), this->connections.end(),
			       [](Connection *a, Connection *b) {
					return (a->getDepartureTime() < b->getDepartureTime());
		});
	}

	std::vector<Connection*>::iterator findFirstDep(unsigned int dep_time) {
		return std::lower_bound(this->connections.begin(), this->connections.end(), dep_time, [](Connection *a, unsigned int time) { return (a->getDepartureTime() < time); });
	}
	std::vector<Connection*>::iterator findFirstDep(unsigned int dep_time, std::vector<Connection*>::iterator lower_itr, std::vector<Connection*>::iterator upper_itr) {
		return std::lower_bound(lower_itr, upper_itr, dep_time, [](Connection *a, unsigned int time) { return (a->getDepartureTime() < time); });
	}
	
	std::vector<Connection*>::iterator findLastDep(unsigned int dep_time) {
		return std::upper_bound(this->connections.begin(), this->connections.end(), dep_time, [](unsigned int time, Connection *a) { return (time < a->getDepartureTime()); });
	}
	std::vector<Connection*>::iterator findLastDep(unsigned int dep_time, std::vector<Connection*>::iterator lower_itr, std::vector<Connection*>::iterator upper_itr) {
		return std::upper_bound(lower_itr, upper_itr, dep_time, [](unsigned int time, Connection *a) { return (time < a->getDepartureTime()); });
	}

	void csa(unsigned int time, unsigned int from_id, unsigned int to_id) {
		unsigned int infty = (~0);

		dense_hash_map<unsigned int, unsigned int> map(this->stations.size());
		map.set_empty_key(~0);

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
		for(auto it = map.begin(); it != map.end(); ++it)
		{
			if (it->second != infty)
				std::cout << it->first << ": " << it->second << "\n";
		}
	};


	std::vector<Connection*> csa_lines(unsigned int time, unsigned int from_id, unsigned int to_id) {
		unsigned int infty = (~0);

		dense_hash_map<unsigned int, Connection*> map(this->stations.size());
		map.set_empty_key(~0);

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


	std::vector<std::array<unsigned int, 2>> latest_dep_prof(unsigned int from_id, unsigned int to_id, unsigned int earliest_dep, unsigned int latest_arr, int counter_prof=5) {
		// earliest_dep is the **now** time, since the user cannot catch an earlier train
		// latest_arr is the latest arrival time
		std::vector<std::array<unsigned int, 2>> profile = this->earliest_arr_profile_walking(from_id, to_id, earliest_dep, latest_arr);
		// find timestamp in the vector
		// since the vector isnt that big, linear scan is faster bcs it fits in memory
		std::vector<std::array<unsigned int, 2>> result;
		result.reserve(counter_prof);
		auto it = profile.rbegin();
		while (it != profile.rend() && result.size() < (unsigned int) counter_prof) {
			if ((*it)[1] <= latest_arr) result.push_back(*it);
			++it;
		}
		return result;
	}

	std::vector<std::array<unsigned int, 2>> earliest_arr_profile(unsigned int from_id, unsigned int to_id, unsigned int lower_bound = 0, unsigned int upper_bound=(~0)) {
		// upper_bound is used as latest departure time
		// csa_overview - Page 15 ff.
		unsigned int infty = (~0);

		dense_hash_map<std::string, unsigned int> T(this->trips.size());
		T.set_empty_key("");
		// Profile <=> int [2] = {dep_time, arr_time}
		dense_hash_map<unsigned int, std::vector<std::array<unsigned int, 2>>> S(this->stations.size());
		S.set_empty_key(~0);
		
		// init T & S
		for (std::vector<std::string>::iterator i = this->trips.begin(); i != this->trips.end(); ++i)
		{
			T[(*i)] = infty;
		}

		for (std::vector<Station*>::iterator i = this->stations.begin(); i != this->stations.end(); ++i)
		{
			S[(*i)->getID()]= {{infty, infty}};
		}

		unsigned int t1, t2, t3, t_c, c_arr_time;
		std::vector<Connection*>::reverse_iterator i, j;
		std::vector<Connection*>::iterator i_forward;

		// limit the scannable connections
		if (upper_bound != infty) i_forward = this->findLastDep(upper_bound);
		else i_forward = this->connections.end();
		i = std::make_reverse_iterator(i_forward);
		if (lower_bound != 0) j = std::make_reverse_iterator(this->findFirstDep(lower_bound, this->connections.begin(), i_forward));
		else j = this->connections.rend();

		// algorithm starts here
		for (; i != j; ++i ) {
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
	}


	std::vector<std::array<unsigned int, 2>> earliest_arr_profile_walking(unsigned int from_id, unsigned int to_id, unsigned int lower_bound = 0, unsigned int upper_bound=(~0)) {
		// upper_bound is used as latest departure time
		// csa_overview - Page 15 ff.
		unsigned int infty = (~0);

		std::vector<Transfer*> transfers = (*this->station_ptr_map[to_id]->getTransfers());
		for (auto transfer = transfers.begin(); transfer != transfers.end(); ++transfer) {
			this->D[(*transfer)->getArrivalID()] = (*transfer)->getDuration();
		}

		this->D[to_id] = 0;

		dense_hash_map<std::string, unsigned int> T(this->trips.size());
		T.set_empty_key("");
		// Profile <=> int [2] = {dep_time, arr_time}
		dense_hash_map<unsigned int, std::vector<std::array<unsigned int, 2>>> S(this->stations.size());
		S.set_empty_key(~0);
		
		// init T & S
		for (std::vector<std::string>::iterator i = this->trips.begin(); i != this->trips.end(); ++i)
		{
			T[(*i)] = infty;
		}

		for (std::vector<Station*>::iterator i = this->stations.begin(); i != this->stations.end(); ++i)
		{
			S[(*i)->getID()]= {{infty, infty}};
		}

		unsigned int t1, t2, t3, t_c, c_arr_time, c_arr_id, c_dep_id;
		std::vector<Connection*>::reverse_iterator i, j;
		std::vector<Connection*>::iterator i_forward;

		// limit the scannable connections
		if (upper_bound != infty) i_forward = this->findLastDep(upper_bound);
		else i_forward = this->connections.end();
		i = std::make_reverse_iterator(i_forward);
		if (lower_bound != 0) j = std::make_reverse_iterator(this->findFirstDep(lower_bound, this->connections.begin(), i_forward));
		else j = this->connections.rend();

		// init stuff for the algorithm
		std::array<unsigned int, 2> p, p_update; 
		std::vector<std::array<unsigned int, 2>>::iterator q, transf_q, profile_itr;
		std::vector<Transfer*> c_footpath;
		// algorithm starts here
		for (; i != j; ++i ) {
			c_arr_time = (*i)->getArrivalTime();
			c_arr_id = (*i)->getArrivalID();
			c_dep_id = (*i)->getDepartureID();
			if (this->D[c_arr_id] != infty) {
				t1 = c_arr_time + this->D[c_arr_id];
			} else {
				t1 = infty;
			}
			t2 = T[(*i)->getTripID()];
			t3 = infty;

			profile_itr = S[c_arr_id].begin();

			while ((*profile_itr)[0] < c_arr_time) profile_itr++;
			t3 = (*profile_itr)[1];

			t_c = std::min({t1, t2, t3});

			p = {{(*i)->getDepartureTime(), t_c}};
			q = S[c_dep_id].begin();
			if (p[0] <= (*q)[0] && p[1] <= (*q)[1]) {
				if (p[0] != (*q)[0]) {
					S[(*i)->getDepartureID()].insert(q, p);
				} else {
					(*q)[1] = p[1];
				}

				c_footpath = (*this->station_ptr_map[c_dep_id]->getTransfers());
				for (auto transfer = c_footpath.begin(); transfer != c_footpath.end(); ++transfer) {
					transf_q = S[(*transfer)->getArrivalID()].begin();
					p_update = {{p[0]-(*transfer)->getDuration(), p[1]}};
					if (p_update[0] != (*transf_q)[0]) {
						S[(*transfer)->getArrivalID()].insert(transf_q, p_update);
					} else {
						(*transf_q)[1] = p_update[1];
					}
				}
			}
			T[(*i)->getTripID()] = t_c;
		}
		// reset D
		for (auto transfer = transfers.begin(); transfer != transfers.end(); ++transfer) {
			this->D[(*transfer)->getArrivalID()] = infty;
		}

		this->D[to_id] = infty;
		return S[from_id];
	}
};

#endif

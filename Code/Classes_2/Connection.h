#ifndef CON_h
#define CON_h

#include <iostream>

class Connection
{
private:
	unsigned int dep_id, arr_id, dep_time, arr_time;
	std::string trip_id;

public:
	Connection(unsigned int dep_id, unsigned int arr_id, unsigned int dep_time, unsigned int arr_time, std::string trip_id = "") {
		this->dep_id = dep_id;
		this->arr_id = arr_id;
		this->dep_time = dep_time;
		this->arr_time = arr_time;
		this->trip_id = trip_id;
	};

	~Connection() {};
	
	unsigned int getDepartureID() { return this->dep_id; }
	unsigned int getArrivalID() { return this->arr_id; }
	unsigned int getDepartureTime() { return this->dep_time; }
	unsigned int getArrivalTime() { return this->arr_time; }
	std::string getTripID() { return this->trip_id; }

	friend bool operator < (Connection const& conn1, Connection const& conn2) {
		return (conn1.dep_time < conn2.dep_time);
	};

	friend bool operator < (Connection const& conn1, unsigned int const& time) {
		return (conn1.dep_time < time);
	};

	friend std::ostream& operator<< (std::ostream& os, const Connection& connection) {
		os << connection.trip_id << ": " << connection.dep_id << " [" << connection.dep_time << "] -> " << connection.arr_id << " [" << connection.arr_time << "]";
		return os;
	}
};
#endif
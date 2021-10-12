#ifndef CON_h
#define CON_h

#include "Station.h"
#include <iostream>

class Connection
{
private:
	Station *dep_ptr, *arr_ptr;
	unsigned int dep_time, arr_time;
	std::string trip_id;

	std::string getTimeAsString(unsigned int time) {
		unsigned int hour = time / 3600;
		time -= hour * 3600;
		unsigned int minutes = time / 60;
		time -= minutes * 60;
		return this->numberAsTwoDigits(hour) + ":" + this->numberAsTwoDigits(minutes) + ":" + this->numberAsTwoDigits(time);
	}

	std::string numberAsTwoDigits(unsigned int number) {
		if (number < 10) return "0"+std::to_string(number);
		return std::to_string(number);
	}

public:
	Connection() {
		this->dep_ptr = nullptr;
		this->arr_ptr = nullptr;
		this->dep_time = (~0);
		this->arr_time = (~0);
		this->trip_id = "";
	};
	Connection(Station * dep_ptr, Station *arr_ptr, unsigned int dep_time, unsigned int arr_time, std::string trip_id = "") {
		this->dep_ptr = dep_ptr;
		this->arr_ptr = arr_ptr;
		this->dep_time = dep_time;
		this->arr_time = arr_time;
		this->trip_id = trip_id;
	};

	~Connection() {};

	Station *getDeparture() { return this->dep_ptr; }
	Station *getArrival() { return this->dep_ptr; }
	std::string getDepartureName() { return this->dep_ptr->getName(); }
	std::string getArrivalName() { return this->arr_ptr->getName(); }
	unsigned int getDepartureID() { return this->dep_ptr->getID(); }
	unsigned int getArrivalID() { return this->arr_ptr->getID(); }
	unsigned int getDepartureTime() { return this->dep_time; }
	unsigned int getArrivalTime() { return this->arr_time; }
	std::string getTripID() { return this->trip_id; }

	friend bool operator < (Connection const& conn1, Connection const& conn2) {
		return (conn1.dep_time < conn2.dep_time);
	};

	friend bool operator < (Connection const& conn1, unsigned int const& time) {
		return (conn1.dep_time < time);
	};

	friend std::ostream& operator<< (std::ostream& os, Connection& connection) {
		os << connection.trip_id << ": " << connection.getDepartureName() << " " << connection.getTimeAsString(connection.dep_time) << " -> " << connection.getArrivalName() << " " << connection.getTimeAsString(connection.arr_time);
		return os;
	}
};
#endif
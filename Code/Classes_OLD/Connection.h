#ifndef CONNECTION_h
#define CONNECTION_h

#include "Station.h"
#include "Date.h"
#include <string>
#include <iostream>

class Connection
{
private:
	Station* departure;
	Station* arrival;
	Date* dep_time;
	Date* arr_time;

public:
	Connection(Station *dep, Station *arr, Date *dep_time, Date *arr_time) {
		this->departure = dep;
		this->arrival = arr;
		this->dep_time = dep_time;
		this->arr_time = arr_time;
	}

	~Connection() {
		delete this->departure;
		delete this->arrival;
		delete this->dep_time;
		delete this->arr_time;
	}

	// pretty print
	void pprint() {
		std::cout << this->departure->getName() << " (" << this->dep_time->getTimeAsString() << ") " << this->arrival->getName() << " (" << this->arr_time->getTimeAsString() << ")" << std::endl;
	}

	bool operator < (const Connection& other) {
		return (this.dep_time < other.dep_time);
	};
};

#endif
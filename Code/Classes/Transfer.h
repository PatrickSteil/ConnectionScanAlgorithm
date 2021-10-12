#ifndef TRAN_h
#define TRAN_h

#include "Station.h"
#include <iostream>

class Transfer
{
private:
	Station *dep_ptr, *arr_ptr;
	unsigned int duration;

public:
	Transfer(Station *dep_ptr, Station *arr_ptr, unsigned int duration) {
		this->dep_ptr = dep_ptr;
		this->arr_ptr = arr_ptr;
		this->duration = duration;
	};

	~Transfer() {};

	unsigned int getDuration() { return this->duration; }

	unsigned int getDepartureID() { return this->dep_ptr->getID(); }

	unsigned int getArrivalID() { return this->arr_ptr->getID(); }

	Station *getDeparture() { return this->dep_ptr; }

	Station *getArrival() { return this->arr_ptr; }

	friend std::ostream& operator<< (std::ostream& os, const Transfer& transfer) {
		os << transfer.dep_ptr->getName() << " -> " << transfer.arr_ptr->getName() << " - " << transfer.duration;
		return os;
	}
};
#endif
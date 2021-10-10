#ifndef TRAN_h
#define TRAN_h

#include <iostream>

class Transfer
{
private:
	unsigned int dep_id, arr_id, duration;

public:
	Transfer(unsigned int dep_id, unsigned int arr_id, unsigned int duration) {
		this->dep_id = dep_id;
		this->arr_id = arr_id;
		this->duration = duration;
	};

	~Transfer() {};

	unsigned int getDuration() { return this->duration; }

	unsigned int getDepartureID() { return this->dep_id; }

	unsigned int getArrivalID() { return this->arr_id; }

	friend std::ostream& operator<< (std::ostream& os, const Transfer& transfer) {
		os << transfer.dep_id << " -> " << transfer.arr_id << " - " << transfer.duration;
		return os;
	}
};
#endif
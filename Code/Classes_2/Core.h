#ifndef CORE_h
#define CORE_h

#include "Connection.h"
#include <algorithm>
#include <vector>

class Core
{
public:
	std::vector<Connection> connections;
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
	void csa(unsigned int time, unsigned int from_id, unsigned int to_id);
};

#endif
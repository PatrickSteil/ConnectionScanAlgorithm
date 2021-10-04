#ifndef STAT_h
#define STAT_h

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

class Station
{
private:
	unsigned int stop_id, location_type;
	std::string stop_name, stop_lat, stop_lon, platform_code;


public:
	Station(unsigned int stop_id, std::string stop_name) {
		this->stop_id = stop_id;
		this->stop_name = stop_name;
	};

	Station(std::string line) {
		std::vector<std::string> out;
    	std::stringstream ss(line);
 
	    std::string s;
	    while (std::getline(ss, s, ',')) {
	        out.push_back(s);
	    }
		
		this->stop_id = std::stoi(out.front());
		out.erase(out.begin());
		this->stop_name = out.front();
		out.erase(out.begin());
		this->stop_lat = out.front();
		out.erase(out.begin());
		this->stop_lon = out.front();
		out.erase(out.begin());
		this->location_type = std::stoi(out.front());
		out.erase(out.begin());
		this->platform_code = out.front();
	};

	~Station() {};


	int getID() {
		return this->stop_id;
	}

	std::string getName() {
		return this->stop_name;
	}


	friend std::ostream& operator<< (std::ostream& os, const Station& station) {
		os << station.stop_id << " " << station.stop_name; //<< "\n" << station->stop_lat << ", " << station->stop_lon << "\n" << station->location_type << " " << station->platform_code;
		return os;
	}
};

#endif
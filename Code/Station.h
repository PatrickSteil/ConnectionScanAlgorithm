#ifndef STATION_h
#define STATION_h

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <regex>

class Station
{
private:
	// stop_id,stop_name,stop_lat,stop_lon,location_type,platform_code
	int stop_id, location_type;
	std::string stop_name, stop_lat, stop_lon, platform_code;


public:
	Station(int stop_id, std::string stop_name) {
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

	void pprint() {
		std::cout << this->stop_id << " " << this->stop_name << "\n" << this->stop_lat << ", " << this->stop_lon << "\n" << this->location_type << " " << this->platform_code << std::endl;
	}
};

// https://www.techiedelight.com/split-string-cpp-using-delimiter/
void tokenize(std::string const &str, const char delim, std::vector<std::string> &out)
{
    // construct a stream from the string
    std::stringstream ss(str);
 
    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}

#endif
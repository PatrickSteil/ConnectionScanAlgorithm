#ifndef STAT_h
#define STAT_h

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <math.h>

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

	// stop_id,stop_name,stop_lat,stop_lon,location_type,platform_code
	Station(unsigned int stop_id, std::string stop_name, std::string stop_lat, std::string stop_lon, unsigned int location_type, std::string platform_code) {
		this->stop_id = stop_id;
		this->stop_name = stop_name;
		this->location_type = location_type;
		this->stop_lat = stop_lat;
		this->stop_lon = stop_lon;
		this->platform_code = platform_code;
	}

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

	float getLatAsFloat() {
		return std::stof(this->stop_lat);
	}

	float getLonAsFloat() {
		return std::stof(this->stop_lon);
	}


	friend std::ostream& operator<< (std::ostream& os, const Station& station) {
		os << station.stop_id << " " << station.stop_name; //<< "\n" << station->stop_lat << ", " << station->stop_lon << "\n" << station->location_type << " " << station->platform_code;
		return os;
	}

	double getDistance(Station station) {
		double dlong = (station.getLonAsFloat() - this->getLonAsFloat()) * (M_PI / 180.0);
		double dlat = (station.getLatAsFloat() - this->getLatAsFloat()) * (M_PI / 180.0);
		double a = pow(sin(dlat/2.0), 2) + cos(this->getLatAsFloat()*(M_PI / 180.0)) * cos(station.getLatAsFloat()*(M_PI / 180.0)) * pow(sin(dlong/2.0), 2);
		double c = 2 * atan2(sqrt(a), sqrt(1-a));
		double d = 6367000 * c;
		return d;
	}
};

#endif
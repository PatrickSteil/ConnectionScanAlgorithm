#ifndef DATE_h
#define DATE_h

#include <string>
#include <iostream>

class Date
{
public:
	Date() {
		this->setDate(0,0,0);
	};

	Date(int hours, int minutes, int seconds) {
		this->setDate(hours, minutes, seconds);
	};
	Date(std::string date_string) {
		// unschön, date_string ist aber immer im Format HH:MM:SS
		int s_hours = std::stoi(date_string.substr(0, 2));
		int s_minutes = std::stoi(date_string.substr(3, 2));
		int s_seconds = std::stoi(date_string.substr(6, 2));

		this->setDate(s_hours, s_minutes, s_seconds);
	};

	~Date() {};

	std::string getTimeAsString() {
		std::string s = std::to_string(this->hours) + ":" + std::to_string(this->minutes) + ":" + std::to_string(this->seconds);
		return s;
	}

	int getHours() {
		return hours;
	}

	int getMinutes() {
		return minutes;
	}

	int getSeconds() {
		return seconds;
	}

	// Comparison
	// HINWEIS: in den Dateien steht für ein Trip, der über Mitternacht hinweg fährt (24, 25, 26, ... Uhr) daher ist 02:00:00 < 20:00:00, aber 26:00:00 > 20:00:00
	bool operator <= (const Date& other) {
		if (this < other) return true;
		if (this.hours != other.hours || date1.minutes != other.minutes || date1.seconds != other.seconds) return false;
		return true;
	};
	bool operator < (const Date& other) {
		if (this.hours < other.hours) return true;
		if (this.hours == other.hours) {
			if (this.minutes < other.minutes) return true;
			if (this.minutes == other.minutes) {
				if (this.seconds < other.seconds) return true;
			}
		}
		return false;
	};
	bool operator > (const Date& other) {
		return !(this <= other)
	};

private:
	int hours, minutes, seconds;

	void setDate(int hours, int minutes, int seconds) {
		this->hours = hours;
		this->minutes = minutes;
		this->seconds = seconds;
	}
};


#endif
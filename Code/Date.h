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

	// https://www.learncpp.com/cpp-tutorial/overloading-the-comparison-operators/
	friend bool operator <= (const Date& date1, const Date& date2);
    friend bool operator < (const Date& date1, const Date& date2);
    friend bool operator > (const Date& date1, const Date& date2);

private:
	int hours, minutes, seconds;

	void setDate(int hours, int minutes, int seconds) {
		this->hours = hours;
		this->minutes = minutes;
		this->seconds = seconds;
	}
};

// Comparison
// HINWEIS: in den Dateien steht für ein Trip, der über Mitternacht hinweg fährt (24, 25, 26, ... Uhr) daher ist 02:00:00 < 20:00:00, aber 26:00:00 > 20:00:00
bool operator <= (const Date& date1, const Date& date2) {
	if (date1 < date2) return true;
	if (date1.hours != date2.hours || date1.minutes != date2.minutes || date1.seconds != date2.seconds) return false;
	return true;
};
bool operator < (const Date& date1, const Date& date2) {
	if (date1.hours < date2.hours) return true;
	if (date1.hours == date2.hours) {
		if (date1.minutes < date2.minutes) return true;
		if (date1.minutes == date2.minutes) {
			if (date1.seconds < date2.seconds) return true;
		}
	}
	return false;
};
bool operator > (const Date& date1, const Date& date2) {
	return ! (date1 <= date2);
};

#endif
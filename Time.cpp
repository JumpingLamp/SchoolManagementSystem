#include "Time.h"
#include <stdexcept>
Time::Time(int hour, int minute)
{
	setHour(hour);
	setMinute(minute);
}

int Time::getHour() const {
	return hour;
}

int Time::getMinute() const {
	return minute;
}

void Time::setHour(int hour) {
	if (hour < 0 || hour > 23) {
		throw std::out_of_range("Hour must be between 0 and 23.");
	}
	this->hour = hour;
}

void Time::setMinute(int minute) {
	if (minute < 0 || minute > 59) {
		throw std::out_of_range("Minute must be between 0 and 59.");
	}
	this->minute = minute;
}

std::string Time::get12HourFormatString() const {
	int displayHour = hour % 12;
	if (displayHour == 0) 
		displayHour = 12; // Handle midnight and noon
	std::string period = (hour < 12) ? "AM" : "PM";
	return std::to_string(displayHour) + ":" + (minute < 10 ? "0" : "") + std::to_string(minute) + " " + period;
}

std::string Time::get24HourFormatString() const {
	return (hour < 10 ? "0" : "") + std::to_string(hour) + ":" + (minute < 10 ? "0" : "") + std::to_string(minute);
}
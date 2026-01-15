#pragma once
#include <string>
class Time
{
private:
		int hour;
		int minute;
public:
	Time() : hour(0), minute(0) {}
		Time(int hour, int minute);
		int getHour() const;
		int getMinute() const;
		void setHour(int hour);
		void setMinute(int minute);
		std::string get12HourFormatString() const;
		std::string get24HourFormatString() const;
};


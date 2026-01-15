#pragma once
#include <string>
class Date
{
private:
	int day;
	int month;
	int year;
public:
	Date() : day(1), month(1), year(2004) {}
	Date(int day, int month, int year);
	int getDay() const;
	int getMonth() const;
	int getYear() const;
	void setDay(int day);
	void setMonth(int month);
	void setYear(int year);
	std::string toString() const;
};


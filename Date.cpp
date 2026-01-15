#include "Date.h"
#include <stdexcept>

Date::Date(int day, int month, int year)
{
	setDay(day);
	setMonth(month);
	setYear(year);
}

int Date::getDay() const {
	return day;
}

int Date::getMonth() const {
	return month;
}

int Date::getYear() const {
	return year;
}

void Date::setDay(int day) {
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) 
	{ // Leap year
		if (month == 2 && (day < 1 || day > 29)) 
		{
			throw std::out_of_range("Day must be between 1 and 29 for February in a leap year.");
		} else if ((month == 4 || month == 6 || month == 9 || month == 11) && (day < 1 || day > 30)) 
		{
			throw std::out_of_range("Day must be between 1 and 30 for April, June, September, and November.");
		}
		else if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && (day < 1 || day > 31))
		{
			throw std::out_of_range("Day must be between 1 and 31 for January, March, May, July, August, October, and December.");
		}
	} else 
	{
		if (month == 2 && (day < 1 || day > 28)) 
		{
			throw std::out_of_range("Day must be between 1 and 28 for February in a non-leap year.");
		}
	}
	this->day = day;
}

void Date::setMonth(int month) {
	if (month < 1 || month > 12) {
		throw std::out_of_range("Month must be between 1 and 12.");
	}
	this->month = month;
}

void Date::setYear(int year) {
	this->year = year;
}

std::string Date::toString() const {
	return (std::to_string(year) + "-" +
		(month < 10 ? "0" : "") + std::to_string(month) + "-" +
		(day < 10 ? "0" : "") + std::to_string(day));
}
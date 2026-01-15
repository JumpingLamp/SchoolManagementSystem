#pragma once
#include <exception>
class Exception :public std::exception
{
public:
	Exception() noexcept = default;
	~Exception() noexcept override = default;
};


#pragma once
#include <mariadb/conncpp.hpp>

class Table
{
public:
	Table() = default;
	virtual void create(sql::Connection& conn) = 0;
	virtual std::unique_ptr<sql::ResultSet> read(sql::Connection& conn) = 0;
	virtual void update(sql::Connection& conn) = 0;
	virtual void remove(sql::Connection& conn) = 0;
	virtual ~Table() = default;
};


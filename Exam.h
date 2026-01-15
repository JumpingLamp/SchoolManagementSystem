#pragma once
#include "MainEntity.h"
#include "Date.h"
class Exam :
    public MainEntity
{
private:
	std::string description;
	Date startDate, endDate;
public:
	Exam() : MainEntity(), description(""), startDate(), endDate() {}
	Exam(std::string id, std::string description, Date startDate, Date endDate) :
		MainEntity(id), description(description), startDate(startDate), endDate(endDate) {}
	void create(sql::Connection& conn) override;
	std::unique_ptr<sql::ResultSet> read(sql::Connection& conn) override;
	void update(sql::Connection& conn) override;
	void remove(sql::Connection& conn) override;
	std::unique_ptr<sql::ResultSet> searchByID(sql::Connection& conn) override;
	~Exam() override = default;
};


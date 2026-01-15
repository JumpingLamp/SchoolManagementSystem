#pragma once
#include "MainEntity.h"
class Subject :
    public MainEntity
{
private:
	std::string subjectName;
public:
	Subject() : MainEntity(), subjectName("") {}
	Subject(sql::SQLString id) : MainEntity(static_cast<std::string>(id)), subjectName("") {}
	Subject(std::string id, std::string subjectName) : MainEntity(id), subjectName(subjectName) {}
	void create(sql::Connection& conn) override;
	std::unique_ptr<sql::ResultSet> read(sql::Connection& conn) override;
	void update(sql::Connection& conn) override;
	void remove(sql::Connection& conn) override;
	std::unique_ptr<sql::ResultSet> searchByID(sql::Connection& conn) override;
	std::string getSubjectName() const { return subjectName; }
	void setSubjectName(const std::string& name) { subjectName = name; }
	~Subject() override = default;
};


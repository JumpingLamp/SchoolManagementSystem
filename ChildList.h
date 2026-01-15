#pragma once
#include "Table.h"
class ChildList :
    public Table
{
private:
	std::string studentID, parentID, relationship;
public:
    ChildList() = default;
	ChildList(const std::string studentID, const std::string parentID, const std::string relationship) 
        :studentID(studentID), parentID(parentID), relationship(relationship) {}
    void create(sql::Connection& conn) override;
    std::unique_ptr<sql::ResultSet> read(sql::Connection& conn) override;
    void update(sql::Connection& conn) override;
    void remove(sql::Connection& conn) override;
	~ChildList() = default;
};


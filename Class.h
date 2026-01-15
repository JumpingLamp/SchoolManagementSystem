#pragma once
#include "MainEntity.h"
class Class :
    public MainEntity
{
private:
    std::string teacherID, roomNum;
public:
    Class();
    Class(std::string id, std::string teacherID, std::string roomNum);
    void create(sql::Connection& conn);
    std::unique_ptr<sql::ResultSet> read(sql::Connection& conn);
    void update(sql::Connection& conn);
    void remove(sql::Connection& conn);
	std::unique_ptr<sql::ResultSet> searchByID(sql::Connection& conn) override;
    ~Class() override = default;
};


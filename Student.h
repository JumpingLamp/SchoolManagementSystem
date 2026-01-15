#pragma once
#include "UserEntity.h"
class Student :
    public UserEntity
{
private:
    std::string classID;
public:
    Student();
    Student(std::string id);
    Student(std::string id, std::string name, std::string phone, std::string address, std::string ic, std::string password, std::string classID);
    void create(sql::Connection& conn) override;
    std::unique_ptr<sql::ResultSet> read(sql::Connection& conn) override;
    void update(sql::Connection& conn) override;
	void remove(sql::Connection& conn) override;
    std::unique_ptr<sql::ResultSet> searchByID(sql::Connection& conn) override;
};


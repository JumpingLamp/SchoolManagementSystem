#pragma once
#include "UserEntity.h"
class Parent :
    public UserEntity
{
public:
    Parent();
    Parent(std::string id, std::string name, std::string phone, std::string address, std::string ic, std::string password);
    void create(sql::Connection& conn) override;
    std::unique_ptr<sql::ResultSet> read(sql::Connection& conn) override;
    void update(sql::Connection& conn) override;
	void remove(sql::Connection& conn) override;
    std::unique_ptr<sql::ResultSet> searchByID(sql::Connection& conn) override;
};


#pragma once
#include "UserEntity.h"
class Teacher :
    public UserEntity
{
private:
    std::string adminID;
public:
    Teacher();
	Teacher(std::string id) : UserEntity(id), adminID("") {}
    Teacher(std::string id, std::string name, std::string phone, std::string address,
        std::string ic, std::string password,sql::SQLString adminID);
    void create(sql::Connection& conn) override;
    std::unique_ptr<sql::ResultSet> read(sql::Connection& conn) override;
    void update(sql::Connection& conn) override;
    void remove(sql::Connection& conn) override;
    std::unique_ptr<sql::ResultSet> searchByID(sql::Connection& conn) override;
	~Teacher() override = default;
};


#pragma once
#include "MainEntity.h"

class Admin :
    public MainEntity
{
private:
    sql::SQLString  name, password;
public:
    Admin();
    Admin(std::string id, std::string name, std::string password);
    void create(sql::Connection& conn) override;
    std::unique_ptr<sql::ResultSet> read(sql::Connection& conn) override;
    std::unique_ptr<sql::ResultSet> searchByID(sql::Connection& conn) override;
    void update(sql::Connection& conn) override;
    void remove(sql::Connection& conn) override;
	void display(const std::unique_ptr<sql::ResultSet>& res);
	~Admin() override = default;
};


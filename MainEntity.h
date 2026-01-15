#pragma once
#include "Table.h"
#include <string>
class MainEntity :
    public Table
{
protected:
    std::string id;
public:
	MainEntity() : id("") {}
	MainEntity(std::string id) : id(id) {}
	void setID(const std::string& id) { this->id = id; }
	std::string getID() const { return id; }
    void create(sql::Connection& conn) override = 0;
    std::unique_ptr<sql::ResultSet> read(sql::Connection& conn) override = 0;
    void update(sql::Connection& conn) override = 0;
    void remove(sql::Connection& conn) override = 0;
    virtual std::unique_ptr<sql::ResultSet> searchByID(sql::Connection& conn) = 0;
	~MainEntity() override = default;
};


#include "Admin.h"
#include <iostream>
#include <iomanip>

Admin::Admin() : MainEntity(), name(""), password("") {}
Admin::Admin(std::string id, std::string name, std::string password):
	MainEntity(id), name(name), password(password) {}

void Admin::create(sql::Connection& conn)
{
	try
	{
		std::unique_ptr<sql::PreparedStatement> stmnt(
			conn.prepareStatement("INSERT INTO admin(adminID, name, password) VALUES (?,?,?)"));
		stmnt->setString(1, id);
		stmnt->setString(2, name);
		stmnt->setString(3, password);
		stmnt->executeUpdate();
	}
	catch (sql::SQLException& e)
	{
		std::cout << "SQL Exception while inserting admin: " << e.what() << std::endl;
	}
}

std::unique_ptr<sql::ResultSet> Admin::read(sql::Connection& conn)
{
	try
	{
		std::unique_ptr<sql::PreparedStatement> stmnt(
			conn.prepareStatement("SELECT adminID, name, password FROM admin"));
		std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery());
		return res;
	}
	catch (sql::SQLException& e)
	{
		std::cout << "SQL Exception while reading admin: " << e.what() << std::endl;
		return nullptr;
	}
}

void Admin::update(sql::Connection& conn)
{
	try
	{
		std::unique_ptr<sql::PreparedStatement> stmnt(
			conn.prepareStatement("UPDATE admin SET name=?,password=? WHERE adminID=?"));
		stmnt->setString(1, name);
		stmnt->setString(2, password);
		stmnt->setString(3, id);
		stmnt->executeUpdate();
	}
	catch (sql::SQLException& e)
	{
		std::cout << "SQL Exception while updating admin: " << e.what() << std::endl;
	}
}

void Admin::remove(sql::Connection& conn)
{
	try
	{
		std::unique_ptr<sql::PreparedStatement> stmnt(
			conn.prepareStatement("DELETE FROM admin WHERE adminID=?"));
		stmnt->setString(1, id);
		stmnt->executeUpdate();
	}
	catch (sql::SQLException& e)
	{
		std::cout << "SQL Exception while deleting admin: " << e.what() << std::endl;
	}
}

std::unique_ptr<sql::ResultSet> Admin::searchByID(sql::Connection& conn)
{
	try
	{
		std::unique_ptr<sql::PreparedStatement> stmnt(
			conn.prepareStatement("SELECT adminID, name, password FROM admin WHERE adminID=?"));
		stmnt->setString(1, id);
		stmnt->executeQuery();
		return std::unique_ptr<sql::ResultSet>(stmnt->getResultSet());
	}
	catch (sql::SQLException& e)
	{
		std::cout << "SQL Exception while reading admin: " << e.what() << std::endl;
		return nullptr;
	}
}

void Admin::display(const std::unique_ptr<sql::ResultSet>& res)
{
	try {
		
	}
	catch (sql::SQLException& e) {
		std::cerr << "Error displaying admin records: " << e.what() << std::endl;
	}
}
#include "Class.h"
#include <iostream>

Class::Class() : MainEntity(), teacherID(""), roomNum("") {}

Class::Class(std::string id, std::string teacherID, std::string roomNum)
	: MainEntity(id), teacherID(teacherID), roomNum(roomNum) {}

void Class::create(sql::Connection& conn) {
	try
	{
		std::unique_ptr<sql::PreparedStatement> pstmt(
			conn.prepareStatement("INSERT INTO CLASS (classID, teacherID, roomNum) VALUES (?, ?, ?)")
		);
		pstmt->setString(1, id);
		pstmt->setString(2, teacherID);
		pstmt->setString(3, roomNum);
		pstmt->executeUpdate();
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "Error creating Class: " << e.what() << std::endl;
	}
}

std::unique_ptr<sql::ResultSet> Class::read(sql::Connection& conn) {
	try
	{
		std::unique_ptr<sql::PreparedStatement> pstmt(
			conn.prepareStatement("SELECT * FROM CLASS WHERE classID = ?")
		);
		pstmt->setString(1, id);
		return std::unique_ptr<sql::ResultSet>(pstmt->executeQuery());
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "Error reading Class: " << e.what() << std::endl;
		return nullptr;
	}
}	

void Class::update(sql::Connection& conn) {
	try
	{
		std::unique_ptr<sql::PreparedStatement> pstmt(
			conn.prepareStatement("UPDATE CLASS SET teacherID=?, roomNum = ? WHERE classID = ?"));
		pstmt->setString(1, teacherID);
		pstmt->setString(2, roomNum);
		pstmt->setString(3, id);
		pstmt->executeUpdate();
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "Error updating Class: " << e.what() << std::endl;
	}
}

void Class::remove(sql::Connection& conn) {
	try
	{
		std::unique_ptr<sql::PreparedStatement> pstmt(
			conn.prepareStatement("DELETE FROM CLASS WHERE classID = ?")
		);
		pstmt->setString(1, id);
		pstmt->executeUpdate();
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "Error deleting Class: " << e.what() << std::endl;
	}
}

std::unique_ptr<sql::ResultSet> Class::searchByID(sql::Connection& conn) {
	try
	{
		std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement("SELECT * FROM CLASS WHERE classID = ?"));
		pstmt->setString(1, id);
		return std::unique_ptr<sql::ResultSet>(pstmt->executeQuery());
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "Error searching Class by ID: " << e.what() << std::endl;
		return nullptr;
	}
}
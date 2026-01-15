#include "ChildList.h"
#include <iostream>

void ChildList::create(sql::Connection& conn)
{
	try 
	{
		std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement("INSERT INTO CHILDLIST (studentID, parentID, relationship) VALUES (?, ?, ?)"));
		pstmt->setString(1, studentID);
		pstmt->setString(2, parentID);
		pstmt->setString(3, relationship);
		pstmt->executeUpdate();
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "Error creating ChildList entry: " << e.what() << std::endl;
	}
}

void ChildList::update(sql::Connection& conn)
{
	try 
	{
		std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement("UPDATE CHILDLIST SET relationship = ? WHERE studentID = ? AND parentID = ?"));
		pstmt->setString(1, relationship);
		pstmt->setString(2, studentID);
		pstmt->setString(3, parentID);
		pstmt->executeUpdate();
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "Error updating ChildList entry: " << e.what() << std::endl;
	}
}

std::unique_ptr<sql::ResultSet> ChildList::read(sql::Connection& conn)
{
	try 
	{
		std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement("SELECT * FROM CHILDLIST"));
		return std::unique_ptr<sql::ResultSet>(pstmt->executeQuery());
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "Error reading ChildList entry: " << e.what() << std::endl;
		return nullptr;
	}
}

void ChildList::remove(sql::Connection& conn)
{
	try 
	{
		std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement("DELETE FROM CHILDLIST WHERE studentID = ? AND parentID = ?"));
		pstmt->setString(1, studentID);
		pstmt->setString(2, parentID);
		pstmt->executeUpdate();
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "Error deleting ChildList entry: " << e.what() << std::endl;
	}
}
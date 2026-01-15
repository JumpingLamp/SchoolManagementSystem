#include "Attendance.h"
#include <iostream>

void Attendance::create(sql::Connection& conn)
{
	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(
			conn.prepareStatement("INSERT INTO ATTENDANCE (date, studentID, status, remarks) VALUES (?, ?, ?, ?)")
		);
		pstmt->setString(1, date.toString());
		pstmt->setString(2, studentID);
		pstmt->setString(3, status);
		pstmt->setString(4, remarks);
		pstmt->executeUpdate();
		std::cout << "Attendance record created successfully." << std::endl;
	}
	catch (sql::SQLException& e) {
		std::cerr << "Error creating attendance record: " << e.what() << std::endl;
	}
}

void Attendance::update(sql::Connection& conn)
{
	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(
			conn.prepareStatement("UPDATE ATTENDANCE SET status = ?, remarks = ? WHERE date = ? AND studentID = ?")
		);
		pstmt->setString(1, status);
		pstmt->setString(2, remarks);
		pstmt->setString(3, date.toString());
		pstmt->setString(4, studentID);
		pstmt->executeUpdate();
		std::cout << "Attendance record updated successfully." << std::endl;
	}
	catch (sql::SQLException& e) {
		std::cerr << "Error updating attendance record: " << e.what() << std::endl;
	}
}

std::unique_ptr<sql::ResultSet> Attendance::read(sql::Connection& conn)
{
	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(
			conn.prepareStatement("SELECT * FROM ATTENDANCE"));
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
		return res;
	}
	catch (sql::SQLException& e) {
		std::cerr << "Error reading attendance record: " << e.what() << std::endl;
		return nullptr;
	}
}

void Attendance::remove(sql::Connection& conn)
{
	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(
			conn.prepareStatement("DELETE FROM ATTENDANCE WHERE date = ? AND studentID = ?")
		);
		pstmt->setString(1, date.toString());
		pstmt->setString(2, studentID);
		pstmt->executeUpdate();
		std::cout << "Attendance record deleted successfully." << std::endl;
	}
	catch (sql::SQLException& e) {
		std::cerr << "Error deleting attendance record: " << e.what() << std::endl;
	}
}
#pragma once
#include <iomanip>
#include "ClassSchedule.h"
#include "Date.h"
#include <iostream>
#include <vector>
#include <cctype>
#include "Time.h"

// Function prototyes
void divider();
void studentAccess(sql::Connection& conn, const sql::SQLString& id, bool parent = false);
void parentAccess(sql::Connection& conn, const sql::SQLString& id);
void teacherAccess(sql::Connection& conn, const sql::SQLString& id);
void adminAccess(sql::Connection& conn, const sql::SQLString& id);
void loginPage(sql::Connection& conn);
void manageAdmins(sql::Connection& conn);
void manageTeachers(sql::Connection& conn, const sql::SQLString& adminID);
void manageStudentsAndParents(sql::Connection& conn);
void manageClassesAndAttendance(sql::Connection& conn);
void manageExamsSubjectsAndResults(sql::Connection& conn);
void analysisAndReport(sql::Connection& conn);
void displayClassSchedule(sql::Connection& conn, const sql::SQLString& classID);
void displayExamResults(sql::Connection& conn, const sql::SQLString& studentID);
void displayStudentAttendanceRecord(sql::Connection& conn, const sql::SQLString& studentID);
void update1attributeByTableAndID(sql::Connection& conn, const sql::SQLString& tableName, const sql::SQLString& attributeName, const sql::SQLString& newValue, const sql::SQLString& condition);
void deleteBasedOn1ID(sql::Connection& conn, const sql::SQLString& tableName, const sql::SQLString& idName, const sql::SQLString& idValue);
bool isValidPositiveInt(const std::string& prompt, int& var);
bool isValidDouble(const std::string& prompt, double& var);
bool isValidSQLStringInput(const std::string& prompt, sql::SQLString& input, const bool& canEmpty = true);


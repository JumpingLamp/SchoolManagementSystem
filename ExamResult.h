#pragma once
#include "Table.h"
#include <string>
class ExamResult :
    public Table
{
private:
    std::string studentID, examID, subjectID;
    double result = 0;
public:
    ExamResult() = default;
    ExamResult(const std::string& studentID): studentID(studentID) {}
    ExamResult(const std::string& studentID, const std::string& examID, const std::string& subjectID, double result)
        : studentID(studentID), examID(examID), subjectID(subjectID), result(result) {}
    void create(sql::Connection& conn) override;
    std::unique_ptr<sql::ResultSet> read(sql::Connection& conn) override;
	std::unique_ptr<sql::ResultSet> searchByStudentID(sql::Connection& conn);
    void update(sql::Connection& conn) override;
    void remove(sql::Connection& conn) override;
	~ExamResult() override = default;
};


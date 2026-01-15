#pragma once
#include "Table.h"
#include "Date.h"
class Attendance :
    public Table
{
private:
    Date date;
    std::string studentID, status, remarks;
public:
    Attendance() = default;
    Attendance(const Date& date, const std::string& studentID, const std::string& status, const std::string& remarks)
        : date(date), studentID(studentID), status(status), remarks(remarks) {}
    void create(sql::Connection& conn) override;
    std::unique_ptr<sql::ResultSet> read(sql::Connection& conn) override;
    void update(sql::Connection& conn) override;
	void remove(sql::Connection& conn) override;
};


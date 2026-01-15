// Includes
#include <iostream>
#include <mariadb/conncpp.hpp>
#include "Header.h"

// Main Process
int main(int argc, char** argv)
{
    try {
        // Instantiate Driver
        sql::Driver* driver = sql::mariadb::get_driver_instance();

        // Configure Connection
        // The URL or TCP connection string format is
        // ``jdbc:mariadb://host:port/database``.
        sql::SQLString url("jdbc:mariadb://127.0.0.1:3306/schoolmanagementdb");

        // Use a properties map for the other connection options
        sql::Properties properties({{"user", "root"},{"password", ""}});

        // Establish Connection
        // Use a smart pointer for extra safety
        std::unique_ptr<sql::Connection> conn(driver->connect(url, properties));

        //connection successful
        std::cout << "schoolmanagementdb connection successful!" << std::endl;
		system("pause ");

        system("cls");
        loginPage(*conn);

        // Close Connection
        conn->close();
    }
    // Catch Exceptions
    catch (sql::SQLException& e) {
        std::cerr << "Error Connecting to the database: "
            << e.what() << std::endl;

        // Exit (Failed)
        return 1;
    }

    return 0;
}

void divider()
{
    std::cout << "\n\033[34m--------------------------END----------------------------\033[0m\n";
}

bool isValidDouble(const std::string& prompt, double& var)
{
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);

    if (input.empty())
    {
        std::cerr << "\33[31mInput cannot be empty. Please try again.\33[0m\n";
        return false;
    }

    // Trim leading/trailing whitespace
    size_t first = input.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
    { // all whitespace
        std::cerr << "\33[31mInput cannot be empty. Please try again.\33[0m\n";
        return false;
    }
    size_t last = input.find_last_not_of(" \t\r\n");
    std::string s = input.substr(first, last - first + 1);

    bool decimalPointSeen = false;
    bool signSeen = false;

    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];

        if (i == 0 && (c == '+' || c == '-')) {
            signSeen = true;
            continue;
        }

        if (c == '.') {
            if (decimalPointSeen)
            {
                std::cerr << "\33[31mInvalid input. Please try again.\33[0m\n";
                return false; // multiple decimal points
            }
            else
                decimalPointSeen = true;
            continue;
        }

        if (!std::isdigit(static_cast<unsigned char>(c)))
        {
            std::cerr << "\33[31mInvalid input. Please try again.\33[0m\n";
            return false; // invalid character
        }
    }

    try {
        var = std::stod(s);
    }
    catch (...)
    {
        std::cerr << "\33[31mInvalid input. Please try again.\33[0m\n";
        return false;
    }

    return true;
}

bool isValidPositiveInt(const std::string& prompt, int& var)
{
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    if (input.empty())
    {
        std::cerr << "\33[31mInput cannot be empty. Please try again.\33[0m\n";
        return false;
    }

    // Trim leading/trailing whitespace
    size_t first = input.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
    {// empty or all whitespace
        std::cerr << "\33[31mInput cannot be empty. Please try again.\33[0m\n";
        return false;
    }
    size_t last = input.find_last_not_of(" \t\r\n");
    std::string s = input.substr(first, last - first + 1);
    if (s[0] == '+')
        s.erase(0, 1);

    for (char c : s)
    {
        if (!std::isdigit(static_cast<unsigned char>(c)))
        {
            std::cerr << "\33[31mInvalid input. Please try again.\33[0m\n";
            return false;
        }
    }
    try
    {
        var = std::stoi(s);
    }
    catch (...)
    {
        std::cerr << "\33[31mInvalid input. Please try again.\33[0m\n";
        return false;
    }
    if (var < 0)
    {
        std::cerr << "\33[31mInput cannot be nagative. Please try again.\33[0m\n";
        return false;
    }
    return true;
}

bool isValidSQLStringInput(const std::string& prompt, sql::SQLString& input, const bool& canEmpty)
{
    char acceptableSymbol[] = { " !@#$%^&()-_={}[]<>,.;:?/|~+*_" };
    std::cout << prompt;
    std::string inputBuff;
    std::getline(std::cin, inputBuff);
    bool containsLegalSymbol = false;
    for (char c : inputBuff)
    {
        if (!std::isalnum(static_cast<unsigned char>(c)))
        {
            for (char s : acceptableSymbol)
            {
                if (c == s)
                {
                    containsLegalSymbol = true;
                    break;
                }
            }
            if (containsLegalSymbol == false)
            {
                std::cerr << "\33[31mInvalid input. Please try again.\33[0m\n\n";
                return false;
            }
        }
    }
    if (canEmpty == false && inputBuff.empty())
    {
        input = "";
        std::cerr << "\33[31mInput cannot be empty. Please try again.\33[0m\n\n";
        return false;
    }
    else
    {
        input = inputBuff;
        return true;
    }
}

void deleteBasedOn1ID(sql::Connection& conn, const sql::SQLString& tableName, const sql::SQLString& idName, const sql::SQLString& idValue)
{
    try
    {
        sql::SQLString checkQuery = "SELECT * FROM " + tableName + " WHERE " + idName + " = ?";
        std::unique_ptr<sql::PreparedStatement> stmtChk(conn.prepareStatement(checkQuery));
        stmtChk->setString(1, idValue);
        std::unique_ptr<sql::ResultSet> resChk(stmtChk->executeQuery());
        if (!resChk->next())
        {
            std::cout << "\033[31mThe target to be deleted does not exist.\033[0m\n\n";
            return;
        }
        sql::SQLString query = "DELETE FROM " + tableName + " WHERE " + idName + " = ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement(query));
        pstmt->setString(1, idValue);
        pstmt->executeUpdate();
        std::cout << "\033[32m" << tableName << ' ' << idValue << " deleted successfully.\033[0m\n\n";
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n";
    }
}

void update1attributeByTableAndID(sql::Connection& conn, const sql::SQLString& tableName, const sql::SQLString& attributeName, const sql::SQLString& newValue, const sql::SQLString& condition)
{
    sql::SQLString query = "UPDATE " + tableName + " SET " + attributeName + " = ?" + condition;
    std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement(query));
    pstmt->setString(1, newValue);
    pstmt->executeUpdate();
    return;
}

void displayStudentAttendanceRecord(sql::Connection& conn, const sql::SQLString& studentID)
{
    std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement("SELECT * FROM ATTENDANCE WHERE studentID = ?"));
    pstmt->setString(1, studentID);
    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
    if (!res->next())
    {
        std::cout << "\033[33mThere is no attendance record for this student.\033[0m\n";
        return;
    }
    else
    {
        std::cout << "Displaying most recent 30 attendance records:\n";
        res->beforeFirst();
        std::cout << std::setw(12) << std::left << "Date";
        std::cout << "| Status\n";
        for (int i = 0; (i < 30 && res->next()); i++)
        {
            std::cout << std::setw(12) << std::left << res->getString("date");
            std::cout << res->getString("status") << std::endl;
        }
    }
    divider();
}

void displayExamResults(sql::Connection& conn, const sql::SQLString& studentID)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> stmnt(conn.prepareStatement("SELECT DISTINCT examID FROM EXAM_RESULT WHERE studentID = ?"));
        stmnt->setString(1, studentID);
        std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery());
        std::vector<sql::SQLString> examIDs;

        if (!res->next())
        {
            std::cout << "\033[33mThe student have no exam taken.\033[0m\n";
            return;
        }
        // fetch exam IDs
        do
        {
            examIDs.push_back(res->getString("examID"));
        } while (res->next());

        std::cout << "Please choose an exam by number.\n";
        // display exam options
        for (int i = 0; i < examIDs.size(); ++i)
        {
            std::cout << i + 1 << ": " << examIDs[i] << std::endl;
        }

        // user choosing exam
        int choice;
        if (!isValidPositiveInt("Choice: ", choice))
            return;

        if (choice < 1 || choice > examIDs.size())
        {
            std::cout << "\033[31mInvalid choice.\033[0m\n";
            return;
        }
        sql::SQLString examID = examIDs[choice - 1]; // selected examID
        std::unique_ptr<sql::PreparedStatement> examStmnt(conn.prepareStatement(
            "SELECT er.*, s.* FROM EXAM_RESULT er LEFT OUTER JOIN SUBJECT s ON er.subjectID = s.subjectID WHERE er.studentID = ? AND er.examID = ?;"));
        examStmnt->setString(1, studentID);
        examStmnt->setString(2, examID);
        std::unique_ptr<sql::ResultSet> examRes(examStmnt->executeQuery());

        // header for exam results
        std::cout << "Results for exam " << examID << ":\n";
        std::cout << std::setw(16) << std::setfill(' ') << "Subjects";
        std::cout << '|';
        std::cout << std::setw(8) << std::setfill(' ') << "Results";
        std::cout << '\n';

        while (examRes->next())
        {
            std::cout << std::setw(17) << examRes->getString("subjectName");
            std::cout << std::setw(8) << examRes->getDouble("result");
            std::cout << '\n';
        }
        divider();
        std::cout << '\n';
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n";
    }
}

void displayClassSchedule(sql::Connection& conn, const sql::SQLString& classID)
{
    try {
        std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement(
            "SELECT day,period,name,subjectName FROM CLASS_SCHEDULE cs LEFT OUTER JOIN TEACHER t USING (teacherID) LEFT OUTER JOIN SUBJECT s USING (subjectID) WHERE classID = ? ORDER BY day,period ASC"));
        stmt->setString(1, classID);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        // Display header
        std::cout << "Class Schedule for " << classID << std::endl;

        if (!res->next())
        {
            std::cout << "\033[33mClass Schedule Not Available Now\033[0m\n";
            return;
        } 
        else
        {
            bool hit = false;
            for (int i = 1; i <= 5; ++i)
            {
                Time time = Time(7, 0);
                sql::SQLString day;
                switch (i)
                {
                case 1:
                    day = ClassSchedule::MONDAY;
                    break;
                case 2:
                    day = ClassSchedule::TUESDAY;
                    break;
                case 3:
                    day = ClassSchedule::WEDNESDAY;
                    break;
                case 4:
                    day = ClassSchedule::THURSDAY;
                    break;
                case 5:
                    day = ClassSchedule::FRIDAY;
                    break;
                }
                std::cout << day << ":\n";
                for (int j = ClassSchedule::FIRST_PERIOD; j <= ClassSchedule::LAST_PERIOD; ++j)
                {
                    if (j == ClassSchedule::LUNCH_BREAK)
                    {
                        std::cout << std::setw(8) << std::right << time.get12HourFormatString()
                            << " - ";
                        time.setMinute(time.getMinute() + 30);
                        std::cout << std::setw(8) << time.get12HourFormatString();

                        std::cout << std::setw(16) << std::left << " : Lunch Break" << std::endl;
                        continue;
                    }
                    else
                    {
                        std::cout << std::setw(8) << std::right << time.get12HourFormatString()
                            << " - ";
                        time.setHour(time.getHour() + 1);
                        std::cout << std::setw(8) << time.get12HourFormatString() << " : ";
                    }
                    if (day == res->getString("day") && j == res->getInt("period"))
                        hit = true;
                    if (hit == true)
                    {
                        std::cout << std::setw(16) << std::left << res->getString("subjectName");
                        std::cout << " (";
                        std::cout << res->getString("name");
                        std::cout << ")\n";
                        hit = false;
                        if (!res->isLast())
                            res->next();
                    }
                    else
                    {
                        std::cout << "Free Period\n";
                    }
                }
                std::cout << std::endl;
            }
        }
        divider();
    }
    catch (sql::SQLException& e) {
        std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n";
    }
}

void manageAdmins(sql::Connection& conn)
{
    std::cout << '\n';
    while (true)
    {
        std::cout << "--- Manage Admins ---\n";
        std::cout << "1: Create Admin\n";
        std::cout << "2: Read Admin\n";
        std::cout << "3: Update Admin\n";
        std::cout << "4: Delete Admin\n";
        std::cout << "5: Search Admin\n";
        std::cout << "0: Back\n";

        int choice;
        if (!isValidPositiveInt("Choice: ", choice))
        {
            continue;
        }

        try
        {
            if (choice == 0)
            {
                system("cls");
                return;
            }

            switch (choice)
            {
            case 1: // Add admin
            {
                sql::SQLString id, name, password;
                if (!isValidSQLStringInput("New Admin ID: ", id, false))
                    continue;
                if (!isValidSQLStringInput("Name: ", name, false))
                    continue;
                if (!isValidSQLStringInput("Password: ", password, false))
                    continue;

                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement("INSERT INTO ADMIN (adminID, name, password) VALUES (?, ?, ?)"));
                stmt->setString(1, id);
                stmt->setString(2, name);
                stmt->setString(3, password);
                stmt->executeUpdate();
                std::cout << "\033[32mAdmin added.\033[0m\n\n";
                break;
            }
            case 2: // List admins
            {
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement("SELECT * FROM ADMIN"));
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                std::cout << std::left << std::setw(10) << "Admin ID | "
                    << std::setw(32) << std::right << "Name | "
                    << std::setw(12) << "Password"
                    << std::endl;

                // Print rows
                while (res->next()) {
                    std::cout << std::left << std::setw(10) << res->getString("adminID")
                        << std::setw(30) << std::right << res->getString("name") << "  "
                        << std::setw(12) << res->getString("password");
                    std::cout << '\n';
                }
                std::cout << "Total " << res->rowsCount() << " admins.\n";
                divider();
                break;
            }

            case 3: // Update admin
            {
                sql::SQLString id, name, password;
                if (!isValidSQLStringInput("Admin ID to update: ", id, false))
                    continue;
                sql::SQLString condition = " WHERE adminID = '" + id + "'";
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement("SELECT * FROM ADMIN" + condition));
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                if (!res->next())
                {
                    std::cout << "\33[31mAdmin " << id << " does not exist.\33[0m\n";
                    break;
                }
                if (!isValidSQLStringInput("New Name(leave blank to keep): ", name))
                    continue;
                if (!isValidSQLStringInput("New Password(leave blank to keep): ", password))
                    continue;


                if (!name.empty())
                {
                    update1attributeByTableAndID(conn, "ADMIN", "name", name, condition);
                }
                if (!password.empty())
                {
                    update1attributeByTableAndID(conn, "ADMIN", "password", password, condition);
                }

                std::cout << "\033[32mUpdate complete.\033[0m\n\n";
                break;
            }

            case 4: // Delete admin
            {
                sql::SQLString id;
                if (!isValidSQLStringInput("Admin ID to delete: ", id, false))
                    continue;
                deleteBasedOn1ID(conn, "ADMIN", "adminID", id);
                break;
            }

            case 5: //search admin
            {
                sql::SQLString id;
                if (!isValidSQLStringInput("Admin ID to search: ", id, false))
                {

                    break;
                }
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement("SELECT * FROM ADMIN WHERE adminID = ?"));
                stmt->setString(1, id);
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                if (!res->next())
                {
                    std::cout << "\033[33mThe admin does not exist.\033[0m\n";
                    continue;
                }
                else
                    res->beforeFirst();

                std::cout << std::left << std::setw(10) << "Admin ID | "
                    << std::setw(32) << std::right << "Name | "
                    << std::setw(12) << "Password"
                    << std::endl;

                // Print rows
                while (res->next()) {
                    std::cout << std::left << std::setw(10) << res->getString("adminID")
                        << std::setw(30) << std::right << res->getString("name") << "  "
                        << std::setw(12) << res->getString("password");
                    std::cout << '\n';
                }
                divider();
                break;
            }
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
        }
        catch (sql::SQLException& e)
        {
            std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n";
        }
    }
}

void manageTeachers(sql::Connection& conn, const sql::SQLString& adminID)
{
    std::cout << '\n';
    while (true)
    {
        std::cout << "--- Manage Teachers ---\n";
        std::cout << "1: Create Teacher\n";
        std::cout << "2: List Teachers\n";
        std::cout << "3: Update Teacher\n";
        std::cout << "4: Delete Teacher\n";
        std::cout << "5: Search Teacher\n";
        std::cout << "0: Back\n";

        int choice;
        if (!isValidPositiveInt("Choice: ", choice))
            continue;


        if (choice == 0)
        {
            system("cls");
            return;
        }

        try
        {
            switch (choice)
            {
            case 1: // Create teacher
            {
                sql::SQLString id, name, phone, address, ic, password;
                id = name = phone = address = ic = password = "";
                if (!isValidSQLStringInput("New Teacher ID: ", id, false))
                    continue;
                if (!isValidSQLStringInput("Name: ", name, false))
                    continue;
                if (!isValidSQLStringInput("Phone: ", phone, false))
                    continue;
                if (!isValidSQLStringInput("Address: ", address))
                    continue;
                if (!isValidSQLStringInput("IC: ", ic, false))
                    continue;
                if (!isValidSQLStringInput("Password: ", password, false))
                    continue;

                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement(
                    "INSERT INTO TEACHER (teacherID, name, phoneNum, address, icNum, password, adminID) VALUES (?, ?, ?, ?, ?, ?, ?)"));
                stmt->setString(1, id);
                stmt->setString(2, name);
                stmt->setString(3, phone);
                stmt->setString(4, address);
                stmt->setString(5, ic);
                stmt->setString(6, password);
                stmt->setString(7, adminID);
                stmt->executeUpdate();

                std::cout << "\033[32mTeacher added.\033[0m\n";
                break;
            }

            case 2: // List teachers
            {
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement("SELECT * FROM TEACHER ORDER BY teacherID"));
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

                std::cout << std::left
                    << std::setw(12) << "TeacherID" << '|'
                    << std::setw(23) << "Name" << '|'
                    << std::setw(14) << "Phone" << '|'
                    << std::setw(32) << "Address" << '|'
                    << std::setw(18) << "IC" << '|'
                    << std::setw(12) << "AdminID"
                    << std::endl;

                while (res->next())
                {
                    std::cout << std::left
                        << std::setw(13) << res->getString("teacherID")
                        << std::setw(24) << res->getString("name")
                        << std::setw(15) << res->getString("phoneNum")
                        << std::setw(33) << res->getString("address")
                        << std::setw(19) << res->getString("icNum")
                        << std::setw(12) << res->getString("adminID")
                        << '\n';
                }
                divider();
                break;
            }

            case 3: // Update teacher
            {
                sql::SQLString id, name, phone, address, ic, password;
                if (!isValidSQLStringInput("Teacher ID to update: ", id, false))
                    continue;
                sql::SQLString condition = " WHERE teacherID = '" + id + "'";
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement("SELECT * FROM TEACHER" + condition));
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                if (!res->next())
                {
                    std::cout << "\33[31mTeacher " << id << " does not exist.\33[0m\n";
                    break;
                }
                if (!isValidSQLStringInput("New Name(leave blank to keep): ", name))
                    continue;
                if (!isValidSQLStringInput("New Phone(leave blank to keep): ", phone))
                    continue;
                if (!isValidSQLStringInput("New Address(leave blank to keep): ", address))
                    continue;
                if (!isValidSQLStringInput("New IC(leave blank to keep): ", ic))
                    continue;
                if (!isValidSQLStringInput("New Password(leave blank to keep): ", password))
                    continue;


                if (!name.empty())
                {
                    update1attributeByTableAndID(conn, "TEACHER", "name", name, condition);
                }
                if (!phone.empty())
                {
                    update1attributeByTableAndID(conn, "TEACHER", "phoneNum", phone, condition);
                }
                if (!address.empty())
                {
                    update1attributeByTableAndID(conn, "TEACHER", "address", address, condition);
                }
                if (!ic.empty())
                {
                    update1attributeByTableAndID(conn, "TEACHER", "icNum", ic, condition);
                }
                if (!password.empty())
                {
                    update1attributeByTableAndID(conn, "TEACHER", "password", password, condition);
                }
                update1attributeByTableAndID(conn, "TEACHER", "adminID", adminID, condition);
                std::cout << "\033[32mTeacher " << id << " update complete\033[0m\n\n";
                break;
            }

            case 4: // Delete teacher
            {
                sql::SQLString id;
                if (!isValidSQLStringInput("Teacher ID to delete: ", id, false))
                    continue;
                deleteBasedOn1ID(conn, "TEACHER", "teacherID", id);
                break;
            }

            case 5: // Search teacher
            {
                sql::SQLString id;
                if (!isValidSQLStringInput("Teacher ID to search: ", id, false))
                    continue;
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement(
                    "SELECT teacherID, name, phoneNum, address, icNum, adminID FROM TEACHER WHERE teacherID = ?"));
                stmt->setString(1, id);
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                if (!res->next())
                {
                    std::cout << "\033[33mTeacher " << id << " does not exist.\033[0m\n";
                    break;
                }
                else
                    res->beforeFirst();

                std::cout << std::left
                    << std::setw(10) << "TeacherID" << '|'
                    << std::setw(20) << "Name" << '|'
                    << std::setw(14) << "Phone" << '|'
                    << std::setw(28) << "Address" << '|'
                    << std::setw(18) << "IC" << '|'
                    << std::setw(12) << "AdminID"
                    << std::endl;


                while (res->next())
                {
                    std::cout << std::left
                        << std::setw(11) << res->getString("teacherID")
                        << std::setw(21) << res->getString("name")
                        << std::setw(15) << res->getString("phoneNum")
                        << std::setw(29) << res->getString("address")
                        << std::setw(19) << res->getString("icNum")
                        << std::setw(12) << res->getString("adminID")
                        << '\n';
                }
                break;
            }

            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
        }
        catch (sql::SQLException& e)
        {
            std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n";
        }
    }
}

void manageStudentsAndParents(sql::Connection& conn)
{
    while (true)
    {
        std::cout << "\n--- Manage Students and Parents ---\n";
        std::cout << "Student operations:\n";
        std::cout << "  1: Create Student\n";
        std::cout << "  2: List Students\n";
        std::cout << "  3: Update Student\n";
        std::cout << "  4: Delete Student\n";
        std::cout << "  5: Search Student\n\n";
        std::cout << "Parent operations:\n";
        std::cout << "  6: Create Parent\n";
        std::cout << "  7: List Parents\n";
        std::cout << "  8: Update Parent\n";
        std::cout << "  9: Delete Parent\n";
        std::cout << " 10: Search Parent\n\n";
        std::cout << " 11: Create Child List\n";
        std::cout << " 12: Display Child List\n";
        std::cout << " 13: Update Child List\n";
        std::cout << " 14: Delete Child List\n";
        std::cout << "  0: Back\n";

        int choice;
        if (!isValidPositiveInt("Choice: ", choice))
            continue;
        if (choice == 0)
        {
            system("cls");
            return;
        }
        try
        {

            switch (choice)
            {
                // --- Students ---
            case 1: // Create Student
            {
                sql::SQLString id, name, phone, address, ic, password, classID;
                if (!isValidSQLStringInput("New Student ID: ", id, false))
                    continue;
                if (!isValidSQLStringInput("Name: ", name, false))
                    continue;
                if (!isValidSQLStringInput("Phone: ", phone))
                    continue;
                if (!isValidSQLStringInput("Address: ", address))
                    continue;
                if (!isValidSQLStringInput("IC: ", ic, false))
                    continue;
                if (!isValidSQLStringInput("Password: ", password, false))
                    continue;
                if (!isValidSQLStringInput("ClassID: ", classID, false))
                    continue;

                std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement("INSERT INTO student (studentID, name, icNum, address, phoneNum, classID, password) VALUES (?,?,?,?,?,?,?)"));
                pstmt->setString(1, id);
                pstmt->setString(2, name);
                pstmt->setString(3, ic);
                pstmt->setString(4, address);
                pstmt->setString(5, phone);
                pstmt->setString(6, classID);
                pstmt->setString(7, password);
                pstmt->executeUpdate();
                std::cout << "\033[32mStudent added.\033[0m\n\n";
                break;
            }

            case 2: // List Students
            {
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement("SELECT * FROM STUDENT"));
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                std::cout << std::left
                    << std::setw(11) << "StudentID" << '|'
                    << std::setw(22) << "Name" << '|'
                    << std::setw(14) << "Phone" << '|'
                    << std::setw(28) << "Address" << '|'
                    << std::setw(18) << "IC" << '|'
                    << std::setw(8) << "Password" << '|'
                    << std::setw(12) << "ClassID"
                    << std::endl;

                while (res->next())
                {
                    std::cout << std::left
                        << std::setw(12) << res->getString("studentID")
                        << std::setw(23) << res->getString("name")
                        << std::setw(15) << res->getString("phoneNum")
                        << std::setw(29) << res->getString("address")
                        << std::setw(19) << res->getString("icNum")
                        << std::setw(9) << res->getString("password")
                        << std::setw(12) << res->getString("classID")
                        << '\n';
                }
                divider();
                break;
            }

            case 3: // Update Student
            {
                sql::SQLString id, name, phone, address, ic, password, classID;
                if (!isValidSQLStringInput("Student ID to update: ", id, false))
                    continue;
                sql::SQLString condition = " WHERE studentID = '" + id + "'";
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement("SELECT * FROM STUDENT" + condition));
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                if (!res->next())
                {
                    std::cout << "\033[31mStudent " << id << " does not exist.\033[0m\n";
                    break;
                }
                else
                    res->beforeFirst();

                if (!isValidSQLStringInput("New Name(leave blank to keep): ", name))
                    continue;
                if (!isValidSQLStringInput("New Phone(leave blank to keep): ", phone))
                    continue;
                if (!isValidSQLStringInput("New Address(leave blank to keep): ", address))
                    continue;
                if (!isValidSQLStringInput("New IC(leave blank to keep): ", ic))
                    continue;
                if (!isValidSQLStringInput("New Password(leave blank to keep): ", password))
                    continue;
                if (!isValidSQLStringInput("New ClassID(leave blank to keep): ", classID))
                    continue;

                if (!name.empty())
                    update1attributeByTableAndID(conn, "STUDENT", "name", name, condition);
                if (!phone.empty())
                    update1attributeByTableAndID(conn, "STUDENT", "phoneNum", phone, condition);
                if (!address.empty())
                    update1attributeByTableAndID(conn, "STUDENT", "address", address, condition);
                if (!ic.empty())
                    update1attributeByTableAndID(conn, "STUDENT", "icNum", ic, condition);
                if (!password.empty())
                    update1attributeByTableAndID(conn, "STUDENT", "password", password, condition);
                if (!classID.empty())
                    update1attributeByTableAndID(conn, "STUDENT", "classID", classID, condition);

                std::cout << "\033[32mStudent " << id << " Update complete.\033[0m\n\n";
                break;
            }

            case 4: // Delete Student
            {
                sql::SQLString id;
                if (!isValidSQLStringInput("Student ID to delete: ", id, false))
                    continue;
                deleteBasedOn1ID(conn, "STUDENT", "studentID", id);
                break;
            }

            case 5: // Search Student
            {
                sql::SQLString id;
                if (!isValidSQLStringInput("Student ID to search: ", id, false))
                    continue;
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement("SELECT * FROM STUDENT WHERE studentID = ?"));
                stmt->setString(1, id);
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                if (!res->next())
                {
                    std::cout << "\033[33mThe student " << id << " does not exist.\033[0m\n\n";
                    break;
                }
                else
                    res->beforeFirst();

                std::cout << std::left
                    << std::setw(11) << "StudentID" << '|'
                    << std::setw(22) << "Name" << '|'
                    << std::setw(14) << "Phone" << '|'
                    << std::setw(28) << "Address" << '|'
                    << std::setw(18) << "IC" << '|'
                    << std::setw(8) << "Password" << '|'
                    << std::setw(12) << "ClassID"
                    << std::endl;

                while (res->next())
                {
                    std::cout << std::left
                        << std::setw(12) << res->getString("studentID")
                        << std::setw(23) << res->getString("name")
                        << std::setw(15) << res->getString("phoneNum")
                        << std::setw(29) << res->getString("address")
                        << std::setw(19) << res->getString("icNum")
                        << std::setw(9) << res->getString("password")
                        << std::setw(12) << res->getString("classID")
                        << '\n';
                }

                std::unique_ptr<sql::PreparedStatement> stmt2(conn.prepareStatement(
                    "SELECT p.name, p.parentID FROM CHILD_LIST cs LEFT OUTER JOIN PARENT p USING (parentID) WHERE studentID=?;"));
                stmt2->setString(1, id);
                std::unique_ptr<sql::ResultSet> res2(stmt->executeQuery());
                std::cout << "Parents:\n";
                std::cout << std::left << std::setw(33) << res2->getString("name") << std::setw(13) << res2->getString("parentID");
                break;
            }

            // --- Parents ---
            case 6: // Create Parent
            {
                sql::SQLString id, name, phone, address, ic, password;
                if (!isValidSQLStringInput("New Parent ID: ", id, false))
                    continue;
                if (!isValidSQLStringInput("Name: ", name, false))
                    continue;
                if (!isValidSQLStringInput("Phone: ", phone))
                    continue;
                if (!isValidSQLStringInput("Address: ", address))
                    continue;
                if (!isValidSQLStringInput("IC: ", ic, false))
                    continue;
                if (!isValidSQLStringInput("Password: ", password, false))
                    continue;

                std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement(
                    "INSERT INTO parent (parentID, name, icNum, address, phoneNum, password) VALUES (?,?,?,?,?,?)"));
                pstmt->setString(1, id);
                pstmt->setString(2, name);
                pstmt->setString(3, ic);
                pstmt->setString(4, address);
                pstmt->setString(5, phone);
                pstmt->setString(6, password);
                pstmt->executeUpdate();
                std::cout << "\033[32mParent added.\033[0m\n\n";
                break;
            }

            case 7: // List Parents
            {
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement("SELECT * FROM PARENT"));
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                std::cout << std::left
                    << std::setw(12) << "ParentID" << '|'
                    << std::setw(23) << "Name" << '|'
                    << std::setw(14) << "Phone" << '|'
                    << std::setw(27) << "Address" << '|'
                    << std::setw(8) << "Password" << '|'
                    << std::setw(18) << "IC"
                    << std::endl;

                while (res->next())
                {
                    std::cout << std::left
                        << std::setw(12) << res->getString("parentID")
                        << std::setw(24) << res->getString("name")
                        << std::setw(14) << res->getString("phoneNum")
                        << std::setw(28) << res->getString("address")
                        << std::setw(9) << res->getString("password")
                        << std::setw(18) << res->getString("icNum")
                        << '\n';
                }
                divider();
                break;
            }

            case 8: // Update parent
            {
                sql::SQLString id, name, phone, address, ic, password;

                if (!isValidSQLStringInput("Parent ID to update: ", id, false))
                    continue;
                sql::SQLString condition = " WHERE parentID = '" + id + "'";
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement("SELECT * FROM PARENT" + condition));
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                if (!res->next())
                {
                    std::cout << "\033[31mPARENT " << id << " does not exist.\033[0m\n";
                    break;
                }
                else
                    res->beforeFirst();
                if (!isValidSQLStringInput("New Name (leave blank to keep): ", name))
                    continue;
                if (!isValidSQLStringInput("New Phone (leave blank to keep): ", phone))
                    continue;
                if (!isValidSQLStringInput("New Address (leave blank to keep): ", address))
                    continue;
                if (!isValidSQLStringInput("New IC (leave blank to keep): ", ic))
                    continue;
                if (!isValidSQLStringInput("New Password (leave blank to keep): ", password))
                    continue;



                if (!name.empty())
                {
                    update1attributeByTableAndID(conn, "PARENT", "name", name, condition);
                }
                if (!phone.empty())
                {
                    update1attributeByTableAndID(conn, "PARENT", "phoneNum", phone, condition);
                }
                if (!address.empty())
                {
                    update1attributeByTableAndID(conn, "PARENT", "address", address, condition);
                }
                if (!ic.empty())
                {
                    update1attributeByTableAndID(conn, "PARENT", "icNum", ic, condition);
                }
                if (!password.empty())
                {
                    update1attributeByTableAndID(conn, "PARENT", "password", password, condition);
                }

                std::cout << "Update complete.\n";
                break;
            }

            case 9: // Delete Parent
            {
                sql::SQLString id;
                if (!isValidSQLStringInput("Parent ID to be deleted: ", id, false))
                    continue;
                deleteBasedOn1ID(conn, "PARENT", "parentID", id);
                break;
            }

            case 10: // Search Parent
            {
                sql::SQLString id;
                if (!isValidSQLStringInput("Parent ID to search: ", id, false))
                    continue;

                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement("SELECT * FROM PARENT WHERE parentID = ?"));
                stmt->setString(1, id);
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                if (!res->next())
                {
                    std::cout << "\033[33mThe parent does not exist.\033[0m\n\n";
                    continue;
                }
                else
                    res->beforeFirst();

                std::cout << std::left
                    << std::setw(12) << "ParentID" << '|'
                    << std::setw(28) << "Name" << '|'
                    << std::setw(14) << "Phone" << '|'
                    << std::setw(25) << "Address" << '|'
                    << std::setw(8) << "Password" << '|'
                    << std::setw(18) << "IC"
                    << std::endl;

                while (res->next())
                {
                    std::cout << std::left
                        << std::setw(13) << res->getString("parentID")
                        << std::setw(29) << res->getString("name")
                        << std::setw(15) << res->getString("phoneNum")
                        << std::setw(26) << res->getString("address")
                        << std::setw(9) << res->getString("password")
                        << std::setw(19) << res->getString("icNum")
                        << '\n';
                }
                std::unique_ptr<sql::PreparedStatement> stmt2(conn.prepareStatement(
                    "SELECT s.name, s.studentID FROM CHILD_LIST cs LEFT OUTER JOIN STUDENT s USING (studentID) WHERE cs.parentID = ?"));
                stmt2->setString(1, id);
                std::unique_ptr<sql::ResultSet> res2(stmt2->executeQuery());

                if (!res->next())
                    break;
                else
                    res->beforeFirst();

                std::cout << "Children:\n";

                while (res->next())
                {
                    std::cout << std::left
                        << std::setw(33) << res2->getString("name")
                        << std::setw(13) << res2->getString("studentID")
                        << '\n';
                }

                break;
            }

            //---Child List---
            case 11: //create
            {
                sql::SQLString studID, parID;
                int rel = 0;
                if (!isValidSQLStringInput("Child's StudentID: ", studID, false) || !isValidSQLStringInput("ParentID :", parID, false))
                    continue;
                std::cout << "Their relationship:\n"
                    << " 1: FATHER\n"
                    << " 2: MOTHER\n"
                    << " 3: GUARDIAN\n";

                if (!isValidPositiveInt("Choose a relation: ", rel))
                    continue;
                if (rel < 1 || rel>3)
                {
                    std::cout << "\033[31mInvalid choice, please try again.\033[0m\n\n";
                    continue;
                }

                std::unique_ptr <sql::PreparedStatement> pstmt(conn.prepareStatement("INSERT INTO child_list (studentID,parentID,relationship) VALUES (?,?,?)"));
                pstmt->setString(1, studID);
                pstmt->setString(2, parID);
                pstmt->setInt(3, rel);
                pstmt->executeUpdate();
                std::cout << "\033[32mChild List Created.\033[0m\n\n";
                break;
            }

            case 12: // Display Child List
            {
                try {
                    std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement(
                        "SELECT cs.studentID, cs.parentID, cs.relationship, s.name AS studentName, p.name AS parentName FROM child_list cs LEFT OUTER JOIN STUDENT s USING (studentID) LEFT OUTER JOIN PARENT p USING (parentID) ORDER BY cs.studentID"));
                    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

                    std::cout << std::left
                        << std::setw(12) << "StudentID" << '|'
                        << std::setw(20) << "StudentName" << '|'
                        << std::setw(12) << "ParentID" << '|'
                        << std::setw(20) << "ParentName" << '|'
                        << std::setw(12) << "Relation" << '\n';

                    while (res->next()) {
                        std::cout << std::left
                            << std::setw(13) << res->getString("studentID")
                            << std::setw(21) << res->getString("studentName")
                            << std::setw(13) << res->getString("parentID")
                            << std::setw(21) << res->getString("parentName")
                            << std::setw(13) << res->getString("relationship") << '\n';
                    }
                    divider();
                }
                catch (sql::SQLException& e) {
                    std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n\n";
                }
                break;
            }

            case 13: // Update Child List (change relationship)
            {
                sql::SQLString studID, parID, condition;
                int newRel = 0;
                if (!isValidSQLStringInput("Child's StudentID: ", studID, false) || !isValidSQLStringInput("ParentID: ", parID, false))
                    continue;
                condition = " WHERE studentID = '" + studID + "'";
                std::unique_ptr<sql::PreparedStatement> chk(conn.prepareStatement("SELECT * FROM child_list" + condition));
                std::unique_ptr<sql::ResultSet> resChk(chk->executeQuery());
                if (!resChk->next()) {
                    std::cout << "\033[31mChild list entry does not exist.\033[0m\n\n";
                    break;
                }

                std::cout << "New relationship:\n 1: FATHER\n 2: MOTHER\n 3: GUARDIAN\n";
                if (!isValidPositiveInt("Choose a relation: ", newRel))
                    continue;
                if (newRel < 1 || newRel > 3)
                {
                    std::cout << "\033[31mInvalid relation choice.\033[0m\n";
                    continue;
                }
                std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement("UPDATE CHILD_LIST (parentID,relationship) VALUES (?,?)" + condition));
                pstmt->setString(1, parID);
                pstmt->setInt(2, newRel);
                std::cout << "\033[32mChild list updated.\033[0m\n\n";
                break;
            }

            case 14: // Delete Child List
            {
                sql::SQLString studID, parID;
                if (!isValidSQLStringInput("Child's StudentID to delete: ", studID, false) || !isValidSQLStringInput("ParentID to delete: ", parID, false))
                    continue;
                try {
                    std::unique_ptr<sql::PreparedStatement> chk(conn.prepareStatement("SELECT * FROM child_list WHERE studentID = ? AND parentID = ?"));
                    chk->setString(1, studID);
                    chk->setString(2, parID);
                    std::unique_ptr<sql::ResultSet> resChk(chk->executeQuery());
                    if (!resChk->next()) {
                        std::cout << "\033[33mChild list entry does not exist.\033[0m\n\n";
                        break;
                    }
                    std::unique_ptr<sql::PreparedStatement> del(conn.prepareStatement("DELETE FROM child_list WHERE studentID = ? AND parentID = ?"));
                    del->setString(1, studID);
                    del->setString(2, parID);
                    del->executeUpdate();
                    std::cout << "\033[32mChild list entry deleted.\033[0m\n\n";
                }
                catch (sql::SQLException& e) {
                    std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n\n";
                }
                break;
            }

            default:
                std::cout << "\033[31mInvalid choice. Please try again.\033[0m\n\n";
                break;
            }
        }
        catch (sql::SQLException& e)
        {
            std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n";
        }
    }
}

void manageClassesAndAttendance(sql::Connection& conn)
{
    while (true)
    {
        std::cout << "\n--- Manage Classes and Attendance ---\n";
        std::cout << "Class operations:\n";
        std::cout << "  1: Create Class\n";
        std::cout << "  2: List Classes\n";
        std::cout << "  3: Update Class\n";
        std::cout << "  4: Delete Class\n";
        std::cout << "  5: Search Class\n\n";
        std::cout << "Attendance operations:\n";
        std::cout << "  6: Create Attendance\n";
        std::cout << "  7: List Attendance\n";
        std::cout << "  8: Update Attendance\n";
        std::cout << "  9: Delete Attendance\n";
        std::cout << " 10: Search Attendance by Student\n\n";
        std::cout << "Class Schedule Operation\n";
        std::cout << " 11: Create Class Schedule\n";
        std::cout << " 12: Update Class Schedule\n";
        std::cout << " 13: Display Class Schedule of A Class\n";
        std::cout << "  0: Back\n";

        int choice;
        if (!isValidPositiveInt("Choice: ", choice))
            continue;
        if (choice == 0)
        {
            system("cls");
            return;
        }
        try
        {
            switch (choice)
            {
                // --- Class ---
            case 1: // Create Class
            {
                sql::SQLString id, teacherID, roomNum;
                if (!isValidSQLStringInput("New Class ID: ", id, false))
                    continue;
                if (!isValidSQLStringInput("Teacher ID: ", teacherID, false))
                    continue;
                if (!isValidSQLStringInput("Room Number: ", roomNum, false))
                    continue;

                std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement("INSERT INTO CLASS (classID, teacherID, roomNum) VALUES (?,?,?)"));
                pstmt->setString(1, id);
                pstmt->setString(2, teacherID);
                pstmt->setString(3, roomNum);
                pstmt->executeUpdate();
                std::cout << "\033[32mClass created.\033[0m\n\n";
                break;
            }

            case 2: // List Classes
            {
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement("SELECT * FROM CLASS"));
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                std::cout << std::left
                    << std::setw(12) << "ClassID" << '|'
                    << std::setw(12) << "TeacherID" << '|'
                    << std::setw(12) << "RoomNum"
                    << std::endl;
                while (res->next())
                {
                    std::cout << std::left
                        << std::setw(13) << res->getString("classID")
                        << std::setw(13) << res->getString("teacherID")
                        << std::setw(12) << res->getString("roomNum")
                        << '\n';
                }
                divider();
                break;
            }

            case 3: // Update Class
            {
                sql::SQLString id, teacherID, roomNum, condition;
                if (!isValidSQLStringInput("Class ID to update: ", id, false))
                    continue;
                condition = " WHERE classID = '" + id + "'";
                std::unique_ptr<sql::PreparedStatement> chk(conn.prepareStatement("SELECT * FROM class" + condition));
                std::unique_ptr<sql::ResultSet> resChk(chk->executeQuery());
                if (!resChk->next()) {
                    std::cout << "\033[31mClass does not exist.\033[0m\n\n";
                    break;
                }
                if (!isValidSQLStringInput("New Teacher ID(leave blank to keep): ", teacherID))
                    continue;
                if (!isValidSQLStringInput("New Room Number(leave blank to keep): ", roomNum))
                    continue;

                if (!teacherID.empty())
                    update1attributeByTableAndID(conn, "CLASS", "teacherID", teacherID, condition);
                if (!roomNum.empty())
                    update1attributeByTableAndID(conn, "CLASS", "roomNum", roomNum, condition);
                std::cout << "\033[32mClass " << id << " updated.\033[0m\n\n";
                break;
            }

            case 4: // Delete Class
            {
                sql::SQLString id;
                if (!isValidSQLStringInput("Class ID to delete: ", id, false))
                    continue;
                deleteBasedOn1ID(conn, "CLASS", "classID", id);
                break;
            }

            case 5: // Search Class
            {
                sql::SQLString id;
                if (!isValidSQLStringInput("Class ID to search: ", id, false))
                    continue;
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement(
                    "SELECT c.classID,c.teacherID,c.roomNum,t.name FROM CLASS c LEFT OUTER JOIN TEACHER t USING(teacherID) WHERE c.classID = ?"));

                //std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement("SELECT * FROM CLASS WHERE classID = ?"));
                stmt->setString(1, id);
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

                if (!res->next())
                {
                    std::cout << "\033[33mThe class does not exist.\033[0m";
                    break;
                }
                else
                    res->beforeFirst();
                std::cout << std::left
                    << std::setw(12) << "ClassID" << '|'
                    << std::setw(12) << "TeacherID" << '|'
                    << std::setw(12) << "RoomNum" << '|'
                    << std::setw(12) << "Teacher Name"
                    << std::endl;
                while (res->next())
                {
                    std::cout << std::left
                        << std::setw(13) << res->getString("classID")
                        << std::setw(13) << res->getString("teacherID")
                        << std::setw(13) << res->getString("roomNum")
                        << std::setw(12) << res->getString("name")
                        << '\n';
                }

                //display class student
                std::unique_ptr<sql::PreparedStatement> stmt2(conn.prepareStatement(
                    "SELECT name,studentID FROM STUDENT WHERE classID = ?"));
                stmt2->setString(1,id);
                std::unique_ptr<sql::ResultSet> res2(stmt2->executeQuery());
                if (!res2->next())
                {
                    std::cout << "\033[33mThe class have no student\033[0m";
                    break;
                }
                else
                {
                    std::cout << "\n\nStudent List:\n";
                    res2->beforeFirst();
                    std::cout << std::left
                        << std::setw(4) << "no." << '|'
                        << std::setw(12) << "studentID" << '|'
                        << std::setw(12) << "name" << '|'
                        << std::endl;

                    for (int i = 0; res2->next(); i++)
                    {
                        std::cout << std::left
                            << std::setw(5) << i
                            << std::setw(13) << res2->getString("studentID")
                            << std::setw(13) << res2->getString("name")
                            << '\n';
                    }
                    divider();
                }
                break;
            }

            // --- Attendance ---
            case 6: // Create Attendance
            {
                sql::SQLString studentID, remarks;
                Date date;
                int year, month, day, status;
                if (!isValidSQLStringInput("Student ID to record attendance: ", studentID, false))
                    continue;
                std::cout << "Status:\n"
                    << " 1: PRESENT\n"
                    << " 2: ABSENT\n";
                if (!isValidPositiveInt("Status(1/2): ", status))
                    continue;
                if ((status != 1) && (status != 2))
                {
                    std::cout << "\033[31mInvalid Choice. Please try again.\033[0m\n\n";
                    continue;
                }
                if (!isValidSQLStringInput("Remarks: ", remarks))
                    continue;
                std::cout << "Date:";
                if (!isValidPositiveInt("Year: ", year))
                    continue;
                date.setYear(year);
                if (!isValidPositiveInt("Month: ", month))
                    continue;
                date.setMonth(month);
                if (!isValidPositiveInt("Date: ", day))
                    continue;
                date.setDay(day);

                std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement("INSERT INTO ATTENDANCE (date, studentID, status, remarks) VALUES (?,?,?,?)"));
                pstmt->setString(1, date.toString());
                pstmt->setString(2, studentID);
                pstmt->setInt(3, status);
                pstmt->setString(4, remarks);
                pstmt->executeUpdate();
                std::cout << "\033[32mAttendance record added.\033[0m\n";
                break;
            }

            case 7: // List Attendance
            {
                std::unique_ptr<sql::PreparedStatement> ps(conn.prepareStatement(
                    "SELECT date, studentID, status, remarks FROM ATTENDANCE ORDER BY `date` DESC"));
                std::unique_ptr<sql::ResultSet> res(ps->executeQuery());
                std::cout << std::left << std::setw(12) << "Date" << '|'
                    << std::setw(12) << "StudentID" << '|'
                    << std::setw(12) << "Status" << '|'
                    << "Remarks" << std::endl;
                while (res->next())
                {
                    std::cout << std::left << std::setw(13) << res->getString("date")
                        << std::setw(13) << res->getString("studentID")
                        << std::setw(12) << res->getString("status")
                        << res->getString("remarks") << '\n';
                }
                break;
            }

            case 8: // Update Attendance
            {
                sql::SQLString studentID, remarks, condition, dateString;
                Date date;
                int year, month, day, status;
                if (!isValidSQLStringInput("Student ID to update: ", studentID, false))
                    continue;
                condition = " WHERE studentID = '" + studentID + "'";
                std::unique_ptr<sql::PreparedStatement> chk(conn.prepareStatement("SELECT * FROM Attendance" + condition));
                std::unique_ptr<sql::ResultSet> resChk(chk->executeQuery());
                if (!resChk->next()) {
                    std::cout << "\033[31mNo attendance recorded for student " << studentID << ".\033[0m\n\n";
                    break;
                }

                std::cout << "Status:\n"
                    << " 1: PRESENT\n"
                    << " 2: ABSENT\n";
                if (!isValidPositiveInt("Status(1/2): ", status))
                    continue;

                if (status != 1 && status != 2)
                {
                    std::cout << "\033[31mInvalid Choice. Please try again.\033[0m\n\n";
                    continue;
                }
                std::cout << "Date:";
                if (!isValidPositiveInt("Year: ", year))
                    continue;
                date.setYear(year);
                if (!isValidPositiveInt("Month: ", month))
                    continue;
                date.setMonth(month);
                if (!isValidPositiveInt("Date: ", day))
                    continue;
                date.setDay(day);
                dateString = date.toString();

                if (!isValidSQLStringInput("New Remarks(leave blank to keep): ", remarks))
                    continue;
                update1attributeByTableAndID(conn, "Attendance", "date", dateString, condition);
                if (!remarks.empty())
                    update1attributeByTableAndID(conn, "Attendance", "date", remarks, condition);
                std::cout << "\033[32mAttendance Updated.\033[0m\n\n";
                break;
            }

            case 9: // Delete Attendance
            {
                sql::SQLString studentID;
                Date date;
                int year, month, day;
                if (!isValidSQLStringInput("Student ID to delete attendance: ", studentID, false))
                    continue;
                std::cout << "Date:";
                if (!isValidPositiveInt("Year: ", year))
                    continue;
                date.setYear(year);
                if (!isValidPositiveInt("Month: ", month))
                    continue;
                date.setMonth(month);
                if (!isValidPositiveInt("Date: ", day))
                    continue;
                date.setDay(day);

                std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement(
                    "DELETE FROM Attendance WHERE studentID = ? AND date = ?"));
                pstmt->setString(1, studentID);
                pstmt->setString(2, date.toString());
                pstmt->executeUpdate();
                std::cout << "\033[32mAttendance deleted.\033[0m\n\n";
                break;
            }

            case 10: // Search Attendance by Student
            {
                sql::SQLString studentID;
                if (!isValidSQLStringInput("StudentID to be searched: ", studentID, false))
                    continue;

                displayStudentAttendanceRecord(conn, studentID);
                divider();
                break;
            }

            //class shcedule
            case 11: //create
            {
                int period, day;
                sql::SQLString classID, teacherID, subjectID;
                if (!isValidSQLStringInput("ClassID: ", classID, false))
                    continue;
                if (!isValidPositiveInt(
                    "Class Period("+std::to_string(ClassSchedule::FIRST_PERIOD)+" - "+std::to_string(ClassSchedule::LAST_PERIOD)+", not "+std::to_string(ClassSchedule::LUNCH_BREAK)+": ", period))
                    continue;
                if (period < ClassSchedule::FIRST_PERIOD || period > ClassSchedule::LAST_PERIOD || period == ClassSchedule::LUNCH_BREAK)
                {
                    std::cout << "\033[31mInvalid Class Period. Please try again.\033[0m\n\n";
                    continue;
                }
                if (!isValidPositiveInt("Day(1-5): ", day))
                    continue;
                if (day < 1 || day > 5)
                {
                    std::cout << "\033[31mInvalid Day. Please try again.\033[0m\n\n";
                    continue;
                }
                if (!isValidSQLStringInput("TeacherID: ", teacherID, false))
                    continue;
                if (!isValidSQLStringInput("SubjectID: ", subjectID, false))
                    continue;
                std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement(
                    "INSERT INTO class_schedule (period,day,classID,teacherID,subjectID) VALUES (?,?,?,?,?)"));
                pstmt->setInt(1, period);
                pstmt->setInt(2, day);
                pstmt->setString(3, classID);
                pstmt->setNull(4, sql::DataType::VARCHAR);
                pstmt->setNull(5, sql::DataType::VARCHAR);
                pstmt->setString(5, subjectID);
                pstmt->executeUpdate();
                std::cout << "\033[32mClass Schedule Recorded.\033[0m\n\n";
                break;
            }

            case 12: // Update Class Schedule
            {
                sql::SQLString classID, newTeacherID, newSubjectID;
                int day = 0, period = 0;
                if (!isValidSQLStringInput("Class ID for schedule to update: ", classID, false))
                    continue;
                if (!isValidPositiveInt("Day (1=Mon .. 5=Fri): ", day))
                    continue;
                if (!isValidPositiveInt(
                    "Class Period(" + std::to_string(ClassSchedule::FIRST_PERIOD) + " - " + std::to_string(ClassSchedule::LAST_PERIOD) + ", not " + std::to_string(ClassSchedule::LUNCH_BREAK) + ": ", period))
                    continue;

                std::unique_ptr<sql::PreparedStatement> chk(conn.prepareStatement(
                    "SELECT * FROM CLASS_SCHEDULE WHERE classID = ? AND day = ? AND period = ?"));
                chk->setString(1, classID);
                chk->setInt(2, day);
                chk->setInt(3, period);
                std::unique_ptr<sql::ResultSet> resChk(chk->executeQuery());
                if (!resChk->next()) {
                    std::cout << "\033[33mClass schedule entry does not exist.\033[0m\n\n";
                    break;
                }

                if (!isValidSQLStringInput("New Teacher ID (leave blank to keep): ", newTeacherID))
                    continue;
                if (!isValidSQLStringInput("New Subject ID (leave blank to keep): ", newSubjectID))
                    continue;

                std::string cond = " WHERE classID = '" + std::string(classID.c_str()) + "' AND day = " + std::to_string(day) + " AND period = " + std::to_string(period);
                sql::SQLString condition(cond.c_str());

                if (!newTeacherID.empty())
                    update1attributeByTableAndID(conn, "CLASS_SCHEDULE", "teacherID", newTeacherID, condition);
                if (!newSubjectID.empty())
                    update1attributeByTableAndID(conn, "CLASS_SCHEDULE", "subjectID", newSubjectID, condition);

                std::cout << "\033[32mClass schedule updated.\033[0m\n\n";
                break;
            }

            case 13: // Display Class Schedule of A Class
            {
                sql::SQLString classID;
                if (!isValidSQLStringInput("ClassID to search: ", classID, false))
                    continue;
                displayClassSchedule(conn, classID);
                break;
            }
            default:
                std::cout << "\033[31mInvalid choice. Please try again.\033[0m\n\n";
                break;
            }
        }
        catch (sql::SQLException& e)
        {
            std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n";
        }
    }
}

void manageExamsSubjectsAndResults(sql::Connection& conn)
{
    while (true)
    {
        std::cout << "\n--- Manage Exams, Subjects and Results ---\n";
        std::cout << "Exam operations:\n";
        std::cout << "  1: Create Exam\n";
        std::cout << "  2: List Exams\n";
        std::cout << "  3: Update Exam\n";
        std::cout << "  4: Delete Exam\n";
        std::cout << "  5: Search Exam\n\n";
        std::cout << "Subject operations:\n";
        std::cout << "  6: Create Subject\n";
        std::cout << "  7: List Subjects\n";
        std::cout << "  8: Update Subject\n";
        std::cout << "  9: Delete Subject\n";
        std::cout << " 10: Search Subject\n\n";
        std::cout << "Exam Result operations:\n";
        std::cout << " 11: Create Exam Result\n";
        std::cout << " 12: List Exam Results\n";
        std::cout << " 13: Update Exam Result\n";
        std::cout << " 14: Delete Exam Result\n";
        std::cout << " 15: Search Results by Student\n";
        std::cout << "  0: Back\n";

        int choice = 0;
        if (!isValidPositiveInt("Your Choice: ", choice))
            continue;
        if (choice == 0)
        {
            system("cls");
            return;
        }

        try
        {
            switch (choice)
            {
                // --- EXAMS ---
            case 1: // Create Exam
            {
                sql::SQLString id, description;
                Date startDate, endDate;
                int year, month, day;
                if (!isValidSQLStringInput("New Exam ID: ", id, false))
                    continue;
                if (!isValidSQLStringInput("Exam Description: ", description, false))
                    continue;

                std::cout << "New Start Date:\n";
                if (!isValidPositiveInt("Year: ", year))
                    continue;
                startDate.setYear(year);
                if (!isValidPositiveInt("Month: ", month))
                    continue;
                startDate.setMonth(month);
                if (!isValidPositiveInt("Date: ", day))
                    continue;
                startDate.setDay(day);

                std::cout << "New End Date:\n";
                if (!isValidPositiveInt("Year: ", year))
                    continue;
                endDate.setYear(year);
                if (!isValidPositiveInt("Month: ", month))
                    continue;
                endDate.setMonth(month);
                if (!isValidPositiveInt("Date: ", day))
                    continue;
                endDate.setDay(day);

                std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement(
                    "INSERT INTO EXAM (examID, description, startDate, endDate) VALUES (?,?,?,?)"));
                pstmt->setString(1, id);
                pstmt->setString(2, description);
                pstmt->setString(3, startDate.toString());
                pstmt->setString(4, endDate.toString());
                pstmt->executeUpdate();
                std::cout << "\033[32mExam created.\033[0m\n";
                break;
            }

            case 2: // List Exams
            {
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement(
                    "SELECT examID, description, startDate, endDate FROM EXAM ORDER BY examID"));
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                std::cout << std::left
                    << std::setw(12) << "ExamID" << '|'
                    << std::setw(25) << "Description" << '|'
                    << std::setw(12) << "Start Date" << '|'
                    << std::setw(12) << "End Date"
                    << std::endl;
                while (res->next())
                {
                    std::cout << std::left
                        << std::setw(13) << res->getString("examID")
                        << std::setw(26) << res->getString("description")
                        << std::setw(13) << res->getString("startDate")
                        << std::setw(12) << res->getString("endDate")
                        << '\n';
                }
                divider();
                break;
            }

            case 3: // Update Exam
            {
                sql::SQLString id, description;
                Date startDate, endDate;
                int year, month, day;
                if (!isValidSQLStringInput("Exam ID to update: ", id, false))
                    continue;

                // check exists
                {
                    std::unique_ptr<sql::PreparedStatement> chk(conn.prepareStatement("SELECT * FROM EXAM WHERE examID = ?"));
                    chk->setString(1, id);
                    std::unique_ptr<sql::ResultSet> resChk(chk->executeQuery());
                    if (!resChk->next())
                    {
                        std::cout << "\033[33mExam does not exist.\033[0m\n\n";
                        break;
                    }
                }

                if (!isValidSQLStringInput("New Description (leave blank to keep): ", description))
                    continue;

                std::cout << "New Start Date (enter 0 for year to keep):\n";
                if (!isValidPositiveInt("Year: ", year))
                    continue;
                if (year != 0)
                {
                    startDate.setYear(year);
                    if (!isValidPositiveInt("Month: ", month))
                        continue;
                    startDate.setMonth(month);
                    if (!isValidPositiveInt("Date: ", day))
                        continue;
                    startDate.setDay(day);
                }

                std::cout << "New End Date (enter 0 for year to keep):\n";
                if (!isValidPositiveInt("Year: ", year))
                    continue;
                if (year != 0)
                {
                    endDate.setYear(year);
                    if (!isValidPositiveInt("Month: ", month))
                        continue;
                    endDate.setMonth(month);
                    if (!isValidPositiveInt("Date: ", day))
                        continue;
                    endDate.setDay(day);
                }

                // Build condition
                sql::SQLString condition = " WHERE examID = '" + id + "'";
                if (!description.empty())
                    update1attributeByTableAndID(conn, "EXAM", "description", description, condition);
                if (startDate.getYear() != 0)
                    update1attributeByTableAndID(conn, "EXAM", "startDate", startDate.toString(), condition);
                if (endDate.getYear() != 0)
                    update1attributeByTableAndID(conn, "EXAM", "endDate", endDate.toString(), condition);

                std::cout << "\033[32mExam updated.\033[0m\n";
                break;
            }

            case 4: // Delete Exam
            {
                sql::SQLString id;
                if (!isValidSQLStringInput("Exam ID to delete: ", id, false))
                    continue;
                deleteBasedOn1ID(conn, "Exam", "examID", id);
                break;
            }

            case 5: // Search Exam
            {
                sql::SQLString id;
                if (!isValidSQLStringInput("Exam ID to search: ", id, false))
                    continue;
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement(
                    "SELECT examID, description, startDate, endDate FROM EXAM WHERE examID = ?"));
                stmt->setString(1, id);
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                if (!res->next())
                {
                    std::cout << "\033[33mThe Exam " << id << " does not exist.\033[0m\n\n";
                    break;
                }
                else
                    res->beforeFirst();
                std::cout << std::left
                    << std::setw(12) << "ExamID" << '|'
                    << std::setw(32) << "Description" << '|'
                    << std::setw(12) << "StartDate" << '|'
                    << std::setw(12) << "EndDate"
                    << std::endl;
                while (res->next())
                {
                    std::cout << std::left
                        << std::setw(12) << res->getString("examID")
                        << std::setw(32) << res->getString("description")
                        << std::setw(12) << res->getString("startDate")
                        << std::setw(12) << res->getString("endDate")
                        << '\n';
                }
                divider();
                break;
            }

            // --- SUBJECTS ---
            case 6: // Create Subject
            {
                sql::SQLString id, name;
                if (!isValidSQLStringInput("New Subject ID: ", id, false))
                    continue;
                if (!isValidSQLStringInput("Subject Name: ", name, false))
                    continue;

                std::unique_ptr<sql::PreparedStatement> p(conn.prepareStatement(
                    "INSERT INTO SUBJECT (subjectID, subjectName) VALUES (?, ?)"));
                p->setString(1, id);
                p->setString(2, name);
                p->executeUpdate();
                std::cout << "\033[32mSubject created.\033[0m\n";

                break;
            }

            case 7: // List Subjects
            {
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement(
                    "SELECT subjectID, subjectName FROM SUBJECT ORDER BY subjectID"));
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                std::cout << std::left
                    << std::setw(12) << "SubjectID" << '|'
                    << std::setw(32) << "SubjectName"
                    << std::endl;
                while (res->next())
                {
                    std::cout << std::left
                        << std::setw(12) << res->getString("subjectID")
                        << std::setw(32) << res->getString("subjectName")
                        << '\n';
                }
                divider();
                break;
            }

            case 8: // Update Subject
            {
                sql::SQLString id, name;
                if (!isValidSQLStringInput("Subject ID to update: ", id, false))
                    continue;
                std::unique_ptr<sql::PreparedStatement> chk(conn.prepareStatement("SELECT * FROM SUBJECT WHERE subjectID = ?"));
                chk->setString(1, id);
                std::unique_ptr<sql::ResultSet> resChk(chk->executeQuery());
                if (!resChk->next())
                {
                    std::cout << "\033[33mSubject does not exist.\033[0m\n\n";
                    break;
                }
                if (!isValidSQLStringInput("New Subject Name (leave blank to keep): ", name))
                    continue;
                if (!name.empty())
                {
                    std::string condition = " WHERE subjectID = '" + std::string(id.c_str()) + "'";
                    update1attributeByTableAndID(conn, "SUBJECT", "subjectName", name, condition);
                }
                std::cout << "\033[32mSubject updated.\033[0m\n";
                break;
            }

            case 9: // Delete Subject
            {
                sql::SQLString id;
                if (!isValidSQLStringInput("Subject ID to delete: ", id, false))
                    continue;
                // Use helper to check and delete without using Subject class
                deleteBasedOn1ID(conn, "SUBJECT", "subjectID", id);
                break;
            }

            case 10: // Search Subject
            {
                sql::SQLString id;
                if (!isValidSQLStringInput("Subject ID to search: ", id, false))
                    continue;
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement(
                    "SELECT subjectID, subjectName FROM SUBJECT WHERE subjectID = ?"));
                stmt->setString(1, id);
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                std::cout << std::left
                    << std::setw(12) << "SubjectID" << '|'
                    << std::setw(32) << "SubjectName"
                    << std::endl;
                while (res->next())
                {
                    std::cout << std::left
                        << std::setw(12) << res->getString("subjectID")
                        << std::setw(32) << res->getString("subjectName")
                        << '\n';
                }
                divider();
                break;
            }

            // --- EXAM RESULTS ---
            case 11: // Create Exam Result
            {
                sql::SQLString studentID, examID, subjectID;
                double result;
                if (!isValidSQLStringInput("Student ID: ", studentID, false))
                    continue;
                if (!isValidSQLStringInput("Exam ID: ", examID, false))
                    continue;
                if (!isValidSQLStringInput("Subject ID: ", subjectID, false))
                    continue;
                if (!isValidDouble("Result(Marks): ", result))
                    continue;

                std::unique_ptr<sql::PreparedStatement> p(conn.prepareStatement(
                    "INSERT INTO EXAM_RESULT (studentID, examID, subjectID, result) VALUES (?,?,?,?)"));
                p->setString(1, studentID);
                p->setString(2, examID);
                p->setString(3, subjectID);
                p->setDouble(4, result);
                p->executeUpdate();
                std::cout << "\033[32mExam result created.\033[0m\n";

                break;
            }

            case 12: // List Exam Results
            {
                std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement(
                    "SELECT studentID, examID, subjectID, result FROM EXAM_RESULT ORDER BY examID, studentID"));
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                std::cout << std::left
                    << std::setw(12) << "StudentID" << '|'
                    << std::setw(12) << "ExamID" << '|'
                    << std::setw(12) << "SubjectID" << '|'
                    << std::setw(12) << "Result"
                    << std::endl;
                while (res->next())
                {
                    std::cout << std::left
                        << std::setw(12) << res->getString("studentID")
                        << std::setw(12) << res->getString("examID")
                        << std::setw(12) << res->getString("subjectID")
                        << std::setw(12) << res->getDouble("result")
                        << '\n';
                }
                divider();
                break;
            }

            case 13: // Update Exam Result
            {
                sql::SQLString studentID, examID, subjectID;
                double result;
                if (!isValidSQLStringInput("Student ID: ", studentID, false))
                    continue;
                if (!isValidSQLStringInput("Exam ID: ", examID, false))
                    continue;
                if (!isValidSQLStringInput("Subject ID: ", subjectID, false))
                    continue;
                std::unique_ptr<sql::PreparedStatement> chk(conn.prepareStatement(
                    "SELECT * FROM EXAM_RESULT WHERE studentID = ? AND examID = ? AND subjectID = ?"));
                chk->setString(1, studentID);
                chk->setString(2, examID);
                chk->setString(3, subjectID);
                std::unique_ptr<sql::ResultSet> resChk(chk->executeQuery());
                if (!resChk->next()) {
                    std::cout << "\033[33mExam result does not exist.\033[0m\n\n";
                    break;
                }
                if (!isValidDouble("New Result (Marks): ", result))
                    continue;
                std::unique_ptr<sql::PreparedStatement> p(conn.prepareStatement(
                    "UPDATE EXAM_RESULT SET result = ? WHERE studentID = ? AND examID = ? AND subjectID = ?"));
                p->setDouble(1, result);
                p->setString(2, studentID);
                p->setString(3, examID);
                p->setString(4, subjectID);

                break;
            }

            case 14: // Delete Exam Result
            {
                sql::SQLString studentID, examID, subjectID;
                if (!isValidSQLStringInput("Student ID: ", studentID, false))
                    continue;
                if (!isValidSQLStringInput("Exam ID: ", examID, false))
                    continue;
                if (!isValidSQLStringInput("Subject ID: ", subjectID, false))
                    continue;

                std::unique_ptr<sql::PreparedStatement> p(conn.prepareStatement(
                    "DELETE FROM EXAM_RESULT WHERE studentID = ? AND examID = ? AND subjectID = ?"));
                p->setString(1, studentID);
                p->setString(2, examID);
                p->setString(3, subjectID);
                int affected = p->executeUpdate();
                if (affected > 0)
                    std::cout << "\033[32mExam result deleted.\033[0m\n";
                else
                    std::cout << "\033[33mNo matching exam result found.\033[0m\n";
                break;
            }

            case 15: // Search Results by Student
            {
                sql::SQLString studentID;
                if (!isValidSQLStringInput("Student ID to search: ", studentID, false))
                    continue;

                displayExamResults(conn, studentID);
                break;
            }

            default:
                std::cout << "\033[31mInvalid choice. Please try again.\033[0m\n\n";
                break;
            }
        }
        catch (sql::SQLException& e)
        {
            std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n";
        }
    }
}

void studentAccess(sql::Connection& conn, const sql::SQLString& id, bool parent)
{
    bool logout = false;
    do {
        try
        {
            //reset result set
            std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement("SELECT * FROM STUDENT WHERE studentID = ?"));
            pstmt->setString(1, id);
            std::unique_ptr<sql::ResultSet> studRes(pstmt->executeQuery());
            studRes->next();
            sql::SQLString classID = studRes->getString("classID");

            //student menu
            std::cout << "Please choose an option:\n";
            std::cout << "1: Display Class Schedule\n";
            std::cout << "2: Search Exam Result\n";
            std::cout << "3: Display Recent Attendance\n";
            parent == false ? std::cout << "0: Logout\n" : std::cout << "0: Back\n";

            //user choice
            int choice = 0;
            if (!isValidPositiveInt("Your Choice: ", choice))
            {
                continue;
            }

            switch (choice)
            {
            case 1:
                displayClassSchedule(conn, classID);
                break;
            case 2:
                displayExamResults(conn, id);
                break;
            case 3:
                displayStudentAttendanceRecord(conn, id);
                break;
            case 0:
                system("cls");
                logout = true;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
        }
        catch (sql::SQLException& e)
        {
            std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n";
        }
    } while (logout == false);
}

void parentAccess(sql::Connection& conn, const sql::SQLString& id)
{
    do
    {
        try
        {
            std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement(
                "SELECT s.name,studentID FROM CHILD_LIST cs LEFT OUTER JOIN STUDENT s USING (studentID) WHERE cs.parentID = ?"));
            pstmt->setString(1, id);
            std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

            int choice = 0;
            if (!res->next())
            {
                std::cout << "\033[31mYou don't have any child in our school.\033[0m\n";
                break;
            }
            else
            {
                std::cout << "Please select a child by number(0 to Logout):\n";
                for (int i = 1; i <= res->rowsCount(); i++)
                {
                    std::cout << i << " : ";
                    std::cout << std::setw(7) << res->getString("studentID");
                    std::cout << std::setw(32) << res->getString("name") << '\n';
                    res->next();
                }
                if (!isValidPositiveInt("Enter your choice: ", choice))
                {
                    continue;
                }
                if (choice == 0)
                {
                    system("cls");
                    break;
                }
                else
                {
                    res->absolute(choice);
                    studentAccess(conn, res->getString("studentID"), true);
                }
            }
        }
        catch (sql::SQLException e)
        {
            std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n";
        }
    } while (true);

}

void teacherAccess(sql::Connection& conn, const sql::SQLString& id)
{
    bool logout = false;
    do
    {
        try
        {
            //teacher menu
            std::cout << "Please choose an option:\n";
            std::cout << "1: Manage Students and Parents\n";
            std::cout << "2: Manage Classes and Attendance\n";
            std::cout << "3: Manage Exams, Subjects and Exam Result\n";
            std::cout << "4: Analysis and Report\n";
            std::cout << "0: Logout\n";

            //user choice
            int choice = 0;
            if (!isValidPositiveInt("Choice: ", choice))
            {
                continue;
            }
            switch (choice)
            {
            case 1:
                manageStudentsAndParents(conn);
                break;
            case 2:
                manageClassesAndAttendance(conn);
                break;
            case 3:
                manageExamsSubjectsAndResults(conn);
                break;
            case 4:
                analysisAndReport(conn);
                break;
            case 0:
                system("cls");
                logout = true;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
        }
        catch (sql::SQLException& e)
        {
            std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n";
        }
    } while (logout == false);
}

void adminAccess(sql::Connection& conn, const sql::SQLString& id)
{
    bool logout = false;
    do
    {
        try
        {
            //admin menu
            std::cout << "Please choose an option:\n";
            std::cout << "1: Manage Admins\n";
            std::cout << "2: Manage Teachers\n";
            std::cout << "3: Manage Students and Parents\n";
            std::cout << "4: Manage Classes and Attendance\n";
            std::cout << "5: Manage Exams, Subjects and Exam Result\n";
            std::cout << "6: Analysis and Report\n";
            std::cout << "0: Logout\n";

            //user choice
            int choice = 0;
            if (!isValidPositiveInt("Choice: ", choice))
            {
                continue;
            }
            switch (choice)
            {
            case 1:
                manageAdmins(conn);
                break;
            case 2:
                manageTeachers(conn, id);
                break;
            case 3:
                manageStudentsAndParents(conn);
                break;
            case 4:
                manageClassesAndAttendance(conn);
                break;
            case 5:
                manageExamsSubjectsAndResults(conn);
                break;
            case 6:
                analysisAndReport(conn);
                break;
            case 0:
                system("cls");
                logout = true;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
        }
        catch (sql::SQLException& e)
        {
            std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n";
        }
    } while (logout == false);
}

void analysisAndReport(sql::Connection& conn)
{
    int year = 0;
    if (!isValidPositiveInt("Please enter a year for annual report: ", year))
    {
        return;
    }

    try
    {
        std::unique_ptr<sql::PreparedStatement> stmt(conn.prepareStatement(
            "SELECT er.examID, er.subjectID, s.subjectName, AVG(er.result) AS avg_result "
            "FROM EXAM_RESULT er "
            "LEFT OUTER JOIN SUBJECT s ON er.subjectID = s.subjectID "
            "WHERE er.examID IN (SELECT examID FROM EXAM WHERE YEAR(startDate) = ?) "
            "GROUP BY er.examID, er.subjectID, s.subjectName "
            "ORDER BY er.examID, s.subjectName"));
        stmt->setInt(1, year);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        if (!res->next())
        {
            std::cout << "\033[33mNo exam results found for the year " << year << ".\033[0m\n\n";
            return;
        }

        // Collect data: map of examID to vector of {subjectName, averages}
        std::map<sql::SQLString, std::vector<std::pair<sql::SQLString, double>>> examData;
        sql::SQLString currentExamID = res->getString("examID");
        double maxAverage = 0;

        do
        {
            sql::SQLString examID = res->getString("examID");
            sql::SQLString subjectName = res->getString("subjectName");
            double avgResult = res->getDouble("avg_result");

            examData[examID].push_back({ subjectName, avgResult });

            if (avgResult > maxAverage)
                maxAverage = avgResult;
        } while (res->next());

        // Display header
        std::cout << "\n=== Annual Report - Exam Results for " << year << " ===\n\n";
        std::cout << std::string(80, '=') << '\n';

        // Display each exam with its subjects
        for (const auto& examEntry : examData)
        {
            std::cout << "\nExam: " << examEntry.first << '\n';
            std::cout << std::string(80, '-') << '\n';

            for (const auto& subjectEntry : examEntry.second)
            {
                const sql::SQLString& subjectName = subjectEntry.first;
                double avgResult = subjectEntry.second;

                // Calculate bar length (max 50 characters)
                int barLength = static_cast<int>((avgResult / maxAverage) * 50);

                // Format output
                std::cout << std::left << std::setw(20) << examEntry.first << " ";
                std::cout << std::left << std::setw(25) << subjectName << " | ";

                // Draw bar
                for (int j = 0; j < barLength; ++j)
                {
                    std::cout << "\xDB";
                }

                // Display value
                std::cout << " " << std::fixed << std::setprecision(1)
                    << "\033[34m ("
                    << std::fixed << std::setprecision(1)
                    << avgResult << "%)\033[0m\n";
            }
        }

        std::cout << std::string(80, '=') << '\n';
        std::cout << "Max Average: " << std::fixed << std::setprecision(2)
            << maxAverage << "\n\n";

        // --- Attendance rate for the year ---
        try {
            std::unique_ptr<sql::PreparedStatement> attPresentStmt(conn.prepareStatement(
                "SELECT COUNT(*) AS present_count FROM ATTENDANCE WHERE status = 1 AND YEAR(`date`) = ?"));
            attPresentStmt->setInt(1, year);
            std::unique_ptr<sql::ResultSet> attResPresent(attPresentStmt->executeQuery());
            int presentCount = 0;
            if (attResPresent->next())
                presentCount = attResPresent->getInt("present_count");

            std::unique_ptr<sql::PreparedStatement> attTotalStmt(conn.prepareStatement(
                "SELECT COUNT(*) AS total_count FROM ATTENDANCE WHERE YEAR(`date`) = ?"));
            attTotalStmt->setInt(1, year);
            std::unique_ptr<sql::ResultSet> attResTotal(attTotalStmt->executeQuery());
            int totalCount = 0;
            if (attResTotal->next())
                totalCount = attResTotal->getInt("total_count");

            std::cout << "\n=== Attendance Summary for " << year << " ===\n";
            if (totalCount == 0)
            {
                std::cout << "No attendance records for the year " << year << ".\n\n";
            }
            else
            {
                double attendanceRate = (static_cast<double>(presentCount) / static_cast<double>(totalCount)) * 100.0;
                std::cout << "Present: " << presentCount << " / " << totalCount << "\033[34m  (" << std::fixed << std::setprecision(2) << attendanceRate << "% )\033[0m\n\n";
            }
        }
        catch (sql::SQLException&)
        {
            std::cerr << "\033[31mUnable to compute attendance summary.\033[0m\n";
        }

        divider();
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n";
    }
}

// Login Page
void loginPage(sql::Connection& conn)
{
    sql::SQLString table, idType, id, password; //table name and id column name
    int roleChoice = 0;
    do //loop until successful login or exit
    {
        //init
        id = password = table = idType = "";
        roleChoice = 0;
        std::cout << "Please select your role:\n";
        std::cout << "1: Admin\n";
        std::cout << "2: Teacher\n";
        std::cout << "3: Student\n";
        std::cout << "4: Parent\n";
        std::cout << "0: Exit\n";

        if (!isValidPositiveInt("Your Choice: ", roleChoice))
            continue;

        switch (roleChoice) {
        case 1:
            table = "ADMIN";
            idType = "adminID";
            break;
        case 2:
            table = "TEACHER";
            idType = "teacherID";
            break;
        case 3:
            table = "STUDENT";
            idType = "studentID";
            break;
        case 4:
            table = "PARENT";
            idType = "parentID";
            break;
        case 0:
            std::cout << "Exiting the system." << std::endl;
            return;
        default:
            std::cout << "\033[31mInvalid choice. Please try again.\033[0m" << std::endl;
            continue;
        }


        // Prompt for ID and Password
        std::cout << "\nPlease enter your ID and password.(Both zeros to exit)" << std::endl;
        if (!isValidSQLStringInput("ID: ", id, false) || !isValidSQLStringInput("Password: ", password, false))
        {
            std::cout << "\033[31mInvalid input. Please try again.\033[0m\n";
            continue;
        }


        try {
            std::unique_ptr<sql::PreparedStatement> stmnt(
                conn.prepareStatement("SELECT password,name FROM " + table + " WHERE " + idType + " = ? "));
            stmnt->setString(1, id);
            std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery());
            if (res->next())
            {
                if (password == res->getString("password"))
                {
                    system("cls");
                    std::cout << "\033[32mLogin successful! \033[0mWelcome, ";
                    switch (roleChoice)
                    {
                    case 1:
                        // Admin functionalities
                        std::cout << "Admin " << res->getString("name") << std::endl;
                        adminAccess(conn, id);
                        break;
                    case 2:
                        std::cout << "Teacher " << res->getString("name") << std::endl;
                        teacherAccess(conn, id);
                        break;
                    case 3:
                        std::cout << "Student " << res->getString("name") << std::endl;
                        studentAccess(conn, id);
                        break;
                    case 4:
                        std::cout << "Parent " << res->getString("name") << std::endl;
                        parentAccess(conn, id);
                        break;
                    default:
                        break;
                    }

                }
                else
                {
                    std::cout << "\033[31mInvalid ID or password. Please try again.\033[0m\n\n";
                }
            }
            else
            {
                std::cout << "\033[31mInvalid ID or password. Please try again.\033[0m\n\n";
            }

        }
        catch (sql::SQLException& e) {
            std::cerr << "\033[31mDatabase error. Please try again.\033[0m\n";
        }
    } while (true);
}
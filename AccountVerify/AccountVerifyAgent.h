#pragma once
#ifndef ACCOUNT_VERIFY_AGENT_H
#define ACCOUNT_VERIFY_AGENT_H
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "mysql_connection.h"

#define DB_LOCATION "localhost:3306/365palm"
#define DB_USER_NAME "ryan"
#define DB_PASSWORD "sfhere303"

using namespace std;
using namespace sql;

class AccountVerifyAgent
{
public:
	bool InitConnection();
	void ResetAccountTable();
	void EnsureAccountTable();
	bool VerifyAcc(string account, string pwd, bool isAutoCreate);
	AccountVerifyAgent() : con(), stmt(), pstmt() {}

private:
	Connection *con;
	Statement *stmt;
	PreparedStatement *pstmt;
};

bool AccountVerifyAgent::InitConnection()
{
	Driver *driver;
	try
	{
		driver = get_driver_instance();
		con = driver->connect(DB_LOCATION, DB_USER_NAME, DB_PASSWORD);
	}
	catch (SQLException e)
	{
		cout << "Could not connect to database : " << DB_LOCATION << "user : " << DB_USER_NAME << "pw : " << DB_PASSWORD
			<< "ErrorMsg : " << e.what() << endl;
		return false;
	}
	return true;
}

void AccountVerifyAgent::ResetAccountTable()
{
	stmt = con->createStatement();
	stmt->execute("DROP TABLE IF EXISTS Account");
	cout << "Finished dropping table (if existed)" << endl;
	stmt->execute("CREATE TABLE Account (id VARCHAR(20) PRIMARY KEY, password VARCHAR(40));");
	cout << "Finished creating table" << endl;
	delete stmt;

	return;
}

void AccountVerifyAgent::EnsureAccountTable()
{
	stmt = con->createStatement();
	stmt->execute("CREATE TABLE IF NOT EXISTS Account (id VARCHAR(20) PRIMARY KEY, password VARCHAR(40));");
	delete stmt;

	return;
}

bool AccountVerifyAgent::VerifyAcc(string account, string pwd, bool isAutoCreate)
{
	ResultSet *result;

	pstmt = con->prepareStatement("SELECT * FROM Account where id = ?;");
	pstmt->setString(1, account.c_str());
	result = pstmt->executeQuery();
	if (result->next())
	{
		int retCode = result->getString(2).compare(pwd.c_str());
		if (retCode == 0)
		{
			delete result;
			delete pstmt;
			return true;
		}
	}
	else if (isAutoCreate)
	{
		pstmt = con->prepareStatement("INSERT INTO Account(id, password) VALUES(?,?)");
		pstmt->setString(1, account.c_str());
		pstmt->setString(2, pwd.c_str());
		pstmt->execute();
		cout << "Account Not Found, AutoCreate...." << endl;

		delete result;
		delete pstmt;
		return true;
	}

	cout << "Password is invalid" << endl;

	delete result;
	delete pstmt;
	return false;
}
#endif // !SQLConnection

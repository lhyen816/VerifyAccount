// ConsoleApplication1.cpp: 定義主控台應用程式的進入點。
//

#include <stdlib.h>
#include <iostream>
#include <string>
#include "stdafx.h"
#include "mysql_connection.h"
#include "AccountVerifyAgent.h"

using namespace std;

int main()
{
	AccountVerifyAgent *pDB365palm = new AccountVerifyAgent();
	if (!pDB365palm->InitConnection())
	{
		system("pause");
		exit(1);
	}
	pDB365palm->EnsureAccountTable();

	string account;
	string password;

	cout << "Enter Account:" << endl;
	cin >> account;
	cout << "Enter Password:" << endl;
	cin >> password;
	if (pDB365palm->VerifyAcc(account, password, true))
	{
		cout << "Login Success" << endl;
	}
	system("pause");

	delete pDB365palm;
	return 0;
}
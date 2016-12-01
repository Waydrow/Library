#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include "Library.h"
using namespace std;

bool isUserLogin;
bool isAdminLogin;

int main() {
	Library myLib = Library();
	while (true) {
		string loginOption;
		cout << "身份选择: 1.管理员 2.学生" << endl<<"请输入: ";
		cin >> loginOption;
		if (loginOption == "1") {
			isAdminLogin = myLib.adminLogin();
			if (isAdminLogin) {
				break;
			}
		} else if (loginOption == "2") {
			isUserLogin = myLib.userLogin();
			if (isUserLogin) {
				break;
			}
		} else {
			continue;
		}
	}

	if (isAdminLogin) {
		while (true) {
			cout << "请选择操作" << endl;
			cout << "--------------------------------" << endl;
			cout << "1-----新书入库" << endl;
			cout << "2-----查看图书库存" << endl;
			cout << "3-----删除旧书" << endl;
			cout << "4-----修改图书信息" << endl;
			cout << "5-----查询图书" << endl;
			cout << "6-----添加用户" << endl;
			cout << "7-----修改用户" << endl;
			cout << "8-----删除用户" << endl;
			cout << "9-----查看所有用户" << endl;
			cout << "10----查询用户" << endl;
			cout << "11----查询用户借阅记录" << endl;
			cout << "0-----退出" << endl;
			cout << "--------------------------------" << endl;

			string adminOption;
			cin >> adminOption;
			if (adminOption == "1") {
				myLib.addBook();
			} else if (adminOption == "2") {
				myLib.displayBook();
			} else if (adminOption == "3") {
				myLib.removeBook();
			} else if (adminOption == "4") {
				myLib.changeBook();
			} else if (adminOption == "5") {
				myLib.userSearchBook();
			} else if (adminOption == "6") {
				myLib.addUser();
			} else if (adminOption == "7") {
				myLib.changeUser();
			} else if (adminOption == "8") {
				myLib.removeUser();
			} else if (adminOption == "9") {
				myLib.displayUser();
			} else if (adminOption == "10") {
				//myLib.userSearchBook();
			} else if (adminOption == "11") {
				myLib.displayOneUserBorrowHistory();
			} else if (adminOption == "0") {
				exit(0);
			} else {
				continue;
			}
		}
	}

	if (isUserLogin) {
		while (true) {
			cout << "请选择操作" << endl;
			cout << "--------------------------------" << endl;
			cout << "1----查看库存" << endl;
			cout << "2----查询图书" << endl;
			cout << "3----借阅" << endl;
			cout << "4----归还" << endl;
			cout << "5----续借" << endl;
			cout << "6----查询借阅记录" << endl;
			cout << "0----退出" << endl;
			cout << "--------------------------------" << endl;

			string userOption;
			cin >> userOption;

			/*
			此处不可用switch case来判断, 因为c++中不支持case string
			option不用int或char的原因为防止用户恶意输入, 进行容错处理
			*/
			if (userOption == "1") {
				myLib.displayBook();
			} else if (userOption == "2") {
				myLib.userSearchBook();
			} else if (userOption == "3") {
				myLib.lendBook();
			} else if (userOption == "4") {
				myLib.backBook();
			} else if (userOption == "5") {
				myLib.borrowAgain();
			} else if (userOption == "6") {
				myLib.displayOneUserBorrowHistory();
			} else if (userOption == "0") {
				exit(0);
			} else {
				continue;
			}
		}
	}
	return 0;
}

#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include "Library.h"
using namespace std;

bool isUserLogin;
int isAdminLogin;

int main() {

	int flag = 1;
	while (flag) {
		flag = 1;
		isUserLogin = false;
		isAdminLogin = 0;
		Library myLib = Library();
		while (true) {
			string loginOption;
			cout << "身份选择: 1.管理员 2.学生 3.退出" << endl << "请输入: ";
			Tools::inputChoice(loginOption);
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
			} else if (loginOption == "3") {
				flag = 0;
				break;
			} else {
				continue;
			}
		}

		// 为超级管理员
		if (isAdminLogin == 2) {
			while (true) {
				cout << "请选择操作" << endl;
				cout << "------------------------------------------------------------------------------------" << endl;
				cout << setiosflags(ios::left);
				cout << setw(30) << "1-----添加用户" << setw(30) << "7 -----新书入库" << setw(30) << "12----添加管理员" << endl;
				cout << setw(30) << "2-----修改用户信息" << setw(30) << "8 -----修改图书信息" << setw(30) << "13----修改管理员信息" << endl;
				cout << setw(30) << "3-----删除用户" << setw(30) << "9 -----删除旧书" << setw(30) << "14----删除管理员" << endl;
				cout << setw(30) << "4-----查看所有用户" << setw(30) << "10-----查看图书库存" << setw(30) << "15----查看所有管理员" << endl;
				cout << setw(30) << "5-----查询用户" << setw(30) << "11-----查询图书" << setw(30) << "16---读入并生成测试样例" << endl;
				cout << setw(30) << "6-----查询用户借阅记录" << setw(30) << "0 -----注销" << endl;
				cout << "------------------------------------------------------------------------------------" << endl;

				string adminOption;
				Tools::inputChoice(adminOption);
				if (adminOption == "1") {
					myLib.addUser();
					// myLib.addBook();
				} else if (adminOption == "2") {
					myLib.changeUser();
				} else if (adminOption == "3") {
					myLib.removeUser();
				} else if (adminOption == "4") {
					myLib.displayUser();
				} else if (adminOption == "5") {
					myLib.searchUser();
				} else if (adminOption == "6") {
					myLib.displayOneUserBorrowHistory();
				} else if (adminOption == "7") {
					myLib.addBook();
				} else if (adminOption == "8") {
					myLib.changeBook();
				} else if (adminOption == "9") {
					myLib.removeBook();
				} else if (adminOption == "10") {
					myLib.displayBook();
				} else if (adminOption == "11") {
					myLib.userSearchBook();
				} else if (adminOption == "12") {
					myLib.addAdmin();
				} else if (adminOption == "13") {
					myLib.changeAdmin();
				} else if (adminOption == "14") {
					myLib.removeAdmin();
				} else if (adminOption == "15") {
					myLib.displayAdmin();
				} else if (adminOption == "16") {
					myLib.generateData();
				} else if (adminOption == "0") {
					break;
				} else {
					continue;
				}
			}
		}

		// 为普通管理员
		if (isAdminLogin == 1) {
			while (true) {
				cout << "请选择操作" << endl;
				cout << "------------------------------------------------------------------------------------" << endl;
				cout << setiosflags(ios::left);
				cout << setw(30) << "1-----添加用户" << setw(30) << "7 -----新书入库" << endl;
				cout << setw(30) << "2-----修改用户信息" << setw(30) << "8 -----修改图书信息" << endl;
				cout << setw(30) << "3-----删除用户" << setw(30) << "9 -----删除旧书" << endl;
				cout << setw(30) << "4-----查看所有用户" << setw(30) << "10-----查看图书库存" << endl;
				cout << setw(30) << "5-----查询用户" << setw(30) << "11-----查询图书" << endl;
				cout << setw(30) << "6-----查询用户借阅记录" << setw(30) << "0 -----注销" << endl;
				cout << "------------------------------------------------------------------------------------" << endl;

				string adminOption;
				Tools::inputChoice(adminOption);
				if (adminOption == "1") {
					myLib.addUser();
				} else if (adminOption == "2") {
					myLib.displayUser();
				} else if (adminOption == "3") {
					myLib.removeUser();
				} else if (adminOption == "4") {
					myLib.displayUser();
				} else if (adminOption == "5") {
					myLib.searchUser();
				} else if (adminOption == "6") {
					myLib.displayOneUserBorrowHistory();
				} else if (adminOption == "7") {
					myLib.addBook();
				} else if (adminOption == "8") {
					myLib.changeBook();
				} else if (adminOption == "9") {
					myLib.removeBook();
				} else if (adminOption == "10") {
					myLib.displayBook();
				} else if (adminOption == "11") {
					myLib.userSearchBook();
				} else if (adminOption == "0") {
					break;
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
				cout << "0----注销" << endl;
				cout << "--------------------------------" << endl;

				string userOption;
				Tools::inputChoice(userOption);

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
					break;
				} else {
					continue;
				}
			}
		}
	}
	return 0;
}

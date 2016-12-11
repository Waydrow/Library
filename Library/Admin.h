#include <iostream>
#include <string.h>
using namespace std;

#define USER_ACCOUNT_SIZE		20
#define USER_PASSWORD_SIZE		20

class Admin {
private:
	int id;
	char account[USER_ACCOUNT_SIZE];
	char password[USER_PASSWORD_SIZE];

public:
	Admin(int id, char account[], char password[]) {
		this->id = id;
		strcpy(this->account, account);
		strcpy(this->password, password);
	}

	Admin(int id, string account, string password) {
		this->id = id;
		strcpy(this->account, account.c_str());
		strcpy(this->password, password.c_str());
	}

	Admin(char account[], char password[]) {
		strcpy(this->account, account);
		strcpy(this->password, password);
	}

	Admin(int id) {
		this->id = id;
		strcpy(this->account, "");
		strcpy(this->password, "");
	}

	Admin() {

	}

	void print() {
		cout << "-------------------------------" << endl;
		cout << "管理员信息如下：" << endl;
		cout << "编号: " << getId() << endl;
		cout << "账号: " << getAccount() << endl;
		cout << "密码: " << "xxxxxxxx" << endl;
		cout << "权限: ";
		if (id == 1) { // 为超级管理员
			cout << "超级管理员" << endl;
		} else {
			cout << "普通管理员" << endl;
		}
		cout << "-------------------------------" << endl;
	}

	void setId(int id) {
		this->id = id;
	}

	void setAccount(char account[]) {
		strcpy(this->account, account);
	}

	void setPassword(char account[]) {
		strcpy(this->password, password);
	}

	int getId() {
		return id;
	}

	char * getAccount() {
		return account;
	}

	char * getPassword() {
		return password;
	}
};

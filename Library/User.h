#include <iostream>
#include <string>
#include <string.h>
using namespace std;

#define USER_NAME_SIZE			20
#define USER_ACCOUNT_SIZE		20
#define USER_PASSWORD_SIZE		20

class User {
private:
	int id;
	char account[USER_ACCOUNT_SIZE];
	char password[USER_PASSWORD_SIZE];
	char name[USER_NAME_SIZE];

public:
	User(int id, char account[], char password[], char name[]) {
		this->id = id;
		strcpy(this->account, account);
		strcpy(this->password, password);
		strcpy(this->name, name);
	}

	User(int id, string account, string password, string name) {
		this->id = id;
		strcpy(this->account, account.c_str());
		strcpy(this->password, password.c_str());
		strcpy(this->name, name.c_str());
	}

	User(int id) {
		this->id = id;
		strcpy(this->account, "");
		strcpy(this->password, "");
		strcpy(this->name, "");
	}

	User() {

	}

	void print() {
		cout << "-------------------------------" << endl;
		cout << "用户信息如下：" << endl;
		cout << "编号: " << id << endl;
		cout << "学号: " << account << endl;
		cout << "密码: " << "xxxxxxxx" << endl;
		cout << "姓名: " << name << endl;
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

	void setName(char name[]) {
		strcpy(this->name, name);
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

	char * getName() {
		return name;
	}
};

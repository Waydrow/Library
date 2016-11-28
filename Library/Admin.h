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

	Admin(char account[], char password[]) {
		strcpy(this->account, account);
		strcpy(this->password, password);
	}

	Admin() {

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

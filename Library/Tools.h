#include <iostream>
#include <stdio.h>
#include <ctime>
#include <conio.h>
using namespace std;

struct tm *local_time = NULL;

class Tools {

public:
	// 时间的格式化输出
	static void printTime(time_t timer) {
		local_time = localtime(&timer);
		printf("%d-%d-%d ", local_time->tm_year + 1900,
			local_time->tm_mon + 1, local_time->tm_mday);
		if (local_time->tm_hour < 10) {
			printf("0");
		}
		printf("%d:", local_time->tm_hour);
		if (local_time->tm_min < 10) {
			printf("0");
		}
		printf("%d:", local_time->tm_min);
		if (local_time->tm_sec < 10) {
			printf("0");
		}
		printf("%d\n", local_time->tm_sec);
	}

	// 判断一个字符串是否全为数字
	static bool isNumber(string str) {
		for (int i = 0; i < str.size(); i++) {
			if (!isdigit(str[i])) {
				return false;
			}
		}
		return true;
	}

	// 输入帐号, 只可以输入数字和字母
	static bool inputAccount(char* account, int size) {
		char c;
		int count = 0;
		while (((c = getch()) != '\r') && count < size - 1) {
			if (c == ' ') {
				continue;
			}
			if (c == 8) { // 退格
				if (count == 0) {
					continue;
				}
				putchar('\b');
				putchar(' ');
				putchar('\b');
				count--;
			}
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
				putchar(c);
				account[count] = c;
				count++;
			}

		}
		account[count] = '\0';
		cout << endl;
		if (count >= USER_ACCOUNT_SIZE - 1) {
			cout << "帐号太长啦 !" << endl;
			return false;
		}
		return true;
	}

	// 输入密码, 隐藏字符, 打印*
	static bool inputPassword(char* password, int size) {
		char c;
		int count = 0;
		while (((c = getch()) != '\r') && count < size - 1) {
			if (c == ' ') {
				continue;
			}
			if (c == 8) { // 退格
				if (count == 0) {
					continue;
				}
				putchar('\b');
				putchar(' ');
				putchar('\b');
				count--;
			}
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
				putchar('*');
				password[count] = c;
				count++;
			}
		}
		password[count] = '\0';
		cout << endl;
		if (count >= USER_PASSWORD_SIZE - 1) {
			cout << "密码太长啦 !" << endl;
			return false;
		}
		return true;
	}

	// 输入选择操作符, 最多输入两位, 且只能为数字
	static void inputChoice(string &str) {
		char c;
		char temp[3];
		int count = 0;
		while ((c = getch()) != '\r') {
			if (c == 8) {
				if (count == 0) {
					continue;
				}
				putchar('\b');
				putchar(' ');
				putchar('\b');
				count--;
			}
			if (count == 2) {
				continue;
			}
			if (c == ' ') {
				continue;
			}
			if (c >= '0' && c <= '9') {
				putchar(c);
				temp[count] = c;
				count++;
			}
		}
		temp[count] = '\0';
		str = temp;
		cout << endl;
	}

	static void writeTestBooks() {
		ofstream ofs;
		ofs.open("data/book_all.txt");
		if (ofs.is_open()) {
			string name = "高级编程";
			string author = "测试作者";
			string introduction = "一本高级编程的书";
			for (int i = 1000; i < 11000; i++) {
				ofs << name << i << " " << author << " " << introduction << i << "\n";
			}
		}
		ofs << flush;
		ofs.close();
	}

	static void writeTestUsers() {
		ofstream ofs;
		ofs.open("data/user_all.txt");
		if (ofs.is_open()) {
			long long account = 14020031144;
			int password = 222;
			string name = "学生";
			for (int i = 0; i < 100; i++) {
				ofs << account + i << " " << password << " " << name << i << "\n";
			}
		}
		ofs << flush;
		ofs.close();
	}

};
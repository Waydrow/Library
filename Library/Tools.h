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

	// 输入图书编号, 最多9位数字, 只能为数字
	static void inputBookNum(int &num) {
		char c;
		char temp[10];
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
			if (count == 9) {
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
		string str = temp;
		str = trim(str);
		if (str.empty()) {
			num = -1;
		} else {
			num = atoi(temp);
		}
		cout << endl;
	}

	// 输入图书名字
	static bool inputBookName(string &name, int size) {
		while (true) {
			cout << "请输入书名(输入0返回): ";
			getline(cin, name);
			name = trim(name);
			if (name.empty()) {
				continue;
			}
			if (name == "0") {
				return false;
			}
			if (name.size() >= size) {
				cout << "书名太长啦 !" << endl;
				continue;
			}
			break;
		}
		return true;
	}

	// 输入图书作者
	static bool inputBookAuthor(string &author, int size) {
		while (true) {
			cout << "请输入作者(输入0返回): ";
			getline(cin, author);
			author = trim(author);
			if (author.empty()) {
				continue;
			}
			if (author == "0") {
				return false;
			}
			if (author.size() >= size) {
				cout << "作者名字太长啦 !" << endl;
				continue;
			}
			break;
		}
		return true;
	}

	// 输入图书简介
	static bool inputBookIntro(string &intro, int size) {
		while (true) {
			cout << "请输入简介(输入0返回): ";
			getline(cin, intro);
			intro = trim(intro);
			if (intro.empty()) {
				continue;
			}
			if (intro == "0") {
				return false;
			}
			if (intro.size() >= size) {
				cout << "简介太长啦 !" << endl;
				continue;
			}
			break;
		}
		return true;
	}

	// 学生学号, 只可输入数字
	static void inputStuNum(string &str, int size) {
		char c;
		char *temp = new char[size];
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
			if (count == size - 1) {
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
		delete[] temp;
		cout << endl;
	}

	// 输入帐号, 只可以输入数字和字母
	static void inputAccount(string &str, int size) {
		char c;
		int count = 0;
		char *account = new char[size];
		while ((c = getch()) != '\r') {
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
			if (count == size - 1) {
				continue;
			}
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
				putchar(c);
				account[count] = c;
				count++;
			}

		}
		account[count] = '\0';
		str = account;
		delete[] account;
		cout << endl;
	}

	// 输入密码, 隐藏字符, 打印*
	static void inputPassword(string &str, int size) {
		char c;
		int count = 0;
		char *password = new char[size];
		while ((c = getch()) != '\r') {

			if (c == 8) { // 退格
				if (count == 0) {
					continue;
				}
				putchar('\b');
				putchar(' ');
				putchar('\b');
				count--;
			}
			if (count == size - 1) {
				continue;
			}
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
				putchar('*');
				password[count] = c;
				count++;
			}
		}
		password[count] = '\0';
		str = password;
		delete[] password;
		cout << endl;
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

	// 输入一位确认符, 只可输入Y,y,N,n, 0返回
	static void inputConfirmYN(string &str) {
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
			if (count == 1) {
				continue;
			}
			if (c == ' ') {
				continue;
			}
			if (c == 'y' || c == 'Y' || c == 'n' || c == 'N') {
				putchar(c);
				temp[count] = c;
				count++;
			}
		}
		temp[count] = '\0';
		str = temp;
		cout << endl;
	}

	// 实现 string 的trim函数, 删除前后空格
	static string trim(string s) {
		if (s.empty())
			return s;
		s.erase(0, s.find_first_not_of(" \t"));
		s.erase(s.find_last_not_of(" \t") + 1);
		return s;
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
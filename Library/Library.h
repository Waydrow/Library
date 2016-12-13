#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <limits>
#include <ctime>
#include <iomanip>
#include <conio.h>
#include "User.h"
#include "Admin.h"
#include "Tools.h"
using namespace std;

#define BOOK_FILE				"data/books"
#define USER_FILE				"data/user"
#define ADMIN_FILE				"data/admin"
#define BORROW_FILE				"data/borrow"

// 图书, 用户, 管理员大小
#define BOOK_SIZE				sizeof(Book)
#define USER_SIZE				sizeof(User)
#define ADMIN_SIZE				sizeof(Admin)

#define BORROW_SIZE				sizeof(Borrow)

// 书名, 作者, 简介大小
#define BOOK_NAME_SIZE			20
#define BOOK_AUTHOR_SIZE		20
#define BOOK_INTRO_SIZE			50
// 书的可借与不可借状态
#define BOOK_CAN_BORROW			1
#define BOOK_CANNOT_BORROW		0

// 用户和管理员
#define USER_NAME_SIZE			20
#define USER_ACCOUNT_SIZE		20
#define USER_PASSWORD_SIZE		20

// 用户登录, 1表示成功, 0表示失败
#define IS_USER_LOGIN			1
#define IS_USER_NOT_LOGIN		0

// 管理员登录, 1表示成功, 0表示失败
#define IS_ADMIN_LOGIN          1
#define IS_ADMIN_NOT_LOGIN      0

// 超级管理员登录, 1表示成功, 0表示失败
#define IS_SUPER_LOGIN          1
#define IS_SUPER_NOT_LOGIN      0

// 删除后ID置为-1
#define ID_REMOVE				-1

// 借阅文件中, 记录是否已归还或续借或超期
#define IS_BACK					1
#define IS_NOT_BACK				0
#define IS_BORROW_AGAIN			2
#define IS_OVER_TIME			3

// 续借次数上限
#define BORROW_AGAIN_LIMITS		1

// 查询用户时无法对象无法返回NULL, 取其id代替
#define ID_NOT_FOUND			-999

// 一次的借书期限 20s, 续借后+20s, 变为40s
#define TIME_LIMITS				20

const int book_size[3] = { BOOK_NAME_SIZE, BOOK_AUTHOR_SIZE, BOOK_INTRO_SIZE };

struct Book {
	// 自增, 唯一标识
	int id;
	char name[BOOK_NAME_SIZE]; //书名
	char author[BOOK_AUTHOR_SIZE]; //作者
	char introduction[BOOK_INTRO_SIZE]; //简介
	int canBorrow; // 0表示不可借, 1表示可借

	Book() {}
	//只有编号的初始化
	Book(int id) {
		this->id = id;
		strcpy(this->name, "");
		strcpy(this->author, "");
		strcpy(this->introduction, "");
		this->canBorrow = BOOK_CAN_BORROW;
	}
	//完整初始化
	Book(int id, char name[BOOK_NAME_SIZE],
		char author[BOOK_AUTHOR_SIZE], char introduction[BOOK_INTRO_SIZE]) {

		this->id = id;
		strcpy(this->name, name);
		strcpy(this->author, author);
		strcpy(this->introduction, introduction);
		this->canBorrow = BOOK_CAN_BORROW;
	}

	// string 参数的constructor
	Book(int id, string name, string author, string introduction) {
		this->id = id;
		strcpy(this->name, name.c_str());
		strcpy(this->author, author.c_str());
		strcpy(this->introduction, introduction.c_str());
		this->canBorrow = BOOK_CAN_BORROW;
	}
	//显示信息
	void print() {
		cout << "-------------------------------" << endl;
		cout << "这本书的信息如下：" << endl;
		cout << "编号: " << id << endl;
		cout << "书名: " << name << endl;
		cout << "作者: " << author << endl;
		cout << "简介: " << introduction << endl;
		if (canBorrow == BOOK_CANNOT_BORROW) {
			cout << "暂不可借 !" << endl;
		} else {
			cout << "可借" << endl;
		}
		cout << "-------------------------------" << endl;
	}

	//重载关系运算符
	bool operator==(const Book &b) const {
		if (this->id == b.id) {
			return true;//编号等即命中
		} else {
			return false;
		}
	}
	bool operator<(const Book &b) const {
		if (this->id < b.id) {
			return true;
		} else {
			return false;
		}
	}
	bool operator>(const Book &b) const {
		if (this->id > b.id) {
			return true;
		} else {
			return false;
		}
	}
};

// 借阅类, 存储用户学号和书的id
struct Borrow {
	int id;
	char userAccount[USER_ACCOUNT_SIZE];
	int bookId;
	// isBack表示是否归还, 0未归, 1归还, 2续借, 3超期
	int isBack;
	// 借书时间
	time_t borrowTime;
	// 理应归还时间
	time_t backTheoryTime;
	// 实际归还时间
	time_t backActualTime;
	// 续借次数
	int borrowAgainTimes;

	Borrow() {}
	Borrow(int id) {
		this->id = id;
		strcpy(this->userAccount, "");
		this->bookId = 0;
	}
	Borrow(int id, char account[], int bookId) {
		this->id = id;
		strcpy(this->userAccount, account);
		this->bookId = bookId;
		this->borrowTime = time(NULL); // 获取借书时间
		this->backTheoryTime = this->borrowTime + TIME_LIMITS;
		this->isBack = IS_NOT_BACK;
		this->borrowAgainTimes = 0;
	}

	void print() {
		cout << "-------------------------------" << endl;
		cout << "学号: " << userAccount << endl;
		cout << "书号: " << bookId << endl;
		cout << "借书时间: ";
		Tools::printTime(borrowTime);
		if (isBack != IS_BACK) {
			cout << "应还日期: ";
			Tools::printTime(backTheoryTime);
		}
		cout << "续借次数: " << borrowAgainTimes << endl;
		cout << "状态: ";
		switch (isBack) {
			case IS_BACK:
				cout << "已归还" << endl;
				cout << "归还时间: ";
				Tools::printTime(backActualTime);
				break;
			case IS_NOT_BACK:
				cout << "未归还" << endl;
				break;
			case IS_BORROW_AGAIN:
				cout << "已续借" << endl;
				break;
			case IS_OVER_TIME:
				cout << "已超期" << endl;
				break;
		}
		cout << "-------------------------------" << endl;
	}
};

ifstream inFile;
ofstream outFile;

class Library {
private:
	/*
		 图书文件中存储的第一本书无实际含义, 其id仅为存储bookTotal的值, 从第二本开始从1计数
		 bookTotal为图书文件中下一本新书的 id, 如果当前文件存储了n本书,
		 则下本书的编号应为 bookTotal = n + 1
	*/
	int bookTotal;
	int userTotal;
	int borrowTotal;
	int adminTotal;
	// 判断是否用户登录
	int isUserLogin;
	// 判断是否普通管理员登录
	int isAdminLogin;
	// 判断是否为超级管理员登录
	int isSuperLogin;

	User currentUser;
	Admin currentAdmin;
	// 输入输出流


	// 记录下改变后的bookTotal值, 即将bookTotal写入图书文件中第一条记录的id中
	void writeBookTotal() {
		outFile.open(BOOK_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			outFile.seekp(0, ios::beg);
			Book aBook = Book(bookTotal);
			outFile.write((char*)&aBook, BOOK_SIZE);
			// cout << "修改bookTotal成功" << endl;
		} else {
			// cout << "修改bookTotal失败" << endl;
		}
		outFile.close();
	}

	// 记录下改变后的userTotal值, 即将userTotal写入图书文件中第一条记录的id中
	void writeUserTotal() {
		outFile.open(USER_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			outFile.seekp(0, ios::beg);
			User aUser = User(userTotal);
			outFile.write((char*)&aUser, USER_SIZE);
			// cout << "修改userTotal成功" << endl;
		} else {
			// cout << "修改userTotal失败" << endl;
		}
		outFile.close();
	}

	// 记录下改变后的borrowTotal值, 即将borrowTotal写入借阅文件中第一条记录的id中
	void writeBorrowTotal() {
		outFile.open(BORROW_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			outFile.seekp(0, ios::beg);
			Borrow aBorrow = Borrow(borrowTotal);
			outFile.write((char*)&aBorrow, BORROW_SIZE);
			// cout << "修改borrowTotal成功" << endl;
		} else {
			// cout << "修改borrowTotal失败" << endl;
		}
		outFile.close();
	}

	// 记录下改变后的adminTotal值, 即将dminTotal写入管理员文件中第一条记录的id中
	void writeAdminTotal() {
		outFile.open(ADMIN_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			outFile.seekp(0, ios::beg);
			Admin aAdmin = Admin(adminTotal);
			outFile.write((char*)&aAdmin, ADMIN_SIZE);
			cout << "修改adminTotal成功" << endl;
		} else {
			cout << "修改adminTotal失败" << endl;
		}
		outFile.close();
	}

	// 系统进行图书信息初始化
	void loadSystemBooks() {
		fstream iofile;
		iofile.open(BOOK_FILE, ios::in | ios::out | ios::binary);
		if (iofile.fail())   // 打开失败则创建新文件, 并写入bookTotal
		{
			// cout << "创建新的图书文件成功!" << endl;
			iofile.close();
			iofile.open(BOOK_FILE, ios::out | ios::binary);
			Book aBook = Book(1);
			bookTotal = 1;
			iofile.write((char*)&aBook, BOOK_SIZE);
			iofile.close();
		} else {
			// cout << "成功打开原有的图书文件!" << endl;
			// 移动到文件开头位置
			iofile.seekg(0, ios::beg);
			Book aBook;
			iofile.read((char*)&aBook, BOOK_SIZE);
			// 获取第一本书的id, 赋值给bookTotal
			bookTotal = aBook.id;
			iofile.close();
		}
		//cout << "系统初始化图书信息成功 !" << endl;
	}

	// 系统进行用户信息初始化
	void loadSystemUsers() {
		fstream iofile;
		iofile.open(USER_FILE, ios::in | ios::out | ios::binary);
		if (iofile.fail())   // 打开失败则创建新文件, 并写入userTotal
		{
			// cout << "创建新的用户文件成功!" << endl;
			iofile.close();
			iofile.open(USER_FILE, ios::out | ios::binary);
			User aUser = User(1);
			userTotal = 1;
			iofile.write((char*)&aUser, USER_SIZE);
			iofile.close();
		} else {
			// cout << "成功打开原有的用户文件!" << endl;
			// 移动到文件开头位置
			iofile.seekg(0, ios::beg);
			User aUser;
			iofile.read((char*)&aUser, USER_SIZE);
			// 获取第一个用户的id, 赋值给userTotal
			userTotal = aUser.getId();
			iofile.close();
		}
		//cout << "系统初始化用户信息成功 !" << endl;
	}

	// 系统进行借阅信息初始化
	void loadSystemBorrows() {
		fstream iofile;
		iofile.open(BORROW_FILE, ios::in | ios::out | ios::binary);
		if (iofile.fail())   // 打开失败则创建新文件, 并写入borrowTotal
		{
			// cout << "创建新的借阅文件成功!" << endl;
			iofile.close();
			iofile.open(BORROW_FILE, ios::out | ios::binary);
			Borrow aBorrow = Borrow(1);
			borrowTotal = 1;
			iofile.write((char*)&aBorrow, BORROW_SIZE);
			iofile.close();
		} else {
			// cout << "成功打开原有的借阅文件!" << endl;
			// 移动到文件开头位置
			iofile.seekg(0, ios::beg);
			Borrow aBorrow;
			iofile.read((char*)&aBorrow, BORROW_SIZE);
			// 获取第一个借阅的id, 赋值给borrowTotal
			borrowTotal = aBorrow.id;
			iofile.close();
		}
		//cout << "系统初始化借阅信息成功 !" << endl;
	}

	// 系统进行管理员信息初始化
	void loadSystemAdmins() {
		fstream iofile;
		iofile.open(ADMIN_FILE, ios::in | ios::out | ios::binary);
		if (iofile.fail()) {
			iofile.close();
			iofile.open(ADMIN_FILE, ios::out | ios::binary);
			// 注意此处是先添加一个当前最大id 再添加一个超级管理员
			Admin aAdmin = Admin(2);
			Admin superAdmin = Admin(1, "222", "222");
			adminTotal = 2;
			iofile.write((char*)&aAdmin, ADMIN_SIZE);
			iofile.write((char*)&superAdmin, ADMIN_SIZE);
			iofile.close();
		} else {
			// 移动到文件开头位置
			iofile.seekp(0, ios::beg);
			Admin aAdmin;
			iofile.read((char*)&aAdmin, ADMIN_SIZE);
			// 获取第一个管理员的id, 赋值给adminTotal
			adminTotal = aAdmin.getId();
			iofile.close();
		}
		//cout << "系统初始化管理员信息成功 !" << endl;
	}

	//写入图书文件
	void writeBookFile(Book aBook, int location) {
		outFile.open(BOOK_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			// 写入到location位置, 即将文件指针从0移动 location 个 book_size
			outFile.seekp(location*BOOK_SIZE, ios::beg);
			outFile.write((char*)&aBook, BOOK_SIZE);
		}
		outFile.close();
	}

	//写入用户文件
	void writeUserFile(User aUser, int location) {
		outFile.open(USER_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			// 写入到location位置, 即将文件指针从0移动 location 个 user_size
			outFile.seekp(location*USER_SIZE, ios::beg);
			outFile.write((char*)&aUser, USER_SIZE);
		}
		outFile.close();
	}

	// 写入借阅文件
	void writeBorrowFile(Borrow aBorrow, int location) {
		outFile.open(BORROW_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			// 写入到location位置, 即将文件指针从0移动 location 个 borrow_size
			outFile.seekp(location*BORROW_SIZE, ios::beg);
			outFile.write((char*)&aBorrow, BORROW_SIZE);
		}
		outFile.close();
	}

	// 写入管理员
	void writeAdminFile(Admin aAdmin, int location) {
		outFile.open(ADMIN_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			// 写入到location位置, 即将文件指针从0移动 location 个 admin_size
			outFile.seekp(location*ADMIN_SIZE, ios::beg);
			outFile.write((char*)&aAdmin, ADMIN_SIZE);
		}
		outFile.close();
	}

	// 查询某一个用户的借阅记录
	vector<Borrow> queryBorrowHistoryOneUser(string account) {
		inFile.open(BORROW_FILE, ios::binary);
		vector<Borrow> vec;
		if (inFile.is_open()) {
			Borrow aBorrow;
			// 从第二条记录开始查询
			inFile.seekg(BORROW_SIZE, ios::beg);
			while (inFile.read((char*)&aBorrow, BORROW_SIZE)) {
				string tempAccount(aBorrow.userAccount);
				if (tempAccount == account) {
					vec.push_back(aBorrow);
				}
			}
		}
		inFile.close();
		return vec;
	}

	// 查询特定用户特定图书的借书记录
	Borrow queryBorrowByUserAndBook(string account, int bookId) {
		inFile.open(BORROW_FILE, ios::in | ios::out | ios::binary);
		int flag = 0;
		Borrow aBorrow;
		if (inFile.is_open()) {
			inFile.seekg(BORROW_SIZE, ios::beg);
			while (inFile.read((char*)&aBorrow, BORROW_SIZE)) {
				string tempAccount(aBorrow.userAccount);
				if (tempAccount == account && aBorrow.bookId == bookId) {
					flag = 1;
					break;
				}
			}
		}
		inFile.close();
		if (flag == 0) {
			aBorrow.id = ID_NOT_FOUND;
		}
		return aBorrow;
	}

	//以图书id为依据进行查找图书信息
	Book searchBookById(int num) {
		int flag = 0; // 0代表没查到, 1代表查到
		inFile.open(BOOK_FILE, ios::binary);
		Book aBook;
		if (inFile.is_open()) {
			// 从第二条记录开始搜索
			inFile.seekg(BOOK_SIZE, ios::beg);
			while (inFile.read((char*)&aBook, BOOK_SIZE)) {
				if (num == aBook.id && aBook.id != ID_REMOVE) {
					flag = 1;
					break;
				}
			}
		}
		inFile.close();
		if (flag == 0) {
			aBook.id = ID_NOT_FOUND;
		}
		return aBook;
	}

	// 以用户id为依据进行查找用户信息
	User searchUserById(int num) {
		int flag = 0;
		User aUser;
		inFile.open(USER_FILE, ios::binary);
		if (inFile.is_open()) {
			inFile.seekg(USER_SIZE, ios::beg);
			while (inFile.read((char*)&aUser, USER_SIZE)) {
				int id = aUser.getId();
				if (id == num && aUser.getId() != ID_REMOVE) {
					flag = 1;
					break;
				}
			}
		}
		inFile.close();
		if (flag == 0) {
			aUser.setId(ID_NOT_FOUND);
		}
		return aUser;
	}

	// 以用户学号为依据进行查找用户信息
	User searchUserByAccount(string _account) {
		int flag = 0;
		User aUser;
		inFile.open(USER_FILE, ios::binary);
		if (inFile.is_open()) {
			inFile.seekg(USER_SIZE, ios::beg);
			while (inFile.read((char*)&aUser, USER_SIZE)) {
				string account(aUser.getAccount());
				if (_account == account && aUser.getId() != ID_REMOVE) {
					flag = 1;
					break;
				}
			}
		}
		inFile.close();
		if (flag == 0) {
			aUser.setId(ID_NOT_FOUND);
		}
		return aUser;
	}

	// 以管理员账号为依据进行查找管理员信息
	Admin searchAdminByAccount(string _account) {
		int flag = 0;
		Admin aAdmin;
		inFile.open(ADMIN_FILE, ios::binary);
		if (inFile.is_open()) {
			inFile.seekg(ADMIN_SIZE, ios::beg);
			while (inFile.read((char*)&aAdmin, ADMIN_SIZE)) {
				string account(aAdmin.getAccount());
				if (_account == account && aAdmin.getId() != ID_REMOVE) {
					flag = 1;
					break;
				}
			}
		}
		inFile.close();
		if (flag == 0) {
			aAdmin.setId(ID_NOT_FOUND);
		}
		return aAdmin;
	}

	// 通过书名查询图书
	vector<Book> searchBookByName(string name) {
		inFile.open(BOOK_FILE, ios::binary);
		vector<Book> result;
		if (inFile.is_open()) {
			Book aBook;
			inFile.seekg(BOOK_SIZE, ios::beg);
			while (inFile.read((char*)&aBook, BOOK_SIZE)) {
				if (strstr(aBook.name, name.c_str())) {
					result.push_back(aBook);
				}
			}
		}
		inFile.close();
		return result;
	}


	// 用户登录确认
	void searchUserFileForLogin(string account, string password) {
		inFile.open(USER_FILE, ios::binary);
		if (inFile.is_open()) {
			User user = User();
			while (inFile.read((char*)&user, USER_SIZE)) {
				string _account(user.getAccount());
				string _password(user.getPassword());
				if (account == _account && password == _password) {
					isUserLogin = IS_USER_LOGIN;
					currentUser = user;
					cout << "用户登录成功" << endl;
					break;
				}
			}
		}
		inFile.close();
		if (isUserLogin == IS_USER_NOT_LOGIN) {
			cout << "账号或密码错误" << endl;
		}
	}

	// 管理员登录确认
	void searchAdminFileForLogin(string account, string password) {
		inFile.open(ADMIN_FILE, ios::binary);
		if (inFile.is_open()) {
			Admin admin = Admin();
			while (inFile.read((char*)&admin, ADMIN_SIZE)) {
				string _account(admin.getAccount());
				string _password(admin.getPassword());
				// 判断是否为超级管理员
				if (account == "222") {
					if (password == _password) {
						isSuperLogin = IS_SUPER_LOGIN;
						currentAdmin = admin;
						cout << "管理员登录成功" << endl;
						break;
					}
				}
				// 如果是普通管理员
				else {
					if (account == _account && password == _password) {
						isAdminLogin = IS_ADMIN_LOGIN;
						currentAdmin = admin;
						cout << "管理员登录成功" << endl;
						break;
					}
				}
			}
		}
		inFile.close();
		if (isAdminLogin == IS_ADMIN_NOT_LOGIN && isSuperLogin == IS_SUPER_NOT_LOGIN) {
			cout << "账号或密码错误" << endl;
		}
	}

	// 用户登录后查询超期图书, 并更新图书状态
	vector<Borrow> searchOverTimeBorrowBooks(string account) {
		fstream ioFile;
		ioFile.open(BORROW_FILE, ios::in | ios::out | ios::binary);
		vector<Borrow> vec;
		if (ioFile.is_open()) {
			Borrow aBorrow;
			while (ioFile.read((char*)&aBorrow, BORROW_SIZE)) {
				string tempAccount(aBorrow.userAccount);
				if (account == tempAccount) {
					// 获取当前时间, 和理应归还日期进行比对
					time_t nowTime = time(NULL);
					if (aBorrow.isBack != IS_BACK && nowTime >= aBorrow.backTheoryTime) {
						aBorrow.isBack = IS_OVER_TIME;
						vec.push_back(aBorrow);
						ioFile.seekp(aBorrow.id * BORROW_SIZE, ios::beg);
						ioFile.write((char*)&aBorrow, BORROW_SIZE);
						ioFile.seekg((aBorrow.id + 1)*BORROW_SIZE, ios::beg);
					}
				}
			}
		}
		ioFile.close();
		return vec;
	}

public:
	Library() {
		loadSystemBooks();
		loadSystemUsers();
		loadSystemBorrows();
		loadSystemAdmins();
		this->isUserLogin = IS_USER_NOT_LOGIN;
		this->isSuperLogin = IS_SUPER_NOT_LOGIN;
		this->isAdminLogin = IS_ADMIN_NOT_LOGIN;
		system("cls");
		cout << setw(60) << "*******************************************" << endl;
		cout << setw(60) << "*                                         *" << endl;
		cout << setw(60) << "*                                         *" << endl;
		cout << setw(60) << "*            图书管理系统V2.0             *" << endl;
		cout << setw(60) << "*                第4小组                  *" << endl;
		cout << setw(60) << "*                                         *" << endl;
		cout << setw(60) << "*                                         *" << endl;
		cout << setw(60) << "*******************************************" << endl;
	}

	//增加图书
	void addBook() {
		cout << "请输入图书信息(书名(20位) 作者(20位) 简介(50位))" << endl;
		string name, author, introduction;
		if (!Tools::inputBookName(name, BOOK_NAME_SIZE)) {
			return;
		}
		if (!Tools::inputBookAuthor(author, BOOK_AUTHOR_SIZE)) {
			return;
		}
		if (!Tools::inputBookIntro(introduction, BOOK_INTRO_SIZE)) {
			return;
		}

		Book new_book = Book(bookTotal, name, author, introduction);
		writeBookFile(new_book, bookTotal);

		cout << "添加成功，信息如下：" << endl;
		new_book.print();
		bookTotal++;
		writeBookTotal();
	}

	//查看图书库存
	void displayBook() {
		cout << "这是现在的库存信息：" << endl;
		inFile.open(BOOK_FILE, ios::in | ios::out | ios::binary);
		if (inFile.is_open()) {
			Book aBook;
			while (inFile.read((char*)&aBook, BOOK_SIZE)) {
				if (aBook.id != ID_REMOVE) {
					aBook.print();//显示这本书的信息
				}
			}
		}
		inFile.close();
	}

	//删除图书
	void removeBook() {
		Book old_book;
		int num;
		while (true) {
			cout << "请输入要删除的图书编号(输入0返回)：";
			Tools::inputBookNum(num);
			if (num == -1) {
				continue;
			}
			if (num == 0) {
				return;
			}
			old_book = searchBookById(num);//通过编号找到这本书的记录
			if (old_book.id == ID_NOT_FOUND) {
				cout << "未找到对应的书, 请重试 !" << endl;
				continue;
			}
			if (old_book.canBorrow == BOOK_CANNOT_BORROW) {
				cout << "这本书被借走了, 暂时不能删除哦 !" << endl;
				continue;
			}
			break;
		}
		cout << "您即将删除这本书的所有信息：" << endl;
		old_book.print();
		string choice;
		while (true) {
			cout << "确认删除这本书吗?(y/n): ";
			Tools::inputConfirmYN(choice);
			if (choice.empty()) {
				continue;
			}
			break;
		}
		if (choice == "y" || choice == "Y") {
			Book aBook = Book(ID_REMOVE, "\0", "\0", "\0");
			writeBookFile(aBook, old_book.id);
			cout << "删除成功 !" << endl;
		}
	}

	//借出图书
	void lendBook() {
		int num;
		Book old_book;
		while (true) {
			cout << "请输入要借的图书编号(输入0返回)：";
			Tools::inputBookNum(num);
			if (num == -1) {
				continue;
			}
			if (num == 0) {
				return;
			}
			old_book = searchBookById(num);//通过编号找到这本书的记录
			if (old_book.id == ID_NOT_FOUND) {
				cout << "未找到对应的书, 请重试 !" << endl;
				continue;
			}
			if (old_book.canBorrow == BOOK_CANNOT_BORROW) {
				cout << "这本书已经被别人借走啦 !" << endl;
				continue;
			}
			break;
		}
		string flag;
		old_book.print();
		while (true) {
			cout << "确认借这本书吗?(y/n): ";
			Tools::inputConfirmYN(flag);
			if (flag.empty()) {
				continue;
			}
			break;
		}
		if (flag == "y" || flag == "Y") {
			old_book.canBorrow = BOOK_CANNOT_BORROW; // 图书状态标为借出
			writeBookFile(old_book, old_book.id); // 写入到图书文件
			Borrow aBorrow = Borrow(borrowTotal, currentUser.getAccount(), old_book.id);
			writeBorrowFile(aBorrow, borrowTotal); // 写入到借阅文件
			borrowTotal++;
			writeBorrowTotal();
			cout << "借书成功 !" << endl;
		}
	}

	// 查看借阅记录
	void displayOneUserBorrowHistory() {
		vector<Borrow> borrows;
		string account;
		User old_user;
		// 先判断是否为管理员登录, 是则先输入学号
		if (!isUserLogin) {
			while (true) {
				cout << "请输入要查找的用户学号(输入0返回): ";
				Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
				if (account.empty()) {
					continue;
				}
				if (account == "0") {
					return;
				}
				old_user = searchUserByAccount(account);
				if (old_user.getId() == ID_NOT_FOUND) {
					cout << "未找到对应的用户, 请重试 !" << endl;
					continue;
				}
				break;
			}
		} else {
			account = currentUser.getAccount();
		}

		borrows = queryBorrowHistoryOneUser(account);
		if (borrows.empty()) {
			cout << "还没有借过书哦 !" << endl;
			return;
		} else {
			cout << "共查询到 " << borrows.size() << " 条借阅记录" << endl;
			vector<Borrow>::iterator it;
			for (it = borrows.begin(); it != borrows.end(); it++) {
				(*it).print();
			}
		}
	}

	//修改图书
	void changeBook() {
		Book old_book;
		int num;
		while (true) {
			cout << "请输入要修改的图书编号(输入0返回)：";
			Tools::inputBookNum(num);
			if (num == -1) {
				continue;
			}
			if (num == 0) {
				return;
			}
			old_book = searchBookById(num);//通过编号找到这本书的记录
			if (old_book.id == ID_NOT_FOUND) {
				cout << "未找到对应的书, 请重试 !" << endl;
				continue;
			}
			if (old_book.canBorrow == BOOK_CANNOT_BORROW) {
				cout << "这本书被借走了, 暂时不能修改哦 !" << endl;
				continue;
			}
			break;
		}
		cout << "这是这本书的当前信息：" << endl;
		//显示这本书之前的信息
		old_book.print();
		while (true) {
			cout << "请输入要进行的操作: 1.修改书名 2.修改作者 3.修改简介 0.返回" << endl;
			string choice;
			Tools::inputChoice(choice);
			if (choice == "1") {
				string name;
				if (!Tools::inputBookName(name, BOOK_NAME_SIZE)) {
					continue;
				}
				Book new_book = Book(old_book.id, name, old_book.author, old_book.introduction);
				// 写入文件
				writeBookFile(new_book, old_book.id);
				cout << "书名修改成功" << endl;
				new_book.print();
				old_book = new_book;
			} else if (choice == "2") {
				string author;
				if (!Tools::inputBookAuthor(author, BOOK_AUTHOR_SIZE)) {
					continue;
				}
				Book new_book = Book(old_book.id, old_book.name, author, old_book.introduction);
				// 写入文件
				writeBookFile(new_book, old_book.id);
				cout << "作者修改成功" << endl;
				new_book.print();
				old_book = new_book;
			} else if (choice == "3") {
				string intro;
				if (!Tools::inputBookIntro(intro, BOOK_INTRO_SIZE)) {
					continue;
				}
				Book new_book = Book(old_book.id, old_book.name, old_book.author, intro);
				// 写入文件
				writeBookFile(new_book, old_book.id);
				cout << "简介修改成功" << endl;
				new_book.print();
				old_book = new_book;
			} else if (choice == "0") {
				return;
			} else {
				continue;
			}
		}
	}

	//归还图书
	void backBook() {
		int num;
		Book old_book;
		Borrow aBorrow;
		while (true) {
			cout << "请输入要归还的图书编号(输入0返回)：";
			Tools::inputBookNum(num);
			if (num == -1) {
				continue;
			}
			if (num == 0) {
				return;
			}
			old_book = searchBookById(num);//通过编号找到这本书的记录
			if (old_book.id == ID_NOT_FOUND) {
				cout << "未找到对应的书, 请重试 !" << endl;
				continue;
			}
			string account = currentUser.getAccount();
			aBorrow = queryBorrowByUserAndBook(account, num);
			if (old_book.canBorrow == BOOK_CAN_BORROW || aBorrow.id == ID_NOT_FOUND) {
				cout << "你还没有借过这本书哦 !" << endl;
				continue;
			}
			break;
		}

		string flag;
		while (true) {
			cout << "确认归还这本书吗?(y/n): ";
			Tools::inputConfirmYN(flag);
			if (flag.empty()) {
				continue;
			}
			break;
		}
		if (flag == "y" || flag == "Y") {
			old_book.canBorrow = BOOK_CAN_BORROW;
			writeBookFile(old_book, old_book.id);
			// 还书标志
			aBorrow.isBack = IS_BACK;
			// 记录下还书时间
			aBorrow.backActualTime = time(NULL);
			writeBorrowFile(aBorrow, aBorrow.id);
			cout << "还书成功 !" << endl;
		}
	}

	// 续借图书
	void borrowAgain() {
		int num;
		Book old_book;
		Borrow aBorrow;
		while (true) {
			cout << "请输入要续借的图书编号(输入0返回)：";
			Tools::inputBookNum(num);
			if (num == -1) {
				continue;
			}
			if (num == 0) {
				return;
			}
			old_book = searchBookById(num);//通过编号找到这本书的记录
			if (old_book.id == ID_NOT_FOUND) {
				cout << "未找到对应的书, 请重试 !" << endl;
				continue;
			}
			string account = currentUser.getAccount();
			aBorrow = queryBorrowByUserAndBook(account, num);
			if (old_book.canBorrow == BOOK_CAN_BORROW || aBorrow.id == ID_NOT_FOUND) {
				cout << "你还没有借过这本书哦 !" << endl;
				continue;
			}
			if (aBorrow.borrowAgainTimes == BORROW_AGAIN_LIMITS) {
				cout << "每本书最多只可续借一次 !" << endl;
				continue;
			}
			break;
		}

		string flag;
		while (true) {
			cout << "确认续借这本书吗?(y/n): ";
			Tools::inputConfirmYN(flag);
			if (flag.empty()) {
				continue;
			}
			break;
		}
		if (flag == "y" || flag == "Y") {
			// 续借标志
			aBorrow.isBack = IS_BORROW_AGAIN;
			// 续借次数++
			aBorrow.borrowAgainTimes++;
			// 更新理应还书时间
			aBorrow.backTheoryTime += TIME_LIMITS;
			writeBorrowFile(aBorrow, aBorrow.id);
			cout << "续借成功 !" << endl;
		}
	}

	// 用户查询图书
	void userSearchBook() {
		string nameInput;
		vector<Book> result;
		while (true) {
			cout << "请输入要查询的图书名字(输入0返回): ";
			getline(cin, nameInput);
			nameInput = Tools::trim(nameInput);
			if (nameInput.empty()) {
				continue;
			}
			if (nameInput == "0") {
				return;
			}
			if (nameInput.size() >= BOOK_NAME_SIZE) {
				cout << "您输入的图书名太长了, 请重试 !" << endl;
				continue;
			}
			result = searchBookByName(nameInput);
			if (result.empty()) {
				cout << "没找到对应的书" << endl;
				continue;
			}
			break;
		}

		cout << "共搜索到符合的条件的 " << result.size() << " 本书" << endl;
		for (vector<int>::size_type ix = 0; ix != result.size(); ix++) {
			result[ix].print();
		}
	}


	// 管理员登录
	int adminLogin() {
		string account;
		while (true) {
			cout << "请输入管理员帐号(输入0返回): " << endl;
			Tools::inputAccount(account, USER_ACCOUNT_SIZE);
			if (account.empty()) {
				continue;
			}
			break;
		}

		if (account == "0") {
			return false;
		}

		string password;
		while (true) {
			cout << "请输入管理员密码(输入0返回): " << endl;
			Tools::inputPassword(password, USER_PASSWORD_SIZE);
			if (password.empty()) {
				continue;
			}
			break;
		}
		if (password == "0") {
			return false;
		}

		searchAdminFileForLogin(account, password);
		if (isAdminLogin == IS_ADMIN_LOGIN) {
			return 1;
		}
		if (isSuperLogin == IS_SUPER_LOGIN) {
			return 2;
		} else {
			return 0;
		}
	}

	// 用户登录
	bool userLogin() {
		string account;
		while (true) {
			cout << "请输入学号(输入0返回): " << endl;
			Tools::inputStuNum(account, USER_PASSWORD_SIZE);
			if (account.empty()) {
				continue;
			}
			break;
		}
		if (account == "0") {
			return false;
		}

		string password;
		while (true) {
			cout << "请输入用户密码(输入0返回): " << endl;
			Tools::inputPassword(password, USER_PASSWORD_SIZE);
			if (password.empty()) {
				continue;
			}
			break;
		}
		if (password == "0") {
			return false;
		}

		searchUserFileForLogin(account, password);
		if (isUserLogin == IS_USER_LOGIN) {
			// 查询超期图书
			vector<Borrow> vec = searchOverTimeBorrowBooks(account);
			if (!vec.empty()) {
				cout << "你有 " << vec.size() << " 本图书已超期, 请即时归还或续借 !" << endl;
				for (vector<Borrow>::size_type ix = 0; ix != vec.size(); ix++) {
					vec[ix].print();
				}
			}
			return true;
		} else {
			return false;
		}
	}

	//添加用户
	void addUser() {
		cout << "请输入用户信息(学号, 密码, 姓名)" << endl;
		string account, password;
		string name;
		User new_user;
		while (true) {
			cout << "请输入学号(输入0返回): ";
			Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
			if (account.empty()) {
				continue;
			}
			new_user = searchUserByAccount(account);
			if (new_user.getId() != ID_NOT_FOUND) {
				cout << "学号已存在 !" << endl;
				continue;
			}
			if (account == "0") {
				return;
			}
			break;
		}
		while (true) {
			cout << "请输入密码(不少于3位, 输入0返回): ";
			Tools::inputPassword(password, USER_PASSWORD_SIZE);

			if (password == "0") {
				return;
			}
			if (password.size() < 3) {
				cout << "密码最少3位哦 !" << endl;
				continue;
			}

			string confirm_password;
			cout << "请确认密码(不少于3位, 输入0返回): ";
			Tools::inputPassword(confirm_password, USER_PASSWORD_SIZE);

			if (confirm_password == "0") {
				return;
			}
			if (confirm_password.size() < 3) {
				cout << "密码最少3位哦 !" << endl;
				continue;
			}
			if (password != confirm_password) {
				cout << "两次输入的密码不一致 !" << endl;
				continue;
			}
			break;
		}


		while (true) {
			cout << "请输入姓名(输入0返回): ";
			getline(cin, name);
			name = Tools::trim(name);
			if (name.empty()) {
				continue;
			}
			if (name == "0") {
				return;
			}
			if (name.size() >= USER_NAME_SIZE) {
				cout << "名字太长啦" << endl;
				continue;
			}
			break;
		}

		new_user = User(userTotal, account, password, name);
		writeUserFile(new_user, userTotal);
		cout << "添加成功，信息如下：" << endl;
		new_user.print();
		userTotal++;
		writeUserTotal();
	}

	//查看所有用户
	void displayUser() {
		cout << "这是现在的用户信息：" << endl;
		inFile.open(USER_FILE, ios::in | ios::binary);
		if (inFile.is_open()) {
			User aUser;
			while (inFile.read((char*)&aUser, USER_SIZE)) {
				if (aUser.getId() != ID_REMOVE) {
					aUser.print();//显示这个用户的信息
				}
			}
		}
		inFile.close();
	}

	//删除用户
	void removeUser() {
		string account;
		User old_user;
		while (true) {
			cout << "请输入要删除的用户学号(输入0返回): ";
			Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
			if (account.empty()) {
				continue;
			}
			if (account == "0") {
				return;
			}
			old_user = searchUserByAccount(account);
			if (old_user.getId() == ID_NOT_FOUND) {
				cout << "未找到对应的用户, 请重试 !" << endl;
				continue;
			}
			break;
		}
		cout << "您即将删除这个用户的所有信息：" << endl;
		old_user.print();

		string choice;
		while (true) {
			cout << "确认删除这个用户吗?(y/n): ";
			Tools::inputConfirmYN(choice);
			if (choice.empty()) {
				continue;
			}
			break;
		}
		if (choice == "y" || choice == "Y") {
			User aUser = User(ID_REMOVE);
			writeUserFile(aUser, old_user.getId());
			cout << "删除成功 !" << endl;
		}
	}

	//修改用户
	void changeUser() {
		string account;
		User old_user;
		while (true) {
			cout << "请输入要修改的用户学号(输入0返回): ";
			Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
			if (account.empty()) {
				continue;
			}
			if (account == "0") {
				return;
			}
			old_user = searchUserByAccount(account);
			if (old_user.getId() == ID_NOT_FOUND) {
				cout << "未找到对应的用户, 请重试 !" << endl;
				continue;
			}
			break;
		}
		cout << "这是这个用户的当前信息：" << endl;
		old_user.print();

		while (true) {
			User new_user = User(old_user.getId());
			cout << "请输入要进行的操作: 1.修改密码 2.修改姓名 0.返回" << endl;
			cout << "请输入: ";
			string choice;
			Tools::inputChoice(choice);
			if (choice == "1") {
				string new_password;
				while (true) {
					cout << "请输入密码(不少于3位, 输入0返回): ";
					Tools::inputPassword(new_password, USER_PASSWORD_SIZE);

					if (new_password == "0") {
						break;
					}
					if (new_password.size() < 3) {
						cout << "密码最少3位哦 !" << endl;
						continue;
					}
					break;
				}
				if (new_password == "0") {
					continue;
				}
				string confirm_password;
				while (true) {
					cout << "确认密码(不少于3位, 输入0返回): ";
					Tools::inputPassword(confirm_password, USER_PASSWORD_SIZE);

					if (confirm_password == "0") {
						break;
					}
					if (confirm_password.size() < 3) {
						cout << "密码最少3位哦 !" << endl;
						continue;
					}
					break;
				}
				if (confirm_password == "0") {
					continue;
				}
				if (new_password == confirm_password) {
					new_user = User(old_user.getId(), account, new_password, old_user.getName());
					writeUserFile(new_user, old_user.getId());
					cout << "密码修改成功" << endl;
					old_user = new_user; //为下一次修改做准备
				} else {
					cout << "两次输入的密码不一致, 请重试！" << endl;
					continue;
				}
			} else if (choice == "2") {
				string name;
				while (true) {
					cout << "请输入姓名(输入0返回): ";
					getline(cin, name);
					name = Tools::trim(name);
					if (name.empty()) {
						continue;
					}
					if (name == "0") {
						break;
					}
					if (name.size() >= USER_NAME_SIZE) {
						cout << "姓名太长啦!" << endl;
						continue;
					}
					break;
				}
				if (name == "0") {
					continue;
				}
				new_user = User(old_user.getId(), account, old_user.getPassword(), name);
				writeUserFile(new_user, old_user.getId());
				cout << "姓名修改成功 !" << endl;
				new_user.print();
			} else if (choice == "0") {
				return;
			} else {
				continue;
			}
		}
	}

	//添加普通管理员
	void addAdmin() {
		cout << "请输入管理员信息(账号, 密码)" << endl;
		string account, password;
		Admin new_admin;
		while (true) {
			cout << "请输入账号(输入0返回): ";
			Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
			if (account.empty()) {
				continue;
			}
			new_admin = searchAdminByAccount(account);
			if (new_admin.getId() != ID_NOT_FOUND) {
				cout << "账号已存在 !" << endl;
				continue;
			}
			if (account == "0") {
				return;
			}
			break;
		}
		while (true) {
			cout << "请输入密码(不少于3位, 输入0返回): ";
			Tools::inputPassword(password, USER_PASSWORD_SIZE);

			if (password == "0") {
				return;
			}
			if (password.size() < 3) {
				cout << "密码最少3位哦 !" << endl;
				continue;
			}

			string confirm_password;
			cout << "请确认密码(不少于3位, 输入0返回): ";
			Tools::inputPassword(confirm_password, USER_PASSWORD_SIZE);

			if (confirm_password == "0") {
				return;
			}
			if (confirm_password.size() < 3) {
				cout << "密码最少3位哦 !" << endl;
				continue;
			}
			if (password != confirm_password) {
				cout << "两次输入的密码不一致 !" << endl;
				continue;
			}
			break;
		}

		new_admin = Admin(adminTotal, account, password);
		writeAdminFile(new_admin, adminTotal);
		cout << "添加成功，信息如下：" << endl;
		new_admin.print();
		adminTotal++;
		writeAdminTotal();
	}

	// 查看所有管理员
	void displayAdmin() {
		cout << "这是现在的管理员信息：" << endl;
		inFile.open(ADMIN_FILE, ios::in | ios::binary);
		if (inFile.is_open()) {
			Admin aAdmin;
			while (inFile.read((char*)&aAdmin, ADMIN_SIZE)) {
				if (aAdmin.getId() != ID_REMOVE) {
					aAdmin.print();//显示这个用户的信息
				}
			}
		}
		inFile.close();
	}

	//修改管理员
	void changeAdmin() {
		string account;
		Admin old_admin;
		while (true) {
			cout << "请输入要修改的管理员账号(输入0返回): ";
			Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
			if (account.empty()) {
				continue;
			}
			if (account == "0") {
				return;
			}
			old_admin = searchAdminByAccount(account);
			if (old_admin.getId() == ID_NOT_FOUND) {
				cout << "未找到对应的管理员, 请重试 !" << endl;
				continue;
			}
			break;
		}
		cout << "这是这个管理员的当前信息：" << endl;
		old_admin.print();

		while (true) {
			Admin new_admin = Admin(old_admin.getId());
			cout << "请输入要进行的操作: 1.修改密码 0.返回" << endl;
			cout << "请输入: ";
			string choice;
			Tools::inputChoice(choice);
			if (choice == "1") {
				string new_password;
				while (true) {
					cout << "请输入密码(不少于3位, 输入0返回): ";
					Tools::inputPassword(new_password, USER_PASSWORD_SIZE);

					if (new_password == "0") {
						break;
					}
					if (new_password.size() < 3) {
						cout << "密码最少3位哦 !" << endl;
						continue;
					}
					break;
				}
				if (new_password == "0") {
					continue;
				}
				string confirm_password;
				while (true) {
					cout << "确认密码(不少于3位, 输入0返回): ";
					Tools::inputPassword(confirm_password, USER_PASSWORD_SIZE);

					if (confirm_password == "0") {
						break;
					}
					if (confirm_password.size() < 3) {
						cout << "密码最少3位哦 !" << endl;
						continue;
					}
					break;
				}
				if (confirm_password == "0") {
					continue;
				}
				if (new_password == confirm_password) {
					new_admin = Admin(old_admin.getId(), account, new_password);
					writeAdminFile(new_admin, old_admin.getId());
					cout << "密码修改成功" << endl;
					old_admin = new_admin; //为下一次修改做准备
				} else {
					cout << "两次输入的密码不一致, 请重试！" << endl;
					continue;
				}
			} else if (choice == "0") {
				return;
			} else {
				continue;
			}
		}
	}

	//删除管理员
	void removeAdmin() {
		string account;
		Admin old_admin;
		while (true) {
			cout << "请输入要删除的管理员账号(输入0返回): ";
			Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
			if (account.empty()) {
				continue;
			}
			if (account == "0") {
				return;
			}
			// 不能删除超级管理员
			if (account == "222") {
				cout << "这个管理员不能被删除 !" << endl;
				continue;
			}
			old_admin = searchAdminByAccount(account);
			if (old_admin.getId() == ID_NOT_FOUND) {
				cout << "未找到对应的管理员, 请重试 !" << endl;
				continue;
			}
			break;
		}
		cout << "您即将删除这个管理员的所有信息：" << endl;
		old_admin.print();

		string choice;
		while (true) {
			cout << "确认删除这个管理员吗?(y/n): ";
			Tools::inputConfirmYN(choice);
			if (choice.empty()) {
				continue;
			}
			break;
		}
		if (choice == "y" || choice == "Y") {
			Admin aAdmin = Admin(ID_REMOVE);
			writeAdminFile(aAdmin, old_admin.getId());
			cout << "删除成功 !" << endl;
		}
	}

	// 查询用户
	void searchUser() {
		string account;
		User old_user;
		while (true) {
			cout << "请输入要查找的用户学号(输入0返回): ";
			Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
			if (account.empty()) {
				continue;
			}
			if (account == "0") {
				return;
			}
			old_user = searchUserByAccount(account);
			if (old_user.getId() == ID_NOT_FOUND) {
				cout << "未找到对应的用户, 请重试 !" << endl;
				continue;
			}
			break;
		}
		cout << "这是这个用户的当前信息：" << endl;
		old_user.print();
	}

	// 批量读入图书数据
	void readTestBooks() {
		ifstream ifs;
		ifs.open("data/book_test.txt");
		int count = 0;
		if (ifs.is_open()) {
			while (!ifs.eof()) {
				Book aBook;
				ifs >> aBook.name >> aBook.author >> aBook.introduction;
				aBook.id = bookTotal;
				aBook.canBorrow = BOOK_CAN_BORROW;
				writeBookFile(aBook, bookTotal);
				bookTotal++;
				count++;
			}
		}
		ifs.close();
		writeBookTotal();
		cout << "共读入了 " << count << " 本图书" << endl;
	}

	// 批量读入用户数据
	void readTestUsers() {
		ifstream ifs;
		ifs.open("data/user_test.txt");
		int count = 0;
		if (ifs.is_open()) {
			while (!ifs.eof()) {
				string account, password, name;
				ifs >> account >> password >> name;
				User aUser = User(userTotal, account, password, name);
				writeUserFile(aUser, userTotal);
				userTotal++;
				count++;
			}
		}
		ifs.close();
		writeUserTotal();
		cout << "共读入了 " << count << " 个用户" << endl;
	}

	// 随机生成借阅记录, 90个用户借阅990本图书, 每人最多借11本
	void generateData() {
		// 写入用户及图书的测试数据
		readTestUsers();
		readTestBooks();
		int user[100];
		// 90个用户借阅
		int user_size = 90;
		// 随机产生用户所在位置的数组
		Tools::permutation(user_size, user);
		/*
		vector<int> vec;
		for (int i = 0; i < 100; i++) {
			vec.push_back(a[i]);
		}
		sort(vec.begin(), vec.end());
		vector<int>::iterator it;
		for (it = vec.begin(); it != vec.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
		*/

		int book[1000];
		// 借阅990本图书
		int book_size = 990;
		// 随机产生图书所在位置的数组
		Tools::permutation(book_size, book);
		
		srand((unsigned)time(NULL));
		int book_index = 0;
		int borrow_count = 0;
		for (int i = 0; i < user_size; i++) {
			// 每人最多借11本
			int ran = rand() % 11 + 1;
			User aUser = searchUserById(user[i]);
			if (aUser.getId() == ID_NOT_FOUND) { // 不存在该用户
				continue;
			}
			for (int j = 0; j < ran; j++) {
				Book aBook = searchBookById(book[book_index]);
				if (aBook.id == ID_NOT_FOUND) { // 不存在该图书
					book_index++;
					continue;
				}
				Borrow aBorrow = Borrow(borrowTotal, aUser.getAccount(), aBook.id);
				if (book[book_index] % 2 == 0) { // 书id为偶数则写入不归还记录
					aBook.canBorrow = BOOK_CANNOT_BORROW;
					writeBookFile(aBook, aBook.id);
					writeBorrowFile(aBorrow, borrowTotal);
				} else { // 书id为奇数则写入归还记录
					aBorrow.isBack = IS_BACK;
					aBorrow.backActualTime = time(NULL) + ran + 10;
					writeBorrowFile(aBorrow, borrowTotal);
				}
				borrowTotal++;
				writeBorrowTotal();
				// 图书index ++
				book_index++;
				// 借阅记录数量++
				borrow_count++;
			}
		}
		cout << "共生成 " << borrow_count << " 条借阅记录" << endl;
	}
};

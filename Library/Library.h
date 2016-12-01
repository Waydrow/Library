#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <limits>
#include "User.h"
#include "Admin.h"
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

// 删除后ID置为-1
#define ID_REMOVE				-1

// 借阅文件中, 记录是否已归还或续借
#define IS_BACK					1
#define IS_NOT_BACK				0
#define IS_BORROW_AGAIN			2

// 查询用户时无法对象无法返回NULL, 取其id代替
#define ID_NOT_FOUND			-999

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
	// isBack表示是否归还, 0未归, 1归还, 2续借
	int isBack = IS_NOT_BACK;

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
	}

	void print() {
		cout << "-------------------------------" << endl;
		cout << "学号: " << userAccount << endl;
		cout << "书名: " << bookId << endl;
		cout << "状态: ";
		switch (isBack) {
			case IS_BACK:
				cout << "已归还";
				break;
			case IS_NOT_BACK:
				cout << "未归还";
				break;
			case IS_BORROW_AGAIN:
				cout << "已续借";
				break;
		}
		cout << endl;
		cout << "-------------------------------" << endl;
	}
};


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
	int isUserLogin = IS_USER_NOT_LOGIN;
	User currentUser;
	// 输入输出流
	ifstream inFile;
	ofstream outFile;

	// 记录下改变后的bookTotal值, 即将bookTotal写入图书文件中第一条记录的id中
	void writeBookTotal() {
		outFile.open(BOOK_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			outFile.seekp(0, ios::beg);
			Book aBook = Book(bookTotal);
			outFile.write((char*)&aBook, BOOK_SIZE);
			cout << "修改bookTotal成功" << endl;
		} else {
			cout << "修改bookTotal失败" << endl;
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
			cout << "修改userTotal成功" << endl;
		} else {
			cout << "修改userTotal失败" << endl;
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
			cout << "修改borrowTotal成功" << endl;
		} else {
			cout << "修改borrowTotal失败" << endl;
		}
		outFile.close();
	}

	// 系统进行图书信息初始化
	void loadSystemBooks() {
		fstream iofile;
		iofile.open(BOOK_FILE, ios::in | ios::out | ios::binary);
		if (iofile.fail()) { // 打开失败则创建新文件, 并写入bookTotal
			cout << "创建新的图书文件成功!" << endl;
			iofile.close();
			iofile.open(BOOK_FILE, ios::out | ios::binary);
			Book aBook = Book(1);
			bookTotal = 1;
			iofile.write((char*)&aBook, BOOK_SIZE);
			iofile.close();
		} else {
			cout << "成功打开原有的图书文件!" << endl;
			// 移动到文件开头位置
			iofile.seekg(0, ios::beg);
			Book aBook;
			iofile.read((char*)&aBook, BOOK_SIZE);
			// 获取第一本书的id, 赋值给bookTotal
			bookTotal = aBook.id;
			iofile.close();
		}
		cout << "系统初始化图书信息成功 !" << endl;
	}

	// 系统进行用户信息初始化
	void loadSystemUsers() {
		fstream iofile;
		iofile.open(USER_FILE, ios::in | ios::out | ios::binary);
		if (iofile.fail()) { // 打开失败则创建新文件, 并写入userTotal
			cout << "创建新的用户文件成功!" << endl;
			iofile.close();
			iofile.open(USER_FILE, ios::out | ios::binary);
			User aUser = User(1);
			userTotal = 1;
			iofile.write((char*)&aUser, USER_SIZE);
			iofile.close();
		} else {
			cout << "成功打开原有的用户文件!" << endl;
			// 移动到文件开头位置
			iofile.seekg(0, ios::beg);
			User aUser;
			iofile.read((char*)&aUser, USER_SIZE);
			// 获取第一个用户的id, 赋值给userTotal
			userTotal = aUser.getId();
			iofile.close();
		}
		cout << "系统初始化用户信息成功 !" << endl;
	}

	// 系统进行借阅信息初始化
	void loadSystemBorrows() {
		fstream iofile;
		iofile.open(BORROW_FILE, ios::in | ios::out | ios::binary);
		if (iofile.fail()) { // 打开失败则创建新文件, 并写入borrowTotal
			cout << "创建新的借阅文件成功!" << endl;
			iofile.close();
			iofile.open(BORROW_FILE, ios::out | ios::binary);
			Borrow aBorrow = Borrow(1);
			borrowTotal = 1;
			iofile.write((char*)&aBorrow, BORROW_SIZE);
			iofile.close();
		} else {
			cout << "成功打开原有的借阅文件!" << endl;
			// 移动到文件开头位置
			iofile.seekg(0, ios::beg);
			Borrow aBorrow;
			iofile.read((char*)&aBorrow, BORROW_SIZE);
			// 获取第一个借阅的id, 赋值给borrowTotal
			borrowTotal = aBorrow.id;
			iofile.close();
		}
		cout << "系统初始化借阅信息成功 !" << endl;
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

	// 查询某一个用户的借阅记录
	vector<Borrow> queryBorrowHistoryOneUser(string account) {
		inFile.open(BORROW_FILE, ios::in | ios::out | ios::binary);
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

	//以图书编号为依据进行查找图书信息
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
		if (flag == 1) {
			return aBook;
		} else {
			return NULL;
		}
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

	// 通过书名查询图书
	vector<Book> searchBookByName(char* name) {
		inFile.open(BOOK_FILE, ios::binary);
		vector<Book> result;
		if (inFile.is_open()) {
			Book aBook;
			inFile.seekg(BOOK_SIZE, ios::beg);
			while (inFile.read((char*)&aBook, BOOK_SIZE)) {
				if (strstr(aBook.name, name)) {
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
public:
	Library() {
		loadSystemBooks();
		loadSystemUsers();
		loadSystemBorrows();
	}

	//增加图书
	void addBook() {
		cout << "请输入图书信息(书名 作者 简介)" << endl;
		char name[BOOK_NAME_SIZE];
		char author[BOOK_AUTHOR_SIZE];
		char introduction[BOOK_INTRO_SIZE];
		cin >> name >> author >> introduction;
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
		cout << "请输入要删除的图书编号：";
		int num;
		cin >> num;
		Book old_book = searchBookById(num);//通过编号找到这本书的记录
		if (old_book == NULL) {
			cout << "未找到对应的书, 请重试 !" << endl;
			return;
		}
		if (old_book.canBorrow == BOOK_CANNOT_BORROW) {
			cout << "这本书被借走了, 暂时不能删除哦 !" << endl;
			return;
		}
		cout << "您即将删除这本书的所有信息：" << endl;
		old_book.print();
		string choice;
		cout << "确认删除这本书吗?(y/n): ";
		cin >> choice;
		if (choice == "y" || choice == "Y") {
			Book aBook = Book(ID_REMOVE, "\0", "\0", "\0");
			writeBookFile(aBook, old_book.id);
			cout << "删除成功 !" << endl;
		}
	}

	//借出图书
	void lendBook() {
		cout << "请输入要借的图书编号：";
		int num;
		cin >> num;
		if (cin.fail()) {
			cerr << "无效的输入!\n";
			cin.clear();
			// 清空输入缓冲区
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return;
		}
		//通过编号找到这本书的记录
		Book old_book = searchBookById(num);
		if (old_book == NULL) {
			cout << "未找到对应的书, 请重试 !" << endl;
			return;
		}
		old_book.print();
		if (old_book.canBorrow == BOOK_CANNOT_BORROW) {
			cout << "这本书已经被别人借走啦！" << endl;
			return;
		}
		string flag;
		cout << "确认借这本书吗?(y/n): ";
		cin >> flag;
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
		// 先判断是否为管理员登录, 是则先输入学号
		if (!isUserLogin) {
			cout << "请输入要查询的用户学号: ";
			cin >> account;
			if (account.size() >= USER_ACCOUNT_SIZE) {
				cout << "输入的学号太长啦 !" << endl;
				return;
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
		cout << "请输入要修改的图书编号：";
		int num;
		cin >> num;
		Book old_book = searchBookById(num);
		if (old_book == NULL) {
			cout << "未找到对应的书, 请重试 !" << endl;
			return;
		}
		if (old_book.canBorrow == BOOK_CANNOT_BORROW) {
			cout << "这本书被借走了, 暂时不能修改哦 !" << endl;
			return;
		}
		cout << "这是这本书的当前信息：" << endl;
		//显示这本书之前的信息
		old_book.print();
		cout << "请输入修改后的图书信息（书名 作者 简介）" << endl;
		char name[BOOK_NAME_SIZE];
		char author[BOOK_AUTHOR_SIZE];
		char introduction[BOOK_INTRO_SIZE];
		cin >> name >> author >> introduction;
		Book new_book = Book(old_book.id, name, author, introduction);
		// 写入文件
		writeBookFile(new_book, old_book.id);
		cout << "这本书的信息已修改为：" << endl;
		new_book.print();
	}

	//归还图书
	void backBook() {
		cout << "请输入要归还的图书编号：";
		int num;
		cin >> num;
		//通过编号找到这本书的记录
		Book old_book = searchBookById(num);
		if (old_book == NULL) {
			cout << "未找到对应的书, 请重试 !" << endl;
			return;
		}
		if (old_book.canBorrow == BOOK_CAN_BORROW) {
			cout << "这本书还未被借出哦 !" << endl;
			return;
		}
		string account = currentUser.getAccount();
		Borrow aBorrow = queryBorrowByUserAndBook(account, num);
		if (aBorrow.id == ID_NOT_FOUND) {
			cout << "你还没有借过这本书哦 !" << endl;
			return;
		}
		string flag;
		cout << "确认归还这本书吗?(y/n): ";
		cin >> flag;
		if (flag == "y" || flag == "Y") {
			old_book.canBorrow = BOOK_CAN_BORROW;
			writeBookFile(old_book, old_book.id);
			aBorrow.isBack = IS_BACK;
			writeBorrowFile(aBorrow, aBorrow.id);
			cout << "还书成功 !" << endl;
		}
	}

	// 用户查询图书
	void userSearchBook() {
		cout << "请输入要查询的图书名字: " << endl;
		char nameInput[BOOK_NAME_SIZE];
		cin >> nameInput;
		if (strlen(nameInput) >= BOOK_NAME_SIZE) {
			cout << "您输入的图书名太长了, 请重试 !" << endl;
			return;
		}
		vector<Book> result = searchBookByName(nameInput);
		if (!result.empty()) {
			cout << "共搜索到符合的条件的 " << result.size() << " 本书" << endl;
			for (vector<int>::size_type ix = 0; ix != result.size(); ix++) {
				result[ix].print();
			}
		} else {
			cout << "没找到对应的书" << endl;
		}
	}


	// 管理员登录
	bool adminLogin() {
		cout << "请输入管理员帐号: " << endl;
		string account;
		cin >> account;
		if (account.size() >= USER_ACCOUNT_SIZE) {
			cout << "帐号太长啦!" << endl;
			return false;
		}
		cout << "请输入管理员密码: " << endl;
		string password;
		cin >> password;
		if (password.size() >= USER_PASSWORD_SIZE) {
			cout << "密码太长啦 !" << endl;
			return false;
		}
		if (account == "222" && password == "222") {
			cout << "登录成功 !" << endl;
			return true;
		} else {
			cout << "帐号或密码有误, 请重试 !" << endl;
			return false;
		}
	}
	// 用户登录
	bool userLogin() {
		cout << "请输入学号: " << endl;
		string account;
		cin >> account;
		if (account.size() >= USER_ACCOUNT_SIZE) {
			cout << "帐号太长啦!" << endl;
			return false;
		}
		cout << "请输入用户密码: " << endl;
		string password;
		cin >> password;
		if (password.size() >= USER_PASSWORD_SIZE) {
			cout << "密码太长啦 !" << endl;
			return false;
		}
		searchUserFileForLogin(account, password);
		if (isUserLogin == IS_USER_LOGIN) {
			return true;
		} else {
			return false;
		}
	}

	//添加用户
	void addUser() {
		cout << "请输入用户信息(学号, 密码, 姓名)" << endl;
		string account, password, name;
		cin >> account >> password >> name;
		if (account.size() >= USER_ACCOUNT_SIZE) {
			cout << "学号太长啦 !" << endl;
			return;
		}
		if (password.size() >= USER_PASSWORD_SIZE) {
			cout << "密码太长啦 !" << endl;
			return;
		}
		if (name.size() >= USER_NAME_SIZE) {
			cout << "姓名太长啦 !" << endl;
			return;
		}
		User new_user = searchUserByAccount(account);
		if (new_user.getId() != ID_NOT_FOUND) {
			cout << "学号已存在 !" << endl;
			return;
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
		cout << "请输入要删除的用户学号：";
		string account;
		cin >> account;
		if (account.size() >= USER_ACCOUNT_SIZE) {
			cout << "学号输入太长啦 !" << endl;
			return;
		}
		User old_user = searchUserByAccount(account);//通过学号找到这个用户的记录
		if (old_user.getId() == ID_NOT_FOUND) {
			cout << "未找到对应的用户, 请重试 !" << endl;
			return;
		}
		cout << "您即将删除这个用户的所有信息：" << endl;
		old_user.print();
		string choice;
		cout << "确认删除这个用户吗?(y/n): ";
		cin >> choice;
		if (choice == "y" || choice == "Y") {
			User aUser = User(ID_REMOVE, '\0', '\0', '\0');
			writeUserFile(aUser, old_user.getId());
			cout << "删除成功 !" << endl;
		}
	}


	//修改用户
	void changeUser() {
		cout << "请输入要修改的用户学号：";
		string account;
		cin >> account;
		if (account.size() >= USER_ACCOUNT_SIZE) {
			cout << "学号太长啦 !" << endl;
			return;
		}
		User old_user = searchUserByAccount(account);
		if (old_user.getId() == ID_NOT_FOUND) {
			cout << "未找到对应的用户, 请重试 !" << endl;
			return;
		}

		cout << "这是这个用户的当前信息：" << endl;
		old_user.print();
		cout << "请输入修改后的用户信息（密码 姓名）" << endl;
		string password, name;
		cin >> password >> name;
		if (password.size() >= USER_PASSWORD_SIZE) {
			cout << "密码太长啦 !" << endl;
			return;
		}
		if (name.size() >= USER_NAME_SIZE) {
			cout << "姓名太长啦 !" << endl;
			return;
		}

		User new_user = User(old_user.getId(), account, password, name);
		// 写入文件
		writeUserFile(new_user, old_user.getId());
		cout << "这个用户的信息已修改为：" << endl;
		new_user.print();
	}


};

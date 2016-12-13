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

// ͼ��, �û�, ����Ա��С
#define BOOK_SIZE				sizeof(Book)
#define USER_SIZE				sizeof(User)
#define ADMIN_SIZE				sizeof(Admin)

#define BORROW_SIZE				sizeof(Borrow)

// ����, ����, ����С
#define BOOK_NAME_SIZE			20
#define BOOK_AUTHOR_SIZE		20
#define BOOK_INTRO_SIZE			50
// ��Ŀɽ��벻�ɽ�״̬
#define BOOK_CAN_BORROW			1
#define BOOK_CANNOT_BORROW		0

// �û��͹���Ա
#define USER_NAME_SIZE			20
#define USER_ACCOUNT_SIZE		20
#define USER_PASSWORD_SIZE		20

// �û���¼, 1��ʾ�ɹ�, 0��ʾʧ��
#define IS_USER_LOGIN			1
#define IS_USER_NOT_LOGIN		0

// ����Ա��¼, 1��ʾ�ɹ�, 0��ʾʧ��
#define IS_ADMIN_LOGIN          1
#define IS_ADMIN_NOT_LOGIN      0

// ��������Ա��¼, 1��ʾ�ɹ�, 0��ʾʧ��
#define IS_SUPER_LOGIN          1
#define IS_SUPER_NOT_LOGIN      0

// ɾ����ID��Ϊ-1
#define ID_REMOVE				-1

// �����ļ���, ��¼�Ƿ��ѹ黹���������
#define IS_BACK					1
#define IS_NOT_BACK				0
#define IS_BORROW_AGAIN			2
#define IS_OVER_TIME			3

// �����������
#define BORROW_AGAIN_LIMITS		1

// ��ѯ�û�ʱ�޷������޷�����NULL, ȡ��id����
#define ID_NOT_FOUND			-999

// һ�εĽ������� 20s, �����+20s, ��Ϊ40s
#define TIME_LIMITS				20

const int book_size[3] = { BOOK_NAME_SIZE, BOOK_AUTHOR_SIZE, BOOK_INTRO_SIZE };

struct Book {
	// ����, Ψһ��ʶ
	int id;
	char name[BOOK_NAME_SIZE]; //����
	char author[BOOK_AUTHOR_SIZE]; //����
	char introduction[BOOK_INTRO_SIZE]; //���
	int canBorrow; // 0��ʾ���ɽ�, 1��ʾ�ɽ�

	Book() {}
	//ֻ�б�ŵĳ�ʼ��
	Book(int id) {
		this->id = id;
		strcpy(this->name, "");
		strcpy(this->author, "");
		strcpy(this->introduction, "");
		this->canBorrow = BOOK_CAN_BORROW;
	}
	//������ʼ��
	Book(int id, char name[BOOK_NAME_SIZE],
		char author[BOOK_AUTHOR_SIZE], char introduction[BOOK_INTRO_SIZE]) {

		this->id = id;
		strcpy(this->name, name);
		strcpy(this->author, author);
		strcpy(this->introduction, introduction);
		this->canBorrow = BOOK_CAN_BORROW;
	}

	// string ������constructor
	Book(int id, string name, string author, string introduction) {
		this->id = id;
		strcpy(this->name, name.c_str());
		strcpy(this->author, author.c_str());
		strcpy(this->introduction, introduction.c_str());
		this->canBorrow = BOOK_CAN_BORROW;
	}
	//��ʾ��Ϣ
	void print() {
		cout << "-------------------------------" << endl;
		cout << "�Ȿ�����Ϣ���£�" << endl;
		cout << "���: " << id << endl;
		cout << "����: " << name << endl;
		cout << "����: " << author << endl;
		cout << "���: " << introduction << endl;
		if (canBorrow == BOOK_CANNOT_BORROW) {
			cout << "�ݲ��ɽ� !" << endl;
		} else {
			cout << "�ɽ�" << endl;
		}
		cout << "-------------------------------" << endl;
	}

	//���ع�ϵ�����
	bool operator==(const Book &b) const {
		if (this->id == b.id) {
			return true;//��ŵȼ�����
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

// ������, �洢�û�ѧ�ź����id
struct Borrow {
	int id;
	char userAccount[USER_ACCOUNT_SIZE];
	int bookId;
	// isBack��ʾ�Ƿ�黹, 0δ��, 1�黹, 2����, 3����
	int isBack;
	// ����ʱ��
	time_t borrowTime;
	// ��Ӧ�黹ʱ��
	time_t backTheoryTime;
	// ʵ�ʹ黹ʱ��
	time_t backActualTime;
	// �������
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
		this->borrowTime = time(NULL); // ��ȡ����ʱ��
		this->backTheoryTime = this->borrowTime + TIME_LIMITS;
		this->isBack = IS_NOT_BACK;
		this->borrowAgainTimes = 0;
	}

	void print() {
		cout << "-------------------------------" << endl;
		cout << "ѧ��: " << userAccount << endl;
		cout << "���: " << bookId << endl;
		cout << "����ʱ��: ";
		Tools::printTime(borrowTime);
		if (isBack != IS_BACK) {
			cout << "Ӧ������: ";
			Tools::printTime(backTheoryTime);
		}
		cout << "�������: " << borrowAgainTimes << endl;
		cout << "״̬: ";
		switch (isBack) {
			case IS_BACK:
				cout << "�ѹ黹" << endl;
				cout << "�黹ʱ��: ";
				Tools::printTime(backActualTime);
				break;
			case IS_NOT_BACK:
				cout << "δ�黹" << endl;
				break;
			case IS_BORROW_AGAIN:
				cout << "������" << endl;
				break;
			case IS_OVER_TIME:
				cout << "�ѳ���" << endl;
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
		 ͼ���ļ��д洢�ĵ�һ������ʵ�ʺ���, ��id��Ϊ�洢bookTotal��ֵ, �ӵڶ�����ʼ��1����
		 bookTotalΪͼ���ļ�����һ������� id, �����ǰ�ļ��洢��n����,
		 ���±���ı��ӦΪ bookTotal = n + 1
	*/
	int bookTotal;
	int userTotal;
	int borrowTotal;
	int adminTotal;
	// �ж��Ƿ��û���¼
	int isUserLogin;
	// �ж��Ƿ���ͨ����Ա��¼
	int isAdminLogin;
	// �ж��Ƿ�Ϊ��������Ա��¼
	int isSuperLogin;

	User currentUser;
	Admin currentAdmin;
	// ���������


	// ��¼�¸ı���bookTotalֵ, ����bookTotalд��ͼ���ļ��е�һ����¼��id��
	void writeBookTotal() {
		outFile.open(BOOK_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			outFile.seekp(0, ios::beg);
			Book aBook = Book(bookTotal);
			outFile.write((char*)&aBook, BOOK_SIZE);
			// cout << "�޸�bookTotal�ɹ�" << endl;
		} else {
			// cout << "�޸�bookTotalʧ��" << endl;
		}
		outFile.close();
	}

	// ��¼�¸ı���userTotalֵ, ����userTotalд��ͼ���ļ��е�һ����¼��id��
	void writeUserTotal() {
		outFile.open(USER_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			outFile.seekp(0, ios::beg);
			User aUser = User(userTotal);
			outFile.write((char*)&aUser, USER_SIZE);
			// cout << "�޸�userTotal�ɹ�" << endl;
		} else {
			// cout << "�޸�userTotalʧ��" << endl;
		}
		outFile.close();
	}

	// ��¼�¸ı���borrowTotalֵ, ����borrowTotalд������ļ��е�һ����¼��id��
	void writeBorrowTotal() {
		outFile.open(BORROW_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			outFile.seekp(0, ios::beg);
			Borrow aBorrow = Borrow(borrowTotal);
			outFile.write((char*)&aBorrow, BORROW_SIZE);
			// cout << "�޸�borrowTotal�ɹ�" << endl;
		} else {
			// cout << "�޸�borrowTotalʧ��" << endl;
		}
		outFile.close();
	}

	// ��¼�¸ı���adminTotalֵ, ����dminTotalд�����Ա�ļ��е�һ����¼��id��
	void writeAdminTotal() {
		outFile.open(ADMIN_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			outFile.seekp(0, ios::beg);
			Admin aAdmin = Admin(adminTotal);
			outFile.write((char*)&aAdmin, ADMIN_SIZE);
			cout << "�޸�adminTotal�ɹ�" << endl;
		} else {
			cout << "�޸�adminTotalʧ��" << endl;
		}
		outFile.close();
	}

	// ϵͳ����ͼ����Ϣ��ʼ��
	void loadSystemBooks() {
		fstream iofile;
		iofile.open(BOOK_FILE, ios::in | ios::out | ios::binary);
		if (iofile.fail())   // ��ʧ���򴴽����ļ�, ��д��bookTotal
		{
			// cout << "�����µ�ͼ���ļ��ɹ�!" << endl;
			iofile.close();
			iofile.open(BOOK_FILE, ios::out | ios::binary);
			Book aBook = Book(1);
			bookTotal = 1;
			iofile.write((char*)&aBook, BOOK_SIZE);
			iofile.close();
		} else {
			// cout << "�ɹ���ԭ�е�ͼ���ļ�!" << endl;
			// �ƶ����ļ���ͷλ��
			iofile.seekg(0, ios::beg);
			Book aBook;
			iofile.read((char*)&aBook, BOOK_SIZE);
			// ��ȡ��һ�����id, ��ֵ��bookTotal
			bookTotal = aBook.id;
			iofile.close();
		}
		//cout << "ϵͳ��ʼ��ͼ����Ϣ�ɹ� !" << endl;
	}

	// ϵͳ�����û���Ϣ��ʼ��
	void loadSystemUsers() {
		fstream iofile;
		iofile.open(USER_FILE, ios::in | ios::out | ios::binary);
		if (iofile.fail())   // ��ʧ���򴴽����ļ�, ��д��userTotal
		{
			// cout << "�����µ��û��ļ��ɹ�!" << endl;
			iofile.close();
			iofile.open(USER_FILE, ios::out | ios::binary);
			User aUser = User(1);
			userTotal = 1;
			iofile.write((char*)&aUser, USER_SIZE);
			iofile.close();
		} else {
			// cout << "�ɹ���ԭ�е��û��ļ�!" << endl;
			// �ƶ����ļ���ͷλ��
			iofile.seekg(0, ios::beg);
			User aUser;
			iofile.read((char*)&aUser, USER_SIZE);
			// ��ȡ��һ���û���id, ��ֵ��userTotal
			userTotal = aUser.getId();
			iofile.close();
		}
		//cout << "ϵͳ��ʼ���û���Ϣ�ɹ� !" << endl;
	}

	// ϵͳ���н�����Ϣ��ʼ��
	void loadSystemBorrows() {
		fstream iofile;
		iofile.open(BORROW_FILE, ios::in | ios::out | ios::binary);
		if (iofile.fail())   // ��ʧ���򴴽����ļ�, ��д��borrowTotal
		{
			// cout << "�����µĽ����ļ��ɹ�!" << endl;
			iofile.close();
			iofile.open(BORROW_FILE, ios::out | ios::binary);
			Borrow aBorrow = Borrow(1);
			borrowTotal = 1;
			iofile.write((char*)&aBorrow, BORROW_SIZE);
			iofile.close();
		} else {
			// cout << "�ɹ���ԭ�еĽ����ļ�!" << endl;
			// �ƶ����ļ���ͷλ��
			iofile.seekg(0, ios::beg);
			Borrow aBorrow;
			iofile.read((char*)&aBorrow, BORROW_SIZE);
			// ��ȡ��һ�����ĵ�id, ��ֵ��borrowTotal
			borrowTotal = aBorrow.id;
			iofile.close();
		}
		//cout << "ϵͳ��ʼ��������Ϣ�ɹ� !" << endl;
	}

	// ϵͳ���й���Ա��Ϣ��ʼ��
	void loadSystemAdmins() {
		fstream iofile;
		iofile.open(ADMIN_FILE, ios::in | ios::out | ios::binary);
		if (iofile.fail()) {
			iofile.close();
			iofile.open(ADMIN_FILE, ios::out | ios::binary);
			// ע��˴��������һ����ǰ���id �����һ����������Ա
			Admin aAdmin = Admin(2);
			Admin superAdmin = Admin(1, "222", "222");
			adminTotal = 2;
			iofile.write((char*)&aAdmin, ADMIN_SIZE);
			iofile.write((char*)&superAdmin, ADMIN_SIZE);
			iofile.close();
		} else {
			// �ƶ����ļ���ͷλ��
			iofile.seekp(0, ios::beg);
			Admin aAdmin;
			iofile.read((char*)&aAdmin, ADMIN_SIZE);
			// ��ȡ��һ������Ա��id, ��ֵ��adminTotal
			adminTotal = aAdmin.getId();
			iofile.close();
		}
		//cout << "ϵͳ��ʼ������Ա��Ϣ�ɹ� !" << endl;
	}

	//д��ͼ���ļ�
	void writeBookFile(Book aBook, int location) {
		outFile.open(BOOK_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			// д�뵽locationλ��, �����ļ�ָ���0�ƶ� location �� book_size
			outFile.seekp(location*BOOK_SIZE, ios::beg);
			outFile.write((char*)&aBook, BOOK_SIZE);
		}
		outFile.close();
	}

	//д���û��ļ�
	void writeUserFile(User aUser, int location) {
		outFile.open(USER_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			// д�뵽locationλ��, �����ļ�ָ���0�ƶ� location �� user_size
			outFile.seekp(location*USER_SIZE, ios::beg);
			outFile.write((char*)&aUser, USER_SIZE);
		}
		outFile.close();
	}

	// д������ļ�
	void writeBorrowFile(Borrow aBorrow, int location) {
		outFile.open(BORROW_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			// д�뵽locationλ��, �����ļ�ָ���0�ƶ� location �� borrow_size
			outFile.seekp(location*BORROW_SIZE, ios::beg);
			outFile.write((char*)&aBorrow, BORROW_SIZE);
		}
		outFile.close();
	}

	// д�����Ա
	void writeAdminFile(Admin aAdmin, int location) {
		outFile.open(ADMIN_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			// д�뵽locationλ��, �����ļ�ָ���0�ƶ� location �� admin_size
			outFile.seekp(location*ADMIN_SIZE, ios::beg);
			outFile.write((char*)&aAdmin, ADMIN_SIZE);
		}
		outFile.close();
	}

	// ��ѯĳһ���û��Ľ��ļ�¼
	vector<Borrow> queryBorrowHistoryOneUser(string account) {
		inFile.open(BORROW_FILE, ios::binary);
		vector<Borrow> vec;
		if (inFile.is_open()) {
			Borrow aBorrow;
			// �ӵڶ�����¼��ʼ��ѯ
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

	// ��ѯ�ض��û��ض�ͼ��Ľ����¼
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

	//��ͼ��idΪ���ݽ��в���ͼ����Ϣ
	Book searchBookById(int num) {
		int flag = 0; // 0����û�鵽, 1����鵽
		inFile.open(BOOK_FILE, ios::binary);
		Book aBook;
		if (inFile.is_open()) {
			// �ӵڶ�����¼��ʼ����
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

	// ���û�idΪ���ݽ��в����û���Ϣ
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

	// ���û�ѧ��Ϊ���ݽ��в����û���Ϣ
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

	// �Թ���Ա�˺�Ϊ���ݽ��в��ҹ���Ա��Ϣ
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

	// ͨ��������ѯͼ��
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


	// �û���¼ȷ��
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
					cout << "�û���¼�ɹ�" << endl;
					break;
				}
			}
		}
		inFile.close();
		if (isUserLogin == IS_USER_NOT_LOGIN) {
			cout << "�˺Ż��������" << endl;
		}
	}

	// ����Ա��¼ȷ��
	void searchAdminFileForLogin(string account, string password) {
		inFile.open(ADMIN_FILE, ios::binary);
		if (inFile.is_open()) {
			Admin admin = Admin();
			while (inFile.read((char*)&admin, ADMIN_SIZE)) {
				string _account(admin.getAccount());
				string _password(admin.getPassword());
				// �ж��Ƿ�Ϊ��������Ա
				if (account == "222") {
					if (password == _password) {
						isSuperLogin = IS_SUPER_LOGIN;
						currentAdmin = admin;
						cout << "����Ա��¼�ɹ�" << endl;
						break;
					}
				}
				// �������ͨ����Ա
				else {
					if (account == _account && password == _password) {
						isAdminLogin = IS_ADMIN_LOGIN;
						currentAdmin = admin;
						cout << "����Ա��¼�ɹ�" << endl;
						break;
					}
				}
			}
		}
		inFile.close();
		if (isAdminLogin == IS_ADMIN_NOT_LOGIN && isSuperLogin == IS_SUPER_NOT_LOGIN) {
			cout << "�˺Ż��������" << endl;
		}
	}

	// �û���¼���ѯ����ͼ��, ������ͼ��״̬
	vector<Borrow> searchOverTimeBorrowBooks(string account) {
		fstream ioFile;
		ioFile.open(BORROW_FILE, ios::in | ios::out | ios::binary);
		vector<Borrow> vec;
		if (ioFile.is_open()) {
			Borrow aBorrow;
			while (ioFile.read((char*)&aBorrow, BORROW_SIZE)) {
				string tempAccount(aBorrow.userAccount);
				if (account == tempAccount) {
					// ��ȡ��ǰʱ��, ����Ӧ�黹���ڽ��бȶ�
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
		cout << setw(60) << "*            ͼ�����ϵͳV2.0             *" << endl;
		cout << setw(60) << "*                ��4С��                  *" << endl;
		cout << setw(60) << "*                                         *" << endl;
		cout << setw(60) << "*                                         *" << endl;
		cout << setw(60) << "*******************************************" << endl;
	}

	//����ͼ��
	void addBook() {
		cout << "������ͼ����Ϣ(����(20λ) ����(20λ) ���(50λ))" << endl;
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

		cout << "��ӳɹ�����Ϣ���£�" << endl;
		new_book.print();
		bookTotal++;
		writeBookTotal();
	}

	//�鿴ͼ����
	void displayBook() {
		cout << "�������ڵĿ����Ϣ��" << endl;
		inFile.open(BOOK_FILE, ios::in | ios::out | ios::binary);
		if (inFile.is_open()) {
			Book aBook;
			while (inFile.read((char*)&aBook, BOOK_SIZE)) {
				if (aBook.id != ID_REMOVE) {
					aBook.print();//��ʾ�Ȿ�����Ϣ
				}
			}
		}
		inFile.close();
	}

	//ɾ��ͼ��
	void removeBook() {
		Book old_book;
		int num;
		while (true) {
			cout << "������Ҫɾ����ͼ����(����0����)��";
			Tools::inputBookNum(num);
			if (num == -1) {
				continue;
			}
			if (num == 0) {
				return;
			}
			old_book = searchBookById(num);//ͨ������ҵ��Ȿ��ļ�¼
			if (old_book.id == ID_NOT_FOUND) {
				cout << "δ�ҵ���Ӧ����, ������ !" << endl;
				continue;
			}
			if (old_book.canBorrow == BOOK_CANNOT_BORROW) {
				cout << "�Ȿ�鱻������, ��ʱ����ɾ��Ŷ !" << endl;
				continue;
			}
			break;
		}
		cout << "������ɾ���Ȿ���������Ϣ��" << endl;
		old_book.print();
		string choice;
		while (true) {
			cout << "ȷ��ɾ���Ȿ����?(y/n): ";
			Tools::inputConfirmYN(choice);
			if (choice.empty()) {
				continue;
			}
			break;
		}
		if (choice == "y" || choice == "Y") {
			Book aBook = Book(ID_REMOVE, "\0", "\0", "\0");
			writeBookFile(aBook, old_book.id);
			cout << "ɾ���ɹ� !" << endl;
		}
	}

	//���ͼ��
	void lendBook() {
		int num;
		Book old_book;
		while (true) {
			cout << "������Ҫ���ͼ����(����0����)��";
			Tools::inputBookNum(num);
			if (num == -1) {
				continue;
			}
			if (num == 0) {
				return;
			}
			old_book = searchBookById(num);//ͨ������ҵ��Ȿ��ļ�¼
			if (old_book.id == ID_NOT_FOUND) {
				cout << "δ�ҵ���Ӧ����, ������ !" << endl;
				continue;
			}
			if (old_book.canBorrow == BOOK_CANNOT_BORROW) {
				cout << "�Ȿ���Ѿ������˽����� !" << endl;
				continue;
			}
			break;
		}
		string flag;
		old_book.print();
		while (true) {
			cout << "ȷ�Ͻ��Ȿ����?(y/n): ";
			Tools::inputConfirmYN(flag);
			if (flag.empty()) {
				continue;
			}
			break;
		}
		if (flag == "y" || flag == "Y") {
			old_book.canBorrow = BOOK_CANNOT_BORROW; // ͼ��״̬��Ϊ���
			writeBookFile(old_book, old_book.id); // д�뵽ͼ���ļ�
			Borrow aBorrow = Borrow(borrowTotal, currentUser.getAccount(), old_book.id);
			writeBorrowFile(aBorrow, borrowTotal); // д�뵽�����ļ�
			borrowTotal++;
			writeBorrowTotal();
			cout << "����ɹ� !" << endl;
		}
	}

	// �鿴���ļ�¼
	void displayOneUserBorrowHistory() {
		vector<Borrow> borrows;
		string account;
		User old_user;
		// ���ж��Ƿ�Ϊ����Ա��¼, ����������ѧ��
		if (!isUserLogin) {
			while (true) {
				cout << "������Ҫ���ҵ��û�ѧ��(����0����): ";
				Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
				if (account.empty()) {
					continue;
				}
				if (account == "0") {
					return;
				}
				old_user = searchUserByAccount(account);
				if (old_user.getId() == ID_NOT_FOUND) {
					cout << "δ�ҵ���Ӧ���û�, ������ !" << endl;
					continue;
				}
				break;
			}
		} else {
			account = currentUser.getAccount();
		}

		borrows = queryBorrowHistoryOneUser(account);
		if (borrows.empty()) {
			cout << "��û�н����Ŷ !" << endl;
			return;
		} else {
			cout << "����ѯ�� " << borrows.size() << " �����ļ�¼" << endl;
			vector<Borrow>::iterator it;
			for (it = borrows.begin(); it != borrows.end(); it++) {
				(*it).print();
			}
		}
	}

	//�޸�ͼ��
	void changeBook() {
		Book old_book;
		int num;
		while (true) {
			cout << "������Ҫ�޸ĵ�ͼ����(����0����)��";
			Tools::inputBookNum(num);
			if (num == -1) {
				continue;
			}
			if (num == 0) {
				return;
			}
			old_book = searchBookById(num);//ͨ������ҵ��Ȿ��ļ�¼
			if (old_book.id == ID_NOT_FOUND) {
				cout << "δ�ҵ���Ӧ����, ������ !" << endl;
				continue;
			}
			if (old_book.canBorrow == BOOK_CANNOT_BORROW) {
				cout << "�Ȿ�鱻������, ��ʱ�����޸�Ŷ !" << endl;
				continue;
			}
			break;
		}
		cout << "�����Ȿ��ĵ�ǰ��Ϣ��" << endl;
		//��ʾ�Ȿ��֮ǰ����Ϣ
		old_book.print();
		while (true) {
			cout << "������Ҫ���еĲ���: 1.�޸����� 2.�޸����� 3.�޸ļ�� 0.����" << endl;
			string choice;
			Tools::inputChoice(choice);
			if (choice == "1") {
				string name;
				if (!Tools::inputBookName(name, BOOK_NAME_SIZE)) {
					continue;
				}
				Book new_book = Book(old_book.id, name, old_book.author, old_book.introduction);
				// д���ļ�
				writeBookFile(new_book, old_book.id);
				cout << "�����޸ĳɹ�" << endl;
				new_book.print();
				old_book = new_book;
			} else if (choice == "2") {
				string author;
				if (!Tools::inputBookAuthor(author, BOOK_AUTHOR_SIZE)) {
					continue;
				}
				Book new_book = Book(old_book.id, old_book.name, author, old_book.introduction);
				// д���ļ�
				writeBookFile(new_book, old_book.id);
				cout << "�����޸ĳɹ�" << endl;
				new_book.print();
				old_book = new_book;
			} else if (choice == "3") {
				string intro;
				if (!Tools::inputBookIntro(intro, BOOK_INTRO_SIZE)) {
					continue;
				}
				Book new_book = Book(old_book.id, old_book.name, old_book.author, intro);
				// д���ļ�
				writeBookFile(new_book, old_book.id);
				cout << "����޸ĳɹ�" << endl;
				new_book.print();
				old_book = new_book;
			} else if (choice == "0") {
				return;
			} else {
				continue;
			}
		}
	}

	//�黹ͼ��
	void backBook() {
		int num;
		Book old_book;
		Borrow aBorrow;
		while (true) {
			cout << "������Ҫ�黹��ͼ����(����0����)��";
			Tools::inputBookNum(num);
			if (num == -1) {
				continue;
			}
			if (num == 0) {
				return;
			}
			old_book = searchBookById(num);//ͨ������ҵ��Ȿ��ļ�¼
			if (old_book.id == ID_NOT_FOUND) {
				cout << "δ�ҵ���Ӧ����, ������ !" << endl;
				continue;
			}
			string account = currentUser.getAccount();
			aBorrow = queryBorrowByUserAndBook(account, num);
			if (old_book.canBorrow == BOOK_CAN_BORROW || aBorrow.id == ID_NOT_FOUND) {
				cout << "�㻹û�н���Ȿ��Ŷ !" << endl;
				continue;
			}
			break;
		}

		string flag;
		while (true) {
			cout << "ȷ�Ϲ黹�Ȿ����?(y/n): ";
			Tools::inputConfirmYN(flag);
			if (flag.empty()) {
				continue;
			}
			break;
		}
		if (flag == "y" || flag == "Y") {
			old_book.canBorrow = BOOK_CAN_BORROW;
			writeBookFile(old_book, old_book.id);
			// �����־
			aBorrow.isBack = IS_BACK;
			// ��¼�»���ʱ��
			aBorrow.backActualTime = time(NULL);
			writeBorrowFile(aBorrow, aBorrow.id);
			cout << "����ɹ� !" << endl;
		}
	}

	// ����ͼ��
	void borrowAgain() {
		int num;
		Book old_book;
		Borrow aBorrow;
		while (true) {
			cout << "������Ҫ�����ͼ����(����0����)��";
			Tools::inputBookNum(num);
			if (num == -1) {
				continue;
			}
			if (num == 0) {
				return;
			}
			old_book = searchBookById(num);//ͨ������ҵ��Ȿ��ļ�¼
			if (old_book.id == ID_NOT_FOUND) {
				cout << "δ�ҵ���Ӧ����, ������ !" << endl;
				continue;
			}
			string account = currentUser.getAccount();
			aBorrow = queryBorrowByUserAndBook(account, num);
			if (old_book.canBorrow == BOOK_CAN_BORROW || aBorrow.id == ID_NOT_FOUND) {
				cout << "�㻹û�н���Ȿ��Ŷ !" << endl;
				continue;
			}
			if (aBorrow.borrowAgainTimes == BORROW_AGAIN_LIMITS) {
				cout << "ÿ�������ֻ������һ�� !" << endl;
				continue;
			}
			break;
		}

		string flag;
		while (true) {
			cout << "ȷ�������Ȿ����?(y/n): ";
			Tools::inputConfirmYN(flag);
			if (flag.empty()) {
				continue;
			}
			break;
		}
		if (flag == "y" || flag == "Y") {
			// �����־
			aBorrow.isBack = IS_BORROW_AGAIN;
			// �������++
			aBorrow.borrowAgainTimes++;
			// ������Ӧ����ʱ��
			aBorrow.backTheoryTime += TIME_LIMITS;
			writeBorrowFile(aBorrow, aBorrow.id);
			cout << "����ɹ� !" << endl;
		}
	}

	// �û���ѯͼ��
	void userSearchBook() {
		string nameInput;
		vector<Book> result;
		while (true) {
			cout << "������Ҫ��ѯ��ͼ������(����0����): ";
			getline(cin, nameInput);
			nameInput = Tools::trim(nameInput);
			if (nameInput.empty()) {
				continue;
			}
			if (nameInput == "0") {
				return;
			}
			if (nameInput.size() >= BOOK_NAME_SIZE) {
				cout << "�������ͼ����̫����, ������ !" << endl;
				continue;
			}
			result = searchBookByName(nameInput);
			if (result.empty()) {
				cout << "û�ҵ���Ӧ����" << endl;
				continue;
			}
			break;
		}

		cout << "�����������ϵ������� " << result.size() << " ����" << endl;
		for (vector<int>::size_type ix = 0; ix != result.size(); ix++) {
			result[ix].print();
		}
	}


	// ����Ա��¼
	int adminLogin() {
		string account;
		while (true) {
			cout << "���������Ա�ʺ�(����0����): " << endl;
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
			cout << "���������Ա����(����0����): " << endl;
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

	// �û���¼
	bool userLogin() {
		string account;
		while (true) {
			cout << "������ѧ��(����0����): " << endl;
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
			cout << "�������û�����(����0����): " << endl;
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
			// ��ѯ����ͼ��
			vector<Borrow> vec = searchOverTimeBorrowBooks(account);
			if (!vec.empty()) {
				cout << "���� " << vec.size() << " ��ͼ���ѳ���, �뼴ʱ�黹������ !" << endl;
				for (vector<Borrow>::size_type ix = 0; ix != vec.size(); ix++) {
					vec[ix].print();
				}
			}
			return true;
		} else {
			return false;
		}
	}

	//����û�
	void addUser() {
		cout << "�������û���Ϣ(ѧ��, ����, ����)" << endl;
		string account, password;
		string name;
		User new_user;
		while (true) {
			cout << "������ѧ��(����0����): ";
			Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
			if (account.empty()) {
				continue;
			}
			new_user = searchUserByAccount(account);
			if (new_user.getId() != ID_NOT_FOUND) {
				cout << "ѧ���Ѵ��� !" << endl;
				continue;
			}
			if (account == "0") {
				return;
			}
			break;
		}
		while (true) {
			cout << "����������(������3λ, ����0����): ";
			Tools::inputPassword(password, USER_PASSWORD_SIZE);

			if (password == "0") {
				return;
			}
			if (password.size() < 3) {
				cout << "��������3λŶ !" << endl;
				continue;
			}

			string confirm_password;
			cout << "��ȷ������(������3λ, ����0����): ";
			Tools::inputPassword(confirm_password, USER_PASSWORD_SIZE);

			if (confirm_password == "0") {
				return;
			}
			if (confirm_password.size() < 3) {
				cout << "��������3λŶ !" << endl;
				continue;
			}
			if (password != confirm_password) {
				cout << "������������벻һ�� !" << endl;
				continue;
			}
			break;
		}


		while (true) {
			cout << "����������(����0����): ";
			getline(cin, name);
			name = Tools::trim(name);
			if (name.empty()) {
				continue;
			}
			if (name == "0") {
				return;
			}
			if (name.size() >= USER_NAME_SIZE) {
				cout << "����̫����" << endl;
				continue;
			}
			break;
		}

		new_user = User(userTotal, account, password, name);
		writeUserFile(new_user, userTotal);
		cout << "��ӳɹ�����Ϣ���£�" << endl;
		new_user.print();
		userTotal++;
		writeUserTotal();
	}

	//�鿴�����û�
	void displayUser() {
		cout << "�������ڵ��û���Ϣ��" << endl;
		inFile.open(USER_FILE, ios::in | ios::binary);
		if (inFile.is_open()) {
			User aUser;
			while (inFile.read((char*)&aUser, USER_SIZE)) {
				if (aUser.getId() != ID_REMOVE) {
					aUser.print();//��ʾ����û�����Ϣ
				}
			}
		}
		inFile.close();
	}

	//ɾ���û�
	void removeUser() {
		string account;
		User old_user;
		while (true) {
			cout << "������Ҫɾ�����û�ѧ��(����0����): ";
			Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
			if (account.empty()) {
				continue;
			}
			if (account == "0") {
				return;
			}
			old_user = searchUserByAccount(account);
			if (old_user.getId() == ID_NOT_FOUND) {
				cout << "δ�ҵ���Ӧ���û�, ������ !" << endl;
				continue;
			}
			break;
		}
		cout << "������ɾ������û���������Ϣ��" << endl;
		old_user.print();

		string choice;
		while (true) {
			cout << "ȷ��ɾ������û���?(y/n): ";
			Tools::inputConfirmYN(choice);
			if (choice.empty()) {
				continue;
			}
			break;
		}
		if (choice == "y" || choice == "Y") {
			User aUser = User(ID_REMOVE);
			writeUserFile(aUser, old_user.getId());
			cout << "ɾ���ɹ� !" << endl;
		}
	}

	//�޸��û�
	void changeUser() {
		string account;
		User old_user;
		while (true) {
			cout << "������Ҫ�޸ĵ��û�ѧ��(����0����): ";
			Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
			if (account.empty()) {
				continue;
			}
			if (account == "0") {
				return;
			}
			old_user = searchUserByAccount(account);
			if (old_user.getId() == ID_NOT_FOUND) {
				cout << "δ�ҵ���Ӧ���û�, ������ !" << endl;
				continue;
			}
			break;
		}
		cout << "��������û��ĵ�ǰ��Ϣ��" << endl;
		old_user.print();

		while (true) {
			User new_user = User(old_user.getId());
			cout << "������Ҫ���еĲ���: 1.�޸����� 2.�޸����� 0.����" << endl;
			cout << "������: ";
			string choice;
			Tools::inputChoice(choice);
			if (choice == "1") {
				string new_password;
				while (true) {
					cout << "����������(������3λ, ����0����): ";
					Tools::inputPassword(new_password, USER_PASSWORD_SIZE);

					if (new_password == "0") {
						break;
					}
					if (new_password.size() < 3) {
						cout << "��������3λŶ !" << endl;
						continue;
					}
					break;
				}
				if (new_password == "0") {
					continue;
				}
				string confirm_password;
				while (true) {
					cout << "ȷ������(������3λ, ����0����): ";
					Tools::inputPassword(confirm_password, USER_PASSWORD_SIZE);

					if (confirm_password == "0") {
						break;
					}
					if (confirm_password.size() < 3) {
						cout << "��������3λŶ !" << endl;
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
					cout << "�����޸ĳɹ�" << endl;
					old_user = new_user; //Ϊ��һ���޸���׼��
				} else {
					cout << "������������벻һ��, �����ԣ�" << endl;
					continue;
				}
			} else if (choice == "2") {
				string name;
				while (true) {
					cout << "����������(����0����): ";
					getline(cin, name);
					name = Tools::trim(name);
					if (name.empty()) {
						continue;
					}
					if (name == "0") {
						break;
					}
					if (name.size() >= USER_NAME_SIZE) {
						cout << "����̫����!" << endl;
						continue;
					}
					break;
				}
				if (name == "0") {
					continue;
				}
				new_user = User(old_user.getId(), account, old_user.getPassword(), name);
				writeUserFile(new_user, old_user.getId());
				cout << "�����޸ĳɹ� !" << endl;
				new_user.print();
			} else if (choice == "0") {
				return;
			} else {
				continue;
			}
		}
	}

	//�����ͨ����Ա
	void addAdmin() {
		cout << "���������Ա��Ϣ(�˺�, ����)" << endl;
		string account, password;
		Admin new_admin;
		while (true) {
			cout << "�������˺�(����0����): ";
			Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
			if (account.empty()) {
				continue;
			}
			new_admin = searchAdminByAccount(account);
			if (new_admin.getId() != ID_NOT_FOUND) {
				cout << "�˺��Ѵ��� !" << endl;
				continue;
			}
			if (account == "0") {
				return;
			}
			break;
		}
		while (true) {
			cout << "����������(������3λ, ����0����): ";
			Tools::inputPassword(password, USER_PASSWORD_SIZE);

			if (password == "0") {
				return;
			}
			if (password.size() < 3) {
				cout << "��������3λŶ !" << endl;
				continue;
			}

			string confirm_password;
			cout << "��ȷ������(������3λ, ����0����): ";
			Tools::inputPassword(confirm_password, USER_PASSWORD_SIZE);

			if (confirm_password == "0") {
				return;
			}
			if (confirm_password.size() < 3) {
				cout << "��������3λŶ !" << endl;
				continue;
			}
			if (password != confirm_password) {
				cout << "������������벻һ�� !" << endl;
				continue;
			}
			break;
		}

		new_admin = Admin(adminTotal, account, password);
		writeAdminFile(new_admin, adminTotal);
		cout << "��ӳɹ�����Ϣ���£�" << endl;
		new_admin.print();
		adminTotal++;
		writeAdminTotal();
	}

	// �鿴���й���Ա
	void displayAdmin() {
		cout << "�������ڵĹ���Ա��Ϣ��" << endl;
		inFile.open(ADMIN_FILE, ios::in | ios::binary);
		if (inFile.is_open()) {
			Admin aAdmin;
			while (inFile.read((char*)&aAdmin, ADMIN_SIZE)) {
				if (aAdmin.getId() != ID_REMOVE) {
					aAdmin.print();//��ʾ����û�����Ϣ
				}
			}
		}
		inFile.close();
	}

	//�޸Ĺ���Ա
	void changeAdmin() {
		string account;
		Admin old_admin;
		while (true) {
			cout << "������Ҫ�޸ĵĹ���Ա�˺�(����0����): ";
			Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
			if (account.empty()) {
				continue;
			}
			if (account == "0") {
				return;
			}
			old_admin = searchAdminByAccount(account);
			if (old_admin.getId() == ID_NOT_FOUND) {
				cout << "δ�ҵ���Ӧ�Ĺ���Ա, ������ !" << endl;
				continue;
			}
			break;
		}
		cout << "�����������Ա�ĵ�ǰ��Ϣ��" << endl;
		old_admin.print();

		while (true) {
			Admin new_admin = Admin(old_admin.getId());
			cout << "������Ҫ���еĲ���: 1.�޸����� 0.����" << endl;
			cout << "������: ";
			string choice;
			Tools::inputChoice(choice);
			if (choice == "1") {
				string new_password;
				while (true) {
					cout << "����������(������3λ, ����0����): ";
					Tools::inputPassword(new_password, USER_PASSWORD_SIZE);

					if (new_password == "0") {
						break;
					}
					if (new_password.size() < 3) {
						cout << "��������3λŶ !" << endl;
						continue;
					}
					break;
				}
				if (new_password == "0") {
					continue;
				}
				string confirm_password;
				while (true) {
					cout << "ȷ������(������3λ, ����0����): ";
					Tools::inputPassword(confirm_password, USER_PASSWORD_SIZE);

					if (confirm_password == "0") {
						break;
					}
					if (confirm_password.size() < 3) {
						cout << "��������3λŶ !" << endl;
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
					cout << "�����޸ĳɹ�" << endl;
					old_admin = new_admin; //Ϊ��һ���޸���׼��
				} else {
					cout << "������������벻һ��, �����ԣ�" << endl;
					continue;
				}
			} else if (choice == "0") {
				return;
			} else {
				continue;
			}
		}
	}

	//ɾ������Ա
	void removeAdmin() {
		string account;
		Admin old_admin;
		while (true) {
			cout << "������Ҫɾ���Ĺ���Ա�˺�(����0����): ";
			Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
			if (account.empty()) {
				continue;
			}
			if (account == "0") {
				return;
			}
			// ����ɾ����������Ա
			if (account == "222") {
				cout << "�������Ա���ܱ�ɾ�� !" << endl;
				continue;
			}
			old_admin = searchAdminByAccount(account);
			if (old_admin.getId() == ID_NOT_FOUND) {
				cout << "δ�ҵ���Ӧ�Ĺ���Ա, ������ !" << endl;
				continue;
			}
			break;
		}
		cout << "������ɾ���������Ա��������Ϣ��" << endl;
		old_admin.print();

		string choice;
		while (true) {
			cout << "ȷ��ɾ���������Ա��?(y/n): ";
			Tools::inputConfirmYN(choice);
			if (choice.empty()) {
				continue;
			}
			break;
		}
		if (choice == "y" || choice == "Y") {
			Admin aAdmin = Admin(ID_REMOVE);
			writeAdminFile(aAdmin, old_admin.getId());
			cout << "ɾ���ɹ� !" << endl;
		}
	}

	// ��ѯ�û�
	void searchUser() {
		string account;
		User old_user;
		while (true) {
			cout << "������Ҫ���ҵ��û�ѧ��(����0����): ";
			Tools::inputStuNum(account, USER_ACCOUNT_SIZE);
			if (account.empty()) {
				continue;
			}
			if (account == "0") {
				return;
			}
			old_user = searchUserByAccount(account);
			if (old_user.getId() == ID_NOT_FOUND) {
				cout << "δ�ҵ���Ӧ���û�, ������ !" << endl;
				continue;
			}
			break;
		}
		cout << "��������û��ĵ�ǰ��Ϣ��" << endl;
		old_user.print();
	}

	// ��������ͼ������
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
		cout << "�������� " << count << " ��ͼ��" << endl;
	}

	// ���������û�����
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
		cout << "�������� " << count << " ���û�" << endl;
	}

	// ������ɽ��ļ�¼, 90���û�����990��ͼ��, ÿ������11��
	void generateData() {
		// д���û���ͼ��Ĳ�������
		readTestUsers();
		readTestBooks();
		int user[100];
		// 90���û�����
		int user_size = 90;
		// ��������û�����λ�õ�����
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
		// ����990��ͼ��
		int book_size = 990;
		// �������ͼ������λ�õ�����
		Tools::permutation(book_size, book);
		
		srand((unsigned)time(NULL));
		int book_index = 0;
		int borrow_count = 0;
		for (int i = 0; i < user_size; i++) {
			// ÿ������11��
			int ran = rand() % 11 + 1;
			User aUser = searchUserById(user[i]);
			if (aUser.getId() == ID_NOT_FOUND) { // �����ڸ��û�
				continue;
			}
			for (int j = 0; j < ran; j++) {
				Book aBook = searchBookById(book[book_index]);
				if (aBook.id == ID_NOT_FOUND) { // �����ڸ�ͼ��
					book_index++;
					continue;
				}
				Borrow aBorrow = Borrow(borrowTotal, aUser.getAccount(), aBook.id);
				if (book[book_index] % 2 == 0) { // ��idΪż����д�벻�黹��¼
					aBook.canBorrow = BOOK_CANNOT_BORROW;
					writeBookFile(aBook, aBook.id);
					writeBorrowFile(aBorrow, borrowTotal);
				} else { // ��idΪ������д��黹��¼
					aBorrow.isBack = IS_BACK;
					aBorrow.backActualTime = time(NULL) + ran + 10;
					writeBorrowFile(aBorrow, borrowTotal);
				}
				borrowTotal++;
				writeBorrowTotal();
				// ͼ��index ++
				book_index++;
				// ���ļ�¼����++
				borrow_count++;
			}
		}
		cout << "������ " << borrow_count << " �����ļ�¼" << endl;
	}
};

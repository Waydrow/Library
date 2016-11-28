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

// ͼ��, �û�, ����Ա��С
#define BOOK_SIZE				sizeof(Book)
#define USER_SIZE				sizeof(User)
#define ADMIN_SIZE				sizeof(Admin)

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

// ɾ����ID��Ϊ-1
#define ID_REMOVE				-1

// ��ѯ�û�ʱ�޷������޷�����NULL, ȡ��id����
#define ID_NOT_FOUND			-999

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


class Library {
private:
	/*
		 ͼ���ļ��д洢�ĵ�һ������ʵ�ʺ���, ��id��Ϊ�洢bookTotal��ֵ, �ӵڶ�����ʼ��1����
		 bookTotalΪͼ���ļ�����һ������� id, �����ǰ�ļ��洢��n����,
		 ���±���ı��ӦΪ bookTotal = n + 1
	*/
	int bookTotal;
	int userTotal;
	int isLogin = 0;
	User currentUser;
	// ���������
	ifstream inFile;
	ofstream outFile;

	// ��¼�¸ı���bookTotalֵ, ����bookTotalд��ͼ���ļ��е�һ����¼��id��
	void writeBookTotal() {
		outFile.open(BOOK_FILE, ios::in | ios::out | ios::binary);
		if (outFile.is_open()) {
			outFile.seekp(0, ios::beg);
			Book aBook = Book(bookTotal);
			outFile.write((char*)&aBook, BOOK_SIZE);
			cout << "�޸�bookTotal�ɹ�" << endl;
		} else {
			cout << "�޸�bookTotalʧ��" << endl;
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
			cout << "�޸�userTotal�ɹ�" << endl;
		} else {
			cout << "�޸�userTotalʧ��" << endl;
		}
		outFile.close();
	}

	// ϵͳ����ͼ����Ϣ��ʼ��
	void loadSystemBooks() {
		fstream iofile;
		iofile.open(BOOK_FILE, ios::in | ios::out | ios::binary);
		if (iofile.fail()) { // ��ʧ���򴴽����ļ�, ��д��bookTotal
			cout << "�����µ�ͼ���ļ��ɹ�!" << endl;
			iofile.close();
			iofile.open(BOOK_FILE, ios::out | ios::binary);
			Book aBook = Book(1);
			bookTotal = 1;
			iofile.write((char*)&aBook, BOOK_SIZE);
			iofile.close();
		} else {
			cout << "�ɹ���ԭ�е�ͼ���ļ�!" << endl;
			// �ƶ����ļ���ͷλ��
			iofile.seekg(0, ios::beg);
			Book aBook;
			iofile.read((char*)&aBook, BOOK_SIZE);
			// ��ȡ��һ�����id, ��ֵ��bookTotal
			bookTotal = aBook.id;
			iofile.close();
		}
		cout << "ϵͳ��ʼ��ͼ����Ϣ�ɹ� !" << endl;
	}

	// ϵͳ�����û���Ϣ��ʼ��
	void loadSystemUsers() {
		fstream iofile;
		iofile.open(USER_FILE, ios::in | ios::out | ios::binary);
		if (iofile.fail()) { // ��ʧ���򴴽����ļ�, ��д��userTotal
			cout << "�����µ��û��ļ��ɹ�!" << endl;
			iofile.close();
			iofile.open(USER_FILE, ios::out | ios::binary);
			User aUser = User(1);
			userTotal = 1;
			iofile.write((char*)&aUser, USER_SIZE);
			iofile.close();
		} else {
			cout << "�ɹ���ԭ�е��û��ļ�!" << endl;
			// �ƶ����ļ���ͷλ��
			iofile.seekg(0, ios::beg);
			User aUser;
			iofile.read((char*)&aUser, USER_SIZE);
			// ��ȡ��һ���û���id, ��ֵ��userTotal
			userTotal = aUser.getId();
			iofile.close();
		}
		cout << "ϵͳ��ʼ���û���Ϣ�ɹ� !" << endl;
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

	//��ͼ����Ϊ���ݽ��в���
	Book searchBookById(int num) {
		int flag = 0; // 0����û�鵽, 1�����鵽
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
		if (flag == 1) {
			return aBook;
		} else {
			return NULL;
		}
	}

	// ���û�ѧ��Ϊ���ݽ��в���
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
		if(flag == 0) {
			aUser.setId(ID_NOT_FOUND);
		}
		return aUser;
	}

	// ͨ��������ѯͼ��
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


	// �û���¼ȷ��
	void searchUserFileForLogin(string account, string password) {
		inFile.open(USER_FILE, ios::binary);
		if (inFile.is_open()) {
			User user = User();
			while (inFile.read((char*)&user, USER_SIZE)) {
				string _account(user.getAccount());
				string _password(user.getPassword());
				if (account==_account && password==_password) {
					isLogin = 1;
					currentUser = user;
					cout << "�û���¼�ɹ�" << endl;
					break;
				}
			}
		}
		inFile.close();
		if (isLogin == 0) {
			cout << "�˺Ż��������" << endl;
		}
	}
public:
	Library() {
		loadSystemBooks();
		loadSystemUsers();
	}

	//����ͼ��
	void addBook() {
		cout << "������ͼ����Ϣ(���� ���� ���)" << endl;
		char name[BOOK_NAME_SIZE];
		char author[BOOK_AUTHOR_SIZE];
		char introduction[BOOK_INTRO_SIZE];
		cin >> name >> author >> introduction;
		Book new_book = Book(bookTotal, name, author, introduction);
		writeBookFile(new_book, bookTotal);

		cout << "���ӳɹ�����Ϣ���£�" << endl;
		new_book.print();
		bookTotal++;
		writeBookTotal();
	}

	//�鿴ͼ����
	void displayBook() {
		cout << "�������ڵĿ����Ϣ��" << endl;
		inFile.open(BOOK_FILE, ios::in | ios::binary);
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
		cout << "������Ҫɾ����ͼ���ţ�";
		int num;
		cin >> num;
		Book old_book = searchBookById(num);//ͨ������ҵ��Ȿ��ļ�¼
		if (old_book == NULL) {
			cout << "δ�ҵ���Ӧ����, ������ !" << endl;
			return;
		}
		if (old_book.canBorrow == BOOK_CANNOT_BORROW) {
			cout << "�Ȿ�鱻������, ��ʱ����ɾ��Ŷ !" << endl;
			return;
		}
		cout << "������ɾ���Ȿ���������Ϣ��" << endl;
		old_book.print();
		string choice;
		cout << "ȷ��ɾ���Ȿ����?(y/n): ";
		cin >> choice;
		if (choice == "y" || choice == "Y") {
			Book aBook = Book(ID_REMOVE, "\0", "\0", "\0");
			writeBookFile(aBook, old_book.id);
			cout << "ɾ���ɹ� !" << endl;
		}
	}

	//���ͼ��
	void lendBook() {
		cout << "������Ҫ���ͼ���ţ�";
		int num;
		cin >> num;
		if (cin.fail()) {
			cerr << "��Ч������!\n";
			cin.clear();
			// ������뻺����
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return;
		}
		//ͨ������ҵ��Ȿ��ļ�¼
		Book old_book = searchBookById(num);
		if (old_book == NULL) {
			cout << "δ�ҵ���Ӧ����, ������ !" << endl;
			return;
		}
		old_book.print();
		if (old_book.canBorrow == BOOK_CANNOT_BORROW) {
			cout << "�Ȿ���Ѿ������˽�������" << endl;
			return;
		}
		string flag;
		cout << "ȷ�Ͻ��Ȿ����?(y/n): ";
		cin >> flag;
		if (flag == "y" || flag == "Y") {
			old_book.canBorrow = BOOK_CANNOT_BORROW; // ͼ��״̬��Ϊ���
			writeBookFile(old_book, old_book.id);
			cout << "����ɹ� !" << endl;
		}
	}

	//�޸�ͼ��
	void changeBook() {
		cout << "������Ҫ�޸ĵ�ͼ���ţ�";
		int num;
		cin >> num;
		Book old_book = searchBookById(num);
		if (old_book == NULL) {
			cout << "δ�ҵ���Ӧ����, ������ !" << endl;
			return;
		}
		if (old_book.canBorrow == BOOK_CANNOT_BORROW) {
			cout << "�Ȿ�鱻������, ��ʱ�����޸�Ŷ !" << endl;
			return;
		}
		cout << "�����Ȿ��ĵ�ǰ��Ϣ��" << endl;
		//��ʾ�Ȿ��֮ǰ����Ϣ
		old_book.print();
		cout << "�������޸ĺ��ͼ����Ϣ������ ���� ��飩" << endl;
		char name[BOOK_NAME_SIZE];
		char author[BOOK_AUTHOR_SIZE];
		char introduction[BOOK_INTRO_SIZE];
		cin >> name >> author >> introduction;
		Book new_book = Book(old_book.id, name, author, introduction);
		// д���ļ�
		writeBookFile(new_book, old_book.id);
		cout << "�Ȿ�����Ϣ���޸�Ϊ��" << endl;
		new_book.print();
	}
	
	//�黹ͼ��
	void backBook() {
		cout << "������Ҫ�黹��ͼ���ţ�";
		int num;
		cin >> num;
		//ͨ������ҵ��Ȿ��ļ�¼
		Book old_book = searchBookById(num);
		if (old_book == NULL) {
			cout << "δ�ҵ���Ӧ����, ������ !" << endl;
			return;
		}
		if (old_book.canBorrow == BOOK_CAN_BORROW) {
			cout << "�㻹û�н��Ȿ��Ŷ !" << endl;
			return;
		}
		string flag;
		cout << "ȷ�Ϲ黹�Ȿ����?(y/n): ";
		cin >> flag;
		if (flag == "y" || flag == "Y") {
			old_book.canBorrow = BOOK_CAN_BORROW;
			writeBookFile(old_book, old_book.id);
			cout << "����ɹ� !" << endl;
		}
	}

	// �û���ѯͼ��
	void userSearchBook() {
		cout << "������Ҫ��ѯ��ͼ������: " << endl;
		char nameInput[BOOK_NAME_SIZE];
		cin >> nameInput;
		if (strlen(nameInput) >= BOOK_NAME_SIZE) {
			cout << "�������ͼ����̫����, ������ !" << endl;
			return;
		}
		vector<Book> result = searchBookByName(nameInput);
		if (!result.empty()) {
			cout << "�����������ϵ������� " << result.size() << " ����" << endl;
			for (vector<int>::size_type ix = 0; ix != result.size(); ix++) {
				result[ix].print();
			}
		} else {
			cout << "û�ҵ���Ӧ����" << endl;
		}
	}


	// ����Ա��¼
	bool adminLogin() {
		cout << "���������Ա�ʺ�: " << endl;
		string account;
		cin >> account;
		if (account.size() >= USER_ACCOUNT_SIZE) {
			cout << "�ʺ�̫����!" << endl;
			return false;
		}
		cout << "���������Ա����: " << endl;
		string password;
		cin >> password;
		if (password.size() >= USER_PASSWORD_SIZE) {
			cout << "����̫���� !" << endl;
			return false;
		}
		if (account == "222" && password == "222") {
			cout << "��¼�ɹ� !" << endl;
			return true;
		} else {
			cout << "�ʺŻ���������, ������ !" << endl;
			return false;
		}
	}
	// �û���¼
	bool userLogin() {
		cout << "������ѧ��: " << endl;
		string account;
		cin >> account;
		if (account.size() >= USER_ACCOUNT_SIZE) {
			cout << "�ʺ�̫����!" << endl;
			return false;
		}
		cout << "�������û�����: " << endl;
		string password;
		cin >> password;
		if (password.size() >= USER_PASSWORD_SIZE) {
			cout << "����̫���� !" << endl;
			return false;
		}
		searchUserFileForLogin(account, password);
		if (isLogin == 1) {
			return true;
		} else {
			return false;
		}
	}

	//�����û�
	void addUser() {
		cout << "�������û���Ϣ(ѧ��, ����, ����)" << endl;
		string account, password, name;
		cin >> account >> password >> name;
		if (account.size() >= USER_ACCOUNT_SIZE) {
			cout << "ѧ��̫���� !" << endl;
			return;
		}
		if (password.size() >= USER_PASSWORD_SIZE) {
			cout << "����̫���� !" << endl;
			return;
		}
		if (name.size() >= USER_NAME_SIZE) {
			cout << "����̫���� !" << endl;
			return;
		}
		User new_user = searchUserByAccount(account);
		if (new_user.getId() != ID_NOT_FOUND) {
			cout << "ѧ���Ѵ��� !" << endl;
			return;
		}
		new_user = User(userTotal, account, password, name);
		writeUserFile(new_user, userTotal);
		cout << "���ӳɹ�����Ϣ���£�" << endl;
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
		cout << "������Ҫɾ�����û�ѧ�ţ�";
		string account;
		cin >> account;
		if (account.size() >= USER_ACCOUNT_SIZE) {
			cout << "ѧ������̫���� !" << endl;
			return;
		}
		User old_user = searchUserByAccount(account);//ͨ��ѧ���ҵ�����û��ļ�¼
		if (old_user.getId() == ID_NOT_FOUND) {
			cout << "δ�ҵ���Ӧ���û�, ������ !" << endl;
			return;
		}
		cout << "������ɾ������û���������Ϣ��" << endl;
		old_user.print();
		string choice;
		cout << "ȷ��ɾ������û���?(y/n): ";
		cin >> choice;
		if (choice == "y" || choice == "Y") {
			User aUser = User(ID_REMOVE, '\0', '\0', '\0');
			writeUserFile(aUser, old_user.getId());
			cout << "ɾ���ɹ� !" << endl;
		}
	}


	//�޸��û�
	void changeUser() {
		cout << "������Ҫ�޸ĵ��û�ѧ�ţ�";
		string account;
		cin >> account;
		if (account.size() >= USER_ACCOUNT_SIZE) {
			cout << "ѧ��̫���� !" << endl;
			return;
		}
		User old_user = searchUserByAccount(account);
		if (old_user.getId() == ID_NOT_FOUND) {
			cout << "δ�ҵ���Ӧ���û�, ������ !" << endl;
			return;
		}
		
		cout << "��������û��ĵ�ǰ��Ϣ��" << endl;
		old_user.print();
		cout << "�������޸ĺ���û���Ϣ������ ������" << endl;
		string password, name;
		cin >> password >> name;
		if (password.size() >= USER_PASSWORD_SIZE) {
			cout << "����̫���� !" << endl;
			return;
		}
		if (name.size() >= USER_NAME_SIZE) {
			cout << "����̫���� !" << endl;
			return;
		}

		User new_user = User(old_user.getId(), account, password, name);
		// д���ļ�
		writeUserFile(new_user, old_user.getId());
		cout << "����û�����Ϣ���޸�Ϊ��" << endl;
		new_user.print();
	}

	
};
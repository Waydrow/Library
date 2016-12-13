#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include "Library.h"
using namespace std;

bool isUserLogin;
int isAdminLogin;

int main() {
	// Tools::writeTestBooks();
	// Tools::writeTestUsers();
	int flag = 1;
	while (flag) {
		flag = 1;
		isUserLogin = false;
		isAdminLogin = 0;
		Library myLib = Library();
		while (true) {
			string loginOption;
			cout << "���ѡ��: 1.����Ա 2.ѧ�� 3.�˳�" << endl << "������: ";
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

		// Ϊ��������Ա
		if (isAdminLogin == 2) {
			while (true) {
				cout << "��ѡ�����" << endl;
				cout << "------------------------------------------------------------------------------------" << endl;
				cout << setiosflags(ios::left);
				cout << setw(30) << "1-----����û�" << setw(30) << "7 -----�������" << setw(30) << "12----��ӹ���Ա" << endl;
				cout << setw(30) << "2-----�޸��û���Ϣ" << setw(30) << "8 -----�޸�ͼ����Ϣ" << setw(30) << "13----�޸Ĺ���Ա��Ϣ" << endl;
				cout << setw(30) << "3-----ɾ���û�" << setw(30) << "9 -----ɾ������" << setw(30) << "14----ɾ������Ա" << endl;
				cout << setw(30) << "4-----�鿴�����û�" << setw(30) << "10-----�鿴ͼ����" << setw(30) << "15----�鿴���й���Ա" << endl;
				cout << setw(30) << "5-----��ѯ�û�" << setw(30) << "11-----��ѯͼ��" << setw(30) << "16---���벢���ɲ�������" << endl;
				cout << setw(30) << "6-----��ѯ�û����ļ�¼" << setw(30) << "0 -----ע��" << endl;
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

		// Ϊ��ͨ����Ա
		if (isAdminLogin == 1) {
			while (true) {
				cout << "��ѡ�����" << endl;
				cout << "------------------------------------------------------------------------------------" << endl;
				cout << setiosflags(ios::left);
				cout << setw(30) << "1-----����û�" << setw(30) << "7 -----�������" << endl;
				cout << setw(30) << "2-----�޸��û���Ϣ" << setw(30) << "8 -----�޸�ͼ����Ϣ" << endl;
				cout << setw(30) << "3-----ɾ���û�" << setw(30) << "9 -----ɾ������" << endl;
				cout << setw(30) << "4-----�鿴�����û�" << setw(30) << "10-----�鿴ͼ����" << endl;
				cout << setw(30) << "5-----��ѯ�û�" << setw(30) << "11-----��ѯͼ��" << endl;
				cout << setw(30) << "6-----��ѯ�û����ļ�¼" << setw(30) << "0 -----ע��" << endl;
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
				cout << "��ѡ�����" << endl;
				cout << "--------------------------------" << endl;
				cout << "1----�鿴���" << endl;
				cout << "2----��ѯͼ��" << endl;
				cout << "3----����" << endl;
				cout << "4----�黹" << endl;
				cout << "5----����" << endl;
				cout << "6----��ѯ���ļ�¼" << endl;
				cout << "0----ע��" << endl;
				cout << "--------------------------------" << endl;

				string userOption;
				Tools::inputChoice(userOption);

				/*
				�˴�������switch case���ж�, ��Ϊc++�в�֧��case string
				option����int��char��ԭ��Ϊ��ֹ�û���������, �����ݴ���
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

#include<iostream>
#include<fstream>
#include<string>

using namespace std;

void write_books()
{
    ofstream ofs;
    ofs.open("data/book111.txt");
    int count = 0;
    if (ofs.is_open())
    {
        string name = "�߼����";
        string author = "��������";
        string introduction = "һ���߼���̵���";
        for (int i = 1000; i < 101000; i++)
        {
            ofs << name << i << " " << author << " " << introduction << i << "\n";
            count++;
        }
    }
    ofs << flush;
    cout<< "��д�� " <<count<<" ����"<<endl;
    ofs.close();
}

void write_users()
{
    ofstream ofs;
    ofs.open("data/user111.txt");
    int count = 0;
    if (ofs.is_open())
    {
        long long account = 14020031144;
        int password = 222;
        string name = "ѧ��";
        for (int i = 0; i < 100; i++)
        {
            ofs << account + i << " " << password << " " << name << i << "\n";
            count++;
        }
    }
    ofs << flush;
    cout<< "��д�� " <<count<<" ���û�"<<endl;
    ofs.close();
}

int main ()
{
    cout<<"please choose: 1. write books  2. write users"<<endl;
    int choice;
    cin>>choice;
    switch(choice)
    {
    case 1:
        write_books();
        break;
    case 2:
        write_users();
        break;
    default:
        break;
    }

    return 0;
}

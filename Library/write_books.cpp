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
        string name = "高级编程";
        string author = "测试作者";
        string introduction = "一本高级编程的书";
        for (int i = 1000; i < 101000; i++)
        {
            ofs << name << i << " " << author << " " << introduction << i << "\n";
            count++;
        }
    }
    ofs << flush;
    cout<< "共写入 " <<count<<" 本书"<<endl;
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
        string name = "学生";
        for (int i = 0; i < 100; i++)
        {
            ofs << account + i << " " << password << " " << name << i << "\n";
            count++;
        }
    }
    ofs << flush;
    cout<< "共写入 " <<count<<" 个用户"<<endl;
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

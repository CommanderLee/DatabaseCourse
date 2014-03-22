/*
ID: lizhen11
PROG:
LANG: C++
*/
#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdio>
#include<string>
#include<cstdlib>

using namespace std;

int main()
{
    ofstream fout("c.txt");
    ofstream qout("c_q.txt");
    
    for (int i = 0; i < 50; ++i)
    {
		for (int j = 0; j < 50; ++j)
			fout << char('a' + rand() % 5);
		fout << endl;
	}
    
    for (int j = 0; j < 50; ++j)
		qout << char('a' + rand() % 5);
	
	fout.close();
	qout.close();
    return 0;
}

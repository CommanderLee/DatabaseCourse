#include "AEE.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
	AEE aee;

	vector<EDExtractResult> resultED;
	vector<JaccardExtractResult> resultJaccard;

	unsigned q = 3, edThreshold = 2;
	double jaccardThreshold = 0.85;

	//aee.createIndex(argv[1], q);
	//aee.aeeJaccard(argv[2], jaccardThreshold, resultJaccard);
	//aee.aeeED(argv[2], edThreshold, resultED);
	//aee.createIndex("aee_entity", q);

	//ifstream fin("aee_doc");
	//string doc;
	//for (int i = 0; getline(fin, doc); ++i)
	//{
	//	cout << "No." << i << "DOC:" << doc << endl;
	//	aee.aeeED(doc.c_str(), edThreshold, resultED);

	//	for (vector<EDExtractResult>::iterator it = resultED.begin(); it != resultED.end(); ++it)
	//	{
	//		cout << it->id << ' ' << it->pos << ' ' << it->len <<  " Sim:" << it->sim << endl;
	//	}
	//	system("pause");
	//}
	
	aee.createIndex("small_aee_entity", q);
	aee.aeeED("it is declarativewlodzimierz drabenticlp", edThreshold, resultED);

	for (vector<EDExtractResult>::iterator it = resultED.begin(); it != resultED.end(); ++it)
	{
		cout << it->id << ' ' << it->pos << ' ' << it->len <<  " Sim:" << it->sim << endl;
	}
	
	system("pause");
	return 0;
}


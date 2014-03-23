#include "SimSearcher.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

int main(int argc, char **argv)
{
	SimSearcher searcher;

	vector<pair<unsigned, unsigned> > resultED;
	vector<pair<unsigned, double> > resultJaccard;

	unsigned q = 2, edThreshold = 40;
	double jaccardThreshold = 0.45;

	ofstream fout("c.txt");
	ofstream qout("c_q.txt");

	srand(time(NULL));
	for (int i = 0; i < 200; ++i)
	{
		for (int j = 0; j < 50; ++j)
			fout << char('a' + rand() % 5);
		fout << endl;
	}

	searcher.createIndex("c.txt", q);

	// while(1)
	// {
		ostringstream sout;
		for (int j = 0; j < 50; ++j)
			sout << char('a' + rand() % 5);
		qout << sout.str() << endl;
		cout << sout.str() << endl;
		searcher.searchED(sout.str().c_str(), edThreshold, resultED);
	// }
	
	fout.close();
	qout.close();

	
	// searcher.createIndex(argv[1], q);
	// 
	// searcher.searchED(sout.str().c_str(), edThreshold, resultED);
	// searcher.searchED("bbbbbabbbbbaabbbbaabaaaaabbaabababbabababaaaaaaabb", edThreshold, resultED);
	// searcher.searchED(argv[3], atoi(argv[2]), resultED);
	for (vector<pair<unsigned, unsigned> >::iterator it(resultED.begin()); it != resultED.end(); ++it)
	{
		cout << "id : " << it->first << ' ' << "ED : " << it->second << endl;
	}
	//searcher.searchED("bbbbbabbbbbaabbbbaabaaaaabbaabababbabababaaaaaaabb", edThreshold, resultED);

	searcher.searchJaccard(sout.str().c_str(), jaccardThreshold, resultJaccard);
	for (vector<pair<unsigned, double> >::iterator it(resultJaccard.begin()); it != resultJaccard.end(); ++it)
	{
		cout << "id : " << it->first << ' ' << "Jaccard : " << it->second << endl;
	}

	system("pause");
	return 0;
}


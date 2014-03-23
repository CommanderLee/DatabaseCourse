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

	unsigned q = 2, edThreshold = 14;
	double jaccardThreshold = 0.85;

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
		// cout << sout.str() << endl;
		searcher.searchED(sout.str().c_str(), edThreshold, resultED);
	// }
	
	fout.close();
	qout.close();

	
	// searcher.createIndex(argv[1], q);
	// searcher.searchJaccard("query", jaccardThreshold, resultJaccard);
	// searcher.searchED(sout.str().c_str(), edThreshold, resultED);
	// searcher.searchED("bbbbbabbbbbaabbbbaabaaaaabbaabababbabababaaaaaaabb", edThreshold, resultED);
	// searcher.searchED(argv[3], atoi(argv[2]), resultED);
	for (vector<pair<unsigned, unsigned> >::iterator it(resultED.begin()); it != resultED.end(); ++it)
	{
		cout << "id : " << it->first << ' ' << "ED : " << it->second << endl;
	}
	//searcher.searchED("bbbbbabbbbbaabbbbaabaaaaabbaabababbabababaaaaaaabb", edThreshold, resultED);

	system("pause");
	return 0;
}


#include "SimSearcher.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	SimSearcher searcher;

	vector<pair<unsigned, unsigned> > resultED;
	vector<pair<unsigned, double> > resultJaccard;

	unsigned q = 2, edThreshold = 30;
	double jaccardThreshold = 0.85;

	searcher.createIndex("c.txt", q);
	// searcher.createIndex(argv[1], q);
	// searcher.searchJaccard("query", jaccardThreshold, resultJaccard);
	searcher.searchED("becdaaababdccceaccadbeddaaaadbaacdebabebcaeabbaaac", edThreshold, resultED);
	// searcher.searchED(argv[3], atoi(argv[2]), resultED);
	for (vector<pair<unsigned, unsigned> >::iterator it(resultED.begin()); it != resultED.end(); ++it)
	{
		cout << "id : " << it->first << ' ' << "ED : " << it->second << endl;
	}
	

	system("pause");
	return 0;
}


#include "SimJoiner.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{

	SimJoiner joiner;

	vector<EDJoinResult> resultED;
	vector<JaccardJoinResult> resultJaccard;

	unsigned q = 7, edThreshold = 10;
	double jaccardThreshold = 0.5;

	joiner.joinJaccard("new.txt", "new.txt", q, jaccardThreshold, resultJaccard);
	for (vector<JaccardJoinResult>::iterator it(resultJaccard.begin()); it != resultJaccard.end(); ++it)
	{
		cout << "id1: " << it->id1 << " id2: " << it->id2 << " ed: " << it->s << endl;
	}
	// cout << "---------------------------" << endl;
	// joiner.joinED(argv[1], argv[2], q, edThreshold, resultED);
	//joiner.joinED("new.txt", "new.txt", q, edThreshold, resultED);

	//for (vector<EDJoinResult>::iterator it(resultED.begin()); it != resultED.end(); ++it)
	//{
	//	cout << "id1: " << it->id1 << " id2: " << it->id2 << " ed: " << it->s << endl;
	//}
	
	system("pause");
	return 0;
}


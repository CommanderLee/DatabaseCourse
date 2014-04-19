#include "SimJoiner.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{

	SimJoiner joiner;

	vector<EDJoinResult> resultED;
	vector<JaccardJoinResult> resultJaccard;

	unsigned q = 2, edThreshold = 2;
	double jaccardThreshold = 0.85;

	// joiner.joinJaccard(argv[1], argv[2], q, jaccardThreshold, resultJaccard);
	// joiner.joinED(argv[1], argv[2], q, edThreshold, resultED);
	joiner.joinED("input1.txt", "input2.txt", q, edThreshold, resultED);

	for (vector<EDJoinResult>::iterator it(resultED.begin()); it != resultED.end(); ++it)
	{
		cout << "id1: " << it->id1 << " id2: " << it->id2 << " ed: " << it->s << endl;
	}
	
	system("pause");
	return 0;
}


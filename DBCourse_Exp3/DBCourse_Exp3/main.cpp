#include "AEE.h"

using namespace std;

int main(int argc, char **argv)
{
	AEE aee;

	vector<EDExtractResult> resultED;
	vector<JaccardExtractResult> resultJaccard;

	unsigned q = 3, edThreshold = 2;
	double jaccardThreshold = 0.85;

	aee.createIndex(argv[1], q);
	aee.aeeJaccard(argv[2], jaccardThreshold, resultJaccard);
	aee.aeeED(argv[2], edThreshold, resultED);

	return 0;
}


#include "AEE.h"

using namespace std;

AEE::AEE()
{
}

AEE::~AEE()
{
}

int AEE::createIndex(const char *filename, unsigned q)
{
	return SUCCESS;
}

int AEE::aeeJaccard(const char *docfilename, double threshold, vector<JaccardExtractResult> &result)
{
	result.clear();
	return SUCCESS;
}

int AEE::aeeED(const char *docfilename, unsigned threshold, vector<EDExtractResult> &result)
{
	result.clear();
	return SUCCESS;
}


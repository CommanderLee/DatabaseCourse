#pragma once
#include <vector>

template <typename _IDType, typename _PosType, typename _LenType, typename _SimType>
struct ExtractResult
{
	_IDType id;
	_PosType pos;
	_LenType len;
	_SimType sim;
};

typedef ExtractResult<unsigned, unsigned, unsigned, unsigned> EDExtractResult;
typedef ExtractResult<unsigned, unsigned, unsigned, double> JaccardExtractResult;

const int SUCCESS = 0;
const int FAILURE = 1;

class AEE {
public:
	AEE();
	~AEE();

	int createIndex(const char *filename, unsigned q);
	int aeeJaccard(const char *docfilename, double threshold, std::vector<JaccardExtractResult> &result);
	int aeeED(const char *docfilename, unsigned threshold, std::vector<EDExtractResult> &result);
};


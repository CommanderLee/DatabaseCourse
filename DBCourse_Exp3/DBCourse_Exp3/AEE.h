#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>

using namespace std;

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
	/** Parameter */
	unsigned							qGram;
	unsigned							EGramMax, EGramMin;

	/** About Entities(Dictionary) */
	vector<string>						entityList;
	unordered_map<string, vector<int>>	entityInvertList;
	vector<vector<int>>					entityPosList;

	/** About Docs */
	unordered_map<string, vector<int>>	docInvertList;

	/** 'Temporal' Variables */
	//unordered_set<string, int>		existGram;		// handle repeated grams

	/** Help Functions */
	//string checkRepeatedGram(const string& str);
	void getCandidateWindows(int eID, int Tl, int TUpper);
	void binarySpan(int st, int ed, int eID, int TUpper);
	int binaryShift(int st, int ed, int eID, int Tl, int TUpper);
public:
	AEE();
	~AEE();

	int createIndex(const char *filename, unsigned q);
	int aeeJaccard(const char *doc, double threshold, std::vector<JaccardExtractResult> &result);
	int aeeED(const char *doc, unsigned threshold, std::vector<EDExtractResult> &result);
};

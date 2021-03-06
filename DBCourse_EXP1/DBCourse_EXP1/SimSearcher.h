#pragma once
#include <vector>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <queue>

const int SUCCESS = 0;
const int FAILURE = 1;

using namespace std;
//const int MAX_LEN = 30;

class SimSearcher
{
	/* 'Permanent' variable */
	unsigned						qGram;			// q: parameter of q-gram algorithm
	unsigned						maxLength;		// max length of the invert table
	unsigned						shortestStrLen;	// shortest length of the string 

	/* vector of empty ID and sorted gram list(vector) */
	vector<unsigned>				emptyID;
	vector<string>	wordList;

	vector<vector<unsigned>>		sortGramList;
	unordered_map<string, unsigned>	gram2id;

	/* 'Temporal' variable */
	vector<unsigned>					startPos;		// start position of each list
	unordered_map<string, unsigned>		countGram;		// handle repeated grams
	vector<unsigned>					possibleList;	// store possible index in the sortGram.
	vector<unsigned>					countID;		// countID[i]: appearance times of wordList[i]
	unordered_set<unsigned>				shortResult;	// candidate from the 'short' part (id)
	unordered_set<unsigned>				longResult;		// candidate from the 'long' part (id)

	vector<pair<unsigned, unsigned>>	poppedLists;	// pair: <wordID, sorted gram list ID>

	double getJac(const char *query, const char *word);
	void doMakeGrams(const char *query);
	void doMergeSkip(const char *query, unsigned th, int shortNum);
	void doMergeOpt(unsigned start, unsigned end, unsigned th);
	// void bruteForceED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, double> > &result);

public:
	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);
};


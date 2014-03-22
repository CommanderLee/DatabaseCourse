#pragma once
#include <vector>
#include <utility>
#include <unordered_map>

const int SUCCESS = 0;
const int FAILURE = 1;

//const int MAX_LEN = 30;

class SimSearcher
{
	/* q: parameter of q-gram algorithm */
	unsigned	qGram;
	unsigned	maxLength;

	//int			distance[MAX_LEN][MAX_LEN];		// distance[query][word]

	/* vector of empty ID and sorted gram list(vector) */
	std::vector<unsigned>		emptyID;
	std::vector<std::string>	wordList;

	std::vector<std::pair<std::string, std::vector<unsigned>>>	sortGram;
	std::unordered_map<std::string, unsigned>					existGram;

	void doMergeSkip(const char *query, unsigned th, std::vector<unsigned> &possibleSet, int shortNum, std::vector<unsigned> &shortResult);
	void doMergeOpt(std::vector<unsigned> &shortResult, std::vector<unsigned> &possibleSet, unsigned st, unsigned ed, unsigned th, 
std::vector<unsigned> &longResult);

	//unsigned getED(const char *query, const char *word, int th);

public:
	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);
};


#pragma once
#include <vector>
#include <utility>

const int SUCCESS = 0;
const int FAILURE = 1;

class SimSearcher
{
	/* q: parameter of q-gram algorithm */
	unsigned	qGram;

	/* vector of empty ID and sorted gram list(vector) */
	std::vector<unsigned>				emptyID;
	std::vector<std::vector<unsigned>>	sortGram;
public:
	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);
};


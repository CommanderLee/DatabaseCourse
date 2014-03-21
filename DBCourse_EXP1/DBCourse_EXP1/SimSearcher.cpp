#include "SimSearcher.h"
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <unordered_map>

using namespace std;

SimSearcher::SimSearcher()
{
	qGram = 0;
	emptyID.clear();
}

SimSearcher::~SimSearcher()
{
}

int SimSearcher::createIndex(const char *filename, unsigned q)
{
	ifstream fin(filename);
	qGram = q;

	/* Create invert-table: sortGram */
	string str;
	unsigned id(0);
	unordered_map<string, vector<int>> originalGram;
	originalGram.clear();
	for (;fin >> str; ++id)
	{
		int len = str.length();
		/* Too short: seems empty */
		if (len < qGram)
		{
			emptyID.push_back(id);
		}
		else
		{
			for (int i = 0; i <= len - qGram; ++i)
			{
				originalGram[str.substr(i, qGram)].push_back(id);
			}
		}
	}

	/* Sort the originalGram by the length of the id list(vector) */

	/* Check the invert-table: sortGram */
	ofstream logout("log.txt");
	for (unordered_map<string, vector<int>>::iterator it(originalGram.begin()); it != originalGram.end(); ++it)
	{
		logout << it->first << ':';
		vector<int> _vec = it->second;
		for (vector<int>::iterator _it(_vec.begin()); _it != _vec.end(); ++_it)
		{
			logout << *_it << ',';
		}
		logout << endl;
	}
	logout.close();
	
	fin.close();
	return SUCCESS;
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
	result.clear();
	return SUCCESS;
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
	result.clear();
	// T-occurrence
	unsigned T = strlen(query) - qGram + 1 - threshold * qGram;


	return SUCCESS;
}


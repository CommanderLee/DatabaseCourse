#include "SimSearcher.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
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

bool gramCompare(const pair<string, vector<unsigned>> a, const pair<string, vector<unsigned>> b)
{
	return a.second.size() < b.second.size();
}

int SimSearcher::createIndex(const char *filename, unsigned q)
{
	ifstream fin(filename);
	qGram = q;

	/* Create invert-table: sortGram */
	string str;
	unsigned id(0);
	unordered_map<string, vector<unsigned>> originalGram;
	originalGram.clear();
	for (;fin >> str; ++id)
	{
		unsigned len = str.length();
		/* Too short: seems empty */
		if (len < qGram)
		{
			emptyID.push_back(id);
		}
		/* Push back into original gram; sort later */
		else
		{
			for (int i = 0; i <= len - qGram; ++i)
			{
				originalGram[str.substr(i, qGram)].push_back(id);
			}
		}
	}

	/* Sort the originalGram by the length of the id list(vector) */
	for (unordered_map<string, vector<unsigned>>::iterator it(originalGram.begin()); it != originalGram.end(); ++it)
	{
		sortGram.push_back(*it);
	}
	sort(sortGram.begin(), sortGram.end(), gramCompare);
	/* Link the gram(string) with the vector index(unsigned) with unordered_map */
	unsigned len = sortGram.size();
	for (unsigned i = 0; i < len; ++i)
	{
		existGram[sortGram[i].first] = i;
	}
	
	

	/* Check the original invert-table */
/*	ofstream logout("log.txt");
	for (unordered_map<string, vector<unsigned>>::iterator it(originalGram.begin()); it != originalGram.end(); ++it)
	{
		logout << it->first << ':';
		vector<unsigned> _vec = it->second;
		for (vector<unsigned>::iterator _it(_vec.begin()); _it != _vec.end(); ++_it)
		{
			logout << *_it << ',';
		}
		logout << endl;
	}
	logout.close();
*/
	/* Check the sorted invert-table */
/*	ofstream logsout("log_sorted.txt");
	for (vector<pair<string, vector<unsigned>>>::iterator it(sortGram.begin()); it != sortGram.end(); ++it)
	{
		logsout << (it - sortGram.begin()) << ' ' << it->first << ':';
		vector<unsigned> _vec = it->second;
		for (vector<unsigned>::iterator _it(_vec.begin()); _it != _vec.end(); ++_it)
		{
			logsout << *_it << ',';
		}
		logsout << endl;
	}
*/	
	/* Check the exist gram map */
/*	logsout << existGram["swe"] << endl;
	logsout << existGram["fff"] << endl;
	logsout.close();
*/
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


#include "SimSearcher.h"
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <map>

using namespace std;

SimSearcher::SimSearcher()
{
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
	map<string, list<int>> sortGram;
	sortGram.clear();
	for (;fin >> str; ++id)
	{
		int len = str.length();
		if (len < qGram)
		{
			emptyID.push_back(id);
		} 
		else
		{
			for (int i = 0; i <= len - qGram; ++i)
			{
				sortGram[str.substr(i, qGram)].push_back(id);
			}
		}
	}



	/* Check the invert-table: sortGram */
	ofstream logout("log.txt");
	for (map<string, list<int>>::iterator it(sortGram.begin()); it != sortGram.end(); ++it)
	{
		logout << it->first << ':';
		list<int> _list = it->second;
		for (list<int>::iterator _it(_list.begin()); _it != _list.end(); ++_it)
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


#include "SimSearcher.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <cmath>
#include <queue>

using namespace std;

SimSearcher::SimSearcher()
{
	qGram = 0;
	emptyID.clear();
}

SimSearcher::~SimSearcher()
{
}

// Sort from short to long.
bool gramCompare(const pair<string, vector<unsigned>> a, const pair<string, vector<unsigned>> b)
{
	return a.second.size() < b.second.size();
}

// Compare function for the heap( in MergeSkip algorithm )
struct heapCompare
{
	bool operator() (const pair<unsigned, unsigned> a, const pair<unsigned, unsigned> b)
	{
		return a.first < b.first;
	}
};

// Sort the id in the result
bool resultCompare(const pair<unsigned, unsigned> a, const pair<unsigned, unsigned> b)
{
	return a.first < b.first;
}

int SimSearcher::createIndex(const char *filename, unsigned q)
{
	ifstream fin(filename);
	qGram = q;

	/* Create invert-table: sortGram */
	string str;
	unsigned id(0);
	unordered_map<string, vector<unsigned>> originalGram;
	unordered_map<string, unsigned>			countGram;
	originalGram.clear();
	for (;fin >> str; ++id)
	{
		wordList.push_back(str);
		unsigned len = str.length();
		/* Too short: seems empty */
		if (len < qGram)
		{
			emptyID.push_back(id);
		}
		/* Push back into original gram; sort later */
		else
		{
			countGram.clear();
			for (int i = 0; i <= len - qGram; ++i)
			{
				string gram(str.substr(i, qGram));
				/* Not found: first appearance */
				if (countGram.find(gram) == countGram.end())
				{
					originalGram[gram].push_back(id);
					countGram[gram] = 0;
				}
				/* Not first */
				else
				{
					unsigned num = countGram[gram]++;
					ostringstream sout;
					sout << gram << num;
					originalGram[sout.str()].push_back(id);
					countGram[sout.str()] = 0;
				}
			}
		}
	}

	/* Sort the originalGram by the length of the id list(vector) */
	for (unordered_map<string, vector<unsigned>>::iterator it(originalGram.begin()); it != originalGram.end(); ++it)
	{
		sortGram.push_back(*it);
	}
	sort(sortGram.begin(), sortGram.end(), gramCompare);
	maxLength = sortGram.back().second.size();

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

unsigned getED(const char *query, string word)
{

	return 0;
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
	result.clear();
	/* T-occurrence threshold */
	int T = strlen(query) - qGram + 1 - threshold * qGram;
	const double mu = 0.0085;

	cout << "T = " << T << endl;

	/* Using DivideSkip algorithm */
	if (T > 0)
	{
		unsigned L = T / (mu * log(double(maxLength)) + 1);		// important parameter in the DivideSkip algorithm

		cout << "L = " << L << endl;

		vector<unsigned> possibleSet;							// store possible index in the sortGram.
		string queryStr(query);
		int len = queryStr.length();
		/* parse the grams */
		if (len >= qGram)
		{
			/* Create the possible(initial) set of gram lists */
			for (int i = 0; i <= len - qGram; ++i)
			{
				string gram(queryStr.substr(i, qGram));
				unordered_map<string, unsigned>::iterator findRes = existGram.find(gram);
				/* this gram exists in the input file */
				if (findRes != existGram.end())
				{
					possibleSet.push_back(findRes->second);
				} 		
			}
			sort(possibleSet.begin(), possibleSet.end());

			int setNum = possibleSet.size();
			int shortNum = setNum - L;			
			vector<unsigned> shortResult;		// candidate from the 'short' part

			/* Use MergeSkip algorithm on L_short set, if not empty. */
			if (shortNum > 0)
			{
				// pair: <wordID, possible gram list ID>
				priority_queue<pair<unsigned, unsigned>, vector<pair<unsigned, unsigned>>, heapCompare> heap;
				vector<pair<unsigned, unsigned>> poppedLists;	// pair: <wordID, possible gram list ID>
				unsigned topVal;
				unsigned *startPos;
				startPos = new unsigned [shortNum];				// start position of each possible gram list( mark down the index )
				memset(startPos, 1, sizeof(startPos));
				int cnt(0);
				int th = T - L;
				/* Initialize the heap */
				for (int i = 0; i < shortNum; ++i)
				{
					heap.push(make_pair(sortGram[possibleSet[i]].second.front(), i));
				}
				shortResult.clear();
				
				/* MergeSkip */
				while (!heap.empty())
				{
					topVal = heap.top().first;
					cnt = 0;
					poppedLists.clear();
					while (!heap.empty() && heap.top().first == topVal)
					{
						++cnt;
						poppedLists.push_back(heap.top());
						heap.pop();
					}
					if (cnt >= th)
					{
						shortResult.push_back(topVal);
						for (vector<pair<unsigned, unsigned>>::iterator it(poppedLists.begin()); it != poppedLists.end(); ++it)
						{
							if (startPos[it->second] < sortGram[possibleSet[it->second]].second.size())
							{
								heap.push(make_pair(sortGram[possibleSet[it->second]].second[startPos[it->second]], it->second));
								++startPos[it->second];
							}
						}
						
					}
					else
					{
						for (int i = 0; i < th - 1 - cnt; ++i)
						{
							poppedLists.push_back(heap.top());
							heap.pop();
						}
						topVal = heap.top().first;
						for (vector<pair<unsigned, unsigned>>::iterator it(poppedLists.begin()); it != poppedLists.end(); ++it)
						{
							vector<unsigned>::iterator findRes = lower_bound(sortGram[possibleSet[it->second]].second.begin(), \
								sortGram[possibleSet[it->second]].second.end(), topVal);
							if (findRes != sortGram[possibleSet[it->second]].second.end())
							{
								heap.push(make_pair(*findRes, it->second));
							}
							startPos[it->second] = findRes - sortGram[possibleSet[it->second]].second.begin() + 1;
						}
					}
				}
				delete [] startPos;

				/* MergeOpt & get real ED */
				unsigned ed(0);
				for (vector<unsigned>::iterator it(shortResult.begin()); it != shortResult.end(); ++it)
				{
					/* For each 'long' lists */
					cnt = 0;
					for (int i = shortNum; i < setNum; ++i)
					{
						if (binary_search(sortGram[possibleSet[i]].second.begin(), sortGram[possibleSet[i]].second.end(), *it))
						{
							++cnt;
						}
					}
					ed = getED(query, wordList[*it]);
					if (cnt >= L && ed <= threshold)
					{
						result.push_back(make_pair(*it, ed));
					}
				}

				sort(result.begin(), result.end(), resultCompare);
			} 
			else
			{
			}
			
		}
		/* The query word is too short: ? */
		else
		{

		}
	} 
	/* Just check it one by one */
	else
	{
		cout << "T <= 0:" << endl;

	}

	return SUCCESS;
}


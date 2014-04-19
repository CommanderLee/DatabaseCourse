#include "SimJoiner.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <algorithm>

using namespace std;

const int MAX_LEN = 3000;

SimJoiner::SimJoiner()
{
}

SimJoiner::~SimJoiner()
{
}

int SimJoiner::joinJaccard(const char *filename1, const char *filename2, unsigned q, double threshold, vector<JaccardJoinResult> &result)
{
	result.clear();
	return SUCCESS;
}

unsigned getED(string& str1, string& str2, unsigned threshold)
{
	int len1(str1.length()), len2(str2.length());
	int ed(0), th(threshold);

	static int distance[MAX_LEN][MAX_LEN];

	if (abs(len1 - len2) <= th)
	{
		for (int i = 0; i <= len1; ++i)
		{
			distance[i][0] = i;
		}
		for (int j = 0; j <= len2; ++j)
		{
			distance[0][j] = j;
		}

		int st2, ed2, tmpMin(th + 1);
		for (int i = 1; i <= len1; ++i)
		{
			st2 = max(1, i - th);
			ed2 = min(len2, i + th);
			tmpMin = th + 1;
			for (int j = st2; j <= ed2; ++j)
			{
				// query[q - 1] is the q_th char of query
				if (str1[i - 1] == str2[j - 1])
				{
					distance[i][j] = distance[i - 1][j - 1];
				} 
				else
				{
					distance[i][j] = distance[i - 1][j - 1] + 1;
				}

				if (abs(i - 1 - j) <= th && distance[i][j] > distance[i - 1][j] + 1)
				// if (distance[q][w] > distance[q - 1][w] + 1)
				{
					distance[i][j] = distance[i - 1][j] + 1;
				}
				if (abs(j - 1 - i) <= th && distance[i][j] > distance[i][j - 1] + 1)
				// if (distance[q][w] > distance[q][w - 1] + 1)
				{
					distance[i][j] = distance[i][j - 1] + 1;
				}

				if (distance[i][j] < tmpMin)
				{
					tmpMin = distance[i][j];
				}
			}
			// Break: no need to search any more
			if (tmpMin > th)
			{
				break;
			}
		}
		if (tmpMin <= th)
		{
			ed = distance[len1][len2];
		}
		else
		{
			ed = th + 1;
		}
	} 
	else
	{
		ed = th + 1;
	}

	return ed;
}

int SimJoiner::joinED(const char *filename1, const char *filename2, unsigned q, unsigned threshold, vector<EDJoinResult> &result)
{
	result.clear();	

	unsigned prefNum = q * threshold + 1;	// from VLDB08-EdJoin; Number of q-grams in the prefix
	unsigned prefLen = prefNum - 1 + q;

	/* Make list2's invert-list									*
	 * For every word in filename2, make grams of their prefix	*
	 * So that the result will in good order of id1-id2			*/
	invertList2.clear();
	wordList2.clear();
	shortList2.clear();

	ifstream fin2(filename2);
	string str2, prefix2;
	unsigned str2Len;
	for (unsigned id2 = 0; getline(fin2, str2); ++id2)
	{
		str2Len = str2.length();
		wordList2.push_back(str2);
		if (str2Len < prefLen)	// Too short, so mark them.
		{
			makeGrams(str2, id2, q, invertList2);
			shortList2.push_back(id2);
		} 
		else
		{
			prefix2 = str2.substr(0, prefLen);
			makeGrams(prefix2, id2, q, invertList2);
		}
	}
	fin2.close();
	
	/* Find each word in list1
	 * For every word, make grams, make candidates, then check */
	ifstream fin1(filename1);
	string str1, prefix1;
	unsigned str1Len;
	for (unsigned id1 = 0; getline(fin1, str1); ++id1)
	{
		invertList1.clear();	// Clear for every word
		possibleID.clear();

		str1Len = str1.length();
		if (str1Len < prefLen)	//	Too short
		{
			makeGrams(str1, id1, q, invertList1);
			possibleID.insert(shortList2.begin(), shortList2.end());
		} 
		else
		{
			prefix1 = str1.substr(0, prefLen);
			makeGrams(prefix1, id1, q, invertList1);
		}
		
		/* Get possible List
		 * Now process every gram in invertList2 */
		for (unordered_map<string, vector<unsigned>>::iterator it(invertList1.begin()); it != invertList1.end(); ++it)
		{
			string gram(it->first);
			unordered_map<string, vector<unsigned>>::iterator findRes = invertList2.find(gram);
			if (findRes != invertList2.end())		// Exist: possible
			{
				vector<unsigned>& idList = findRes->second;
				possibleID.insert(idList.begin(), idList.end());
			} 
		}

		sortedPossibleID.clear();
		for (unordered_set<unsigned>::iterator it(possibleID.begin()); it != possibleID.end(); ++it)
		{
			sortedPossibleID.push_back(*it);
		}
		
		sort(sortedPossibleID.begin(), sortedPossibleID.end());
		
		/* Check each ID in sortedPossibleID */
		unsigned ed(0);
		for (vector<unsigned>::iterator it(sortedPossibleID.begin()); it != sortedPossibleID.end(); ++it)
		{
			// cout << "possible" << *it << endl;
			ed = getED(str1, wordList2[*it], threshold);
			if (ed <= threshold)
			{
				// result.emplace_back(id1, *it, ed);
				result.push_back(EDJoinResult(id1, *it, ed));
			}
		}
	}
	fin2.close();

	// sort(result.begin(), result.end());
	return SUCCESS;
}

void SimJoiner::makeGrams(string& str, unsigned id, unsigned q, unordered_map<string, vector<unsigned>>& invertList)
{
	countGram.clear();
	unsigned len(str.length()), num;
	string gram;
	for (int i = 0; i <= len - q; ++i)
	{
		gram = str.substr(i, q);

		if (countGram.find(gram) == countGram.end())	// Not found
		{
			invertList[gram].push_back(id);
			countGram[gram] = 0;
		} 
		else											// Exist
		{
			num = countGram[gram]++;
			ostringstream sout;
			sout << gram << num;
			invertList[sout.str()].push_back(id);
			countGram[sout.str()] = 0;
		}
	}
}


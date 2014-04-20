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
				{
					distance[i][j] = distance[i - 1][j] + 1;
				}
				if (abs(j - 1 - i) <= th && distance[i][j] > distance[i][j - 1] + 1)
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

	/* Make list2's global invert-list ( frequency list )		*
	 * For every word in filename2, make grams					*
	 * So that the result will in good order of id1-id2			*/
	freqList2.clear();
	wordList2.clear();
	shortList2.clear();

	ifstream fin2(filename2);
	string str2;
	unsigned str2Len;
	for (unsigned id2 = 0; getline(fin2, str2); ++id2)
	{
		str2Len = str2.length();
		wordList2.push_back(str2);
		if (str2Len < prefLen)	// Too short, so mark them.
		{
			shortList2.push_back(id2);
		} 
		makeFrequencyList2(str2, q);
	}
	fin2.close();
	
	/* Generate List2's prefix invert list */
	invertList2.clear();
	makeInvertListsforList2(prefLen, prefNum, q);

	/* Find each word in list1									*
	 * For every word, make grams, make candidates, then check	*/
	ifstream fin1(filename1);
	string str1;
	unsigned str1Len, currPrefNum;
	for (unsigned id1 = 0; getline(fin1, str1); ++id1)
	{
		gramList1.clear();		// Clear for every word
		possibleID.clear();

		str1Len = str1.length();
		if (str1Len < prefLen)	//	Too short
		{
			possibleID.insert(shortList2.begin(), shortList2.end());
		} 
		makeGramsbyFreq(str1, q, gramList1);

		/* Get possible List					*
		 * Now process every gram in gramList1	*/
		currPrefNum = min(prefNum, str1Len - q + 1);
		for (unsigned gramNo(0); gramNo < currPrefNum; ++gramNo) 
		{
			unordered_map<string, vector<unsigned>>::iterator findRes = invertList2.find(gramList1[gramNo].second);
			if (findRes != invertList2.end())		// Exist: possible when using prefix-filter
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
			ed = getED(str1, wordList2[*it], threshold);
			if (ed <= threshold)
			{
				result.push_back(EDJoinResult(id1, *it, ed));
			}
		}
	}
	fin1.close();

	return SUCCESS;
}

void SimJoiner::makeFrequencyList2(string& str, unsigned q)
{
	countGram.clear();
	unsigned len(str.length()), num;
	string gram;
	//if (len >= q)
	//{
	for (int i = 0; i <= len - q; ++i)
	{
		gram = str.substr(i, q);

		if (countGram.find(gram) == countGram.end())	// Not found
		{
			freqList2[gram]++;
			countGram[gram] = 0;
		} 
		else											// Exist
		{
			num = countGram[gram]++;
			ostringstream sout;
			sout << gram << num;
			freqList2[sout.str()]++;
			countGram[sout.str()] = 0;
		}
	}
	//} 
	//else
	//{
	//	freqList2[str]++;
	//}
}

void SimJoiner::makeInvertListsforList2(unsigned prefLen, unsigned prefNum, unsigned q)
{
	string str2;
	for (unsigned id2(0); id2 < wordList2.size(); ++id2)
	{
		str2 = wordList2[id2];
		if (str2.length() < prefLen)	// Too short: just do naive method
		{
			countGram.clear();
			unsigned len(str2.length()), num;
			string gram;
			//if (len >= q)
			//{
				for (int i = 0; i <= len - q; ++i)
				{
					gram = str2.substr(i, q);
		
					if (countGram.find(gram) == countGram.end())	// Not found
					{
						invertList2[gram].push_back(id2);
						countGram[gram] = 0;
					} 
					else											// Exist
					{
						num = countGram[gram]++;
						ostringstream sout;
						sout << gram << num;
						invertList2[sout.str()].push_back(id2);
						countGram[sout.str()] = 0;
					}
				}
			//} 
			//else
			//{
			//	invertList2[str2].push_back(id2);
			//}

		} 
		else							// Long enough: get lowest frequency prefix
		{	
			gramList2.clear();
			makeGramsbyFreq(str2, q, gramList2);
			for (int g(0); g < prefNum; ++g)
			{
				invertList2[gramList2[g].second].push_back(id2);
			}
		}
	}
}

void SimJoiner::makeGramsbyFreq(string& str, unsigned q, vector<pair<unsigned, string>>& gramList)
{
	countGram.clear();
	unsigned len(str.length()), num;
	string gram;
	//if (len >= q)
	//{
		for (int i = 0; i <= len - q; ++i)
		{
			gram = str.substr(i, q);
	
			if (countGram.find(gram) == countGram.end())	// Appear at the 1st time
			{
				countGram[gram] = 0;
			} 
			else											// Appear before
			{
				num = countGram[gram]++;
				ostringstream sout;
				sout << gram << num;
				countGram[sout.str()] = 0;
				gram = sout.str();
			}
	
			unordered_map<string, unsigned>::iterator findRes = freqList2.find(gram);		
			if (findRes != freqList2.end())			// Exist in the List2
			{
				gramList.push_back(make_pair(findRes->second, gram));
			} 
			else
			{
				gramList.push_back(make_pair(0, gram));
			}
		}
	//} 
	//else
	//{
	//	unordered_map<string, unsigned>::iterator findRes = freqList2.find(str);		
	//	if (findRes != freqList2.end())			// Exist in the List2
	//	{
	//		gramList.push_back(make_pair(findRes->second, str));
	//	} 
	//	else
	//	{
	//		gramList.push_back(make_pair(0, str));
	//	}
	//}

	sort(gramList.begin(), gramList.end());
}
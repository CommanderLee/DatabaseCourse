/************************************************************************/
/* Main Algorithm is published as Faerie Algorithm by Guoliang Li       */
/* Author: Zhen Li, Tsinghua University.                                */
/************************************************************************/

#include "AEE.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

const int MAX_LEN = 3000;

AEE::AEE()
{
}

AEE::~AEE()
{
}

inline int gram2len(int gram, int q)
{
	return gram + q - 1;
}

inline int len2gram(int len, int q)
{
	return len + 1 - q;
}

//string AEE::checkRepeatedGram(const string& gram)
//{
//	/* Process same grams in one string */
//	int num, len;
//	string ret;
//	/* Not found: first appearance */
//	if (existGram.find(gram) == existGram.end())
//	{
//		ret = gram;
//		existGram.insert(gram);
//		// countGram[gram] = 0;
//	}
//	/* Not first */
//	else
//	{
//		ret = "";
//		/* num = countGram[gram]++;
//		ostringstream sout;
//		sout << gram << num;
//		ret = sout.str();
//		countGram[ret] = 0;*/
//	}
//
//	return ret;
//}

void AEE::getCandidateWindows(int eID, int Tl, int TUpper)
{
	vector<int> &PList = entityPosList[eID];
	int endPos(PList.size() - Tl + 1), j;
	for (int i = 0; i <= endPos;)
	{
		j = i + Tl - 1;
		if (PList[j] - PList[i] + 1 <= TUpper)
		{
			binarySpan(i, j, eID, TUpper);
			++i;
		}
		else
		{
			i = binaryShift(i, j, eID, Tl, TUpper);
		}
	}
	
}

void AEE::binarySpan(int st, int ed, int eID, int TUpper)
{
	vector<int> &PList = entityPosList[eID];
	int lower(ed), upper(st + TUpper - 1), mid;
	while (lower <= upper)
	{
		mid = (upper + lower) / 2;
		if (mid * 2 < upper + lower)
		{
			++mid;
		}

		if (PList[mid] - PList[st] + 1 > TUpper)
		{
			upper = mid - 1;
		}
		else
		{
			lower = mid + 1;
		}
	}
	mid = upper;
	/** Find Candidate Windows in D[st...mid] */
}

int AEE::binaryShift(int st, int ed, int eID, int Tl, int TUpper)
{
	vector<int> &PList = entityPosList[eID];
	int lower(st), upper(ed), mid;
	while (lower <= upper)
	{
		mid = (upper + lower) / 2;
		if (mid * 2 < upper + lower)
		{
			++mid;
		}

		if (PList[ed] + mid - st - PList[mid] + 1 > TUpper)
		{
			lower = mid + 1;
		}
		else
		{
			upper = mid - 1;
		}
	}
	st = lower;
	ed = st + Tl - 1;
	if (PList[ed] - PList[st] + 1 > TUpper)
	{
		st = binaryShift(st, ed, eID, Tl, TUpper);
	}
	
	return st;
}

int AEE::createIndex(const char *filename, unsigned q)
{
	/** Preparing */
	ifstream fin(filename);
	qGram = q;
	string str, gram;
	unsigned len, ELenMax(-1), ELenMin(MAX_LEN);
	entityInvertList.clear();

	/** For each entity, create q-gram, inverted list */
	for (int id = 0; getline(fin, str); ++id)
	{
		entityList.push_back(str);
		len = str.length();
		if (len > ELenMax)
		{
			EGramMax = len;
		}
		else if (len < ELenMin)
		{
			EGramMin = len;
		}
		
		//existGram.clear();
		for (int i = 0; i + qGram <= len; ++i)
		{
			//gram = checkRepeatedGram(str.substr(i, qGram));
			gram = str.substr(i, qGram);
			if (entityInvertList[gram].empty() || entityInvertList[gram].back() != id)
			{
				entityInvertList[gram].push_back(id);
			}
		}
	}
	fin.close();
	
	/** Calculate EGramMax/Min */
	EGramMax = len2gram(ELenMax, q);
	EGramMin = len2gram(ELenMin, q);

	entityPosList.resize(entityList.size());

	return SUCCESS;
}

int AEE::aeeJaccard(const char *doc, double threshold, vector<JaccardExtractResult> &result)
{
	result.clear();
	
	/** Convert threshold(jac-delta) to T; calculate SGramMax/MinJac */

	/** Filter: find candidate */

	/** Verify: calculate Jaccard */

	return SUCCESS;
}

int AEE::aeeED(const char *doc, unsigned threshold, vector<EDExtractResult> &result)
{
	/** Preparing */
	result.clear();
	string docStr(doc), gram;
	int len = docStr.length();
	docInvertList.clear();

	/** Faerie Algorithm:
	  * Create inverted list of Doc;
	  * Generate Position List. */
	for (int i = 0; i + qGram <= len; ++i)
	{
		//gram = checkRepeatedGram(str.substr(i, qGram));
		gram = docStr.substr(i, qGram);
		if (docInvertList[gram].empty() || docInvertList[gram].back() != i)
		{
			docInvertList[gram].push_back(i);
		}

		if (entityInvertList.find(gram) != entityInvertList.end())
		{
			vector<int> &IL = entityInvertList[gram];
			for (vector<int>::iterator it(IL.begin()); it != IL.end(); ++it)
			{
				entityPosList[*it].push_back(i);
			}
		}
	}

	/** Check each entity's P List with candidate window algorithm */
	int eNo(entityPosList.size());
	int Tl, TUpper, eGram;							
	for (int id = 0; id < eNo; ++id)
	{
		if (!entityPosList[id].empty())
		{
			eGram = len2gram(entityList[id].length(), qGram);
			Tl = eGram - threshold * qGram;
			TUpper = eGram + threshold;
			if (entityPosList[id].size() >= Tl)
			{
				getCandidateWindows(id, Tl, TUpper);
			}
			
		}
		
	}
	

	/** Verify: calculate ED */

	return SUCCESS;
}

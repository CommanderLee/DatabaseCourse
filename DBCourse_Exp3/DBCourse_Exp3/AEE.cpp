/************************************************************************/
/* Main Algorithm is published as Faerie Algorithm by Guoliang Li       */
/* Author: Zhen Li, Tsinghua University.                                */
/************************************************************************/

#include "AEE.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

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

bool resultEDCmp(const EDExtractResult& res1, const EDExtractResult& res2)
{
	bool jug(0);
	if (res1.id != res2.id)
	{
		jug = res1.id < res2.id;
	}
	else if (res1.pos != res2.pos)
	{
		jug = res1.pos < res2.pos;
	}
	else if (res1.len != res2.len)
	{
		jug = res1.len < res2.len;
	}
	return jug;
}

bool resultJacCmp(const JaccardExtractResult& res1, const JaccardExtractResult& res2)
{
	bool jug(0);
	if (res1.id != res2.id)
	{
		jug = res1.id < res2.id;
	}
	else if (res1.pos != res2.pos)
	{
		jug = res1.pos < res2.pos;
	}
	else if (res1.len != res2.len)
	{
		jug = res1.len < res2.len;
	}
	return jug;
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

int getED(string& str1, string& str2, unsigned threshold)
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
				/** query[q - 1] is the q_th char of query */
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
			/** Break: no need to search any more */
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

int AEE::getIntersetNum(string& str1, string& str2)
{
	countGram.clear();
	interSet.clear();
	int interNum(0);

	/* Process same grams in one string */
	int num, str1Len(str1.length()), str2Len(str2.length());
	for (int i = 0; i + qGram <= str1Len; ++i)
	{
		string gram(str1.substr(i, qGram));
		/* Not found: first appearance */
		if (countGram.find(gram) == countGram.end())
		{
			interSet.insert(gram);
			countGram[gram] = 0;
		}
		/* Not first */
		else
		{
			num = countGram[gram]++;
			ostringstream sout;
			sout << gram << num;
			interSet.insert(sout.str());
			countGram[sout.str()] = 0;
		}
	}

	countGram.clear();
	for (int j = 0; j + qGram <= str2Len; ++j)
	{
		string gram(str2.substr(j, qGram));
		/* Not found: first appearance */
		if (countGram.find(gram) == countGram.end())
		{
			if (interSet.find(gram) != interSet.end())
			{
				++interNum;
			}
			countGram[gram] = 0;
		}
		/* Not first */
		else
		{
			num = countGram[gram]++;
			ostringstream sout;
			sout << gram << num;
			if (interSet.find(sout.str()) != interSet.end())
			{
				++interNum;
			}
			countGram[sout.str()] = 0;
		}
	}
	return interNum;
}
//
//double AEE::getJac(string& str1, string& str2)
//{
//	int interNum = getIntersetNum(str1, str2);
//	int gramNum1(max(0, len2gram(str1.length(), qGram)));
//	int gramNum2(max(0, len2gram(str2.length(), qGram)));
//	return double(interNum) / (gramNum1 + gramNum2 - interNum);
//}

void AEE::getCandidateWindows(int eID, int Tl, int TUpper, int TLower)
{
	vector<int> &PList = entityPosList[eID];
	int endPos(PList.size() - Tl + 1), j;
	for (int i = 0; i < endPos;)
	{
		j = i + Tl - 1;
		if (PList[j] - PList[i] + 1 <= TUpper)
		{
			binarySpan(i, j, eID, TUpper, TLower);
			++i;
		}
		else
		{
			i = binaryShift(i, j, eID, Tl, TUpper);
		}
	}
	
}

void AEE::binarySpan(int st, int ed, int eID, int TUpper, int TLower)
{
	vector<int> &PList = entityPosList[eID];
	int lower(ed), upper(min(st + TUpper - 1, (int)PList.size() - 1)), mid;
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
	int left, right;
	for (; mid > st; --mid)
	{
		left = max(PList[mid] - TUpper + 1, 0);
		if (st > 0)
		{
			left = max(left, PList[st - 1] + 1);
		}
		right = min(PList[st] + TUpper - 1, docGramMax - 1);
		if (mid + 1 < PList.size())
		{
			right = min(right, PList[mid + 1] - 1);
		}
	
		/** Find candidates */
		int sGram, sLen, eGram(len2gram(entityList[eID].length(), qGram));
		int ED, interNum, T;
		double Jac;
		string sub;
		for (int l = left; l <= PList[st]; ++l)
		{
			for (int r = PList[mid]; r <= right; ++r)
			{
				sGram = r - l + 1;
				sLen = gram2len(sGram, qGram);
				if (sGram >= TLower && sGram <= TUpper)
				{
					sub = docStr.substr(l, sLen);
					if (category == CAT_ED)
					{
						ED = getED(entityList[eID], sub, thED);
						if ( ED <= thED)
						{
							EDExtractResult res;
							res.id = eID;
							res.pos = l;
							res.len = sLen;
							res.sim = ED;
							pResED->push_back(res);
						}
					}
					else if (category == CAT_JAC)
					{
						interNum = getIntersetNum(entityList[eID], sub);
						T = int((sGram + eGram) * thJac / (1 + thJac));
						if (interNum >= T)
						{
							Jac = double(interNum) / (sGram + eGram - interNum);
							if (Jac >= thJac)
							{
								JaccardExtractResult res;
								res.id = eID;
								res.pos = l;
								res.len = sLen;
								res.sim = Jac;
								pResJac->push_back(res);
							}
						}
					}
				}
			}
		}
	}
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
	ed = min(st + Tl - 1, (int)PList.size() - 1);
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
	int len, ELenMax(-1), ELenMin(MAX_LEN);
	entityList.clear();
	entityInvertList.clear();

	/** For each entity, create q-gram, inverted list */
	for (int id = 0; getline(fin, str); ++id)
	{
		entityList.push_back(str);
		len = str.length();
		if (len > ELenMax)
		{
			ELenMax = len;
		}
		if (len < ELenMin)
		{
			ELenMin = len;
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

	return SUCCESS;
}

int AEE::aeeJaccard(const char *doc, double threshold, vector<JaccardExtractResult> &result)
{
	/** Preparing */
	category = CAT_JAC;
	result.clear();
	entityPosList.clear();
	entityPosList.resize(entityList.size());

	docStr = string(doc);
	int len = docStr.length();
	docInvertList.clear();
	docGramMax = len2gram(len, qGram);
	thJac = threshold;

	pResJac = &result;
	string gram;

	/** Faerie Algorithm:
	  * Create inverted list of Doc;
	  * Generate Position List. */
	for (int i = 0; i + qGram <= len; ++i)
	{
		//gram = checkRepeatedGram(str.substr(i, qGram));
		gram = docStr.substr(i, qGram);
		//if (docInvertList[gram].empty() || docInvertList[gram].back() != i)
		//{
			docInvertList[gram].push_back(i);
		//}

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
	int Tl, TUpper, TLower, eGram;							
	for (int id = 0; id < eNo; ++id)
	{
		if (!entityPosList[id].empty())
		{
			eGram = len2gram(entityList[id].length(), qGram);
			Tl = int(eGram * thJac);
			if (eGram * thJac - Tl > 1e-6)
			{
				++Tl;
			}
			
			TUpper = int(eGram / thJac);
			TLower = Tl;
			if (entityPosList[id].size() >= Tl)
			{
				getCandidateWindows(id, Tl, TUpper, TLower);
			}
		}
	}
	
	sort(result.begin(), result.end(), resultJacCmp);
	//cout << "DEBUG:" << endl;
	//cout << getED(entityList[0], docStr.substr(15, 15), qGram) << endl;
	return SUCCESS;
}


int AEE::aeeED(const char *doc, unsigned threshold, vector<EDExtractResult> &result)
{
	/** Preparing */
	category = CAT_ED;
	result.clear();
	entityPosList.clear();
	entityPosList.resize(entityList.size());

	docStr = string(doc);
	int len = docStr.length();
	docInvertList.clear();
	docGramMax = len2gram(len, qGram);
	thED = threshold;

	pResED = &result;
	string gram;

	/** Faerie Algorithm:
	  * Create inverted list of Doc;
	  * Generate Position List. */
	for (int i = 0; i + qGram <= len; ++i)
	{
		//gram = checkRepeatedGram(str.substr(i, qGram));
		gram = docStr.substr(i, qGram);
		//if (docInvertList[gram].empty() || docInvertList[gram].back() != i)
		//{
			docInvertList[gram].push_back(i);
		//}

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
	int Tl, TUpper, TLower, eGram;							
	for (int id = 0; id < eNo; ++id)
	{
		if (!entityPosList[id].empty())
		{
			eGram = len2gram(entityList[id].length(), qGram);
			Tl = eGram - threshold * qGram;
			TUpper = eGram + threshold;
			TLower = eGram - threshold;
			if (entityPosList[id].size() >= Tl)
			{
				getCandidateWindows(id, Tl, TUpper, TLower);
			}
			
		}
		
	}
	
	sort(result.begin(), result.end(), resultEDCmp);
	//cout << "DEBUG:" << endl;
	//cout << getED(entityList[0], docStr.substr(15, 15), qGram) << endl;
	return SUCCESS;
}

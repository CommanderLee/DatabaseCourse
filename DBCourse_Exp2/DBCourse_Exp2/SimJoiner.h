#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

template <typename _IDType, typename _SimType>
struct JoinResult
{
	_IDType id1;
	_IDType id2;
	_SimType s;

	JoinResult(){}
	JoinResult(_IDType _id1, _IDType _id2, _SimType _s): id1(_id1), id2(_id2), s(_s){}

	bool operator < (const JoinResult& jr) const
	{
		if (id1 != jr.id1)
		{
			return id1 < jr.id1;
		} 
		else
		{
			return id2 < jr.id2;
		}
	}
};

typedef JoinResult<unsigned, double> JaccardJoinResult;
typedef JoinResult<unsigned, unsigned> EDJoinResult;

const int SUCCESS = 0;
const int FAILURE = 1;

class SimJoiner
{
	vector<pair<unsigned, string>>			gramList1, gramList2;
	unordered_map<string, vector<unsigned>> invertList2;
	unordered_map<string, unsigned>			freqList2;
	vector<string>							wordList2;
	vector<unsigned>						shortList2;
	unordered_set<unsigned>					possibleID;
	vector<unsigned>						sortedPossibleID;
	unordered_map<string, unsigned>			countGram;		// count the number of each gram

public:
	SimJoiner();
	~SimJoiner();

	void makeGramsbyFreq(string& str, unsigned q, vector<pair<unsigned, string>>& gramList);
	void makeFrequencyList2(string& str, unsigned q);
	void makeInvertListsforList2(string& str, unsigned id, unsigned prefLen, unsigned prefNum, unsigned q);

	int joinJaccard(const char *filename1, const char *filename2, unsigned q, double threshold, std::vector<JaccardJoinResult> &result);
	int joinED(const char *filename1, const char *filename2, unsigned q, unsigned threshold, std::vector<EDJoinResult> &result);
};


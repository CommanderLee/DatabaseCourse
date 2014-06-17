/* Database Final Exp. Li, Zhen, June 10th, 2014. */

#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

class WordList
{
private:
	vector<string> words;

public:
	WordList()
	{
		words.push_back("0");
	}

	void createIndex()
	{
		words.clear();
		words.push_back("hehe");
		words.push_back("haha");
	}

	string searchKWord(string query)
	{
		string ret = query + words[0];
		cout << "CPP:" << ret << endl;
		return ret;
	}
};

extern "C"
{
	WordList* getWordList()
	{
		return new WordList();
	}
	
	bool wordListCreateIndex(WordList* wl)
	{
		wl->createIndex();
		return true;
	}
	
	const char* wordListSearch(WordList* wl, char* query)
	{
		printf("C: %s\n", query);
		string ret = wl->searchKWord(string(query));
		return ret.c_str();
	}
}

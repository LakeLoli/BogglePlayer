#ifndef TST_HPP
#define TST_HPP

#include <string>
#include <utility>

using namespace std;

class TST
{
public:
    struct TSTNode
	{
		char c;
		int value;
		TSTNode* left;
		TSTNode* right;
		TSTNode* mid;

		TSTNode (const char &ch, const int &val=0, TSTNode* lt=0, TSTNode* rt=0, TSTNode* md=0):
			c(ch), value(val), left(lt), right(rt), mid(md){}
	};


	
	TST(): root(nullptr), thesize(0){}
	//TST(const TST & rhs);
	~TST();

	//maybe modify the return value for the convenience of latter program
	void insert (const string &key, int val);
	pair<TSTNode*, bool> find (const string &key) const;
	void makeEmpty();
	bool isEmpty() const;
	int size() const;

private:

	TSTNode* root;
    int thesize;

	void insert (const string &key, int val, unsigned int index, TSTNode* &t);
	pair<TSTNode*, bool> find (const string &key, unsigned int index, TSTNode* t) const;
	void makeEmpty(TSTNode* &t);

};
#endif
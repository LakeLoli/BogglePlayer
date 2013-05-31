#include "TST.hpp"

TST::~TST(){
    makeEmpty();
}

bool TST::isEmpty() const{
    return thesize==0;
}

int TST::size() const{
    return thesize+1;
}

void TST::insert(const string &key, int val){
	insert (key, val, 0, root);
}
void TST::insert (const string &key, int val, unsigned int index, TSTNode* &t){
	char ch=key.at(index);

	if (t==nullptr) 
		t= new TSTNode(ch);
	if (ch < t->c)
		insert (key, val, index, t->left);
	else if (ch > t->c)
		insert (key, val, index, t->right);
	else if (key.length()-1 > index)
		insert (key, val, index+1, t->mid);
	else {
		t->value=val;
		thesize++;
	}
}

pair<TST::TSTNode*, bool> TST::find(const string &key) const{
    return find (key, 0, root);
}

pair<TST::TSTNode*, bool> TST::find (const string &key, unsigned int index, TSTNode* t) const{
	
	char ch=key.at(index);
	if(t==nullptr || index >= key.length())
		return make_pair(nullptr, false);
	if (ch < t->c) 
		return find(key, index, t->left);
	else if (ch > t->c)
		return find(key, index, t->right);
	else if (index < key.length()-1)
		return find(key, index+1, t->mid);
	else
		return make_pair(t, true);

}

void TST::makeEmpty(){
    makeEmpty(root);
	thesize=0;
}

void TST::makeEmpty(TSTNode* &t){

	if (t !=nullptr){
	    makeEmpty(t->left);
		makeEmpty(t->right);
		makeEmpty(t->mid);
		delete t;
	}
	t=nullptr;
}

/*
int main(){
    TST t;
	pair<TST::TSTNode*, bool> p;

	t.insert ("aa", 1);
	t.insert ("aah", 2);
	t.insert ("aahed", 3);
	t.insert ("bad", 4);
	t.insert ("cad", 5);
	t.insert ("ded", 6);

	printf("The size of the TST is %d\n", t.size());
	printf("isEmpty= %d\n", t.isEmpty());

	p=t.find("aa");
	p=t.find("aaa");
	p=t.find("bad");
	p=t.find("cad");
	p=t.find("asfew");
	p=t.find("aqefrre");
	
	t.makeEmpty();
    printf("The size of the TST is %d\n", t.size());
	printf("isEmpty= %d\n", t.isEmpty());

	//t.insert ("csd", 1);
	return 0;
}
*/
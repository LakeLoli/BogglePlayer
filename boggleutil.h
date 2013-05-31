#ifndef BOGGLEUTIL_H
#define BOGGLEUTIL_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <utility>
#include <set>
#include <list>

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
    pair<TSTNode*, bool> find(const string &key) const;
    pair<TSTNode*, bool> find(const string &, unsigned int, TSTNode*) const;
    pair<TSTNode*, bool> isPrefix (const string &key) const;
    pair<TSTNode*, bool> isPrefix (const string &key, unsigned int index, TSTNode* t) const;
    void makeEmpty();
    bool isEmpty() const;
    int size() const;

private:

    TSTNode* root;
    int thesize;

    void insert (const string &key, int val, unsigned int index, TSTNode* &t);
    void makeEmpty(TSTNode* &t);

};

class Vertex {
public:
    vector<Vertex*> adj;
    string name;
    bool visited;
    int location;
    Vertex(string s, int lt):name(s), visited(false), location(lt){}
};

class Graph{
public:
    //reconsider the time complexity of find operation
    typedef map<int, Vertex*> vmap;
    vmap work;
    void addVertex(const string &, int );
    void addEdge(int lt1, int lt2); 
    int size();
    int maxStrlen();
};
#endif

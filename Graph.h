#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <string>
#include <iostream>

using namespace std;

class Vertex {
public:
	//typedef pair<int, Vertex*> edge;
	vector<Vertex*> adj;
	string name;
	Vertex(string s):name(s){}
};

class Graph{
public:
	typedef map<string, Vertex*> vmap;
	vmap work;
	void addVertex(const string &);
	void addEdge(const string& _m_from_int, const string& to); 
	int size();
};
#endif
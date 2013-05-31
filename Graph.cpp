#include "Graph.h"

void Graph::addVertex(const string &name){
	vmap::iterator itr=work.begin();
	itr=work.find(name);
	if (itr==work.end()){
	    Vertex *v;
		v= new Vertex(name);
		work[name]=v;
		return;
	}
	cout << "Vertex already exists!" << endl;
}

void Graph::addEdge(const string &from, const string &to){
    Vertex *v1=(work.find(from)->second);
	Vertex *v2=(work.find(to)->second);
	v1->adj.push_back(v2);
	v2->adj.push_back(v1);
}

int Graph::size(){
    return work.size();
}

/*
int main(){
    Graph g;
	g.addVertex("a");
	g.addVertex("b");
	g.addVertex("c");
	g.addVertex("d");
	g.addVertex("abcd");
	g.addEdge("a", "b");
	g.addEdge("abcd", "c");
	return 0;
}
*/
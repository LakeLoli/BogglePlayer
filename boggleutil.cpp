#include "boggleutil.h"

//Graph Class Implementation
void Graph::addVertex(const string &name, int location){
    Vertex* v;
    v= new Vertex(name, location);
    work[location]=v;
    return;
}

void Graph::addEdge(int lt1, int lt2){
    //haven't implementted checking for same edge
    Vertex *v1=(work.find(lt1)->second);
    Vertex *v2=(work.find(lt2)->second);
    v1->adj.push_back(v2);
    v2->adj.push_back(v1);
}

int Graph::size(){
    return work.size();
}

int Graph::maxStrlen(){
    int maxlength=0;
    for (vmap::iterator itr=work.begin(); itr!=work.end(); itr++){
        if(itr->second->name.length() > maxlength)
            maxlength=itr->second->name.length();
    }
    return maxlength;
}

Graph::~Graph(){
    for (vmap::iterator itr=work.begin(); itr!=work.end(); itr++) 
    {
        delete (*itr).second;
        (*itr).second=nullptr;
    }
}

// Ternary Search Tree implementation
TST::~TST(){
    makeEmpty();
}
bool TST::isEmpty() const{
    return thesize==0;
}

int TST::size() const{
    return thesize;
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
    pair <TSTNode*, bool> p=isPrefix(key);
    if (p.second == false)  // if not found;
        return p;
    else if (p.first->value==0)  //if the key is a prefix, return false
        return make_pair(p.first, false);
    else 
        return p; // if the key is not the prefix, return the end node and true;
}

pair<TST::TSTNode*, bool> TST::find(const string &key, unsigned int index, TSTNode* t) const{
    pair <TSTNode*, bool> p=isPrefix(key, index, t);
    if (p.second == false)  // if not found;
        return p;
    else if (p.first->value==0)  //if the key is a prefix, return false
        return make_pair(p.first, false);
    else 
        return p; // if the key is not the prefix, return the end node and true;
}

pair<TST::TSTNode*, bool> TST::isPrefix (const string &key) const{
    return isPrefix(key, 0, root);
}

pair<TST::TSTNode*, bool> TST::isPrefix (const string &key, unsigned int index, TSTNode* t) const{
    
    char ch=key.at(index);
    if(t==nullptr || index >= key.length())
        return make_pair(nullptr, false);
    if (ch < t->c) 
        return isPrefix(key, index, t->left);
    else if (ch > t->c)
        return isPrefix(key, index, t->right);
    else if (index < key.length()-1)
        return isPrefix(key, index+1, t->mid);
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

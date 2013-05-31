#include "boggleplayer.h"

string listTostring (list<Vertex*> &path);
int locaiton (int r, int c, int cols);

void BogglePlayer::buildLexicon(const set<string>& word_list){
    set<string>::iterator itr;
    vector<string> rand_word_list;
    vector<string>::iterator vitr;
    long i=1;
    longestWL=0;

    for (itr=word_list.begin(); itr != word_list.end(); ++itr){
        rand_word_list.push_back(*itr);    
    }
    //randomly shuffle the lex set;
    random_shuffle (rand_word_list.begin(), rand_word_list.end());
    for (vitr=rand_word_list.begin(); vitr != rand_word_list.end(); ++vitr){
        lextst.insert(*vitr, i++);
        if(longestWL < vitr->length())
            longestWL=vitr->length();
    }
    buildLexCalled=true; 
}

int location (int r, int c, int cols){
    return r*cols+c;
}

void BogglePlayer::setBoard(unsigned int rows, unsigned int cols, string** diceArray){
    
    for(unsigned int r=0;r<rows;r++){
        for(unsigned int c=0;c<cols;c++){
            g.addVertex(diceArray[r][c], location (r,c,cols));
        }
    }

    for(unsigned int r=0;r<rows;r++){
        for(unsigned int c=0;c<cols;c++){
            if (c< cols-1)
                g.addEdge(location (r,c,cols), location (r,c+1,cols));
            if (r< rows-1)
                g.addEdge(location (r,c,cols), location (r+1,c,cols));
            if (c< cols-1 && r< rows-1)
                g.addEdge(location (r,c,cols), location (r+1,c+1,cols));
            if (c>0 && r< rows-1)
                g.addEdge(location (r,c,cols),location (r+1,c-1,cols));
        }
    }
    setBoardCalled=true;
}

//return value:
//0 means the vertex is denied;
//1 means the vertex is a prefix;
//2 means the path has been found;
int checkPrefix(list<Vertex*> &path, Vertex* &nextV, const string& word_to_check, int &pathLength){
    
    if (word_to_check.length() < pathLength)
        return 0;

    else if (word_to_check.length() < pathLength+nextV->name.length()){
        return 0;
    }

    else if(word_to_check.length() == pathLength+nextV->name.length()){
        if (word_to_check.substr(pathLength, nextV->name.length()) == nextV->name)
            return 2;
        else 
            return 0;
    }

    else{
        if (word_to_check.substr(pathLength, nextV->name.length()) == nextV->name)
            return 1;
        else 
            return 0;
    }
}

bool findPath(list<Vertex*> &path, const string& word_to_check, int &pathLength){
    Vertex* currV;
    Vertex* nextV;
    stack<Vertex*> adjCad;
    currV=path.back();
    bool found;
    int checkstate;

    if (word_to_check.length() == pathLength){
        return true;
    }
    else if (word_to_check.length() < pathLength)
        return false;

    //traversal the adj list
    for(int i=0; i < currV->adj.size(); i++){
        if (!currV->adj.at(i)->visited)
            adjCad.push(currV->adj.at(i));
    }

    while (!adjCad.empty()){

        nextV=adjCad.top();
        adjCad.pop();
        checkstate=checkPrefix(path, nextV, word_to_check, pathLength);

        if (checkstate==0) // if the next vertex fails in matching,
            continue;      // go to next availabe candidate in adj of current vertex
        else{
            pathLength+=nextV->name.length();  // if next vertex matches, update vairiables
            path.push_back(nextV);             // push next vertex into path list
            nextV->visited=true;               // indicate next vertex is pushed into the list,
                                               // not aviable for searching
            if (checkstate==2)                 // if the path is found, return true;
                return true;
            else if (checkstate==1){                    // if it is only prefix, recursively searching
                found=findPath(path, word_to_check, pathLength);
                if (found) 
                    return true;
                else{                                   // if the findPath function false, recover the previous state;
                    pathLength-=nextV->name.length();   // if next vertex matches, update vairiables
                    path.pop_back();                    // push next vertex into path list
                    nextV->visited=false;               // indicate next vertex is pushed into the list,
                }

            }
        }
    }

    return false;
    // if all the vertexs in the adj list all mismatch;
    // this findpath function failed, return false;
}


vector<int> BogglePlayer::isOnBoard(const string& word_to_check){
    
    vector<int> empty;
    if(!setBoardCalled)
        return empty;
    //using stack structure to pop the candidate with long prefix first;
    stack <Vertex*> verCad;
    int pathLength;
    bool found=false;
    int j=0;
    
    typedef map<int, Vertex*> vmap;
    for (int i=1; i<=g.maxStrlen(); i++){
        for (vmap::iterator itr=g.work.begin(); itr!=g.work.end(); ++itr){
            if (word_to_check.substr(0, i)==itr->second->name)
                verCad.push(itr->second);
        }
    }
    list<Vertex*> wordList;

    while(!verCad.empty()){
        //Initializaiton for every vertex candidate
        //Using list structure to construct the word_to_check;
        //remember clear visited of the vertex in the list
        Vertex* head=verCad.top();
        verCad.pop();
        wordList.push_front(head);
        head->visited=true;
        pathLength=head->name.length();
        
        found=findPath(wordList, word_to_check, pathLength);
        if(found)
            break;
        else{
            head->visited=false;
            wordList.clear();
        }
    }
 
    if(found){
        vector<int> pathLocation(int(wordList.size()));
        for (list<Vertex*>::iterator litr=wordList.begin(); litr!=wordList.end(); ++litr){
            (*litr)->visited=0;// initialize the variable "visited"
            pathLocation.at(j++)=(*litr)->location;
        }
        return pathLocation;
    }
    else 
        return empty;
}

bool BogglePlayer::getAllValidWords(unsigned int minimum_word_length, set<string>* words){
    allPrefix(minimum_word_length, words);
    if (setBoardCalled && buildLexCalled)
        return true;
    else
        return false;
}

void BogglePlayer::allPrefix(unsigned int minimum_word_length, set<string>* words){
    list<Vertex*> wordList;
    for (map<int, Vertex*>:: iterator itr=g.work.begin(); itr!=g.work.end(); ++itr){
        wordList.push_back(itr->second);
        pair <TST::TSTNode*, bool> p=lextst.isPrefix(wordList.front()->name);

        if (p.second==false){ // if the first vertex is not a prefix, jump to next vertex;
            wordList.pop_back();
            continue;
        }
        else if (p.second==true && p.first->value!=0 && p.first->mid==nullptr){ 
            if (wordList.front()->name.length()>= minimum_word_length)
                words->insert(listTostring(wordList));
            wordList.pop_back(); //pop the node out; go to next available candidate
            continue;
        }
        else{
            wordList.front()->visited=true;    // indicates that next vertex is pushed into the list,   
            if (p.first->value!=0& wordList.front()->name.length()>= minimum_word_length)
                words->insert(listTostring(wordList));
            findPrefix(wordList, int(itr->second->name.length()), p.first, minimum_word_length, words);
            wordList.front()->visited=false;
            wordList.clear();
        }
    }
}

void BogglePlayer::findPrefix(list<Vertex*> &path, int pathLength, TST::TSTNode* t, unsigned int minimum_word_length, set<string>* words){
    Vertex* currV;
    Vertex* nextV;
    stack<Vertex*> adjCad;
    bool found;
    string key;
    
    if (pathLength >= longestWL){
        return;
    }

    //Push the vertexs in adj list into a candidate stack
    currV=path.back();
    for(int i=0; i < currV->adj.size(); i++){
        if (!currV->adj.at(i)->visited)
            adjCad.push(currV->adj.at(i));
    }

    while (!adjCad.empty()){
        
        nextV=adjCad.top();
        adjCad.pop();
        path.push_back(nextV);
        pair <TST::TSTNode*, bool> p=lextst.isPrefix(nextV->name, 0, t->mid);

        if (p.second==false){ // if the next vertex doesn't form prefix,
            path.pop_back();  //If it is not the prefix, pop it out
            continue;         // go to next availabe candidate in adj of current vertex
        }
        // Find complete word and not the prefix of other word
        else if (p.second==true && p.first->value!=0 && p.first->mid==nullptr){
            //pushInfo(path, p.first, true); //push the list in to the string vector of the vertex!!!
            if ((pathLength+nextV->name.length())>=minimum_word_length){
                words->insert(listTostring(path));
            }
            path.pop_back();     
            continue;
        }
        //The other situations are regards as prefix situation
        //include purely prefix and prefix (but a complete word)
        else{
            pathLength+=nextV->name.length();           // if it is prefix, update length (used in termination of searching)
            nextV->visited=true;                        // indicate next vertex is pushed into the list,

            //p.first->value!=0 means threre is no end bit;
            if (p.first->value!=0 & pathLength>=minimum_word_length){
                words->insert(listTostring(path));
            }
            findPrefix(path, pathLength, p.first, minimum_word_length, words);

            pathLength-=nextV->name.length();           // return from findPrefix means nextV is handled
            path.pop_back();                            // pop nextV out of path list
            nextV->visited=false;                       // means the nextV is not on the list, can be search again;
        }
    }
    //all the vertexs in the adj list are handled, return;
    return;
}

string listTostring (list<Vertex*> &path){
    string s;
    for (list<Vertex*>::iterator itr=path.begin(); itr!=path.end(); ++itr){
        s.append((*itr)->name);
    }
    return s;
}

bool BogglePlayer::isInLexicon(const string& word_to_check){
    if(!buildLexCalled) 
        return false;
    return lextst.find(word_to_check).second;
}

void BogglePlayer::getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols){

}


BogglePlayer::~BogglePlayer(){
}

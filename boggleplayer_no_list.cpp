#include "boggleplayer.h"

string listTostring (list<Vertex*> &path);
string revStr(string s);

void BogglePlayer::buildLexicon(const set<string>& word_list){
	set<string>::iterator itr;
	long i=1;
	longestWL=0;
	for (itr=word_list.begin(); itr != word_list.end(); itr++){
		lextst.insert(*itr, i++);
		if(longestWL < itr->length())
			longestWL=itr->length();
	}
	/*
	i=1;
	for (itr=word_list.begin(); itr != word_list.end(); itr++){
		rlextst.insert(revStr(*itr), i++);
	}
	*/
	printf("The size of TST tree is %d\n", lextst.size());
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
			path.push_back(nextV);           // push next vertex into path list
			nextV->visited=true;             // indicate next vertex is pushed into the list,
			                                 // not aviable for searching
			if (checkstate==2)          // if the path is found, return true;
				return true;
	        else if (checkstate==1){               // if it is only prefix, recursively searching
				found=findPath(path, word_to_check, pathLength);
				if (found) 
					return true;
				else{                                // if the findPath function false, recover the previous state;
				    pathLength-=nextV->name.length();  // if next vertex matches, update vairiables
			        path.pop_back();                 // push next vertex into path list
			        nextV->visited=false;            // indicate next vertex is pushed into the list,
				}

			}
		}
	}

	return false;
	// if all the vertexs in the adj list all mismatch;
	// this findpath function failed, return false;
}


vector<int> BogglePlayer::isOnBoard(const string& word_to_check){
	
	//using stack structure to pop the candidate with long prefix first;
	stack <Vertex*> verCad;
	int pathLength;
	bool found=false;
	int j=0;
	
	typedef map<int, Vertex*> vmap;
	for (int i=1; i<=g.maxStrlen(); i++){
	    for (vmap::iterator itr=g.work.begin(); itr!=g.work.end(); itr++){
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
		for (list<Vertex*>::iterator litr=wordList.begin(); litr!=wordList.end(); litr++){
			(*litr)->visited=0;// initialize the variable "visited"
		    pathLocation.at(j++)=(*litr)->location;
		}
		return pathLocation;
	}
	
	else {
		vector<int> empty;
		empty.clear();
		return empty;
	}
}


bool BogglePlayer::getAllValidWords(unsigned int minimum_word_length, set<string>* words){
	preMode=true;
	allPrefix(lextst,  minimum_word_length, words);
	/*
	allPostfix(rlextst);
	string word_to_check;
	vector<Vertex::PreInfo*> preV;
	vector<Vertex::PreInfo*> postV;
	for (map<int, Vertex*>:: iterator itr=g.work.begin(); itr!=g.work.end(); ++itr){
		preV=itr->second->PreInfoV;
		postV=itr->second->PosInfoV;

		for (int i=0; i< postV.size(); i++){
			if((postV.at(i))->endFlag){
				if(postV.at(i)->t->mid==nullptr){
				    words->insert(revStr(postV.at(i)->preStr));
					postV.erase(postV.begin()+i);
				}
			}
		}

		for (vector<Vertex::PreInfo*>::iterator preitr=preV.begin(); preitr!=preV.end(); ++preitr){
			if((*preitr)->endFlag){
				if((*preitr)->t->mid==nullptr){
				    words->insert((*preitr)->preStr);
					break;
				}
				else{
				    words->insert((*preitr)->preStr);
				}
			}
		    for (vector<Vertex::PreInfo*>::iterator postitr=postV.begin(); postitr!=postV.end(); ++postitr){
			
				if ((*preitr)->preStr.length()+(*postitr)->preStr.length()-1 < longestWL/2 + 2){
				    continue;
				}

				for(set<int>::iterator sitr=(*postitr)->lset.begin(); sitr!=(*postitr)->lset.end(); ++sitr){
				    if (*sitr != itr->second->location){
					    if ((*preitr)->lset.find(*sitr)!=(*preitr)->lset.end())
						    continue;
					}
				}


				string rps=(*postitr)->preStr;
				rps.pop_back();
				word_to_check=(*preitr)->preStr + revStr(rps);

				if (words->find(word_to_check)!=words->end())
					continue;

				bool isWord=this->isInLexicon(revStr((*postitr)->preStr), 0, (*preitr)->t);
				if(isWord)
					words->insert(word_to_check);
		    }
		}
	}
	*/
	return true;
}

void BogglePlayer::allPostfix(TST &t){
	preMode=false;
	for (map<int, Vertex*>:: iterator itr=g.work.begin(); itr!=g.work.end(); itr++){
	    itr->second->name=revStr(itr->second->name);
	}
	//allPrefix(rlextst);
	for (map<int, Vertex*>::iterator itr=g.work.begin(); itr!=g.work.end(); itr++){
	    itr->second->name=revStr(itr->second->name);
	}
}


void BogglePlayer::allPrefix(TST &lex, unsigned int minimum_word_length, set<string>* words){
	list<Vertex*> wordList;
	string s;
	for (map<int, Vertex*>:: iterator itr=g.work.begin(); itr!=g.work.end(); itr++){
		wordList.push_back(itr->second);
		pair <TST::TSTNode*, bool> p=lex.isPrefix(wordList.front()->name);

		if (p.second==false){ // if the first vertex is not a prefix, jump to next vertex;
			wordList.pop_back();
			continue;
		}
		else if (p.second==true && p.first->value!=0 && p.first->mid==nullptr){ 
			Vertex::PreInfo* pre;
			//pushInfo(wordList, p.first, true);      //push the list in to the string vector of the vertex!!!
			s.clear();
			for (list<Vertex*>::iterator itr=wordList.begin(); itr!=wordList.end(); itr++)
	        	s.append((*itr)->name);
			words->insert(s);
			wordList.pop_back();             //pop the node out; go to next available candidate
			continue;
		}
		else{
			wordList.front()->visited=true;    // indicate next vertex is pushed into the list,   
			//pushInfo(wordList, p.first, p.first->value!=0);       //p.first->value!=0 means threre is no end bit;  
			if (p.first->value!=0& wordList.front()->name.length()>= minimum_word_length){
			    s.clear();
				for (list<Vertex*>::iterator itr=wordList.begin(); itr!=wordList.end(); itr++)
	        	    s.append((*itr)->name);
				words->insert(s);
			}
		    findPrefix(wordList, int(itr->second->name.length()), lex, p.first, minimum_word_length, words);
			wordList.front()->visited=false;
			wordList.clear();
		}
	}
}

void BogglePlayer::findPrefix(list<Vertex*> &path, int pathLength, TST &lex, unsigned int minimum_word_length, set<string>* words){
	Vertex* currV;
	Vertex* nextV;
	stack<Vertex*> adjCad;
	bool found;
	string key;
	string s;
	
	if (pathLength > longestWL){
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
		key= listTostring(path);
		pair <TST::TSTNode*, bool> p=lex.isPrefix(key);

		if (p.second==false){ // if the next vertex doesn't form prefix,
			path.pop_back();  //If it is not the prefix, pop it out
			continue;         // go to next availabe candidate in adj of current vertex
		}
		// Find complete word and not the prefix of other word
		else if (p.second==true && p.first->value!=0 && p.first->mid==nullptr){
			//pushInfo(path, p.first, true); //push the list in to the string vector of the vertex!!!
			if (pathLength>=minimum_word_length){
			    s.clear();
				for (list<Vertex*>::iterator itr=path.begin(); itr!=path.end(); itr++)
	        	    s.append((*itr)->name);
				words->insert(s);
			}
			path.pop_back();               //pop the node out; go to next available candidate
			continue;
		}
		//The other situations are regards as prefix situation
		//include purely prefix and prefix (but a complete word)
		else{
	        pathLength+=nextV->name.length();           // if it is prefix, update length (used in termination of searching)
			nextV->visited=true;                        // indicate next vertex is pushed into the list,
			                                            // not aviable for searching          
			//pushInfo(path, p.first, p.first->value!=0); //push the list in to the list vector of the vertex!!! 
			                                            //p.first->value!=0 means threre is no end bit;
			if (p.first->value!=0 & pathLength>=minimum_word_length){
			    s.clear();
				for (list<Vertex*>::iterator itr=path.begin(); itr!=path.end(); itr++)
	        	    s.append((*itr)->name);
				words->insert(s);
			}
            findPrefix(path, pathLength, lex,  minimum_word_length, words);

			pathLength-=nextV->name.length();           // return from findPrefix means nextV is handled
			path.pop_back();                            // pop nextV out of path list
			nextV->visited=false;                       // means the nextV is not on the list, can be search again;
		}
	}
	//all the vertexs in the adj list are handled, return;
	return;
}

void BogglePlayer::pushInfo(list<Vertex*> &wordList, TST::TSTNode* &t, bool endFlag){
	
	Vertex::PreInfo* pre= new Vertex::PreInfo;
	for (list<Vertex*>::iterator itr=wordList.begin(); itr!=wordList.end(); itr++){
		pre->lset.insert((*itr)->location);
	}
	pre->t=t;
	pre->preStr=listTostring(wordList);
	pre->endFlag=endFlag;

	if(preMode) 
		wordList.back()->PreInfoV.push_back(pre);
	else 
		wordList.back()->PosInfoV.push_back(pre);
	
}

string listTostring (list<Vertex*> &path){
	string s;
	for (list<Vertex*>::iterator itr=path.begin(); itr!=path.end(); itr++){
		s.append((*itr)->name);
	}
	return s;
}

string revStr(string s){
	string rs;
    for (string::reverse_iterator ritr=s.rbegin(); ritr!=s.rend(); ++ritr)
        rs.push_back(*ritr);
	return rs;
} 

bool BogglePlayer::isInLexicon(const string& word_to_check){
	//return false when buildLexicon is not called
	return lextst.find(word_to_check).second;
}

bool BogglePlayer::isInLexicon(const string& word_to_check, unsigned index, TST::TSTNode* t){
	return lextst.find(word_to_check, index, t).second;
}

bool BogglePlayer::isPrefix(const string& word_to_check){
	return lextst.isPrefix(word_to_check).second;
}
void BogglePlayer::getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols){

}


BogglePlayer::~BogglePlayer(){
    //preInfo
}
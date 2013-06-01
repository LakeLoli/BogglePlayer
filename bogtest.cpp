#include "boggleplayer.h"
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <fstream>
#include <time.h>

int main (int argc, char* argv[]) {
  set<string> words;
  set<string> lex;
  ifstream infile;
  ////////////////////////////////////////////////
  infile.open("lex.txt");
  if(! infile.is_open()) {
      std::cout<<"Could not open lexicon file " << "lex.txt" << ", exiting." <<std::endl;
      exit(-1);
  }
  std::cout<<"Reading lexicon from " << "lex" << "..." <<std::endl;
  std::string word;
  int i=0;

  while(infile.is_open() && infile.good())
  {
      std::getline(infile,word);
      if(word.size() < 1) continue;

      lex.insert(word);
      i++;
  }
  std::cout << lex.size() << " distinct words read from " << "lex.txt" << "." <<std::endl;
  infile.close();
  ////////////////////////////////////////////////////
  string** board;
  board = new std::string* [20];
  for(unsigned int r=0; r < 20; r++) {
    board[r] = new std::string[23];
  }
  ifstream boardfile;
  boardfile.open("brd.txt");
  if(! boardfile.is_open()) {
      std::cout<<"Could not open lexicon file " << "brd.txt" << ", exiting." <<std::endl;
      exit(-1);
  }
  std::cout<<"Reading lexicon from " << "lex" << "..." <<std::endl;
  std::string word2;
  i=0;
  
  while(boardfile.is_open() && boardfile.good())
  {
      std::getline(boardfile,word2);
      if(word2.size() < 1) continue;

	  board[i/23][i%23]=word2;
      i++;
  }
  std::cout << "brd.txt" << "." <<std::endl;
  boardfile.close();
  /////////////////////////////////////////////////////////
  BaseBogglePlayer * p = new BogglePlayer();
  string wordA("fkcariecbacqp");
  string wordX("z");
  vector<int> locations;
  p->buildLexicon(lex);
  p->setBoard(20,23,board);
  vector<int> temp=p->isOnBoard(wordX);
  /*
  if(p->isInLexicon(wordX)) {
    std::cerr << "Apparent problem with isInLexicon #1." << std::endl;
  }
  if(p->isPrefix(wordX)) {
    std::cerr << "Apparent problem with isInLexicon #1." << std::endl;
  }

  if(p->isOnBoard(wordX).size() > 0) {
    std::cerr << "Apparent problem with isOnBoard #1." << std::endl;

  }
  
  if(!p->isInLexicon(wordA)) {
    std::cerr << "Apparent problem with isInLexicon #2." << std::endl;
  }
  */ 
  clock_t t;
  t=clock();
  p->getAllValidWords(2,&words);
  t=clock()-t;
  cout << "the time interval is" << float(t)/ CLOCKS_PER_SEC <<endl; 
  cout << "the size of the words is " << words.size() << endl;
    /*
  for (set<string>::iterator sitr=words.begin(); sitr!=words.end(); sitr++){
    cout << *sitr << endl;
    temp= p->isOnBoard(*sitr);
    for (int i=0; i<temp.size();i++)
    {
        cout << temp.at(i)<< ' ';
    }
    cout<< endl;
  }
    */

  locations.clear();
  locations = p->isOnBoard(wordA);
  if(locations.size() != 1 || locations[0] != 3) {
    std::cerr << "Apparent problem with isOnBoard #2." << std::endl;
  }
  
  

  if(words.size() != 1 || words.count(wordA) != 1) {
    std::cerr << "Apparent problem with getAllValidWords #2." << std::endl;
  }
  
  delete p;
  p=nullptr;
  for (i=0; i<20; i++){
    delete [] board[i];
  }
  delete [] board;
  board=nullptr;
  return 0;

}

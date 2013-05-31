#include "baseboggleplayer.h"
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
  infile.open("boglex.txt");
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
  board = new std::string* [4];
  for(unsigned int r=0; r < 4; r++) {
    board[r] = new std::string[23];
  }
  ifstream boardfile;
  boardfile.open("brd_4.txt");
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

	  board[i/4][i%4]=word2;
      i++;
  }
  std::cout << "brd.txt" << "." <<std::endl;
  boardfile.close();
  /////////////////////////////////////////////////////////////

  BogglePlayer * p = new BogglePlayer();
  string wordA("fkcariecbacqp");
  string wordX("z");
  //lex.insert(wordA);
  //lex.insert("fdbacq");
  //lex.insert("fk");
  //lex.insert("f");
  //lex.insert("awccrcwcaec");
  string row0[] = {"o","w", "n", "g"};
  string row1[] = {"y","s", "o", "qu"};
  string row2[] = {"t","c", "v", "r"};
  string row3[] = {"e","f", "o", "v"};
  string row4[] = {"f","k", "c", "a"};
  string row5[] = {"d","e", "i", "r"};
  string row6[] = {"b","a", "c", "q"};
  string row7[] = {"a","w", "c", "p"};
  string row8[] = {"a", "z"};
  string row9[] = {"z", "x"};

  //string* board[] = {row4,row5};

  vector<int> locations;

  p->buildLexicon(lex);
  p->setBoard(4,4,board);
  //p->setBoard(20,23,board);
  vector<int> temp=p->isOnBoard(wordX);
  if(p->isInLexicon(wordX)) {
    std::cerr << "Apparent problem with isInLexicon #1." << std::endl;
  }
  if(p->isOnBoard(wordX).size() > 0) {
    std::cerr << "Apparent problem with isOnBoard #1." << std::endl;

  }
  
  if(!p->isInLexicon(wordA)) {
    std::cerr << "Apparent problem with isInLexicon #2." << std::endl;
  }
  
  clock_t tick;
  tick=clock();
  p->getAllValidWords(4,&words); 
  tick=clock()-tick;
  cout << "the time interval is" << float(tick)/ CLOCKS_PER_SEC <<endl; 
  locations.clear();
  locations = p->isOnBoard(wordA);
  /*
  if(locations.size() != 1 || locations[0] != 3) {
    std::cerr << "Apparent problem with isOnBoard #2." << std::endl;
    return -1;
  }
  */
  
  
  if(words.size() != 1 || words.count(wordA) != 1) {
    std::cerr << "Apparent problem with getAllValidWords #2." << std::endl;
    return -1;
  }
  
  delete p;
  return 0;

}

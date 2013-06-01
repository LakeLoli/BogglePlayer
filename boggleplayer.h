#ifndef BOGGLEPLAYER_H
#define BOGGLEPLAYER_H

#include <set>
#include <vector>
#include <string>
#include <stack>
#include <list>
#include "boggleutil.h"
#include <algorithm>
#include <cstdlib>
#include "baseboggleplayer.h"

//#include "baseboggleplayer.h"

using namespace std;


//class BogglePlayer: public BaseBogglePlayer{
class BogglePlayer: public BaseBogglePlayer{

public:
  BogglePlayer():setBoardCalled(false), buildLexCalled(false){}
  void buildLexicon(const set<string>& word_list);
  void setBoard(unsigned int rows, unsigned int cols, string** diceArray);
  bool getAllValidWords(unsigned int minimum_word_length, set<string>* words);
  bool isInLexicon(const string& word_to_check);
  vector<int> isOnBoard(const string& word_to_check);
  void getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols);
  ~BogglePlayer();

private:
  Graph g;
  TST lextst;
  int longestWL;
  bool setBoardCalled, buildLexCalled;

  void findPrefix(list<Vertex*> &path, int pathLength, TST::TSTNode* t, unsigned int minimum_word_length, set<string>* words);
  void allPrefix(unsigned int minimum_word_length, set<string>* words);
};

#endif // BASEBOGGLEPLAYER_H

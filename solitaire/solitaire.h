// UMBC - CMSC 341 - Fall 2024 - Proj0
#ifndef SOLITAIRE_H
#define SOLITAIRE_H
#include <iostream>
#include <string>
using namespace std;
class Grader;//this class is for grading purposes, no need to do anything
class Tester;//this is your tester class, you add your test functions in this class
// ENGLISH board shape has 33 holes
// DIAMOND board tepe has 41 holes
// We do not support FRENCH board
enum BOARDSHAPE { NONE, ENGLISH, FRENCH, DIAMOND };
const int ENGLISHSIZE = 7; // used to create a 7x7 matrix
const int DIAMONDSIZE = 9; // used to create a 9x9 matrix
const int NUMENGLISHMARBLES = 32; // total number of marbles used on an english board
const int NUMDIAMONDMARBLES = 40; // total number of marbles used on a diamond board
const int OUT = 0;
const int HOLE = 1;
const int MARBLE = 2;
#define REDSTAR "\033[1;31m\u2731\033[0m " // represents a marble (red bold astrisk)
#define BLACKSTAR "\u23E3 " // represents a hole (BENZENE RING WITH CIRCLE)

class Solitaire {
public:
    friend class Grader;
    friend class Tester;
    Solitaire(); // creates an empty object
    Solitaire(BOARDSHAPE board); // creates the specific board, allocates mem
    Solitaire(const Solitaire& rhs);
    ~Solitaire();
    void clear(); // clears memory and reinitializes all members
    bool newBoard(); // reinitializes the game with the same board shape
    void changeBoard(BOARDSHAPE board); // creates a new board
    bool play(pair<int, int> origin, pair<int, int> destination);// makes a move
    void dumpBoard();       // for debugging purposes
    int reportNumMarbles(); // for debugging purposes
private:
    int m_numRows;
    int m_numColumns;
    int** m_board;     // the 2d structure to store game 
    BOARDSHAPE m_shape; // the board shape (either english or diamond)
    int m_numMarbles;   // current number of marbles

    /******************************************
    * Private function declarations go here! *
    ******************************************/

};
#endif

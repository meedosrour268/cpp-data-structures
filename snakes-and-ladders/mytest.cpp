#include "snakes.h"
#include <iostream>
using namespace std;

class Tester {
public:
    bool testAlternativeConstructorNormal() {
        Snakes test(27);
        if (test.m_start != nullptr && test.m_playerTurn == PLAYER1TURN)  {
            return true;
        }
        return false;
    }

    bool testAlternativeConstructorError(){
        Snakes test(8);
        if (test.m_start == nullptr)  {
            return true;
        }
         return false;

     }


    bool testRestart(){
        Snakes newGame(9);
        if (newGame.m_player1 == newGame.m_start
           && newGame.m_player2 == newGame.m_start
           && newGame.m_playerTurn == PLAYER1TURN) {
            return true;
        }
        return false;

    }

    bool testPlayLadder() {
        Snakes ladderBoard(9);

        ladderBoard.play(3);

    if (ladderBoard.findCell(3)->getNorth()) {

        
        if (ladderBoard.m_player1 != ladderBoard.findCell(3)->getNorth()){
            return true;
        } else {

                      return false;
        }
    }
  
        return true;

    }

    bool testPlaySnake() {
        Snakes SnakesBoard(9);

        SnakesBoard.play(3);

         if(SnakesBoard.findCell(3)-> getSouth()) {

        if( SnakesBoard.m_player1 != (SnakesBoard.findCell(3))-> getSouth()){
        return true;
    } else {
        return false;
    }

     }

         return true;

    }

    bool testDefaultBoardNormal() {
        Snakes SnakeBoard(9);
        SnakeBoard.makeDefaultBoard(9);
        SnakeBoard.dumpBoard();
        cout << "HERE" << endl;

    }

};
int main() {

    Tester tester;
    bool result = tester.testAlternativeConstructorNormal();
    if (result) {
        cout << "Test Alternative Contructor Normal :  Passed" << endl;
    } else {
        cout << "Test Alternative Contructor Normal :  Failed" << endl;

    }

    Tester tester1;
    bool result2= tester.testAlternativeConstructorError();
    if (result2) {
        cout << "Test Alternative Constructor Error :  Passed " << endl;
    } else {
        cout << "Test Alternative Cosntructor Error :  Failed " << endl;

    return 0;

    }

    Tester tester2;
    bool result3= tester.testRestart();
    if (result3) {
        cout << "Test for Restart Reinitialization  :  Passed " << endl;
    } else {
        cout << "Test for Restart Reinitialization  :  Failed " << endl;
    }

    Tester tester3;
    bool result4= tester.testPlayLadder();
    if (result4) {
        cout << "Test for play on valid ladder move :  Passed" << endl;

    } else {
        cout << "Test for play on valid ladder move :  Failed" << endl;

    }

    Tester tester4;
    bool result5= tester.testPlaySnake();
    if (result5) {
        cout << "Test for play on valid Snake move  :  Passed" << endl;

    } else {
        cout << "Test for play on valid Snake move  :   Failed" << endl;
    }
    Tester tester5;
    bool result6 = tester.testDefaultBoardNormal();


}


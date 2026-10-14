// UMBC - CMSC 341 - Fall 2024 - Proj0
#include "solitaire.h"
using namespace std;


// Need NewBoardErrorCase. 
class Tester {
public:
    bool testCopyConstructorNormal(const Solitaire& solitaire);
    bool testCopyConstructorError(const Solitaire& solitaire);
    bool testEnglishNormalCase();  
    bool testFRENCHERRORCase(); 
    bool ConstructorNormalCase(); 
    bool ConstructorErrorCase(); 
    bool NewBoardNormalCase();
    bool NewBoardErrorCase();// need error case for this
    bool ChangeBoardDifShape();
    bool ChangeBoardSameShape();
    
};
int main() {
    Tester tester;
    Solitaire pegSolitaire(NONE);
    bool result = tester.testCopyConstructorError(pegSolitaire); // How do we test that this one passes?
    // Call the test method on the Tester instance
    bool result1 = tester.testEnglishNormalCase();
    bool result2 = tester.testFRENCHERRORCase();
    bool result3 = tester.ConstructorNormalCase();
    bool result4 = tester.ConstructorErrorCase(); // result 4
    bool result5 = tester.NewBoardNormalCase(); // result 5
    bool result6 = tester.NewBoardErrorCase();
    bool result7 = tester.ChangeBoardDifShape(); // result 6 
    bool result8 = tester.ChangeBoardSameShape();  // result 7 

    // Output the result
    cout << "Test "<< endl;
    cout << "Test English Normal Case: " << (result1 ? "Passed" : "Failed") << endl;
    cout << "Test FRENCH ERROR Case: " << (result2 ? "Passed" : "Failed") << endl;
    cout << "Test Constructor Normal Case: " << (result3 ? "Passed" : "Failed") << endl;
    cout << "Test Constructor Error Case: " << (result4 ? "Passed" : "Failed") << endl;

    cout << "Test New Board Normal Case: " << (result5 ? "Passed" : "Failed") << endl;
    cout << "Test New Board Error Case: " << (result6 ? "Passed" : "Failed") << endl;
    cout << "Test Change Board Different Shape: " << (result7 ? "Passed" : "Failed") << endl;
    cout << "Test Change Board Same Shape: " << (result8 ? "Passed" : "Failed") << endl;

    return 0;
} 
bool Tester :: testCopyConstructorNormal(const Solitaire& solitaire) {
    Solitaire copy(solitaire);
    if (solitaire.m_numColumns == copy.m_numColumns &&
        solitaire.m_numRows == copy.m_numRows &&
        solitaire.m_shape == copy.m_shape &&
        solitaire.m_numMarbles == copy.m_numMarbles &&
        solitaire.m_board != copy.m_board) {
        for (int i = 0; i < solitaire.m_numRows; i++) {
            for (int j = 0; j < solitaire.m_numColumns; j++)
            {
                if ((solitaire.m_board[i][j] != copy.m_board[i][j]) ||   //check the value
                    (solitaire.m_board[i] == copy.m_board[i])) {          //check the ith pointer
                    return false;
                }
            }
        }
        return true;// everything is correct
    }
    else return false;//everthing else
}
bool Tester::testCopyConstructorError(const Solitaire& solitaire) {
    Solitaire copy(solitaire);

    if (solitaire.m_numColumns == copy.m_numColumns &&
        solitaire.m_numRows == copy.m_numRows &&
        solitaire.m_shape == copy.m_shape &&
        solitaire.m_numMarbles == copy.m_numMarbles &&
        solitaire.m_board != copy.m_board) {
        for (int i = 0; i < solitaire.m_numRows; i++) {
            for (int j = 0; j < solitaire.m_numColumns; j++)
            {
                if ((solitaire.m_board[i][j] != copy.m_board[i][j]) ||   //check the value
                    (solitaire.m_board[i] == copy.m_board[i])) {          //check the ith pointer
                    return false;
                }
            }
        }
        return true;// everything is correct
    }
    else return false;//everthing else

}
bool Tester :: testEnglishNormalCase() { //1st bullet

    Solitaire a(ENGLISH);
    bool result = false;

    result = a.m_shape == ENGLISH
        && a.m_numRows == ENGLISHSIZE
        && a.m_numMarbles == NUMENGLISHMARBLES
        && a.m_numColumns == ENGLISHSIZE
        && (a.m_board != nullptr);

    return result;
}

bool Tester::  testFRENCHERRORCase() { //2nd bullet

    Solitaire a(FRENCH);
    bool result = false;
    if (a.m_board == nullptr && a.m_numRows == 0 && a.m_numColumns == 0 && a.m_numMarbles == 0) {
        result = true;
        return result;
    }
    else {
        result = false;
        return result;
    }
    return result;
}
bool Tester :: ConstructorNormalCase() { //4th bullet
    Solitaire a(ENGLISH);
    bool result = false;
    result = a.m_shape == ENGLISH
        && a.m_numRows == ENGLISHSIZE
        && a.m_numMarbles == NUMENGLISHMARBLES
        && a.m_numColumns == ENGLISHSIZE
        && (a.m_board != nullptr)
        && a.m_shape == ENGLISH;
    return result;
}
bool Tester:: ConstructorErrorCase() { //3rdth bullet
    Solitaire a(FRENCH);
    bool result = false;
    result = a.m_shape == 0
        && a.m_numRows == 0
        && a.m_numMarbles == 0
        && a.m_numColumns == 0
        && (a.m_board == nullptr)
        && a.m_shape == NONE;
    return result;
}
bool Tester::NewBoardNormalCase() {
    Solitaire a(ENGLISH);
    bool result = false;
    int MarbleNum1 = a.reportNumMarbles();
    a.play({ 1,3 }, { 3,3 });
    int MarbleNum2 = a.reportNumMarbles();
    if (MarbleNum1 == MarbleNum2) {
        
        return result;
    }else{
        result = true;
      return result;  
    }
}
bool Tester::NewBoardErrorCase() {
    Solitaire a(FRENCH);
    bool result = false;
    int MarbleNum1 = a.reportNumMarbles();
    a.play({ 1,3 }, { 3,3 });
    int MarbleNum2 = a.reportNumMarbles();
    if (MarbleNum1 == MarbleNum2) {
        result = true;
        return result;
    }
    return result;
}


 bool Tester :: ChangeBoardDifShape() {
        Solitaire a(ENGLISH);
        a.changeBoard(DIAMOND);
        bool result = false;
        result = a.m_shape == DIAMOND
            && a.m_numRows == DIAMONDSIZE                  //This one failed. Maybe smth in change board in solitaire. 
            && a.m_numMarbles == NUMDIAMONDMARBLES
            && a.m_numColumns == DIAMONDSIZE
            && (a.m_board != nullptr); //few iterations none shape to english shape.
            
        return result;
    }
 bool Tester::ChangeBoardSameShape() {
     Solitaire a(ENGLISH);
     a.changeBoard(ENGLISH);
     bool result = false;
     result = a.m_shape == ENGLISH
         && a.m_numRows == ENGLISHSIZE
         && a.m_numMarbles == NUMENGLISHMARBLES
         && a.m_numColumns == ENGLISHSIZE
         && (a.m_board != nullptr); //few iterations none shape to english shape.

     return result;

 }
// UMBC - CMSC 341 - Fall 2024 - Proj1
#include "snakes.h"
Snakes::Snakes(){ 
    m_start= nullptr;
    m_player1 = nullptr;
    m_player2 = nullptr;
    m_playerTurn = 0;
}
Snakes::~Snakes(){
    Cell *curr = m_start;
    while (curr != nullptr) {
        Cell *next = curr -> getNext();
        delete curr;
        curr = next;
    }
   m_start= nullptr;
    m_player1 = nullptr;
    m_player2 = nullptr;
    m_playerTurn = 0;

}
void Snakes::clear(){
      Cell *curr = m_start;
    
    while (curr != nullptr) {
        Cell *next = curr -> getNext();
        delete curr;
        curr = next;
    }
    m_start= nullptr;
    m_player1 = nullptr;
    m_player2 = nullptr;
    m_playerTurn = 0;
}
const Snakes & Snakes::operator=(const Snakes & rhs){


clear();
Cell* newRHS= rhs.m_start;
if(newRHS !=nullptr){
    m_start = new Cell(newRHS->getID());
    Cell*rhsNext= m_start;
    newRHS =newRHS->getNext();

    while(newRHS !=nullptr){
        rhsNext->setNext(new Cell(newRHS->getID()));
        rhsNext = rhsNext->getNext();
        newRHS = newRHS->getNext();
    }
}
    m_player1= findCell(rhs.m_player1->getID());
    m_player2= findCell(rhs.m_player2->getID());
    m_playerTurn = rhs.m_playerTurn;
    
    return *this;
}

int Snakes::rollDice(){

    return m_dice.getRandNum();
}
Snakes::Snakes(int boardSize){

if (boardSize < 9){
    m_start= nullptr;
    m_player1 = nullptr;
    m_player2 = nullptr;
    m_playerTurn = 0;
    m_dice.init (1,6);   

    }
  makeDefaultBoard(boardSize);
    
}                                                                     

bool Snakes::makeDefaultBoard(int boardSize){
    if (boardSize < 9){
    return false;
    }
  
    boardSize = (int) sqrt(boardSize) * (int) sqrt(boardSize); 

    int ID = 1;
    m_start = new Cell(ID);
    Cell*curr = m_start;
    while (ID < boardSize) { 

        curr -> m_next = new Cell ( ++ ID); 

        curr = curr -> m_next; 

    }
    int totalSnakesLadders = (boardSize /4) - 1;
    int nomOfSnakes = totalSnakesLadders /2;
    int nomOfLadders = nomOfSnakes;
    int Space = (boardSize - 1) / totalSnakesLadders;

   
    // Create snakes and ladders
    for (int i = Space; i < (boardSize - 1) && (nomOfLadders > 0 || nomOfSnakes > 0); i += Space) {
        Cell* ladderBase = findCell(i);
        if (ladderBase != nullptr && nomOfLadders > 0) {
            
            // Generate a random position for the ladder top
            int ladderTopID = ladderBase->getID() + (rand() % (boardSize - ladderBase->getID()));
            Cell* ladderTop = findCell(ladderTopID);
            if (ladderTop != nullptr && ladderTop->getID() > ladderBase->getID()) {
                ladderBase->setNorth(ladderTop); // Set ladder
                nomOfLadders--;
            }
        }

        // Create snakes similarly
        if (ladderBase != nullptr && nomOfSnakes > 0) {
            int snakeBottomID = ladderBase->getID() - (rand() % (ladderBase->getID() - 1));
            Cell* snakeBottom = findCell(snakeBottomID);
            if (snakeBottom != nullptr && snakeBottom->getID() < ladderBase->getID()) {
                ladderBase->setSouth(snakeBottom); // Set snake
                nomOfSnakes--;
            }
        }
    }
    m_player1 = m_start,
    m_player2 = m_start;
    m_playerTurn = PLAYER1TURN;
    return true;
}


Cell * Snakes :: findCell (int ID) {
    Cell * Temp = m_start;
    while ( Temp != nullptr && Temp -> m_cellID != ID) {
        Temp = Temp -> m_next ;
    }
return Temp;
}



void Snakes::makeRandomBoard(int boardSize, int numSnakesLadders){
 if (boardSize < 9) {
        return;
    }
    clear();
    makeDefaultBoard(boardSize);
    int snakes = 0;
    int ladders =0;

    if(numSnakesLadders <2  ){
    int snakes = 1;
    int ladders = 1;
    }
    else if (numSnakesLadders >((boardSize/2)-2)){
    numSnakesLadders=(boardSize/2)-2;
    int snakes = numSnakesLadders / 2;
    int ladders = numSnakesLadders - snakes;
    }
    else{
    int snakes = numSnakesLadders / 2;
    int ladders = numSnakesLadders - snakes;
    }
    

    while (ladders > 0 || snakes > 0) {
        int randPos = rand() % (boardSize - 2) + 1; // Position from 1 to boardSize-1
        Cell* baseCell = findCell(randPos);
        
        if (baseCell != nullptr) {
            if (ladders > 0) {
                int topPos = randPos + (rand() % (boardSize - randPos));
                Cell* topCell = findCell(topPos);
                if (topCell != nullptr && topCell->getID() > baseCell->getID()) {
                    baseCell->setNorth(topCell);
                    ladders--;
                }
            }

            if (snakes > 0) {
                int bottomPos = randPos - (rand() % randPos);
                Cell* bottomCell = findCell(bottomPos);
                if (bottomCell != nullptr && bottomCell->getID() < baseCell->getID()) {
                    baseCell->setSouth(bottomCell);
                    snakes--;
                }
            }
        }
    }   
}

bool Snakes::play(int dice){

Cell * playerPlaying;

if ( m_playerTurn == PLAYER1TURN) {
    playerPlaying == m_player1;

} else if (m_playerTurn == PLAYER2TURN) {   
    playerPlaying = m_player2;



} else if (m_playerTurn == 0) {
    return false;
}
int playerMove= playerPlaying->getID() +dice;

Cell*playerMoveTo =findCell(playerMove);
if (playerPlaying->getNorth()!= nullptr){
    playerPlaying= playerPlaying->getNorth();
}else if( playerPlaying->getSouth()!= nullptr){
    playerPlaying = playerPlaying->getSouth();
}
    
if ( m_playerTurn == PLAYER1TURN) {
   m_player1 ==playerPlaying ;
   m_playerTurn=PLAYER2TURN;

} else if (m_playerTurn == PLAYER2TURN) {   
    m_player2 =playerPlaying ;
    m_playerTurn=PLAYER1TURN;
}

return true;
}
void Snakes::reStart(){
    //Resetting both players to the starting position to start a new game

    m_player1 = m_start;
    m_player2 = m_start;

    // Starting the game with player 1's turn

    m_playerTurn = PLAYER1TURN;





}
void Snakes::dumpBoard(){
    if (m_start != nullptr){
        int ladderCode, snakeCode;
        Cell *temp = m_start;
        cout << "START => ";
        while(temp != nullptr){
            cout << temp->m_cellID;
            if (temp == m_player1) cout << " (Player 1)";
            if (temp == m_player2) cout << " (Player 2)";
            if (temp->m_north != nullptr) ladderCode = temp->m_north->m_cellID;
            else ladderCode = -1;
            if (temp->m_south != nullptr) snakeCode = temp->m_south->m_cellID;
            else snakeCode = -1;
            if (ladderCode != -1 || snakeCode != -1){
                cout << " (";
                if (ladderCode != -1) cout << "\033[1;32mLadder to: " << ladderCode << "\033[0m";// green text
                if (snakeCode != -1) cout << "\033[1;31mSnake to: " << snakeCode << "\033[0m";//red text
                cout << ")";
            }
            cout << " => ";
            temp = temp->m_next;
        }
        cout << "END" << endl;
    }
}
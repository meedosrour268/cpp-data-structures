// UMBC - CMSC 341 - Fall 2024 - Proj0
#include "solitaire.h"
#include <cmath>

Solitaire::Solitaire() {
    m_numRows = 0;
    m_numColumns = 0;
    m_numMarbles = 0;
    m_board = nullptr;
    m_shape = NONE; // if m_board = no pointer, return true.

}

Solitaire::Solitaire(BOARDSHAPE board) {
        m_board = nullptr;
        if (board == ENGLISH) {                             
        m_numRows = ENGLISHSIZE;
        m_numColumns = ENGLISHSIZE;
        m_numMarbles = NUMENGLISHMARBLES;
        m_shape = ENGLISH;
        }
        else if (board == DIAMOND) {
        m_numRows = DIAMONDSIZE;
        m_numColumns = DIAMONDSIZE;              // if m_board is not= new pointer
        m_numMarbles = NUMDIAMONDMARBLES;
        m_shape = DIAMOND;
        }
        else {
        m_numRows = 0;
        m_numColumns = 0;
        m_numMarbles = 0;
        
        m_shape = NONE; 
        return;
        }


        m_board = new int* [m_numRows];

        for (int i = 0; i < m_numRows; ++i) {
        m_board[i] = new int[m_numColumns];
        }
        if (board == ENGLISH) {
            
            for (int i = 0; i <= 6; ++i) {
                 for (int j = 0; j <= 6; ++j) {
                    m_board[i][j] = MARBLE;
                    if ((i == 0 || i == 1 || i == 5 || i == 6) && (j == 0 || j == 1 || j == 5 || j == 6)) {
                    m_board[i][j] = OUT;
                    }
                }       
            } 
            m_board[3][3] = HOLE;  
            
        }

        else if (board == DIAMOND) {
           for (int i = 0; i < m_numRows; ++i) {
                for (int j = 0; j < m_numColumns; ++j) {
                    if (abs(i - 4) + abs(j - 4) <= 4) {
                     m_board[i][j] = MARBLE;
                    }
                }
             }
            m_board[4][4] = HOLE;  // Central hole
           
        }else if (board != DIAMOND && board != ENGLISH ){
            m_numRows = 0;
            m_numColumns = 0;
            m_numMarbles = 0;
            m_board = nullptr;
            m_shape = NONE; 
        }

        
}

Solitaire::Solitaire(const Solitaire& rhs) {
    m_numRows = rhs.m_numRows;
    m_numColumns = rhs.m_numColumns;
    m_numMarbles = rhs.m_numMarbles;
    m_shape = rhs.m_shape;
    m_board = nullptr;

    if (rhs.m_board != nullptr) {
         
        if (m_numRows > 0 && m_numColumns > 0) {
            m_board = new int* [m_numRows];
            for (int i = 0; i < m_numRows; ++i) {
                m_board[i] = new int[m_numColumns];
            }
            for (int i = 0; i < m_numRows; ++i) {
                for (int j = 0; j < m_numColumns; ++j) {
                    m_board[i][j] = rhs.m_board[i][j];
                }
            }
        }
    }

}
Solitaire::~Solitaire() {
        for (int i = 0; i < m_numRows; ++i) {
            delete[] m_board[i];
        }
        delete[] m_board;
}
void Solitaire::clear() {
    
    if (m_board != nullptr) {
        for (int i = 0; i < m_numRows; ++i) {
            delete[] m_board[i];
        }
        delete[] m_board;
    }
    m_numRows = 0;
    m_numColumns = 0;
    m_numMarbles = 0;
    m_board = nullptr;
    m_shape = NONE;
}


bool Solitaire::play(pair<int, int> origin, pair<int, int> destination) {
    int originRow = origin.first;
    int originCol = origin.second;
    int destinationRow = destination.first;
    int destinationCol = destination.second;

    // Check if the origin and destination cells are valid
    if (m_board[originRow][originCol] != MARBLE || m_board[destinationRow][destinationCol] != HOLE) {
        return false;
    }

    
    int middleRow = (originRow + destinationRow) /2;
    int middleCol = (originCol + destinationCol) /2;

    // Check if the move is valid
    if ((originRow == destinationRow && (originCol - destinationCol == 2 || originCol - destinationCol == -2) && m_board [middleRow] [middleCol] == MARBLE) || 
      (originCol == destinationCol && (originRow - destinationRow == 2 || originRow - destinationRow == -2) && m_board[middleRow][middleCol] == MARBLE))  { // Vertical move
        
        // Perform the move
        m_board[originRow][originCol] = HOLE;            // Remove marble from origin
        m_board[middleRow][middleCol] = HOLE;                  // Remove the jumped-over marble
        m_board[destinationRow][destinationCol] = MARBLE; // Place marble in destination

        // Update the number of marbles
        m_numMarbles--;

        return true;
    }

    // If any check fails, the move is invalid
    return false;
}




void Solitaire::changeBoard(BOARDSHAPE board) {

    if (board != m_shape) {
        // If the new shape is different, update the shape
        m_shape = board;
        // Reinitialize the board with the new shape
        newBoard();
    }
    else {
        // If the shape is the same, just reinitialize the current board
        if (m_shape == ENGLISH) {
            m_numRows = ENGLISHSIZE;
            m_numColumns = ENGLISHSIZE;
            m_numMarbles = NUMENGLISHMARBLES;

            for (int i = 0; i <= 6; ++i) {
                for (int j = 0; j <= 6; ++j) {
                    m_board[i][j] = MARBLE;
                    if ((i == 0 || i == 1 || i == 5 || i == 6) && (j == 0 || j == 1 || j == 5 || j == 6)) {
                        m_board[i][j] = OUT;
                    }
                }
            }
            m_board[3][3] = HOLE;
        }
        else if (m_shape == DIAMOND) {
            m_numRows = DIAMONDSIZE;
            m_numColumns = DIAMONDSIZE;
            m_numMarbles = NUMDIAMONDMARBLES;

            for (int i = 0; i < m_numRows; ++i) {
                for (int j = 0; j < m_numColumns; ++j) {
                    if (abs(i - 4) + abs(j - 4) <= 4) {
                        m_board[i][j] = MARBLE;
                    }
                }
            }
            m_board[4][4] = HOLE;  // Central hole
        }
    }
}
bool Solitaire::newBoard() {
    if ((m_shape == NONE) || (m_shape == FRENCH)) {
        return false;
    }
    clear();  // Clear the existing board and reset the object

    // Reinitialize the board based on the current shape
    if (m_shape == ENGLISH) {
        m_numRows = ENGLISHSIZE;
        m_numColumns = ENGLISHSIZE;
        m_numMarbles = NUMENGLISHMARBLES;
    }
    else if (m_shape == DIAMOND) {
        m_numRows = DIAMONDSIZE;
        m_numColumns = DIAMONDSIZE;
        m_numMarbles = NUMDIAMONDMARBLES;
    }

    // Allocate memory for the new board
    m_board = new int* [m_numRows];
    for (int i = 0; i < m_numRows; ++i) {
        m_board[i] = new int[m_numColumns];
    }

    // Initialize the board cells
    for (int i = 0; i < m_numRows; ++i) {
        for (int j = 0; j < m_numColumns; ++j) {
            m_board[i][j] = OUT;  // Initialize all cells to OUT
        }
    }

   if (m_shape == ENGLISH) { //TEST THIS 
            
            for (int i = 0; i <= 6; ++i) {
                 for (int j = 0; j <= 6; ++j) {
                    m_board[i][j] = MARBLE;
                    if ((i == 0 || i == 1 || i == 5 || i == 6) && (j == 0 || j == 1 || j == 5 || j == 6)) {
                    m_board[i][j] = OUT;
                    }
                }       
            } 
            m_board[3][3] = HOLE;  
        }
    else if (m_shape == DIAMOND) {
           for (int i = 0; i < m_numRows; ++i) {
                for (int j = 0; j < m_numColumns; ++j) {
                    if (abs(i - 4) + abs(j - 4) <= 4) {
                     m_board[i][j] = MARBLE;
                    }
                }
             }
            m_board[4][4] = HOLE;  // Central hole
        }
   return true;
}
void Solitaire::dumpBoard() {
    // this is to facilitate debugging 
    cout << "  ";
    for (int k = 0; k < m_numColumns; k++) {
        cout << k << " ";
    }
    cout << endl;
    for (int i = 1; i <= m_numRows; i++) {
        cout << i - 1 << " ";
        for (int j = 1; j <= m_numColumns; j++) {
            if (m_board[i - 1][j - 1] == OUT)
                cout << "  ";
            else if (m_board[i - 1][j - 1] == HOLE)
                cout << BLACKSTAR;
            else
                cout << REDSTAR;
        }
        cout << endl;
    }
    cout << endl;
}
int Solitaire::reportNumMarbles() {
    // this is to facilitate debugging 
   // cout << "You have " << m_numMarbles << " marbles to remove!" << endl;
    return m_numMarbles ;
}
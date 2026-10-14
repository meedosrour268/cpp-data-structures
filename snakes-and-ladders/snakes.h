// UMBC - CMSC 341 - Fall 2024 - Proj1
#ifndef SNAKES_H
#define SNAKES_H
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <random>
#include <vector>
#include <ctime>     //used to get the current time
using namespace std;
class Grader;//this class is for grading purposes, no need to do anything
class Tester;//this is your tester class, you add your test functions in this class
class Snakes;
const int NOGAME = 0;
const int PLAYER1TURN = 1;
const int PLAYER2TURN = 2;
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(){}
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }
    void init(int min, int max){
        m_min = min;
        m_max = max;
        m_type = UNIFORMINT;
        m_generator = std::mt19937(10);// 10 is the fixed seed value
        m_unidist = std::uniform_int_distribution<>(min,max);
    }
    void getShuffle(vector<int> & array){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

    string getRandString(int size){
        // the parameter size specifies the length of string we ask for
        // to use ASCII char the number range in constructor must be set to 97 - 122
        // and the Random type must be UNIFORMINT (it is default in constructor)
        string output = "";
        for (int i=0;i<size;i++){
            output = output + (char)getRandNum();
        }
        return output;
    }
    
    int getMin(){return m_min;}
    int getMax(){return m_max;}
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Cell{
public:
    friend class Grader;
    friend class Tester;
    friend class Snakes;
    Cell():m_cellID(0),
    m_next(nullptr),
    m_north(nullptr),
    m_south(nullptr){}
    Cell(int id):m_cellID(id),
    m_next(nullptr),
    m_north(nullptr),
    m_south(nullptr){}
    ~Cell(){}

    void setID(int id){m_cellID = id;}
    int getID(){return m_cellID;}

    void setNorth(Cell * aNode){m_north = aNode;}
    Cell * getNorth(){return m_north;}

    void setSouth(Cell * aNode){m_south = aNode;}
    Cell * getSouth(){return m_south;}
    
    void setNext(Cell * aNode){m_next=aNode;}
    Cell * getNext(){return m_next;}
private:
    int    m_cellID;
    Cell * m_next;
    Cell * m_north; // used to store a ladder destination
    Cell * m_south; // used to store a snake destination
};
class Snakes{
public:
    friend class Grader;
    friend class Tester;
    Snakes();
    Snakes(int boardSize);
    ~Snakes();
    void clear();
    int rollDice();
    bool play(int dice);
    void reStart();
    bool makeDefaultBoard(int boardSize);
    void makeRandomBoard(int boardSize, int numSnakesLadders);
    int whoseTurn() {return m_playerTurn;}
    void dumpBoard();
    const Snakes & operator=(const Snakes & rhs);
private:
    Cell * m_start;     // pointer to the first cell of the board
    Random m_dice;      // random number generator between 1 and 6
    Cell * m_player1;   // current location on the board
    Cell * m_player2;   // current location on the board
    int m_playerTurn;   // set to 1 when it is player1 turn, 0 means there is no game

    /******************************************
    * Private function declarations go here! *
    ******************************************/
  Cell * findCell (int ID);

};
#endif 
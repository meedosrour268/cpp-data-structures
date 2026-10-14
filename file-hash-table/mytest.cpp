// CMSC 341 - Fall 2024 - Project 4
#include "filesys.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;
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

unsigned int hashCode(const string str) {
   unsigned int val = 0 ;
   const unsigned int thirtyThree = 33 ;  // magic number from textbook
   for (unsigned int i = 0 ; i < str.length(); i++)
      val = val * thirtyThree + str[i] ;
   return val ;
}

class Tester{
public:
    bool TestInsert();
    bool TestFileErrorCase();
    bool TestNonCollidingKeys();
    bool TestCollidingKeys();
    bool RemoveNonColliding();
    bool RemoveColliding();
    bool RehashTriggerInsert();
    bool RehashTriggerRemove();
    bool RehashAfterRatio();
    bool RehashAfterLoad();

private:
    // Helper function to allow verification that insertion is working.
    bool VerifyInsert(FileSys &fileSystem, const File files[], int Filenum);
};

int main() {
    Tester tester;

    if (tester.TestInsert()) {
        cout << "\tTest insertion operation in the hash table             : Passed" << endl;
    } else {
        cout << "\tTest insertion operation in the hash table             : Failed" << endl;
    }

    if (tester.TestFileErrorCase()) {
        cout << "\tTest Get File error case:                              : Passed" << endl;
    } else {
        cout << "\tTest Get File normal case:                             : Failed" << endl;
    }

    if (tester.TestNonCollidingKeys()) {
        cout << "\tTest File with non colliding keys                      : Passed" << endl;
    } else {
        cout << "\tTest File with non colliding keys                      : Failed" << endl;
    }

    if (tester.TestCollidingKeys()) {
        cout << "\tTest File with colliding keys                          : Passed" << endl;
    } else {
        cout << "\tTest File with colliding keys                          : Failed" << endl;
    }

    if (tester.RemoveNonColliding()) {
        cout << "\tTest File with removing non colliding keys             : Passed" << endl;
    } else {
        cout << "\tTest File with removing non colliding keys             : Failed" << endl;
    }

    if (tester.RemoveColliding()) {
        cout << "\tTest File with removing colliding keys                 : Passed" << endl;
    } else {
        cout << "\tTest File with removing colliding keys                 : Failed" << endl;
    }

    if (tester.RehashTriggerInsert()) {
        cout << "\tTest File with triggering rehash insertion             : Passed" << endl;
    } else {
        cout << "\tTest File with triggering rehash insertion             : Failed" << endl;
    }

    if (tester.RehashTriggerRemove()) {
        cout << "\tTest File with triggering rehash removal               : Passed" << endl;
    } else {
        cout << "\tTest File with triggering rehash removal               : Failed" << endl;
    }

    if (tester.RehashAfterRatio()) {
        cout << "\tTest File with triggering rehash due to ratio          : Passed" << endl;
    } else {
        cout << "\tTest File with triggering rehash due to ratio          : Failed" << endl;
    }

    if (tester.RehashAfterLoad()) {
        cout << "\tTest File with triggering rehash due to load factor    : Passed" << endl;
    } else {
        cout << "\tTest File with triggering rehash due to load factor    : Failed" << endl;
    }

   



    return 0;
}

/*
Test function that will insert 10 files into the table using a linear probing approach. I have made an array with manually defined files and used the simple hash function 
given to us in out driver.cpp file. 
*/
bool Tester::TestInsert() {
    int TableSize = MINPRIME;
    FileSys FilingSystem(TableSize, hashCode, LINEAR);
    const int Filenum = 10;
    File FileTests[Filenum] = { 
        File("fileA", 100000, true),
        File("fileB", 100001, true),
        File("fileC", 100002, true),
        File("fileD", 100003, true),
        File("fileE", 100004, true),
        File("fileF", 100005, true),
        File("fileG", 100006, true),
        File("fileH", 100007, true),
        File("fileI", 100008, true),
        File("fileJ", 100009, true)
    };

    for (int i = 0; i < Filenum; ++i) {
        if (!FilingSystem.insert(FileTests[i])) {
            return false;
        }
    }

    return VerifyInsert(FilingSystem, FileTests, Filenum);
}

/*

This helper function verifies the correct insertion of a set of files into a hash table. 

it ensures that the hash table is not empty by verifying that the load factor lambda is greater than zero.
it then compares the actual number of entries in the hash table (m_currentSize) with the expected number of files 
(Filenum) to confirm that all files have been successfully inserted.For each file, the function calculates its 
expected position in the hash table using a hash function and checks if the file exists at that position or through
linear probing.

*/

bool Tester::VerifyInsert(FileSys &fileSystem, const File files[], int Filenum) {
    if (fileSystem.lambda() <= 0) {
        return false;
    }

    if (fileSystem.m_currentSize != Filenum) {
        return false;
    }

    for (int i = 0; i < Filenum; ++i) {
        const File &file = files[i];
        unsigned int hashValue = hashCode(file.getName());
        unsigned int ProbingIndex = hashValue % fileSystem.m_currentCap;

        unsigned int index = ProbingIndex;
        int j = 0;
        bool found = false;

        while (j < fileSystem.m_currentCap) {
            File *currentFile = fileSystem.m_currentTable[index];

            if (currentFile == nullptr) {
                break;
            } else if (currentFile->getUsed()) {
                if (currentFile->getName() == file.getName() &&
                    currentFile->getDiskBlock() == file.getDiskBlock()) {
                    found = true;
                    break;
                }
            }

            j++;
            index = (ProbingIndex + j) % fileSystem.m_currentCap;
        }

        if (!found) {
            return false;
        }
    }

    return true;
}

/*

This test function makes up a hash table with MINPRIME as the defined size and inserts a number offiles into the hash table. 
Each file is uniquely named from "fileA" to "fileJ" with sequential disk block numbers starting from 100000. The function 
ensures each file is successfully inserted, returning false if the test fails and true if the test passes. 

*/
bool Tester::TestFileErrorCase() {

int TableSize = MINPRIME;
    FileSys FilingSystem(TableSize, hashCode, LINEAR);
    const int Filenum = 10;
    File FileTests[Filenum] = { 
        File("fileA", 100000, true),
        File("fileB", 100001, true),
        File("fileC", 100002, true),
        File("fileD", 100003, true),
        File("fileE", 100004, true),
        File("fileF", 100005, true),
        File("fileG", 100006, true),
        File("fileH", 100007, true),
        File("fileI", 100008, true),
        File("fileJ", 100009, true)
    };

    for (int i = 0; i < Filenum; ++i) {
        if (!FilingSystem.insert(FileTests[i])) {
            return false;
        }
    }

string NotValidFile = "Not a valid file";
int NotValidDiskBlock = 999999;

File FoundFile = FilingSystem.getFile(NotValidFile, NotValidDiskBlock);

if (FoundFile.getUsed()) {
    return false;
    
}

return true;

}

/*
This test function sets up the hash table with a fixed size like the previous test and makes ten unique files. Each file is inserted and 
then retrieved to verify that the hash table correctly handles insertion and search operations under non-colliding conditions. 

*/
bool Tester :: TestNonCollidingKeys() { 

int TableSize = MINPRIME;
    FileSys FilingSystem(TableSize, hashCode, LINEAR);
    const int Filenum = 10;
    File FileTests[Filenum] = { 
        File("fileA", 100000, true),
        File("fileB", 100001, true),
        File("fileC", 100002, true),
        File("fileD", 100003, true),
        File("fileE", 100004, true),
        File("fileF", 100005, true),
        File("fileG", 100006, true),
        File("fileH", 100007, true),
        File("fileI", 100008, true),
        File("fileJ", 100009, true)
    };

    for (int i = 0; i < Filenum; ++i) {
        if (!FilingSystem.insert(FileTests[i])) {
            return false;
        }
    }

      for (int i = 0; i < Filenum; ++i) {
        const File & FirstFile = FileTests[i];
        File retrievedFile = FilingSystem.getFile(FirstFile.getName(), FirstFile.getDiskBlock());

        
        if (!retrievedFile.getUsed()) {
            
            return false;
        }

        if (retrievedFile.getName() != FirstFile.getName() ||
            retrievedFile.getDiskBlock() != FirstFile.getDiskBlock()) {
            
            return false;
        }
    }

    
    return true;
}

/*
This test function sets up the hash table with a fixed size like the previous test and makes ten unique files. Each file is inserted and 
then retrieved to verify that the hash table correctly handles insertion and search operations under colliding conditions. 
*/

bool Tester ::TestCollidingKeys() {

int TableSize = MINPRIME;
    FileSys FilingSystem(TableSize, hashCode, LINEAR);
    const int Filenum = 10;
    File FileTests[Filenum] = { 
        File("fileA", 100000, true),
        File("fileA", 100001, true),
        File("fileA", 100002, true),
        File("fileA", 100003, true),
        File("fileA", 100004, true),
        File("fileA", 100005, true),
        File("fileA", 100006, true),
        File("fileA", 100007, true),
        File("fileA", 100008, true),
        File("fileA", 100009, true)
    };

    for (int i = 0; i < Filenum; ++i) {
        if (!FilingSystem.insert(FileTests[i])) {
            return false;
        }
    }

for (int i = 0; i < Filenum; ++i) {
        const File &FirstFile = FileTests[i];
        File retrievedFile = FilingSystem.getFile(FirstFile.getName(), FirstFile.getDiskBlock());

        
        if (!retrievedFile.getUsed()) {
            
            return false;
        }

        if (retrievedFile.getName() != FirstFile.getName() ||
            retrievedFile.getDiskBlock() != FirstFile.getDiskBlock()) {
           
            return false;
        }
    }

    
    return true;
}


/*
This test function inserts a small set of files into the hash table, then removes two of these files 
and verifies that they are no longer used in the hash table.
*/

bool Tester ::RemoveNonColliding() { 

int TableSize = MINPRIME;
    FileSys FilingSystem(TableSize, hashCode, LINEAR);
    const int Filenum = 5;
    File FileTests[Filenum] = { 
        File("fileA", 100000, true),
        File("fileB", 100001, true),
        File("fileC", 100002, true),
        File("fileD", 100003, true),
        File("fileE", 100004, true),
        
    };

    for (int i = 0; i < Filenum; ++i) {
        if (!FilingSystem.insert(FileTests[i])) {
            return false;
        }
    }

if (!FilingSystem.remove(FileTests[1])) { // Removing file B
        return false;
    }
    if (!FilingSystem.remove(FileTests[3])) { // Removing file D
        return false;
    }

    
    File retrievedFile = FilingSystem.getFile(FileTests[1].getName(), FileTests[1].getDiskBlock());
    if (retrievedFile.getUsed()) {
      
        return false;
    }

    retrievedFile = FilingSystem.getFile(FileTests[3].getName(), FileTests[3].getDiskBlock());
    if (retrievedFile.getUsed()) {
        
        return false;
    }

   
    for (int i = 0; i < Filenum; ++i) {
        if (i != 1 && i != 3) {
            retrievedFile = FilingSystem.getFile(FileTests[i].getName(), FileTests[i].getDiskBlock());
            if (!retrievedFile.getUsed()) {
               
                return false;
            }
            if (retrievedFile.getName() != FileTests[i].getName() ||
                retrievedFile.getDiskBlock() != FileTests[i].getDiskBlock()) {
                
                return false;
            }
        }
    }

    
    return true;
}

/*
This test function inserts five files with identical names but unique disk blocks to intentionally create collisions
within the hash table. It then removes two of these files and verifies that they have been 
correctly marked as not used within the hash table. 
*/
bool Tester ::RemoveColliding() {

int TableSize = MINPRIME;
    FileSys FilingSystem(TableSize, hashCode, LINEAR);
    const int Filenum = 5;
    File FileTests[Filenum] = { 
        File("fileA", 100000, true),
        File("fileA", 100001, true),
        File("fileA", 100002, true),
        File("fileA", 100003, true),
        File("fileA", 100004, true),
        
    };

    for (int i = 0; i < Filenum; ++i) {
        if (!FilingSystem.insert(FileTests[i])) {
            return false;
        }
    }
if (!FilingSystem.remove(FileTests[1])) { 
        return false;
    }
    if (!FilingSystem.remove(FileTests[3])) { 
        return false;
    }

   
    File retrievedFile = FilingSystem.getFile(FileTests[1].getName(), FileTests[1].getDiskBlock());
    if (retrievedFile.getUsed()) {
        
        return false;
    }

    retrievedFile = FilingSystem.getFile(FileTests[3].getName(), FileTests[3].getDiskBlock());
    if (retrievedFile.getUsed()) {
       
        return false;
    }

    
    for (int i = 0; i < Filenum; ++i) {
        if (i != 1 && i != 3) {
            retrievedFile = FilingSystem.getFile(FileTests[i].getName(), FileTests[i].getDiskBlock());
            if (!retrievedFile.getUsed()) {
                
                return false;
            }
            if (retrievedFile.getName() != FileTests[i].getName() ||
                retrievedFile.getDiskBlock() != FileTests[i].getDiskBlock()) {
                
                return false;
            }
        }
    }

    
    return true;
}

/*
The function begins by setting a threshold for the hash table's load factor at 50% of its capacity, 
then proceeds to insert enough files to exceed this threshold, which initiatiates a rehash. After the rehash, 
the function checks to ensure that the table's capacity has indeed increased, which confirms that the rehash occurred.
It further verifies that each file inserted prior to the rehash can still be accurately retrieved from the new table.
*/

bool Tester  ::RehashTriggerInsert() {

int TableSize = MINPRIME; 
    FileSys fileSystem(TableSize, hashCode, LINEAR);

   
    int Capacity = fileSystem.m_currentCap; 

    
    double loadFactorTrigger = 0.5;
    int numInsertions = static_cast<int>(loadFactorTrigger * Capacity) + 1; 

    
    std::vector<File> insertedFiles;
    for (int i = 0; i < numInsertions; ++i) {
        string fileName = "file" + to_string(i);
        int diskBlock = 100000 + i;
        File file(fileName, diskBlock, true);
        if (!fileSystem.insert(file)) {
            
            return false;
        }
        insertedFiles.push_back(file);
    }

   
    if (fileSystem.m_currentCap == Capacity) {
        
        return false;
    }

    
    for (int i = 0; i < static_cast<int>(insertedFiles.size()); ++i) {
        const File& file = insertedFiles[i];
        File Fileaccess = fileSystem.getFile(file.getName(), file.getDiskBlock());
        if (!Fileaccess.getUsed()) {
            
            return false;
        }
        if (Fileaccess.getName() != file.getName() ||
            Fileaccess.getDiskBlock() != file.getDiskBlock()) {
            
            return false;
        }
    }

  
    return true;
}

/*

This test initially populates the table with 30 files and then systematically removes each one to simulate conditions 
that might prompt a rehash due to a high deleted ratio. After clearing the table, it inserts a new file to act as a 
trigger for rehashing. The function then verifies that the old table has been correctly cleared post-rehash and confirms
that the new "trigger" file is not only present but also correctly stored and retrievable with its specified properties. 

*/
bool Tester ::RehashTriggerRemove() {

   int TableSize = MINPRIME;
    FileSys fileSystem(TableSize, hashCode, LINEAR);

   
    vector<File> insertedFiles;
    for (int i = 0; i < 30; ++i) {
        string fileName = "File" + to_string(i);
        int diskBlock = 100000 + i;
        File file(fileName, diskBlock, true);
        if (!fileSystem.insert(file)) {
            return false;  
        }
        insertedFiles.push_back(file);
    }

    
    for (int i = 0; i < 30; ++i) {
        if (!fileSystem.remove(insertedFiles[i])) {
            return false;  
        }
    }

    
    fileSystem.insert(File("trigger", 100030, true));  

    
    if (fileSystem.m_oldTable != nullptr) {
        return false;  
    }

    
    File retrieved = fileSystem.getFile("trigger", 100030);
    if (!retrieved.getUsed() || retrieved.getName() != "trigger" || retrieved.getDiskBlock() != 100030) {
        return false; 
    }

    return true; 
}

/*

This test initially populates the table with 30 files and then systematically removes each one to simulate conditions that 
might prompt a rehash due to a high deleted ratio. After clearing the table, it inserts a new file to act as a trigger for rehashing.

*/

bool Tester:: RehashAfterRatio() {

     int TableSize = MINPRIME; 
    FileSys fileSystem(TableSize, hashCode, LINEAR);

    
    int numInsertions = 30;  
    for (int i = 0; i < numInsertions; ++i) {
        string fileName = "File" + to_string(i);
        int diskBlock = 100000 + i;
        if (!fileSystem.insert(File(fileName, diskBlock, true))) {
            return false;  
        }
    }

   
    for (int i = 0; i < numInsertions; ++i) {
        if (!fileSystem.remove(File("File" + to_string(i), 100000 + i, true))) {
            return false;  
        }
    }

   
    fileSystem.insert(File("trigger", 100001 + numInsertions, true)); 

    
    if (fileSystem.m_oldTable != nullptr) {
        return false;  
    }

    
    
    File retrieved = fileSystem.getFile("trigger", 100001 + numInsertions);
    if (!retrieved.getUsed() || retrieved.getName() != "trigger" || retrieved.getDiskBlock() != (100001 + numInsertions)) {
        return false;  
    }

    return true;  
}

/*
It inserts each file, verifying successful insertion, and checks for the old table's presence afterward to ensure that rehashing is initiated as expected. After rehashing, 
it confirms every inserted file remains accurately retrievable, maintaining data integrity. 

*/

bool Tester::RehashAfterLoad() {
    int TableSize = MINPRIME;  
    FileSys fileSystem(TableSize, hashCode, LINEAR);

   
    double loadFactorThreshold = 0.5;  
    int numInsertionsNeeded = static_cast<int>(loadFactorThreshold * TableSize) + 1; 

    
    for (int i = 0; i < numInsertionsNeeded; ++i) {
        string fileName = "File" + to_string(i);
        int diskBlock = 100000 + i;
        if (!fileSystem.insert(File(fileName, diskBlock, true))) {
            return false;  
        }
    }


    if(!fileSystem.m_oldTable){
        return false;  
    }


    for (int i = 0; i < numInsertionsNeeded; ++i) {
        string fileName = "File" + to_string(i);
        int diskBlock = 100000 + i;
        File retrieved = fileSystem.getFile(fileName, diskBlock);
        if (!retrieved.getUsed() || retrieved.getName() != fileName || retrieved.getDiskBlock() != diskBlock) {
            return false;  
        }
    }

    
    if (fileSystem.m_currentCap != fileSystem.findNextPrime((51 + 0) * 4)){
        return false;
    }

    return true; 
}
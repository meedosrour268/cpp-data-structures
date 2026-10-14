// UMBC - CMSC 341 - Fall 2024 - Proj2
#include "swarm.h"          
#include <iostream>
#include <vector>
#include <algorithm>        // For std::find and std::shuffle found in the random class. 
#include <random>
#include <cmath>
using namespace std;

// Enumeration for random number generation types
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};

// Random class to generate random numbers and shuffle sequences 
class Random {
public:
    // Default constructor
    Random(){}

    // Parameterized constructor
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20)
        : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            m_generator = std::mt19937(m_device());
            m_normdist = std::normal_distribution<>(mean, stdev);
        }
        else if (type == UNIFORMINT){
            m_generator = std::mt19937(10);
            m_unidist = std::uniform_int_distribution<>(min, max);
        }
        else if (type == UNIFORMREAL){
            m_generator = std::mt19937(10);
            m_uniReal = std::uniform_real_distribution<double>((double)min, (double)max);
        }
        else{
            m_generator = std::mt19937(m_device());
        }
    }

    // Set seed for the random number generator
    void setSeed(int seedNum){
        m_generator = std::mt19937(seedNum);
    }

    // Initialize the Random class with new parameters
    void init(int min, int max){
        m_min = min;
        m_max = max;
        m_type = UNIFORMINT;
        m_generator = std::mt19937(10);
        m_unidist = std::uniform_int_distribution<>(min, max);
    }

    // Shuffle a vector of integers
    void getShuffle(vector<int>& array){
        for (int i = m_min; i <= m_max; i++){
            array.push_back(i);
        }
        std::shuffle(array.begin(), array.end(), m_generator);
    }

    // Shuffle an array of integers
    void getShuffle(int array[], size_t size){
        vector<int> temp;
        for (int i = m_min; i <= m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        for (size_t i = 0; i < size && i < temp.size(); i++){ // Changed 'int i' to 'size_t i'
            array[i] = temp[i];
        }
    }

    // Generate a random integer based on the distribution type
    int getRandNum(){
        int result = 0;
        if(m_type == NORMAL){
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            result = m_unidist(m_generator);
        }
        return result;
    }

    // Generate a random real number with two decimal precision
    double getRealRandNum(){
        double result = m_uniReal(m_generator);
        result = std::floor(result * 100.0) / 100.0;
        return result;
    }

    // Generate a random string of specified size
    string getRandString(int size){
        string output = "";
        for (int i = 0; i < size; i++){
            output += static_cast<char>(getRandNum());
        }
        return output;
    }

    // Get minimum value
    int getMin(){ return m_min; }

    // Get maximum value
    int getMax(){ return m_max; }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;
    std::uniform_int_distribution<> m_unidist;
    std::uniform_real_distribution<double> m_uniReal;
};


class Tester {
public:

    // AVL TEST FUNCTIONS:

    // Test function to check AVL tree balance.
    bool AVLbalance();

    // Test function to check if the AVL TREE balanced after multiple removals.
    bool AVLbalanceRemoval();



    //BST TEST FUNCTIONS: 

    // Test function to check that the BST is preserved after all insertions.
    bool BSTproperty();


    // Test function to check that trying to remove a node from a tree infact results in a tree without the node.
    bool BSTNormalCase();


    //Test function to check that removing the only node in a BST results in an empty tree.
    bool BSTedgeCase();


    //Test function to test whether the BST property is preserved after multiple removals from a BST tree and an AVL tree.
    bool BSTPropertyRemovals();


    // Test function to check and see if the heught of the BST is correctly updated after a node has been removed. 
    bool BSTheightAfterRemovals();


    //Test function for the assignment operator for a normal case.
    bool AssignmentOperatorNormal();

    //Test function for the assignment operator for a error case.
    bool AssignmentOperatorError();



    // SPLAY TEST FUNCTIONS:

    //Test function to determine if the heights of the splay tree are correct after node insertions.
    bool SplayHeights();

    //Test function to insure that the splay tree performs the correct splay operations.

    bool SplayOperations();

private:

//AVL TREE HELPER FUNCTIONS:

    // The Helper function is used to check AVL balance recursively
    bool CheckAVLbalance(Robot* node);

    // The Helper function is used to get the height of a node
    int getNodeHeight(Robot* node);

    // The Helper function is used to calculate the balance factor of a node
    int getBalance(Robot* node);


//BST HELPER FUNCTIONS:

    // The inOrderTraversal helper function will perform an in-order traversal of the tree, collecting all node keys in a vector.
    void BSTinOrderTraversal(Robot* node, vector<int>& keys);

    // The Helper function is used to search for a specific node in the BST, which allows us to verify the existence or absence of nodes.
    Robot* BSTsearch(Robot* node, int id);

    //Helper function that verifies the BST property by traversing the tree in-order. 
    bool BSTPropertyCheck(Robot* node, int& EndingID);

    // Helper function to compare two trees.
    bool CompareTheTrees(Robot* node1, Robot* node2, TREETYPE type);

    //Helper function to calculate to help calculate the heights correctly similar to swarm.cpp get height and get balance function. 
    bool BSTheights(Robot* node, Swarm& swarm);


};

int main() {
    Tester tester;

    // Running the tests and showing the output of each of the results
    
    if (tester.AVLbalance()) {
        cout << " \tTest whether the AVL tree is balanced                                  :Passed" << endl;
    } else {
        cout << " \tTest whether the AVL tree is balanced                                  :Failed" << endl;
    }

    if (tester.BSTproperty()) {
        cout << " \tBST property preservation test after insertions                        :Passed" << endl;
    } else {
        cout << " \tBST property preservation test after insertions                        :Failed" << endl;
    }

    if (tester.AVLbalanceRemoval()) {
        cout << " \tAVL balance test after multiple removals                               :Passed" << endl;
    } else {
        cout << " \tAVL balance test after multiple removals                               :Failed" << endl;
    }

    if (tester.BSTNormalCase()) {
        cout << "\tRemove function for a normal case in the BST tree                      :Passed" << endl;
    } else {
        cout << "\tRemove function for a normal case in the BST tree                      :Failed" << endl;
    }

    if (tester.BSTedgeCase()) {
        cout << "\tRemove function for an edge case in the BST tree.                      :Passed" << endl;
    } else {
        cout << "\tRemove function for an edge case in the BST tree.                      :Failed" << endl;
    }

    if (tester.BSTPropertyRemovals()) {
        cout << "\tBST property preservation after multiple removals in BST and AVL trees :Passed" << endl;
    } else {
        cout << "\tBST property preservation after multiple removals in BST and AVL trees :Failed" << endl;
    }

    if (tester.AssignmentOperatorNormal()) {
        cout << "\tTest the assignment operator for a normal case                         :Passed" << endl;
    } else {
        cout << "\tTest the assignment operator for a normal case                         :Failed" << endl;
    }

    if (tester.AssignmentOperatorError()) {
        cout << "\tTest the assignment operator for a error case                          :Passed" << endl;
    } else {
        cout << "\tTest the assignment operator for a error case                          :Failed" << endl;
    }

    if (tester.BSTheightAfterRemovals()) {
        cout << "\tTest height after removal for BST                                      :Passed" << endl;
    } else {
        cout << "\tTest height after removal for BST                                      :Failed" << endl;
    }

    if (tester.SplayHeights()) {
        cout << "\tTest height after insertions for Splay                                 :Passed" << endl;
    } else {
        cout << "\tTest height after insertions for Splay                                 :Failed" << endl;
    }

    if (tester.SplayOperations()) {
        cout << "\tTest splay tree operations                                             :Passed" << endl;
    } else {
        cout << "\tTest splay tree operations                                             :Failed" << endl;
    }
  
    return 0;
}

// Get the height of a node
int Tester::getNodeHeight(Robot* node) {
    if (node == nullptr) {
        return -1; // By definition, null nodes have height -1
    }
    return node->m_height;
}

// Calculate the balance factor of a node
int Tester::getBalance(Robot* node) {
    if (node == nullptr) {
        return 0;
    }
    return getNodeHeight(node->m_left) - getNodeHeight(node->m_right);
}

// Helper function to recursively check if the AVL tree is balanced
bool Tester::CheckAVLbalance(Robot* node) {
    if (node == nullptr) {
        return true; // An empty tree is balanced
    }

    int balance = getBalance(node);

    
    if (balance < -1 || balance > 1) {
        return false;
    }

    // Recursively check the left and right subtrees
        return CheckAVLbalance(node->m_left) && CheckAVLbalance(node->m_right);
}

// AVL TREE TESTS:


/*
  The Testing function will ensure that after inserting multiple Robot instances into the AVL tree (Swarm)
  the tree maintains its balanced property, which is important for the efficiency of AVL trees.

*/

bool Tester::AVLbalance() {
     // Initialize Random generators
    Random idGen(MINID, MAXID);      // For generating unique robot IDs
    Random typeGen(0, 4);            // For generating ROBOTTYPE values
    Swarm team(AVL);                 // Initialize the Swarm object as an AVL tree    

    size_t teamSize = 300;             // Needed number of robots
    vector<int> ids;                   // Vector to store unique IDs

    // Generating 300 unique random IDs
    while (ids.size() < teamSize) {
        int ID = idGen.getRandNum();
        // Check if ID is already in the vector using std::find
        if (find(ids.begin(), ids.end(), ID) == ids.end()) {
            ids.push_back(ID);        // Add unique ID to the list
        }
        
    }

    // Inserting robots into the AVL tree
    for (int ID : ids) {
        // Convert random number to ROBOTTYPE using static_cast for type safety
        ROBOTTYPE roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);       // Creating a new Robot instance
        team.insert(robot);              // Inserting Robot into the AVL tree
    }

    // Checking if the AVL tree is balanced after all insertions
    if (CheckAVLbalance(team.m_root)) {
        // If it is balanced, return true indicating the test passed.
        return true; 
    } else {
        // If not balanced, return false indicating the test failed.
        return false; 
    }
}

 /*

  The testing function will ensure that after inserting 300 Robot nodes into the AVL tree and then removing 150 nodes, 
  the AVL tree maintains its balanced property.

*/


bool Tester::AVLbalanceRemoval() {
    // Initialize Random generators
    Random idGen(MINID, MAXID);               // For generating unique robot IDs
    Random typeGen(0, 4);                     // For generating ROBOTTYPE values
    Random removeGen(0, 299, SHUFFLE);        // For shuffling IDs to remove

    // Initialize the Swarm object as an AVL tree
    Swarm team(AVL);                          

    size_t teamSize = 300;                     // Total number of robots to insert
    size_t removals = 150;                     // Number of robots to remove
    vector<int> ids;                           // Vector to store unique IDs

    // Generate 300 unique random IDs
    while (ids.size() < teamSize) {
        int ID = idGen.getRandNum();
        // Check if ID is already in the vector using std::find
        if (find(ids.begin(), ids.end(), ID) == ids.end()) {
            ids.push_back(ID);                  // Add unique ID to the list
        }
        
    }

    // Insert robots into the AVL tree
    for (int ID : ids) {
        
        ROBOTTYPE roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);                // Creating a new Robot instance
        team.insert(robot);                       // Inserting Robot into the AVL tree
    }

    // Shuffle the IDs to randomize removal
    vector<int> shuffledIds = ids;               // Create a copy of IDs
    removeGen.getShuffle(shuffledIds);           // Shuffle the copied IDs

    // Remove the first 150 IDs from the shuffled list
    for (size_t i = 0; i < removals && i < shuffledIds.size(); i++) {
        team.remove(shuffledIds[i]);              // Removing robot by ID
    }

    // Check if the AVL tree is balanced after removals
    if (CheckAVLbalance(team.m_root)) {
        return true;                              // Will pass if AVL tree is balanced after removals
    } else {
        return false;                             // Will fail if AVL tree is not balanced after removals
    }
}


        




// BINARY SEARCH TREE TESTS:


   // Performing in-order traversal and collecting keys.
void Tester::BSTinOrderTraversal(Robot* node, vector<int>& key) {
    if (node == nullptr) {
        return;
    }
    BSTinOrderTraversal(node->m_left, key);      // To the Traverse left subtree
    key.push_back(node->m_id);                   // To Visit current node
    BSTinOrderTraversal(node->m_right, key);     // To Traverse right subtree


}

/* 
This testing function will ensure that the AVL tree maintains the Binary Search Tree (BST) property after all insertions. 
   This means that for every node, all keys in the left subtree are less than the node's key, 
   and all keys in the right subtree are greater.
*/
bool Tester::BSTproperty() {
    Random idGen(MINID, MAXID);       // Initialize Random generator for IDs
    Random typeGen(0, 4);             // Initialize Random generator for ROBOTTYPE
    Swarm team(AVL);                  // Initialize Swarm as an AVL tree

    size_t teamSize = 300;            // Checking 300 nodes to make sure that they adhere to the definition.
    vector<int> ids;

    // Generating the  300 unique random IDs
    while (ids.size() < teamSize) {
        int ID = idGen.getRandNum();
        
        if (find(ids.begin(), ids.end(), ID) == ids.end()) {
            ids.push_back(ID);
        }
    }

    // Inserting of the robots into the AVL tree
    for (int ID : ids) {
        
        ROBOTTYPE roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());

        Robot robot(ID, roboType);

        team.insert(robot);
    }

    // Performing the in-order traversal to collect the keys
    vector<int> key;
    BSTinOrderTraversal(team.m_root, key);

    // Checks if the collected keys are in ascending order
    for (size_t i = 1; i < key.size(); i++) {
        if (key[i] <= key[i - 1]) {
            return false; 
        }
    }

    return true; 

} 

// Using the helper function to search for a specific node by ID in the BST

Robot* Tester::BSTsearch(Robot* node, int id) {
    
    if (node == nullptr || node->m_id == id) {
        return node;
    }
    
    if (id < node->m_id) {
        return BSTsearch(node->m_left, id);

    } else {
    
         return BSTsearch(node->m_right, id);
    }
}

    
/* 
    This testing function is used verify that the remove function correctly deletes a specified node from the BST.
*/


bool Tester::BSTNormalCase() {
    Swarm team(BST);

    // Insert a set of nodes into the BST 
    int chosenIDs[] = {16, 26, 37, 56, 28, 13, 49, 52, 55, 60};

    // Insert nodes into the BST
    for (int i = 0; i < 10; ++i) {
        int ID = chosenIDs[i];
        ROBOTTYPE roboType = DRONE;  
        Robot robot(ID, roboType);
        team.insert(robot);
    }

    // Node to be removed (normal case)
    int removeID = 26;

    // Remove the specified node (In this case: 26)
    team.remove(removeID);

    // Verify that the node has been removed
    Robot* nodeAfterRemoval = BSTsearch(team.m_root, removeID);
    if (nodeAfterRemoval == nullptr) {
        // The node was removed successfully
        return true;
    } else {
        // The node was not removed correctly, the test fails
        return false;
    }
}

/* 
      This testing function verifies that removing the only node in a BST results in an empty tree.
*/

bool Tester::BSTedgeCase() {
    
    Swarm team(BST);

    // Inserting a single Robot node into the BST
    int OneID = 12345; 
    ROBOTTYPE roboType = DRONE; 
    Robot robot(OneID, roboType);
    team.insert(robot);

   
    Robot* PreRemoval = BSTsearch(team.m_root, OneID); 

    if (PreRemoval == nullptr) {                             
        
        return false;
    }


    // Removing the single node
    team.remove(OneID);

    Robot* PostRemoval = BSTsearch(team.m_root, OneID);

    if (PostRemoval != nullptr) {
        
        return false;
    }

    // Verify that the root is now nullptr, indicating an empty tree
    if (team.m_root != nullptr) {
        
        return false;
    }

    
    return true;
}

/*

 Helper function that verifies the BST property by traversing the tree in-order. 
 It ensures that each node's ID is greater than the previous node's ID.

*/
bool Tester::BSTPropertyCheck(Robot* node, int& EndingID) {
    if (node == nullptr) {
        return true; 
    }
    if (!BSTPropertyCheck(node->m_left, EndingID)) {
        return false;
    }

    // Check the current node's ID
    if (EndingID != -1 && node->m_id <= EndingID) {
        return false; // BST property violated
    }

    // Update lastID to the current node's ID
    EndingID = node->m_id;

    // Traverse the right subtree
    if (!BSTPropertyCheck(node->m_right, EndingID)) {
        return false;
    }

    return true;
}

/* 
     This testing function will Verify that after inserting multiple nodes into both a BST (Swarm initialized with BST type) 
     and an AVL tree (Swarm initialized with AVL type), removing multiple nodes from each tree will still preserve the BST property.
*/

bool Tester::BSTPropertyRemovals() {
    // Initializing Swarm objects as BST and AVL trees
    Swarm BSTTree(BST);
    Swarm AVLTree(AVL);

    // Inserting 300 unique Robot nodes with IDs from 1 to 300
    for (int ID = 1; ID <= 300; ++ID) {
        ROBOTTYPE roboType = DRONE; 
        Robot robot(ID, roboType);
        BSTTree.insert(robot);
        AVLTree.insert(robot);
    }

    // Removing the first 150 even IDs (IDs divisible by 2)
    for (int ID = 2; ID <= 300; ID += 2) {
        BSTTree.remove(ID);
        AVLTree.remove(ID);
    }

   

    // Verifying the BST property for the BST tree

    int EndingBSTid = -1; // Initializing lower value than any possible ID in the tree.

    bool BSTValidity = BSTPropertyCheck(BSTTree.m_root, EndingBSTid);

    // Verify BST property for AVL tree
    int EndingAVL = -1; // Initializing  lower value than any possible ID

    bool AVLValidity = BSTPropertyCheck(AVLTree.m_root, EndingAVL );

    // Return true only if both trees maintain the BST property
    if (BSTValidity && AVLValidity) {

        return true;

    } else {

        return false;
        
    }
}

// Helper function to iterate through each objects root and compare the robot ids and heights. 

bool Tester::CompareTheTrees(Robot* node1, Robot* node2, TREETYPE type) {

// In this condition, both nodes are null.

    if (node1 == nullptr && node2 == nullptr) {
        return true;
    }
// In this condition, one node is null and the other is not null.

    if (node1 == nullptr || node2 == nullptr) {
        return false;
    }

// Comparing the data of the current node

    if (type == AVL && node1->m_height != node2->m_height) {
        return false;
    }

// This will recursively compare the left and the right trees

bool CompareLeftTree = CompareTheTrees(node1->m_left, node2->m_left, type);

bool CompareRightTree = CompareTheTrees(node1->m_right, node2->m_right, type);
    

return CompareLeftTree && CompareRightTree;

}


/* 
     This function compares an original tree to a copy of tree and compares the type and heights of each individual node and returns true if equal. 
*/
bool Tester::AssignmentOperatorNormal() {

    Swarm OriginalTree (BST);

    int chosenIDs[] = {25, 15, 45, 15, 28, 35, 16, 13, 17, 23};
    for (int i = 0; i < 10; ++i) {
        int ID = chosenIDs[i];
        ROBOTTYPE roboType = DRONE;  
        Robot robot(ID, roboType);
        OriginalTree.insert(robot);
    }
    
    Swarm NewSwarm = OriginalTree;
    
    if (OriginalTree.m_root == NewSwarm.m_root) {
        
        return true;
    }

    if (OriginalTree.getType() != NewSwarm.getType()) {
        
        return false;
    }

    
    bool EqualTrees = CompareTheTrees (OriginalTree.m_root, NewSwarm.m_root, OriginalTree.getType());
        
    if (!EqualTrees) {
        return false;
    }else{
        return true;
    }

        

}



/* 
     This test function tests the assignment operator, more spesifically how the assignment operator handles the case where we assign an empty object
     to an empty object.
*/

bool Tester::AssignmentOperatorError() {
    
    Swarm swarm1; 
    Swarm swarm2; 

    swarm2 = swarm1; 

    if (swarm1.m_root != nullptr || swarm2.m_root != nullptr) {
        
        return false;
    }

    if (swarm1.getType() != swarm2.getType()) {
        
        return false;
    }

    
    bool treesEqual = CompareTheTrees(swarm1.m_root, swarm2.m_root, swarm1.getType());
    if (!treesEqual) {
        
        return false;

    }

    
    return true;
}

//The helper function will calculate the heights similar to getheight and getbalance in swarm.cpp and help us determine that the heights are updating correctly after node removal.
bool Tester::BSTheights(Robot* node, Swarm& swarm) {
    if (node == nullptr) {
        
        return true;
    }

    bool leftside = BSTheights(node->m_left, swarm);
    bool rightside = BSTheights(node->m_right, swarm);
                            
    int leftSideHeight = swarm.getHeight(node->m_left);
    int rightSideHeight = swarm.getHeight(node->m_right);

    
    int HeightOfChild = (leftSideHeight > rightSideHeight) ? leftSideHeight : rightSideHeight;
    int CorrectHeight = 1 + HeightOfChild;

    
    if (node->m_height != CorrectHeight){

        return false;
    }

    return leftside && rightside;
    
}

/* 
     This test function will help us determine if the heights of the BST are correct after a node has been removed from the tree. 
*/

bool Tester:: BSTheightAfterRemovals() {
    Swarm swarm(BST);

    
    vector<int> insertingIDs = {28, 25, 80, 90, 85, 65, 20, 15, 14, 18, 27, 30, 55, 95, 75};
    for (int id : insertingIDs) {
        ROBOTTYPE roboType = DRONE; 
        Robot robot(id, roboType);
        swarm.insert(robot);
    }

    if (!BSTheights(swarm.m_root, swarm)) {
       
        return false;

    }

    vector<int> removingIDs = {15, 25, 80, 75}; 
    for (int id : removingIDs) {
        swarm.remove(id);
    }

    if (!BSTheights(swarm.m_root, swarm)) {
        return false;
    }

    return true;
}



// SPLAY TREE TESTS

/* 
     This test function will help determine that the height values of the Splay tree are correct after inserting mutiple nodes into the tree. 
*/

bool Tester:: SplayHeights() {

    Swarm swarm(SPLAY); 

    
    std::vector<int> insertingIDs = {30, 48, 65, 28, 15, 50, 60, 80, 23, 33, 44, 90, 84, 71, 84};

    for (int id : insertingIDs) {
        ROBOTTYPE roboType = DRONE; 
        Robot robot(id, roboType);
        swarm.insert(robot);

    if (!BSTheights(swarm.m_root, swarm)) {

        return false;

        }
    }

    if (!BSTheights(swarm.m_root, swarm)) {
        
        return false;
    }

    
    return true;

}

/* 
     This test function will help determine that the height values of the Splay tree are correct after inserting mutiple nodes into the tree. 
*/

bool Tester:: SplayOperations() {

    Swarm swarm(SPLAY); 

    std::vector<int> insertingIDs = {35, 45, 60, 15, 14, 70, 90, 15, 20, 30, 23, 19, 95, 66, 83};

    for (int id : insertingIDs) {
        ROBOTTYPE roboType = DRONE; 
        Robot robot(id, roboType);
        swarm.insert(robot);
        
        if (!BSTheights(swarm.m_root, swarm)) {
            
            return false;
        }

        
    int LastID = -1; 
        if (!BSTPropertyCheck(swarm.m_root, LastID)) {

            return false;
        }
    }

        if (!BSTheights(swarm.m_root, swarm)) {
        
        return false;

    }
    

    int LastIDend = -1;
    if (!BSTPropertyCheck(swarm.m_root, LastIDend)) {

        return false;

    }

    return true;
}

    




    





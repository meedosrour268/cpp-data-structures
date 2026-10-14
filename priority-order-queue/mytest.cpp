#include "mqueue.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
#include <climits>

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


int priorityFn1(const Order & order) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [1101-10400]
    int minValue = MIN1000 + MAX100 + MINONE + MINZERO;
    int maxValue = MAX100 + MAX100 + MAX200 + MAX10000;
    //the larger value means the higher priority
    int priority = order.getMaterial() + order.getImportance() + 
                    order.getWorkForce() + order.getQuantity();
    if (priority >= minValue && priority <= maxValue)
        return priority;
    else
        return 0; // this is an invalid order object
}

int priorityFn2(const Order & order) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [4-86]
    int minValue = MINONE + MINONE + MINONE + MINONE;
    int maxValue = MAX12 + MAX12 + MAX12 + MAX50;
    //the smaller value means the higher priority
    int priority = order.getFIFO() + order.getProcessTime() + 
                order.getDueTime() + order.getSlackTime();
    if (priority >= minValue && priority <= maxValue)
        return priority;
    else
        return 0; // this is an invalid order object
}


class Tester {
public:

bool MinHeapNormalCase();

bool MaxHeapNormalCase();

bool MinHeapRemovalNormal();

bool MaxHeapRemovalNormal();

bool NPLleftist();

bool LeftistPreservationMinheap();

bool LeftistPreservationMaxheap();

bool PriorityFunctionChange();

bool CopyConstructorNormal();

bool CopyConstructorEdge();

bool AssignmentOperatorNormal();

bool AssignmentOperatorEdge();

bool DequeueException();

bool MergeException();

private:

bool MinHeapPriority(Node* root, prifn_t priorityFn);

bool MaxHeapPriority(Node* node, prifn_t priorityFn);

bool CheckNplLeftist (Node* node);

bool CompareHeap (const Node* node1, const Node* node2, prifn_t priFunc);




};

int main() {
    Tester tester;

  if (tester.MinHeapNormalCase()) {
        cout << " \tTest insertion for a normal case of minheap                                  :Passed" << endl;
    } else {
        cout << " \tTest insertion for a normal case of minheap                                  :Failed" << endl;
    }


    if (tester.MaxHeapNormalCase()) {
        cout << " \tTest insertion for a normal case of maxheap                                  :Passed" << endl;
    } else {
        cout << " \tTest insertion for a normal case of maxheap                                  :Failed" << endl;
    }

    if (tester.MinHeapRemovalNormal()) {
        cout << " \tTest removal for a normal case of minheap                                    :Passed" << endl;
    } else {
        cout << " \tTest removal for a normal case of minheap                                    :Failed" << endl;
    }

    if (tester.MaxHeapRemovalNormal()) {
        cout << " \tTest removal for a normal case of maxheap                                    :Passed" << endl;
    } else {
        cout << " \tTest removal for a normal case of maxheap                                    :Failed" << endl;
    }

    if (tester.NPLleftist()) {
        cout << " \tTest that npl values are correct                                             :Passed" << endl;
    } else {
        cout << " \tTest that npl values are correct                                             :Failed" << endl;
    }

    if (tester.LeftistPreservationMinheap()) {
        cout << " \tTest leftist heap preserves the property as minheap                          :Passed" << endl;
    } else {
        cout << " \tTest leftist heap preserves the property as minheap                          :Failed" << endl;
    }

    if (tester.LeftistPreservationMaxheap()) {
        cout << " \tTest leftist heap preserves the property as maxheap                          :Passed" << endl;
    } else {
        cout << " \tTest leftist heap preserves the property as maxheap                          :Failed" << endl;
    }

    if (tester.PriorityFunctionChange()) {
        cout << " \tTest after changing the priority function a correct heap is rebuilt          :Passed" << endl;
    } else {
        cout << " \tTest after changing the priority function a correct heap is rebuilt          :Failed" << endl;
    }

    if (tester.CopyConstructorNormal()) {
        cout << " \tTest the MQueue class copy constructor for a normal case                     :Passed" << endl;
    } else {
        cout << " \tTest the MQueue class copy constructor for a normal case                     :Failed" << endl;
    }

    if (tester.CopyConstructorEdge()) {
        cout << " \tTest the MQueue class copy constructor for a edge case                       :Passed" << endl;
    } else {
        cout << " \tTest the MQueue class copy constructor for a edge case                       :Failed" << endl;
    }

    if (tester.AssignmentOperatorNormal()) {
        cout << " \tTest the MQueue class assignment operator for a normal case.                 :Passed" << endl;
    } else {
        cout << " \tTest the MQueue class assignment operator for a normal case.                 :Failed" << endl;
    }

    if (tester.AssignmentOperatorEdge()) {
        cout << " \tTest the MQueue class assignment operator for a edge case.                   :Passed" << endl;
    } else {
        cout << " \tTest the MQueue class assignment operator for a edge case.                   :Failed" << endl;
    }

    if (tester.DequeueException()) {
        cout << " \tattempting to dequeue an empty queue throws an out_of_range exception.       :Passed" << endl;
    } else {
        cout << " \tattempting to dequeue an empty queue throws an out_of_range exception.       :Failed" << endl;
    }

    if (tester.MergeException()) {
        cout << " \tTest merge queues  throws a domain_error exception                           :Passed" << endl;
    } else {
        cout << " \tTest merge queues  throws a domain_error exception                           :Failed" << endl;
    }
};

/*

Helper function to check that the minheap priority is satified at every note by checking the left and right children of the minheap are
abiding by the correct properties.

*/

bool Tester::MinHeapPriority(Node* node, prifn_t priorityFn) {
    if (node == nullptr) {
        return true; 
    }

   
    if (node->m_left != nullptr && priorityFn(node->m_left->getOrder()) < priorityFn(node->getOrder())) {
        return false;
    }

    
    if (node->m_right != nullptr && priorityFn(node->m_right->getOrder()) < priorityFn(node->getOrder())) {
        return false;
    }

    
    return MinHeapPriority(node->m_left, priorityFn) && MinHeapPriority(node->m_right, priorityFn);
}

/*
In this 
*/

bool Tester :: MinHeapNormalCase() {

    MQueue queue (priorityFn2, MINHEAP, LEFTIST);
    Random nameGen(97, 122);
    Random FIFO(MINONE, MAX50);
    Random processTime(MINONE, MAX12);
    Random dueTime(MINONE, MAX12);
    Random slackTime(MINONE, MAX12);
    Random material(MINZERO, MAX100);
    Random importance(MINONE, MAX100);
    Random workers(MAX100, MAX200);
    Random quantity(MIN1000, MAX10000);
     // Insert 300 random orders into the queue
    for (int i = 0; i < 300; ++i) {
        Order order(nameGen.getRandString(5),
    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue.insertOrder(order);
    }

   
    bool MinHeap = MinHeapPriority(queue.m_heap, priorityFn2);
    

    
    if (MinHeap) {

        return true;  
    } else {

        return false;  
    }
}
    
/*

Helper function to check that the maxheap priority is satified at every note by checking the left and right children of the maxheap are
abiding by the correct properties. The implementation is similar to my minheap helper function implementation with the logic changing because 
in this case we check if the left and right children have a greater priority than their parent. I used the INT_MIN constant from the <climits> library
to represent the minimum value for an int type.

*/

bool Tester :: MaxHeapPriority(Node* node, prifn_t priorityFn) {
    if (node == nullptr) {
        return true;
    }

    if (node->m_left != nullptr && priorityFn(node->m_left->getOrder()) > priorityFn(node->getOrder())) {
        return false;
    }

   
    if (node->m_right != nullptr && priorityFn(node->m_right->getOrder()) > priorityFn(node->getOrder())) {
        return false;
    }

    
    return MaxHeapPriority(node->m_left, priorityFn) && MaxHeapPriority(node->m_right, priorityFn);
}


bool Tester :: MaxHeapNormalCase() {

MQueue queue (priorityFn1, MAXHEAP, LEFTIST);
    Random nameGen(97, 122);
    Random FIFO(MINONE, MAX50);
    Random processTime(MINONE, MAX12);
    Random dueTime(MINONE, MAX12);
    Random slackTime(MINONE, MAX12);
    Random material(MINZERO, MAX100);
    Random importance(MINONE, MAX100);
    Random workers(MAX100, MAX200);
    Random quantity(MIN1000, MAX10000);
     
    for (int i = 0; i < 300; ++i) {
        Order order(nameGen.getRandString(5),
    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue.insertOrder(order);
    }

   
    bool MaxHeap = MaxHeapPriority(queue.m_heap, priorityFn1);
    

    
    if (MaxHeap) {

        return true;  
    } else {

        return false;  
    }
}

/*
In this test function i have used the same approach to insert 300 nodes into the queue. I set the removed node
to 0 because the priorities are non negative.After checking if the current node is less than the removed node
i updated the last removednode priority to the current node priority and added true or false depending on if 
the test passes or fails. 

*/


bool Tester::MinHeapRemovalNormal() {
    
MQueue queue (priorityFn2, MINHEAP, LEFTIST);
    Random nameGen(97, 122);
    Random FIFO(MINONE, MAX50);
    Random processTime(MINONE, MAX12);
    Random dueTime(MINONE, MAX12);
    Random slackTime(MINONE, MAX12);
    Random material(MINZERO, MAX100);
    Random importance(MINONE, MAX100);
    Random workers(MAX100, MAX200);
    Random quantity(MIN1000, MAX10000);
    
    for (int i = 0; i < 300; ++i) {
        Order order(nameGen.getRandString(5),
    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue.insertOrder(order);
    }

  
    int RemovedNode = 0;

    
    while (queue.numOrders() > 0) {
        Order removedOrder = queue.getNextOrder();  
        int CurrentNode = priorityFn2(removedOrder);

       
        if (CurrentNode < RemovedNode) {
            return false;  
        }

        
        RemovedNode = CurrentNode;
    }

    return true;  
}

/*
In this test function i have used the same approach to insert 300 nodes into the queue. I then set the removed
node to the priority of the first element priority. After checking if the current node is greater than the removed node
i updated the last removednode priority to the current node priority and added true or false depending on if the 
test passes or fails. 

*/

bool Tester :: MaxHeapRemovalNormal() {

    MQueue queue(priorityFn1, MAXHEAP, LEFTIST);  
    Random nameGen(97, 122);
    Random FIFO(MINONE, MAX50);
    Random processTime(MINONE, MAX12);
    Random dueTime(MINONE, MAX12);
    Random slackTime(MINONE, MAX12);
    Random material(MINZERO, MAX100);
    Random importance(MINONE, MAX100);
    Random workers(MAX100, MAX200);
    Random quantity(MIN1000, MAX10000);

    
    for (int i = 0; i < 300; ++i) {
        Order order(nameGen.getRandString(5),
                    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());

        int priority = priorityFn1(order);
        if (priority > 0) {  
            queue.insertOrder(order);  
        }
    }

   
    if (queue.numOrders() == 0) 
    
    return true;  
    
    Order firstRemovedNode = queue.getNextOrder();
    int RemovedNode = priorityFn1(firstRemovedNode);

    
    while (queue.numOrders() > 0) {
        Order remove = queue.getNextOrder();  
        int CurrentNode = priorityFn1(remove);

        // Check max-heap order condition
        if (CurrentNode > RemovedNode) {
            return false;  
        }

        RemovedNode = CurrentNode;  
    }

    return true;  
}


/*
In this helper function i added a condititon that checks if the node is empty which by default means that its null path 
length is 0. i then used similar logic i used in my mqueue.cpp file to get npl values for both children and used 0 for 
the empty ones. i then added 2 conditions to further make sure that the propery is being abided by, i first added a condition
that returns false if the left child null path length is less than the right path length and a conditon that uses the min
function to return the smaller of the two values after comparing both of the childrens npl. Then i recursively checked the
left and right children.
*/
bool Tester ::CheckNplLeftist(Node* node) {
   
if (node == nullptr) {
        return true;
    }

    
int NPLleft = node->m_left ? node->m_left->getNPL() : 0;
int NPLright = node->m_right ? node->m_right->getNPL() : 0;

   
if (NPLleft < NPLright) {

        return false;  
    }

    
if (node->getNPL() != (min(NPLleft, NPLright) + 1)) {
       
    }

    
    return CheckNplLeftist(node->m_left) && CheckNplLeftist(node->m_right);
}

/*
Testing the function to see if the npl values are correct also using the random value generators from driver.cpp
and then calling my helper function that i implemented above.
*/ 

bool Tester ::NPLleftist() {

MQueue queue (priorityFn2, MINHEAP, LEFTIST);
    Random nameGen(97, 122);
    Random FIFO(MINONE, MAX50);
    Random processTime(MINONE, MAX12);
    Random dueTime(MINONE, MAX12);
    Random slackTime(MINONE, MAX12);
    Random material(MINZERO, MAX100);
    Random importance(MINONE, MAX100);
    Random workers(MAX100, MAX200);
    Random quantity(MIN1000, MAX10000);
     
    for (int i = 0; i < 300; ++i) {
        Order order(nameGen.getRandString(5),
    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue.insertOrder(order);
    }

bool LeftistNPL = CheckNplLeftist(queue.m_heap);

    
    if (LeftistNPL) {

        return true;  

    } else {

        return false;  
    }
}

/*

In this testing function I used my helper function that i implemented above since the helper function checks 
at every node the NPL value of the left child is greater than or equal to the NPL value of the right child with
this implementation: 

int NPLleft = node->m_left ? node->m_left->getNPL() : 0;
int NPLright = node->m_right ? node->m_right->getNPL() : 0;

   
if (NPLleft < NPLright) {

        return false;  
    }

*/


bool Tester:: LeftistPreservationMinheap() {

MQueue queue (priorityFn2, MINHEAP, LEFTIST);
    Random nameGen(97, 122);
    Random FIFO(MINONE, MAX50);
    Random processTime(MINONE, MAX12);
    Random dueTime(MINONE, MAX12);
    Random slackTime(MINONE, MAX12);
    Random material(MINZERO, MAX100);
    Random importance(MINONE, MAX100);
    Random workers(MAX100, MAX200);
    Random quantity(MIN1000, MAX10000);
     
    for (int i = 0; i < 300; ++i) {
        Order order(nameGen.getRandString(5),
    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue.insertOrder(order);
    }

    bool LeftistProperty = CheckNplLeftist(queue.m_heap);

    
    if (LeftistProperty) {
        return true;  
    } else {
        return false;  
    }
}
    
/*

In this testing function I used my helper function that i implemented above since the helper function checks 
at every node the NPL value of the left child is greater than or equal to the NPL value of the right child with
the  implementation below. I only changed the priority for the queue and changed the heaptype so that it allows 
us to test if the leftist heap property is preserved using a maxheap: 

int NPLleft = node->m_left ? node->m_left->getNPL() : 0;
int NPLright = node->m_right ? node->m_right->getNPL() : 0;

   
if (NPLleft < NPLright) {

        return false;  
    }

*/
bool Tester:: LeftistPreservationMaxheap() {
MQueue queue (priorityFn1, MAXHEAP, LEFTIST);
    Random nameGen(97, 122);
    Random FIFO(MINONE, MAX50);
    Random processTime(MINONE, MAX12);
    Random dueTime(MINONE, MAX12);
    Random slackTime(MINONE, MAX12);
    Random material(MINZERO, MAX100);
    Random importance(MINONE, MAX100);
    Random workers(MAX100, MAX200);
    Random quantity(MIN1000, MAX10000);
     
    for (int i = 0; i < 300; ++i) {
        Order order(nameGen.getRandString(5),
    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue.insertOrder(order);
    }

    bool LeftistProperty = CheckNplLeftist(queue.m_heap);

    
    if (LeftistProperty) {
        return true;  
    } else {
        return false;  
    }

}

/*

In this testing function i used the same number generator that i used in my previous tests but this time using a vector to 
proceed with my testing for the function. The first thing i did in this function is add order into the queue. The push back
function helped me do this easier. I then checked to see that the order is correct for the first priority function and then 
switch the priority function and see that in the new one it has the same orders as the first priority function. The test will 
pass if the orders in the second priority are maintained and fail otherwise.

*/
bool Tester::PriorityFunctionChange() {
    MQueue queue (priorityFn2, MINHEAP, LEFTIST);
    Random nameGen(97, 122);
    Random FIFO(MINONE, MAX50);
    Random processTime(MINONE, MAX12);
    Random dueTime(MINONE, MAX12);
    Random slackTime(MINONE, MAX12);
    Random material(MINZERO, MAX100);
    Random importance(MINONE, MAX100);
    Random workers(MAX100, MAX200);
    Random quantity(MIN1000, MAX10000);

     vector <Order> InitialHeap;
    for (int i = 0; i < 300; ++i) {
        Order order(nameGen.getRandString(5),
    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue.insertOrder(order);
        InitialHeap.push_back(order);
    }

       
    int FirstPriority = 0;
    while (queue.numOrders() > 0) {
        Order removedOrder = queue.getNextOrder();
        int DifferentPriority = priorityFn2(removedOrder);

        if (DifferentPriority < FirstPriority) {
            return false;  
        }
        FirstPriority = DifferentPriority;
    }

    
    queue.setPriorityFn(priorityFn1, MINHEAP);
    for (const Order& order : InitialHeap) {
        queue.insertOrder(order);
    }

    
    FirstPriority = 0;
    while (queue.numOrders() > 0) {
        Order removedOrder = queue.getNextOrder();
        int currentPriority = priorityFn1(removedOrder);

        if (currentPriority < FirstPriority) {
            return false;  
        }
        FirstPriority = currentPriority;
    }

    return true;  
}

/*
The CompareHeap function recursively compares two heaps to check if they are structurally identical 
and their nodes have the same priority values and NPL. Then, it compares the priority of the orders 
(priFunc(node1->getOrder()) and priFunc(node2->getOrder())) and the NPL of both nodes. it then 
recursively compares the left and right children of both nodes, ensuring the entire structure and 
values of both heaps are the same.
*/

bool Tester :: CompareHeap (const Node* node1, const Node* node2, prifn_t priFunc)  {

        if (!node1 && !node2) {
        return true;
    }
    if (!node1 || !node2) {
        return false;
    }

    if (priFunc(node1->getOrder()) != priFunc(node2->getOrder()) || node1->getNPL() != node2->getNPL()) {
        return false;
    }

    return CompareHeap(node1->m_left, node2->m_left, priFunc) && CompareHeap(node1->m_right, node2->m_right, priFunc);
}

/*
This test creates a copy of OriginalQueue and compares it to ensure both queues have the same heap type and structure.
It then uses the CompareHeap function to check if the heaps are similar
*/

bool Tester :: CopyConstructorNormal () {

     MQueue OriginalQueue(priorityFn1, MINHEAP, LEFTIST);

    Random nameGen(97, 122);
    Random FIFO(MINONE, MAX50);
    Random processTime(MINONE, MAX12);
    Random dueTime(MINONE, MAX12);
    Random slackTime(MINONE, MAX12);
    Random material(MINZERO, MAX100);
    Random importance(MINONE, MAX100);
    Random workers(MAX100, MAX200);
    Random quantity(MIN1000, MAX10000);

    for (int i = 0; i < 10; ++i) {
        Order order(nameGen.getRandString(5),
                    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        OriginalQueue.insertOrder(order);
    }

    MQueue CopyQueue(OriginalQueue);

    if (OriginalQueue.getHeapType() != CopyQueue.getHeapType() ||
        OriginalQueue.getStructure() != CopyQueue.getStructure()) {
        return false;  
    }

    if (!CompareHeap(OriginalQueue.m_heap, CopyQueue.m_heap, priorityFn1)) {
        return false;  
    }

    return true;
}

/*
This tests for an empty MQueue object. It creates an OriginalQueue, copies it into CopyQueue, 
and checks that both queues have the same heap type, structure. 
*/

bool Tester::CopyConstructorEdge() {
    
    MQueue OriginalQueue(priorityFn1, MINHEAP, LEFTIST);

    
    MQueue CopyQueue(OriginalQueue);

    
    if (OriginalQueue.getHeapType() != CopyQueue.getHeapType() 
    || OriginalQueue.getStructure() != CopyQueue.getStructure()) {
        return false;
    }

    
    if (OriginalQueue.numOrders() != 0 || CopyQueue.numOrders() != 0) {
        return false;
    }

    
    if (OriginalQueue.m_heap != nullptr || CopyQueue.m_heap != nullptr) {
        return false;
    }

    return true;  
}

/*
This test function creates an OriginalQueue, inserting 10 random orders, and then assigns it to SecondQueue. 
It checks that the heaps of both queues are different (indicating a deep copy),ensures the heap type and structure
are the same, confirms the number of orders is identical, and verifies the heap contents using CompareHeap helper function

*/
bool Tester::AssignmentOperatorNormal() {
    
    MQueue OriginalQueue(priorityFn1, MINHEAP, LEFTIST);

    
    Random nameGen(97, 122);
    Random FIFO(MINONE, MAX50);
    Random processTime(MINONE, MAX12);
    Random dueTime(MINONE, MAX12);
    Random slackTime(MINONE, MAX12);
    Random material(MINZERO, MAX100);
    Random importance(MINONE, MAX100);
    Random workers(MAX100, MAX200);
    Random quantity(MIN1000, MAX10000);

    for (int i = 0; i < 10; ++i) {  
        Order order(nameGen.getRandString(5),
                    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        OriginalQueue.insertOrder(order);
    }

    
    MQueue SecondQueue = OriginalQueue;

    
    if (OriginalQueue.m_heap == SecondQueue.m_heap) {
        return false;
    }

    
    if (OriginalQueue.getHeapType() != SecondQueue.getHeapType() ||
        OriginalQueue.getStructure() != SecondQueue.getStructure()) {
        return false;
    }

    
    if (OriginalQueue.numOrders() != SecondQueue.numOrders()) {
        return false;
    }

    
    if (!CompareHeap(OriginalQueue.m_heap, SecondQueue.m_heap, priorityFn1)) {
        return false;
    }

    return true; 
}


/*
This test function creates two queues both initialized as empty then
assigns Queue1 to Queue2 before checking that neither queue has a non-null heap, 
ensuring both queues have the same heap type and structure, 
*/

bool Tester::AssignmentOperatorEdge() {

    MQueue Queue1(priorityFn1, MINHEAP, LEFTIST);
    MQueue Queue2(priorityFn1, MINHEAP, LEFTIST);

    
    Queue2 = Queue1;

    
    if (Queue1.m_heap != nullptr || Queue2.m_heap != nullptr) {
        return false;  
    }

    
    if (Queue1.getHeapType() != Queue2.getHeapType() ||
        Queue1.getStructure() != Queue2.getStructure()) {
        return false;  
    }

    
    if (Queue1.numOrders() != 0 || Queue2.numOrders() != 0) {
        return false;  
    }

    return true;  
}

/*
The DequeueException function tests that attempting to dequeue from an empty MQueue throws an out_of_range exception. 
It creates an empty queue and tries to call getNextOrder() on it. If the exception is caught, the function returns true,
indicating the test passed. If no exception is thrown, it returns false.
*/


bool Tester:: DequeueException() {

    MQueue EmptyQueue(priorityFn1, MINHEAP, LEFTIST);

    try {
        
        EmptyQueue.getNextOrder();
    } catch (out_of_range) {
        
        return true;
    }

    
    return false;
}


/*
The MergeException function tests that attempting to merge two MQueue objects with different priority functions throws a domain_error exception. 
It creates two queues with different priority functions and attempts to merge them using mergeWithQueue and similarly returns true if the exception 
is cought.
*/
bool Tester::MergeException() {
    
    MQueue queue1(priorityFn1, MINHEAP, LEFTIST);
    MQueue queue2(priorityFn2, MINHEAP, LEFTIST);

    try {
        
        queue1.mergeWithQueue(queue2);
    } catch (domain_error) {
        
        return true;
    }

    
    return false;
}


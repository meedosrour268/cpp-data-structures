// CMSC 341 - Fall 2024 - Project 3
#include "mqueue.h"

/* 
The constructor provides a pointer to the prioritization function, the type of heap, 
and the desired data structure. 
*/

MQueue::MQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure) {

  m_size = 0;
  m_heap = nullptr;
  m_priorFunc = priFn;
  m_heapType = heapType;
  m_structure = structure;

  
}

//The destructor deallocates the memory and re-initializes the member variables. 

MQueue::~MQueue() {

ClearNodes(m_heap); //Using the helper function to deallocate the memory.

 m_size = 0;
 m_heap = nullptr;
 m_priorFunc = nullptr;

}

/*
This function clears the queue, delete all the nodes in the heap using the helper funtion
"ClearNodes", leaves the heap empty, and re-initializes the member variables. 
*/
void MQueue::clear() {

  if (m_heap != nullptr) {
  ClearNodes(m_heap); //Calling the helper function made below to delete the heap recursively. 
  m_size = 0;
  m_heap = nullptr;
  
   
  
  }
}

/*
Helper function to clear the nodes recursively. iT checks to see if node is empty then 
recursivley calls itslef on the left and right child and then deletes the node.
*/
void MQueue:: ClearNodes(Node * node) { 
  if (node == nullptr) {
    return;
  }
  
  ClearNodes (node -> m_left);
  ClearNodes (node -> m_right);
  delete node;
  
}

/*
The copy constructor must make a deep copy of the rhs object 
and creates an exact same copy of rhs. I also added a conditional
statement to checks if rhs.m_heap is not nullptrand creates a copy
using the helper function i made below

*/
MQueue::MQueue(const MQueue& rhs) {

  m_size = rhs.m_size;
  m_priorFunc = rhs.m_priorFunc;
  m_heapType = rhs.m_heapType;
  m_structure = rhs.m_structure;
  m_heap = (rhs.m_heap != nullptr) ? CopyQueue(rhs.m_heap) : nullptr;
  

}

/*Helper function to make make a deep copy of the rhs object and create
a new Node with the same order as the current node, copiy its
 NPL, and then recursively copiy the left and right subtrees.
*/
Node* MQueue:: CopyQueue (Node * node) {

  if (node == nullptr) {
    return nullptr;
  }

 Node* CopyNodes = new Node (node->getOrder());
 CopyNodes->setNPL(node->getNPL());
 CopyNodes-> m_left  =  CopyQueue(node ->m_left);
 CopyNodes-> m_right =  CopyQueue(node ->m_right);

  return CopyNodes;
}


/*

The assignment operator creates an exact same copy of rhs and 
does not call the previous copy constructor. it also has a conditional
statement to check if rhs.m_heap is not nullptr.

*/
MQueue& MQueue::operator=(const MQueue& rhs) {
  
  if (this != &rhs) {
    clear();
  
  m_size = rhs.m_size;
  m_priorFunc = rhs.m_priorFunc;
  m_heapType = rhs.m_heapType;
  m_structure = rhs.m_structure;
  m_heap = (rhs.m_heap != nullptr) ? CopyQueue(rhs.m_heap) : nullptr; //Using the helper function

    

  }
  return *this;
} 

/*
This function compares the heap types, priority functions, 
and structures of both queues, throwing an exception if they differ.
then If rhs has a heap, it merges the heaps of this and rhs using the MergingNodes function.
*/ 

void MQueue::mergeWithQueue(MQueue& rhs) { 

  if( &rhs== this) {
    return;
  }

  if (rhs.m_heapType != m_heapType || rhs.m_priorFunc != m_priorFunc || rhs.m_structure != m_structure) {
    throw domain_error("The queues have different priority functions");

  }


  if (rhs.m_heap != nullptr) { //If rhs is not empty, rhs will be merged into the heap.
    
  m_heap = MergingNodes(m_heap, rhs.m_heap); //Calling the helper function made below to combine both of the objects. 
  

    }

  rhs.clear();

}



/*

This function allows us to insert a proper order into the queue and 
checks that the order's values fall within predefined valid ranges. if
the ranges are not valid it will return false. I then create a new node
and merge it using the merging nodes helper function done below.
Finally, the size of the heap is then incremented

*/
bool MQueue::insertOrder(const Order& order) {
  if (order.m_FIFO < MINONE || order.m_FIFO > MAX50
        || order.m_processTime < MINONE || order.m_processTime > MAX12
        || order.m_dueTime < MINONE || order.m_dueTime > MAX12
        || order.m_slackTime < MINONE || order.m_slackTime > MAX12
        || order.m_material < MINZERO || order.m_material > MAX100
        || order.m_importance < MINONE || order.m_importance > MAX100
        || order.m_workForce < MAX100 || order.m_workForce > MAX200
        || order.m_quantity < MIN1000 || order.m_quantity > MAX10000)
        return false;


  Node * newNode = new Node(order); //Creating a new node for the order queue

  m_heap = MergingNodes(m_heap, newNode); //Using the helper function to ensure that merging is done after every step to ensure proper merge handling.

 m_size++;

return true; 

}

/* 
- Helper function to help ensure that correct heap properties are maintained
 while the heaps are being merged. I originally had issues with my code output when
 my leftist heap was not maintaining the correct properties. The top node would not
 be the smallest node. I have now adjusted my code so that the priorities are properly
 compared and then the merging and updating the null path length and swapping oF the 
 nodes is done based on the heap structure. My dump in my tree tool looks proper without
 me explicitly having a condition for skew because the skew and leftst strcutures share
 similar priorities in terms of merging nodes. 

 -My code had issues updating the npl lenghts so i had to restructure my code after
 having condiotions checking if the heap was a minheap or a maxheap. 

 -Using this code and after checking my tree viewer tool, i noticed that my skew
 structure for minheap was a little left heavy. I enquired about this and understood
 that it is okay for it to be left heavy as long as it abides the properties.

 -My skew strcuture looks better on the tree viewer tool even though it is still not 
 a complete tree which i now know that it does not need to be a complete tree.


*/

Node* MQueue::MergingNodes( Node* higherNode, Node *lowerNode) {
if (higherNode == nullptr) {

        return lowerNode;

    }

if (lowerNode == nullptr) {

        return higherNode;
    }

if (m_heapType == MINHEAP) {

// Checking the priority and determining whether or not it needs a swap.
  if (m_priorFunc(lowerNode->getOrder()) < m_priorFunc(higherNode->getOrder())) {


// This will swap the nodes if necessary
Node* temp = higherNode;
higherNode = lowerNode;
lowerNode = temp;
  }

} else { //if the heap type is a MAXHEAP:
        
if (m_priorFunc(lowerNode->getOrder()) > m_priorFunc(higherNode->getOrder())) {
            
Node* temp = higherNode;
higherNode = lowerNode;
lowerNode = temp;
  }
}

//if (m_structure == SKEW) {

//Swapping then merging

Node* temp = higherNode->m_left;
higherNode->m_left = higherNode->m_right;
higherNode->m_right = temp;

higherNode->m_left = MergingNodes(higherNode->m_left, lowerNode);

/*

In the follwing lines of code, i calculate the null path length for the left and right child of higher node. I previously used:

higherNode->setNPL(higherNode->m_right == nullptr ? 0 : higherNode->m_right->getNPL() + 1); 

to calculate the null path lenth, however, i kept experiencing issues with the output of my leftist heap in the tree viewer tool. The way that i previously used
did not take account for both the children which is really needed to update the null path length so that it maintains the left-
ist property. I have now taken into consideration the right and left children and properly updating afterwards. This approach
gave me better and more accurate results in the tree viewer tool with 5 being the root node for min heap and the npls of the 
leftist heap were now accurate. 

*/
if (m_structure == LEFTIST) {

/*
-In this step i used the terenary operator and used it to check if the child of the left and the right node are empty, if it is
then the null path length for it will be considered 0. 

- I then set the null path length of the higher node to add 1 to the null path length.

- I then added a condittion to check if the null path length of the left child is less than that of the right child and then swap if necessary. 

*/
int NPLleft = (higherNode->m_left != nullptr) ? higherNode->m_left->getNPL() : 0;
int NPLright = (higherNode->m_right != nullptr) ? higherNode->m_right->getNPL() : 0;

        
higherNode->setNPL((NPLleft < NPLright ? NPLleft : NPLright) + 1);

        
if (NPLleft < NPLright) {
Node* temp = higherNode->m_left;
higherNode->m_left = higherNode->m_right;
higherNode->m_right = temp;
        }
    }

    return higherNode;
}

// The function returns the current number of orders in the queue. 
int MQueue::numOrders() const
{
  return m_size;
}

// This function returns the current priority function. 
prifn_t MQueue::getPriorityFn() const { 

  return m_priorFunc;

}

/* 

The function throws an out_of_range exception if the queue
is empty when the function is called. The function saves the 
current heap (m_heap) in PreviousHeap and then merges the left
and right subtrees of the heap using MergingNodes helper function.
it decrements the queue size and returns the stored order.

*/
Order MQueue::getNextOrder() { 

if (m_size == 0) {

  throw out_of_range("The queue is empty!");

  }

    
Order Temp = m_heap->m_order;

Node * PreviousHeap = m_heap;

m_heap = MergingNodes(m_heap->m_left, m_heap->m_right);

delete PreviousHeap;

m_size --;

return Temp;

}

/*
This function sets a new priority function and its corresponding heap type.
It also saves the current heap in PreviousHeap, sets m_heap to nullptr, and 
calls TraverseHeap helper function.
*/

void MQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) { 

if(heapType == m_heapType && priFn == m_priorFunc) {

  return;

}

m_priorFunc = priFn;

m_heapType = heapType;

Node * PreviousHeap = m_heap;

m_heap = nullptr;

TraverseHeap(PreviousHeap);

         
}

/*
This helper function traverses the heap  in a post-order manner.
For each node, it first recursively traverses the left and right children 
and then sets both the left (m_left) and right (m_right) pointers of the 
current node to nullptrbefore it merges the current node with the existing
heap using the MergingNodes function/
*/
void MQueue:: TraverseHeap (Node* node) {

if (node == nullptr) {
    return;
  }

TraverseHeap(node->m_left);
TraverseHeap(node->m_right);

node->m_left = nullptr;

node->m_right = nullptr;

m_heap = MergingNodes(node,m_heap);

}

/* This function sets the data structure, i.e. it is either SKEW or LEFTIST. 
rebuild a heap with the new structure using the nodes in the current data structure by using the
traverse heap helper function again. 
*/

void MQueue::setStructure(STRUCTURE structure){ 

if (structure == m_structure) {
      return;
    }

Node * Temp = m_heap;

m_heap = nullptr;

m_structure = structure;

TraverseHeap(Temp);


}

//This function returns the structure of the current heap, i.e. it is either SKEW or LEFTIST. 
STRUCTURE MQueue::getStructure() const { 

  return m_structure;
}


// This function returns the heap type, i.e. it is either MINHEAP or MAXHEAP
HEAPTYPE MQueue::getHeapType() const{ 

  return m_heapType;
}

/*
It prints the contents of the queue using preorder traversal. Although the first order printed
should have the highest priority, the remaining orders will not necessarily be in priority order.
*/
void MQueue::printOrderQueue() const { 


PrintOrderHelper(m_heap);

}

/*
This helper function recursively calls itself for the left and right children of the node to continue 
the traversal in a pre-order traversal. It also prints the priority value of the order.

*/
void MQueue:: PrintOrderHelper (Node* node) const {

if( node != nullptr) {

cout << "[" << m_priorFunc(node->m_order) << "] " << *node << endl;
PrintOrderHelper(node->m_left);
PrintOrderHelper(node->m_right);

  }


}

void MQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}
void MQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.m_customer;
    else
        cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.m_customer << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Order& order) {
  sout  << "Customer: " << order.getCustomer()
        << ", importance: " << order.getImportance() 
        << ", quantity: " << order.getQuantity() 
        << ", order placement time: " << order.getFIFO();
  return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.m_order;
  return sout;
}
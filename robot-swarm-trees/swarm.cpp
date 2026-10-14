// UMBC - CMSC 341 - Fall 2024 - Proj2
#include "swarm.h"

//  The default constructor creates an empty object and sets the type of the tree to none.
Swarm::Swarm(){ 

    m_root= nullptr;
    m_type= NONE;

}
// The constructor also creates an empty object but also specifies the type of tree to any type. 
Swarm::Swarm(TREETYPE type){

    m_root= nullptr;
    m_type = type;
 
}
// The destructor cleans up the memory deaalocations and uses the helper function implemented below.
Swarm::~Swarm(){

    ClearTree(m_root);
    m_root = nullptr;
}

// The clear function deallocates all memory in an empty tree.
void Swarm::clear(){
     
    ClearTree(m_root);  
    m_root= nullptr;
     
}

// The helper function that will recursively clean up memory.
void Swarm::ClearTree(Robot* node){  
    // Check if the current node is not a nullptr
    if (node != nullptr) { 

        //Clearing the left subtree of the current node recursively.
        ClearTree(node ->getLeft());

        //Clearing the right subtree of the current node recursively.
        ClearTree(node ->getRight());

        //Deallocating the memory to get rid of any memory leaks.
        delete node; 
    }
}

// This helper function sets the height of the node according to the heights of the left and right children of the tree.
void Swarm::UpdateHeights(Robot* node) {
    // Checking if the node is null
    if (node == nullptr) {
        return;
    }
    // Calculating the height of the left and right children of the node.
    int leftHeight = getHeight(node->m_left);
    int rightHeight = getHeight(node->m_right);

    // Setting the heights of the current node as one more than the maximum.
    node->m_height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// This function inserts a Robot object into the tree in the proper position.
void Swarm::insert(const Robot& robot){
    if (robot.getID() < MINID || robot.getID() > MAXID)  
        return;
    switch(m_type){ //Switch statement based on the type of tree. 
        case NONE:
        break;
        case BST: //If the tree is a binary search function, the insertBST function will be called.
        m_root = insertBST(m_root,robot);
        break;
        case AVL: //If the tree is a AVL tree function, the insertAVL function will be called.
        m_root = insertAVL(m_root,robot);
        break;
        case SPLAY: //If the tree is a SPLAY tree function, the insertSPLAY function will be called.
        m_root = insertSPLAY(m_root, robot);
        break;
        default:
        break;
    }
}
Robot* Swarm::insertBST(Robot* m_root, const  Robot& robot){
    if(m_root == nullptr){
        Robot* newNode = new Robot(robot);
        newNode->m_height = 0; // Initializing height for leaf nodes
        return newNode;
    }

    // The Left subtree
    if(robot.getID() < m_root->getID()){
        m_root->m_left = insertBST(m_root->m_left, robot);
    }
    // The Right subtree
    else {
        m_root->m_right = insertBST(m_root->m_right, robot);
    }

    // Updating the height of the current node
    UpdateHeights(m_root);

    return m_root;

}
Robot* Swarm::insertAVL(Robot* m_root, const Robot& robot) {
    // Perform standard BST insertion
    if (m_root == nullptr) {
        Robot* newNode = new Robot(robot);
        newNode->m_height = 0; // Initialize height for leaf nodes
        return newNode;
    }

    if (robot.getID() < m_root->getID()) {
        m_root->m_left = insertAVL(m_root->m_left, robot);

    } else if (robot.getID() > m_root->getID()) {
        m_root->m_right = insertAVL(m_root->m_right, robot);
    } else {
        
        return m_root;
    }

    // Update the height of the current node
    UpdateHeights(m_root);

    // Get the balance factor to check for imbalances
    int balance = getBalance(m_root);

    // Left Left Case
    if (balance > 1 && robot.getID() < m_root->m_left->getID()) {
        return rightRotate(m_root);
    }

    // Right Right Case
    if (balance < -1 && robot.getID() > m_root->m_right->getID()) {
        return leftRotate(m_root);
    }

    // Left Right Case
    if (balance > 1 && robot.getID() > m_root->m_left->getID()) {
        m_root->m_left = leftRotate(m_root->m_left);
        return rightRotate(m_root);
    }

    // Right Left Case
    if (balance < -1 && robot.getID() < m_root->m_right->getID()) {
        m_root->m_right = rightRotate(m_root->m_right);
        return leftRotate(m_root);
    }

   
    return m_root;
}

Robot* Swarm::insertSPLAY(Robot* root, const Robot& robot) {
    if (root == nullptr) {

        Robot* newNode = new Robot(robot);
        newNode->m_height = 0;
        return newNode;
    }

    // Splay the tree with the given key
    root = splay(root, robot.getID());

    // If the key already exists, do not insert
    if (root->getID() == robot.getID()) {
        return root;
    }

    // Create new node
    Robot* newNode = new Robot(robot);
    newNode->m_height = 0;

    if (robot.getID() < root->getID()) {
        newNode->m_right = root;
        newNode->m_left = root->m_left;
        root->m_left = nullptr;
    } else {
        
        newNode->m_left = root;
        newNode->m_right = root->m_right;
        root->m_right = nullptr;
    }

    // Update heights
    UpdateHeights(root);
    UpdateHeights(newNode);

    return newNode; // New node becomes the new root
}


Robot* Swarm::splay(Robot* root, int key) {
    if (root == nullptr || root->getID() == key) {
        return root;
    }

    if (key < root->getID()) {
        if (root->m_left == nullptr) {
            return root;
        } 

        // Zig-Zig (Left Left)
        if (key < root->m_left->getID()) {
            root->m_left->m_left = splay(root->m_left->m_left, key);
            root = rightRotate(root);
        }
        // Zig-Zag (Left Right)
        else if (key > root->m_left->getID()) {
            root->m_left->m_right = splay(root->m_left->m_right, key);
            if (root->m_left->m_right != nullptr)
                root->m_left = leftRotate(root->m_left);
        }

        return (root->m_left == nullptr) ? root : rightRotate(root);

    } else { 
        if (root->m_right == nullptr) return root;

        // Zag-Zig (Right Left)
        if (key < root->m_right->getID()) {
            root->m_right->m_left = splay(root->m_right->m_left, key);
            if (root->m_right->m_left != nullptr)
                root->m_right = rightRotate(root->m_right);
        }
        // Zag-Zag (Right Right)
        else if (key > root->m_right->getID()) {
            root->m_right->m_right = splay(root->m_right->m_right, key);
            root = leftRotate(root);
        }

        return (root->m_right == nullptr) ? root : leftRotate(root);
    }
}

// This function gets the heights of a node in the tree.
int Swarm::getHeight(Robot* node) {
   if (node == nullptr) {
        return -1; 
    }
        return node->m_height;
}

// This function calculated the balance factor of a node. Used mostly in the avl tree function implementations.
int Swarm::getBalance(Robot* node) {
    if (node == nullptr) {
        return 0;
    }
    
        return getHeight(node->m_left) - getHeight(node->m_right);

}

// Function to perform right rotations. 
Robot* Swarm::rightRotate(Robot* y) {
      if (y == nullptr || y->m_left == nullptr) {
        return y; 
    }
    Robot* x = y->m_left;
    Robot* Temp = x->m_right;

    // Perform rotation
    x->m_right = y;
    y->m_left = Temp;

    // Updating heights using the UpdateHeights function
    UpdateHeights(y);
    UpdateHeights(x);

    // Return new root
        return x;
}

// Function to perform left rotations. 
Robot* Swarm::leftRotate(Robot* x) {
         if (x == nullptr || x->m_right == nullptr) {
        return x; 
    }
    Robot* y = x->m_right;
    Robot* Temp = y->m_left;

    // Perform rotation
    y->m_left = x;
    x->m_right = Temp;

    // Update heights using the UpdateHeights function
    UpdateHeights(x);
    UpdateHeights(y);

    
        return y;
}

void Swarm::remove(int id){
     if (m_type != NONE) {
        m_root = NodeRemoval(m_root, id);
    }
}

 /*Helper function for splay so that it handles removal in splay trees.
//Robot* Swarm::removeSPLAY(Robot* root, int key) {
    if (root == nullptr) return nullptr;

     Splay the key to the root
    root = splay(root, key);

     If the key is not present, return root
    if (root->getID() != key) return root;

    Robot* temp;
    if (root->m_left == nullptr) {
        temp = root->m_right;
    } else {
         Splay the maximum node in left subtree to the root of left subtree
        temp = splay(root->m_left, key);
        temp->m_right = root->m_right;
    }

    delete root;
    return temp;

 */

// Function to remove nodes from trees.
Robot* Swarm::NodeRemoval(Robot* node, int id) {
    if (node == nullptr) {
        return node; // Node not found
    }

    // Traversing the tree to locate the node to be deleted
        if (id < node->getID()) {

        node->m_left = NodeRemoval(node->m_left, id);

    } else if (id > node->getID()) {

        node->m_right = NodeRemoval(node->m_right, id);

    } else {
        
        // Node with only one child or no child
        if (node->m_left == nullptr) {
            Robot* temp = node->m_right;
            delete node;

        return temp;
        } else if (node->m_right == nullptr) {

            Robot* temp = node->m_left;
            delete node;

        return temp;
        }

        Robot* temp = MinValueNode(node->m_right);

        
        node->setID(temp->getID());

        
        node->m_right = NodeRemoval(node->m_right, temp->getID());
    }

    // Update the height of the current node after the deletion
    UpdateHeights(node);

    // Get the balance factor to check for imbalances
    int balance = getBalance(node);


    // Left Left Case
    if (balance > 1 && getBalance(node->m_left) >= 0) {
        return rightRotate(node);
    }

    // Left Right Case
    if (balance > 1 && getBalance(node->m_left) < 0) {
        node->m_left = leftRotate(node->m_left);
        return rightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && getBalance(node->m_right) <= 0) {
        return leftRotate(node);
    }

    // Right Left Case
    if (balance < -1 && getBalance(node->m_right) > 0) {
        node->m_right = rightRotate(node->m_right);
        return leftRotate(node);
    }

    return node;
}

// The function returns the minimum value node in a tree. 
Robot* Swarm::MinValueNode(Robot* node) {

    Robot* curr = node;

    while ( curr && curr -> m_left != nullptr) {
        curr = curr -> m_left;

    }

    return curr;

}

//This function overloads the assignment operator for the class Swarm. It creates an exact deep copy of the rhs. 
const Swarm & Swarm::operator=(const Swarm & rhs){
    
    if (this == &rhs) {
    return *this; // Self-assignment check
    }

    // Clear the existing tree
    ClearTree(m_root);
    m_root = nullptr;

    // Deep copy the tree from rhs
    m_root = CopyofTree(rhs.m_root);
    m_type = rhs.m_type;

    return *this;
}
//Helper function to copy the tree recursively.
Robot * Swarm::CopyofTree(Robot* node) { 
      if (node == nullptr) {
        return nullptr;
    }

    // Create a new Robot node with the same ID and height
    Robot* newNode = new Robot(node->getID(), node->getType(), node->getState());

    newNode->m_height = node->m_height;

    // Recursively copy the left and right subtrees
    newNode->m_left = CopyofTree(node->m_left);
    newNode->m_right = CopyofTree(node->m_right);

        return newNode;
}

// This function returns the current type of the tree. 
TREETYPE Swarm::getType() const{

        return m_type;

}

// This function sets the type of an existing Swarm object.
void Swarm::setType(TREETYPE type) {
    if (type == NONE) {
        clear();
        m_type = NONE;
        return;
    }

    if (type == AVL && (m_type == BST || m_type == SPLAY)) {
        int nodeCount = NodeCount(m_root);
        if (nodeCount == 0) {
            m_type = AVL;
            return;
        }

        // Allocate memory for storing node pointers
        Robot** nodeArray = new Robot*[nodeCount];
        int index = 0;
        InOrder(m_root, nodeArray, index);

        // Rebuilding the AVL tree from the sorted array
        Robot* newRoot = AVLFromArray(nodeArray, 0, nodeCount - 1);

        // Clearing the original BST or Splay tree nodes
        clear(); 

        // Assign the new AVL root
        m_root = newRoot;

        // Deallocate the node array to prevent memory leaks.
        delete[] nodeArray;
        m_type = AVL;
        return;
    }

    if (type == BST || type == SPLAY) {
        m_type = type;
        return;
    }
}


// This function recursively counts and returns the total amount of nodes in a BST.
int Swarm::NodeCount(Robot* node) {
    if (node == nullptr) {
        return 0;
    }
    return 1 + NodeCount(node->getLeft()) + NodeCount(node->getRight());
}

// This function performs in order traversal of a BST.
void Swarm::InOrder(Robot* node, Robot** arr, int& index) {
    if (node == nullptr) {
        return;
    }
    
    InOrder(node->getLeft(), arr, index);

    arr[index++] = node;  
    InOrder(node->getRight(), arr, index);
}

// Function that builds the avl trees using sorted arrays of robot nodes.
Robot* Swarm::AVLFromArray(Robot** arr, int start, int end) {
   if (start > end) {
        return nullptr;
    }

    
    int mid = (start + end) / 2;
    
    
    Robot* root = new Robot(arr[mid]->getID());
    root->m_height = 0; 

    // Recursively building the left and right subtrees
    root->m_left  =  AVLFromArray(arr, start, mid - 1);
    root->m_right = AVLFromArray(arr, mid + 1, end);

    // Updating the height of the current node 
    UpdateHeights(root);

    // Getting the balance factor to check for imbalances
    int balance = getBalance(root);

    // Left Left Case
    if (balance > 1 && getBalance(root->m_left) >= 0) {
        return rightRotate(root);
    }

    // Left Right Case
    if (balance > 1 && getBalance(root->m_left) < 0) {
        root->m_left = leftRotate(root->m_left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->m_right) <= 0) {
        return leftRotate(root);
    }

    // Right Left Case
    if (balance < -1 && getBalance(root->m_right) > 0) {
        root->m_right = rightRotate(root->m_right);
        return leftRotate(root);
    }

    return root;
}

void Swarm::dumpTree() const

{
    dump(m_root);
}

void Swarm::dump(Robot* aBot) const{
    if (aBot != nullptr){
        cout << "(";
        dump(aBot->m_left); //first visit the left child
        cout << aBot->m_id << ":" << aBot->m_height;  //second visit the node itself
        dump(aBot->m_right);//third visit the right child
        cout << ")";
        }
}
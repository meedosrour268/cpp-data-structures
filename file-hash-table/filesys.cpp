// CMSC 341 - Fall 2024 - Project 4
#include "filesys.h"
FileSys::FileSys(int size, hash_fn hash, prob_t probing = DEFPOLCY){ 

/*
The constructor initializes the variables and has a condition to ceck that the size of the table
is a prime number MINPRIME and MAXPRIME. It also allocates memory for the current table.
*/ 

    m_currProbing = probing;
    m_currentCap = size;
    m_hash = hash;
    m_newPolicy = probing;
    m_currentSize = 0;
    m_currNumDeleted = 0;
    m_oldTable = nullptr;
    m_oldCap = 0;
    m_oldSize = 0;
    m_oldProbing = probing;
    m_transferIndex = 0;
    rehashCounter = 0;


if (size < MINPRIME) {
    m_currentCap = MINPRIME;
}

if ( size > MAXPRIME) {
    m_currentCap = MAXPRIME;
}

if ( !isPrime(size)) {
    m_currentCap = findNextPrime(size);
} else {
    m_currentCap = size;
}

m_currentTable = new File * [m_currentCap];
    for (int i = 0; i < m_currentCap; i++) {
        m_currentTable [i] = nullptr;

    }

    
}

// The destructor deallocates the memory previously allocated.
FileSys::~FileSys(){
    
    if (m_currentTable != nullptr) {
        for (int i=0; i < m_currentCap; i++) {
            delete m_currentTable[i];    
        }
    delete[] m_currentTable;

    }

    if (m_oldTable != nullptr) {
        for (int i=0; i < m_oldCap; i++) {
            delete m_oldTable[i];    
        }
    delete[] m_oldTable;

    }


}
// The new policy is stored in the variablle m_newPolicy.
void FileSys::changeProbPolicy(prob_t policy){
    m_newPolicy = policy;

    //mcurpolicy = mnewpolicy 
}

/*
The insert file checks for the disblock and file names of the keys going into our hash table. 
It then computes the hash index using the three forms of probing we now know how to use. 
I previously had performance issuses in my code due to having too many calculations within my loops
that slowed the runtime by almost 10 minutes. 

I initialized the unsigned Hash to keep account for our hash
and then called it inside of my calculations. The calculations are done quadratically, linearly, and using 
the double hash proobing equation given to us in our project document, The insert function also allocates
memory for a new file using m_currentTable and increments the size. I have also added a condition to check 
for the rehashing criteria where the rehash is triggered when load factor excees 50 percent and deleted 
ratio exceeds 80 percent. 

*/
bool FileSys::insert(File file){

    
if (getFile(file.getName(),file.getDiskBlock()).getUsed()) {
    return false;

}



if (file.getDiskBlock() < DISKMIN || file.getDiskBlock() >DISKMAX) {
    return false;
}

// Computing the hash index.

unsigned int Hash = m_hash(file.getName());
unsigned int index;
int i = 0;

while (i < m_currentCap) {


    if (m_currProbing == QUADRATIC) {
        index = (Hash + i * i) % m_currentCap;
    
    } else if (m_currProbing == LINEAR) {
        index = (Hash + i) % m_currentCap;

    } else if (m_currProbing == DOUBLEHASH) {
        unsigned int PrimaryHash = Hash % m_currentCap;
        unsigned int SecondaryHash = 11 - (Hash % 11);
        index = (PrimaryHash + i * SecondaryHash ) % m_currentCap;

    } else {
        return false;
    }

    if(m_currentTable[index] == nullptr || !m_currentTable[index] -> getUsed()) {

        break;
    }

    i++;
}

if (i== m_currentCap) {

    return false;
}

m_currentTable[index] = new File(file);

m_currentSize ++;

if (lambda() >= 0.5 || deletedRatio() > 0.8) {
        Rehash();
    }

return true;

}

/*
-The rehash helper function checks for the rehashing criteria before saving the information 
for the old table. 

-The rehash function then updates the probing policy and calculates a new capacity
for the new table and was given to us as ((m_currentSize - m_numDeleted)*4) in the 
project document. 

-Similar to the constructor the rehash helper function also checks to make sure that the numbers added
to the table are prime numbers and then creates the new table. 

-The process of incremental rehashing of the table is then done by breaking down or dividing the old table
into four quarters using a private variable named rehash counter. first from the beginning to one quarter then the second quarter from one quarter to the 
half of the tables capacity then the third quarter from half to three quarters of the tables capacity and then
lastly from the last quarter to the full capacity of the table. 

- After that, index probing is done again and the private variable rehash counter is incremented. 
*/

void FileSys:: Rehash () {

if (lambda() >= 0.5 || deletedRatio() > 0.8) {

        
m_oldSize = m_currentSize;
m_oldProbing = m_currProbing;
m_oldCap = m_currentCap;
m_oldTable = m_currentTable;
m_oldNumDeleted = m_currNumDeleted;

        
if (m_newPolicy != m_currProbing) {
            m_currProbing = m_newPolicy;
        }

        // Calculate new capacity
        m_currentCap = ((m_currentSize - m_currNumDeleted) * 4);

if (m_currentCap < MINPRIME) {
        m_currentCap = MINPRIME;
    }

if (!isPrime(m_currentCap)) {
         m_currentCap = findNextPrime(m_currentCap);
    }

        // Create new table
        m_currentTable = new File * [m_currentCap];

    for (int i = 0; i < m_currentCap; i++) {
            m_currentTable[i] = nullptr;
    }

        m_currentSize = 0;
        m_currNumDeleted = 0;

        int start = 0;
        int end = 0;

    if (rehashCounter == 0) {
        start = 0;
        end   = m_oldCap / 4;
    } else if (rehashCounter == 1) {
        start = m_oldCap / 4;
        end = m_oldCap / 2;
    } else if (rehashCounter == 2) {
        start = m_oldCap / 2;
        end = (3 * m_oldCap) / 4;
    } else {
        start = (3 * m_oldCap) / 4;
        end = m_oldCap;
        }

        for (int i = start; i < end; i++) {

if (m_oldTable[i] != nullptr && m_oldTable[i]->getUsed()) {
                
        File file = *m_oldTable[i];
        unsigned int Hash = m_hash(file.getName());
        int j = 0;
        unsigned int index;

                
    while (j < m_currentCap) {

    if (m_currProbing == QUADRATIC) {
            index = (Hash + j * j) % m_currentCap;
    } else if (m_currProbing == LINEAR) {
            index = (Hash + j) % m_currentCap;
    } else if (m_currProbing == DOUBLEHASH) {
            unsigned int PrimaryHash = Hash % m_currentCap;
            unsigned int SecondaryHash = 11 - (Hash % 11);
            index = (PrimaryHash + j * SecondaryHash ) % m_currentCap;
    } else {
                       
        break;
        }

    if (m_currentTable[index] == nullptr || !m_currentTable[index]->getUsed()) {
                        
        break;
        }

        j++;
}

    if (j < m_currentCap) {
        m_currentTable[index] = new File(file);
        m_currentSize++;
     } else {
                   
}

        delete m_oldTable[i];
        m_oldTable[i] = nullptr;
        }
    }

    if (rehashCounter == 3) {
        delete [] m_oldTable;
        m_oldTable = nullptr;
        m_oldCap = 0;
        m_oldNumDeleted = 0;
        m_oldSize = 0;
        rehashCounter = 0;

    } else {

        rehashCounter++;
        }
    }
}

/*
The remove function removes a data point from either the current hash table or the old hash table where the object is stored. 

*/
bool FileSys::remove(File file) {
    if (file.getDiskBlock() < DISKMIN || file.getDiskBlock() > DISKMAX) {
        return false;
    }

bool FoundFile = false;
int ProbingIndex = 0;
unsigned int index;
unsigned int Hash = m_hash(file.getName());

// Condition to check if the file from the old has table still exists after trying to find and remove it.
if (m_oldTable != nullptr) {
ProbingIndex = 0;

    while (ProbingIndex < m_oldCap) {


// The probing methods here are used to calculate the indexes for the old table
     if (m_oldProbing == QUADRATIC) {
                index = (Hash + ProbingIndex * ProbingIndex) % m_oldCap;

    } else if (m_oldProbing == LINEAR) {

                index = (Hash + ProbingIndex) % m_oldCap;

    } else if (m_oldProbing == DOUBLEHASH) {

    unsigned int PrimaryHash = Hash % m_oldCap;
    unsigned int SecondaryHash = 11 - (Hash % 11);
    index = (PrimaryHash + ProbingIndex * SecondaryHash) % m_oldCap;
            }

// Here the searching stops if the index position in the old table is empty
     if (m_oldTable[index] == nullptr) {
                break;
            }
// Checking if the file is in the current bucket and recognize that it is not used if it is found.
     if (m_oldTable[index]->getName() == file.getName() &&
                m_oldTable[index]->getDiskBlock() == file.getDiskBlock() && m_oldTable[index] -> getUsed()) {

                m_oldTable[index]->setUsed(false);
                m_oldNumDeleted += 1;
                FoundFile = true;
                break;
            }

            ProbingIndex += 1;
        }
    }

/* 
iF its not found in the old table then we search the current table and see if it is found. 
The indexes are then calculated again for the current table.
*/
    if (!FoundFile && m_currentTable != nullptr) {
        ProbingIndex = 0;
     while (ProbingIndex < m_currentCap) {

       if (m_currProbing == QUADRATIC) {
                index = (Hash + ProbingIndex * ProbingIndex) % m_currentCap;

} else if (m_currProbing == LINEAR) {
                index = (Hash + ProbingIndex) % m_currentCap;

} else if (m_currProbing == DOUBLEHASH) {
                unsigned int PrimaryHash = Hash % m_currentCap;
                unsigned int SecondaryHash = 11 - (m_hash(file.getName()) % 11);
                index = (PrimaryHash + ProbingIndex * SecondaryHash) % m_currentCap;
            } else {
                break;
            }

// If is position is emoty then we stop searching. 
        if (m_currentTable[index] == nullptr) {
                break;
            }
// If the file is found, we set it as not used and then update the current table.
        if (m_currentTable[index]->getName() == file.getName() &&
                m_currentTable[index]->getDiskBlock() == file.getDiskBlock() && 
                m_currentTable[index] ->getUsed()) {
                m_currentTable[index]->setUsed(false);
                m_currNumDeleted += 1;
                FoundFile = true;
                break;
            }

            ProbingIndex += 1;
        }
    }
// Checking the rehashing criteria and use the rehash helper function describes above.
    if (lambda() >= 0.5 || deletedRatio() > 0.8) {
        Rehash();
    }

    return FoundFile;
}


/*
-This function looks for the File object with the name and the file block number in the database. 

The function also checks for both if the old table is empty and if the current table is empty  and 
uses the probing index calculations as well.
*/
const File FileSys::getFile(string name, int block) const{
 
unsigned int Hash = m_hash(name);
int ProbingIndex = 0;
unsigned int index;

 

  if (m_oldTable != nullptr) {
       
        while (ProbingIndex < m_oldCap) {
            if (m_oldProbing == QUADRATIC) {

                index = (Hash + ProbingIndex * ProbingIndex) % m_oldCap;
            }

    else if (m_oldProbing == LINEAR) {

                index = (Hash + ProbingIndex) % m_oldCap;
            }
    else if (m_oldProbing == DOUBLEHASH) {
        unsigned int  primaryHash = Hash % m_oldCap;
        unsigned int  secondaryHash = 11 - (Hash % 11);
                index = (primaryHash + ProbingIndex * secondaryHash) % m_oldCap;

            } else {
                break;
            }


    if (m_oldTable[index] == nullptr) {
                break;
    }

    if (m_oldTable[index]->getName() == name && m_oldTable[index]->getDiskBlock() == block && m_oldTable[index]->getUsed()) {
                return *m_oldTable[index];
    }

            ProbingIndex += 1;

        }
    }

    if (m_currentTable != nullptr) {
       
        while (ProbingIndex < m_currentCap) {
            if (m_currProbing == QUADRATIC) {
                index = (Hash + ProbingIndex * ProbingIndex) % m_currentCap;
            }
            else if (m_currProbing == LINEAR) {
                index = (Hash + ProbingIndex) % m_currentCap;
            }
            else if (m_currProbing == DOUBLEHASH) {

             unsigned int primaryHash = Hash % m_currentCap;
             unsigned int secondaryHash = 11 - (Hash % 11);
             index = (primaryHash + ProbingIndex * secondaryHash) % m_currentCap;
            } else {
                break;
            }

            if (m_currentTable[index] == nullptr) {
                break;
            }
            if (m_currentTable[index]->getName() == name && m_currentTable[index]->getDiskBlock() == block && m_currentTable[index]->getUsed()) {
                return * m_currentTable[index];
            }
            ProbingIndex += 1;
        }
    }

    return File(); 
}


/*
This function looks for the File object in the database, if the object is found the function updates its block number and returns true, 
otherwise the function returns false. 

It similarly also uses the index probing calculation and has a condition to check if the file is not found in the old table and then finds
and updates.

*/
bool FileSys::updateDiskBlock(File file, int block){
    
    bool Foundfile = false;
    unsigned int index;
    unsigned int Hash = m_hash(file.getName());
 

    if (m_oldTable != nullptr) {
        int ProbingIndex = 0;
        while (ProbingIndex < m_oldCap) {
            if  (m_oldProbing == QUADRATIC) {

                index = (Hash + ProbingIndex * ProbingIndex) % m_oldCap;
            }
            else if (m_oldProbing == LINEAR) {


                index = (Hash + ProbingIndex) % m_oldCap;
            }
            else if (m_oldProbing == DOUBLEHASH) {
              unsigned int primaryHash = Hash % m_oldCap;
              unsigned int  secondaryHash = 11 - (Hash % 11);
              index = (primaryHash + ProbingIndex * secondaryHash) % m_oldCap;
            }

            if (m_oldTable[index] == nullptr) {
                break;
            }

            if (m_oldTable[index]->getName() == file.getName() && m_oldTable[index]->getDiskBlock() == file.getDiskBlock() && m_oldTable[index]->getUsed()) {
                m_oldTable[index]->setDiskBlock(block);
                Foundfile = true;
                break;
            }
            ProbingIndex += 1;
        }
    }

    if (!Foundfile && m_currentTable != nullptr) {
        
       int Probingindex = 0;
       
        while (Probingindex < m_currentCap) {
            if (m_currProbing == QUADRATIC) {
                index = (Hash + Probingindex * Probingindex) % m_currentCap;
            }
            else if (m_currProbing == LINEAR) {
                index = (Hash + Probingindex) % m_currentCap;
            }
            else if (m_currProbing == DOUBLEHASH) {
              unsigned int  primaryHash = Hash % m_currentCap;
              unsigned int  secondaryHash = 11 - (Hash % 11);
              index = (primaryHash + Probingindex * secondaryHash) % m_currentCap;

            } else {
                break;
            }

            if (m_currentTable[index] == nullptr) {
                break;
            }

// The disk block is updates if the file is found. 
            if (m_currentTable[index]->getName() == file.getName() && m_currentTable[index]->getDiskBlock() == file.getDiskBlock() && m_currentTable[index]->getUsed()) {
                m_currentTable[index]->setDiskBlock(block);
                Foundfile = true;
                break;
            }

            Probingindex += 1;
        }
    }

    return Foundfile;
}


float FileSys::lambda() const {

// The load factor is the ratio of occupied buckets to the table capacity. 

// It is calculated as the number of elements inserted into the table by the number of slots in the table.

    return static_cast<float> (m_currentSize) / m_currentCap;


}

// The deletedRatio function calculates and returns the ratio of the number of deleted entries to the total capacity.
float FileSys::deletedRatio() const {
   
    
    return static_cast<float> (m_currNumDeleted) / m_currentCap;
    
}


void FileSys::dump() const {
    cout << "Dump for the current table: " << endl;
    if (m_currentTable != nullptr)
        for (int i = 0; i < m_currentCap; i++) {
            cout << "[" << i << "] : " << m_currentTable[i] << endl;
        }
    cout << "Dump for the old table: " << endl;
    if (m_oldTable != nullptr)
        for (int i = 0; i < m_oldCap; i++) {
            cout << "[" << i << "] : " << m_oldTable[i] << endl;
        }
}

bool FileSys::isPrime(int number){
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int FileSys::findNextPrime(int current){
    //we always stay within the range [MINPRIME-MAXPRIME]
    //the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME-1;
    for (int i=current; i<MAXPRIME; i++) { 
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0) 
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}
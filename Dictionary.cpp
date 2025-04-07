// Implementation file for the Dictionary class
// Stores data in an hash table, double hashing for collisions
// By Mary Elaine Califf and Nick Kolesar

#include "Dictionary.h"
#include "bits/stdc++.h"
using namespace std;

Dictionary::Dictionary() : size(0), tableSizeIndex(0)
{
    // allocate and initialize all indicies to nullptr
    hashTable = new string*[getTableSize()] {};
}


Dictionary& Dictionary::operator=(const Dictionary& rhs)
{
    if (this != &rhs) //comparing the tree roots because that's as effective as comparing the object addresses here
    {
        clear();           // delete the current tree;
        copy(rhs); // copy rhs's tree into this
    }
    return *this;
}


void Dictionary::clear()
{
    // delete contents of hashTable
    for (int i = 0; i < getTableSize(); i++)
        if (hashTable[i])
            delete hashTable[i];

    // delete hashTable itself
    delete[] hashTable;

    // reset variables
    tableSizeIndex = 0;
    size = 0;
}


void Dictionary::copy(const Dictionary& rhs)
{
    // static copying
    size = rhs.size;
    tableSizeIndex = rhs.tableSizeIndex;

    // allocate new hashTable
    hashTable = new string*[getTableSize()];

    for (int i = 0; i < getTableSize(); i++)
    {
        if (rhs.hashTable[i])
            hashTable[i] = new string(*rhs.hashTable[i]);
        else
            hashTable[i] = nullptr;
    }
}


bool Dictionary::findEntry(const string& anEntry) const
{
    uint32_t hashIndex = hash(anEntry, getTableSize()),
             offset = calcOffset(anEntry);

    while (hashTable[hashIndex])
    {    
        if (*hashTable[hashIndex] == anEntry)
            return true;
        
        hashIndex = (hashIndex + offset) % getTableSize();
    }
    return false;
}


void Dictionary::addEntry(string* anEntry)
{
    // if 2(size+1) > table size, then capacity > 50% 
    if (2 * (size + 1) > getTableSize())
        rehash();

    insert(anEntry);
}


void Dictionary::insert(string* anEntry)
{
    // get initial hash index
    uint32_t hashIndex = hash(*anEntry, getTableSize());

    // find valid insertion index if collision happens
    if (hashTable[hashIndex])
        hashIndex = secondaryHash(*anEntry, hashIndex);
    // actual insertion
    hashTable[hashIndex] = anEntry;
    
    size++;
}


uint32_t Dictionary::hash(const string& key, const uint32_t modVal)
{
    uint32_t hashVal = 0;

    for (char letter: key)
        hashVal = 37 * hashVal + letter;

    return hashVal % modVal;
}

uint32_t Dictionary::secondaryHash(const string& key, uint32_t hashIndex) const
{
    // calculate the offset
    uint32_t offset = calcOffset(key);

    // keep double hashing until a nullptr is found
    while (hashTable[hashIndex])
        hashIndex = (hashIndex + offset) % getTableSize();

    return hashIndex;
}


void Dictionary::rehash()
{
    tableSizeIndex++;
    // size will be redetermined by number of addEntry()s
    size = 0;
    
    // save the old hash table
    string** oldTable = hashTable;

    // make new hash table
    hashTable = new string*[getTableSize()] {};

    for (int i = 0; i < TABLE_SIZE_ARR[tableSizeIndex - 1]; i++)
        if (oldTable[i])
            insert(oldTable[i]);

    delete[] oldTable;
}


void Dictionary::printDictionaryInOrder(ostream& outputStream) const
{
    vector<string*> sortedTable;
    sortedTable.reserve(size);

    // load sorted table with keys
    for (int i = 0; i < getTableSize(); i++)
        if (hashTable[i])
            sortedTable.emplace_back(hashTable[i]);
    
    sort(sortedTable.begin(), sortedTable.end(), compareKeys);

    // print the sorted table
    for (string* key : sortedTable)
        outputStream << *key << '\n';
}


void Dictionary::printDictionaryKeys(ostream& outputStream) const
{
    for (int i = 0; i < getTableSize(); i++)
    {
        outputStream << i << ": ";
        // print key if there is one
        if (hashTable[i])
            outputStream << *hashTable[i];
        
        outputStream << '\n';
    }
}
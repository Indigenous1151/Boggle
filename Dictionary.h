// A dictionary created using a double hash table
// By Mary Elaine Califf and Nick Kolesar
#include <string>
#include <vector>
using namespace std;

#ifndef DICTIONARY_H
#define DICTIONARY_H


class Dictionary
{
private:
    // variable to hold hash table
    string** hashTable;

    // constant arrays
    static constexpr uint32_t TABLE_SIZE_ARR[] = { 101, 211, 431, 863, 1733, 3469, 6947, 13901, 27803, 55609, 111227, 222461, 444929, 889871 };
    static constexpr uint32_t DOUBLE_HASH_ARR[] = { 97, 199, 421, 859, 1723, 3467, 6917, 13883, 27799, 55603, 111217, 222437, 444901, 889829 };

    uint32_t tableSizeIndex;
    uint32_t size;

public:
    // Constructor
    Dictionary();

    // Copy constructor
    Dictionary(const Dictionary& orig) { copy(orig); }

    // Destructor
    ~Dictionary() { clear(); }

    // Assignment operator
    Dictionary& operator=(const Dictionary& rhs);

    // Check to see if the Dictionary isEmpty
    inline bool isEmpty() const { return size == 0; }

    // Add an entry
    // Precondition: the dictionary must not have the identical string already stored in the dictionary
    // Postcondition: the string has been added to the dictionary
    void addEntry(std::string* anEntry);

    // Determine whether the string is in the dictionary
    // Returns true if the string is stored in the dictionary and false otherwise
    bool findEntry(const std::string& key) const;

    // Print entries in order
    // Calls printEntry on each Entry in order
    void printDictionaryInOrder(std::ostream& outputStream) const;

    // Prints the dictionary keys only, demonstrating the dictionary structure
    // For the binary search tree, this is an easy to do tree format
    void printDictionaryKeys(std::ostream& outputStream) const;

private:

    // clear helper method for copy constructor and assignment operator
    void clear();

    // copy helper method for destructor and assignment operator
    void copy(const Dictionary& orig);

    // returns double hash index
    uint32_t secondaryHash(const string& key, uint32_t currentIndex) const;

    // increases capacity of `hashTable` to next value in `TABLE_SIZE_ARR`
    void rehash();

    // performs the actual insertion into the hash table
    // pre-condition: capacity after insert is less than 50%
    void insert(string* anEntry);

    // returns first hash index for `key`
    static uint32_t hash(const string& key, const uint32_t modVal);

    // second hashing formula for getting next index after collision
    uint32_t calcOffset(const string& key) const { return hash(key, getModValue()) + 1; }


    // returns `TABLE_SIZE_ARR[tableSizeIndex]`
    inline int getTableSize() const { return TABLE_SIZE_ARR[tableSizeIndex]; }
    inline int getModValue() const { return DOUBLE_HASH_ARR[tableSizeIndex]; }

    static bool compareKeys(string* key1, string* key2) { return *key1 < *key2; }
};

#endif
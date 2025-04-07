// Program to build and play Boggle
// The program will take 3 command line parameters, the names of two input files and one output file.
// The input files will be
//      1) a list of words, which will be stored in a dictionary
//      2) a file containing inputs for the boggle board as indicies
// The output file will show the structure of the dictionary that was created (done for grading purposes)
// The program will output line-by-line which words are valid or invalid
// By Mary Elaine Califf and Nick Kolesar

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include "Dictionary.h"
#include "BoggleBoard.h"

using namespace std;

// function prototypes

// opens and reads the word list input file, building the dictionary
void buildDictionary(const string& inFileName, Dictionary& dict);

// reads word list input file and returns a frequency table of letters
// for use in generation of the boggle board
vector<int> getFrequencyTable(const string& inFileName);

// randomly generates letter a-z based on `frequencyTable`
char weightedRand(const vector<int>& frequencyTable, const int sum);

// main functionality of the program that validates inFileName inputs
// by comparing build strings to words in dictionary
void playBoggle(const Dictionary& dict, const vector<int>& frequencyTable, 
                const unsigned int ROWS=5, const unsigned int COLS=5);

// print board to console
void printBoard(const vector<vector<char>>& board);

// open the dictionary structure file and write the structure of the dictionary to it
void writeDictionaryStructure(const string& outFileName, const Dictionary& dict);

// Extra prototypes

// take input string and build result
// returns true if the provided input is valid, otherwise false
bool parseUserInput(string& input, const vector<vector<char>>& board, 
                    string& curWord, const unsigned int ROWS=5, const unsigned int COLS=5);

// helper function to determine if two cells are next to each other using manhattan distance
bool isAdjacent(const int previousCell, const int curCell, 
                const unsigned int ROWS=5, const unsigned int COLS=5);


int main(int argc, char** argv)
{
    // gather the file name arguments
    if (argc < 4)
    {
        // we didn't get enough arguments, so complain and quit
        cout << "Usage: " << argv[0] << " wordListFile dictionaryStructureFile randomSeed" << endl;
        exit(1);
    }

    string wordListFileName = argv[1];
    string dictFileName = argv[2];
    string randSeed = argv[3];

    // entering 0 for the seed produces a random board
    if (randSeed == "0")
        srand(time(nullptr));
    else
        srand(stoi(randSeed));

    Dictionary dict; // set up the dictionary

    // build the dictionary
    buildDictionary(wordListFileName, dict);

    BoggleBoard board(dict, wordListFileName);

    board.play(std::cout);

    // write dictionary structure to dictionaryStructureFile
    writeDictionaryStructure(dictFileName, dict);
}


void buildDictionary(const string& inFileName, Dictionary& dict)
{
    string curWord = "";

    // open the file
    ifstream infile(inFileName);
    if (infile.fail())
    {
        cerr << "Could not open " << inFileName << " for input" << endl;
        exit(1);
    }

    // allow infile to read spaces
    infile >> noskipws;

    char letter;
    // Read file contents
    while (infile >> letter)
    {
        // Add lowercase letter to the current word 
        if (isalpha(letter))
            curWord += tolower(letter);
        else if (curWord != "")
        {
            // Add new word to the dictionary
            if (!dict.findEntry(curWord))
                dict.addEntry(new string(curWord));
            
            // empty out curWord before getting a new word
            curWord = "";
        }
    }
    // close the file
    infile.close();
}


void writeDictionaryStructure(const string& outFileName, const Dictionary& dict)
{
    ofstream outfile(outFileName);
    if (outfile.fail())
    {
        cerr << "Could not open " << outFileName << " for output\n";
        cerr << "Dictionary structure file not written " << endl;
        return;
    }
    dict.printDictionaryKeys(outfile);

    // testing in order printing
    // dict.printDictionaryInOrder(outfile);
    outfile.close();
}

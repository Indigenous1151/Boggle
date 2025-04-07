// Implmentation file for BoggleBoard class
// By Nick Kolesar
#include <set>
#include <fstream>
#include <sstream>
#include "BoggleBoard.h"
#include "weightedRand.h"

/* === Constructor Stuff === */

void BoggleBoard::_copy(const BoggleBoard& orig)
{
    _dict = orig._dict;
    _frequencyTable = orig._frequencyTable;
    _frequencySum = orig._frequencySum;
    _board = orig._board;
}

// default constructor
BoggleBoard::BoggleBoard(const Dictionary& dict, const string& wordListFileName)
{
    _dict = &dict; // point _dict at the built dictionary
    _frequencyTable = _buildFrequencyTable(wordListFileName);
    _frequencySum = _calcFrequencySum();
    _buildBoard();
}

// copy assignment operator
BoggleBoard& BoggleBoard::operator=(const BoggleBoard& orig)
{
    if (this != &orig)
    {
        // no clear because there isn't any dynamic allocation
        _copy(orig);
    }

    return *this;
}

/* === Public Methods === */

void BoggleBoard::play(std::ostream& outputStream)
{
    // ENDING WILL BE REACHED WHEN NOTHING IS ENTERED
    outputStream << "Press [enter] without text to end program.\n\n";
    
    // show user the board
    printBoard(outputStream);

    bool userComplete = false;
    while (!userComplete)
    {
        outputStream << "Enter a sequence of indicies: ";
        // convert the user inputted string to a vector of integers
        vector<int> indices = _parseUserInput(_getUserInput());
        // validate the integers
        if (indices.empty())
            userComplete = true;
        else if (_validateSteps(indices))
        {
            // convert the indices to an actual word
            string curWord = _translateToWord(indices);

            if (_dict->findEntry(curWord))
                outputStream << curWord << " is a valid word\n";
            else
                outputStream << curWord << " is not a valid word\n";
        }
        else
            outputStream << "ERROR: Invalid Entry\n";
    }
}


void BoggleBoard::printBoard(std::ostream& outputStream) const
{
    outputStream << "Boggle Board:\n";
    for (vector<char> row : _board)
    {
        for (char element : row)
            outputStream << element << ' ';
        
        outputStream << '\n';
    }    
    outputStream << endl;
}

/* === Private Methods === */

void BoggleBoard::_buildBoard()
{
    for (int i = 0; i < ROWS; i++)
    {
        // temp row to populate
        vector<char> row;
        for (int j = 0; j < COLS; j++)
            row.push_back(weightedRand(_frequencyTable, _frequencySum));
        // push populated row
        _board.push_back(row); 
    }
}

vector<int> BoggleBoard::_buildFrequencyTable(const string& wordListFileName) const
{
    vector<int> table(26, 0); // local frequency table

    // read from dict file
    ifstream fin(wordListFileName);
    
    if (!fin)
    {
        cerr << "ERROR: unable to read from file '" << wordListFileName << "'" << endl;
    }
    else // file exists, so good to go
    {
        string word;
    
        // populate frequency table word by word
        while (fin >> word)
            for (char letter : word)
                // handle rogue spaces in dict files
                if (isalpha(letter))
                    table[tolower(letter)-'a']++;
        
        // ensure proper file closure
        fin.close();
    }

    return table;
}

int BoggleBoard::_calcFrequencySum() const
{
    int sum = 0;
    for (int num : _frequencyTable)
        sum += num;
    return sum;
}

string BoggleBoard::_getUserInput() const
{
    string input;
    getline(cin, input);
    return input;
}

bool BoggleBoard::_isAdjacent(const int from, const int to) const
{
    // get row and col for both cells
    int prevRow = from / COLS;
    int prevCol = from % COLS;

    int curRow = to / COLS;
    int curCol = to % COLS;

    // take difference of cur and prev for comparison
    int rowDiff = abs(curRow-prevRow);
    int colDiff = abs(curCol-prevCol);

    // check that cells are adjacent making sure previous and curcell are different
    return ((rowDiff <= 1 && colDiff <= 1) && from != to);
}

vector<int> BoggleBoard::_parseUserInput(const string& charSequence) const
{
    vector<int> indices;
    stringstream ss(charSequence);
    string token;

    // tokenize input
    while(getline(ss, token, ' '))
    {
        // handle double space
        if (token.empty()) continue;
        // add integer token to indices or break with magic error
        try
        {
            indices.push_back(stoi(token));
        }
        catch(const std::exception& e)
        {
            indices.clear();
            indices.push_back(-1); // signal error
            break;
        }
    }
    return indices;
}

string BoggleBoard::_translateToWord(const vector<int>& indices) const
{
    string word = "";
    
    for (int index : indices)
        word += _board[index/COLS][index%ROWS];
    
    return word;
}

bool BoggleBoard::_validateSteps(const vector<int>& indices) const
{
    // holds visited cells to check repeats
    set<int> visitedCells;
    int prev = -1; // no previous yet
    int numCells = ROWS*COLS; // calc now so repeated multiplication doesn't happen

    for (int index : indices)
    {
        // either repeated cell or invalid cell number or prev not adjacent to index
        if (visitedCells.find(index) != visitedCells.end() || 
            index >= numCells || index < 0 ||
            (prev != -1 && !_isAdjacent(prev, index)))
        {
            return false;
        } 
        // update visitedCells and prev
        visitedCells.insert(index);
        prev = index;
    }
    
    // no issues found
    return true;
}


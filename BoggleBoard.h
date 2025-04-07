// BoggleBoard class which handles all boggle board functionality.
// By Nick Kolesar
#pragma once

#include <iostream>
#include "Dictionary.h"

class BoggleBoard {
    /* === Private methods === */

    // Helper method for building the `_frequencyTable`
    // this table is based on the letter frequencies
    // in the provided `_dict`, not a general frequency count.
    vector<int> _buildFrequencyTable(const string& wordListFileName) const;
    // Take sum of frequency values in `_frequencyTable`.
    int _calcFrequencySum() const;
    // Helper method to build the actual boggle board.
    void _buildBoard();
    // Method to get user input.
    string _getUserInput() const;
    // Method to convert user input into set of indices
    vector<int> _parseUserInput(const string& charSequence) const;
    // Method to check ensure indices follow valid steps
    bool _validateSteps(const vector<int>& indices) const;
    // helper for `_validateSteps` to check adjacency
    bool _isAdjacent(const int from, const int to) const;
    // Method to translate indices into word
    string _translateToWord(const vector<int>& indices) const;
    
    // Helper method for constructors.
    void _copy(const BoggleBoard& orig);

public:
    BoggleBoard(const Dictionary& dict, const string& wordListFileName); // Default constructor
    BoggleBoard(const BoggleBoard& orig) { _copy(orig); } // Copy constructor
    BoggleBoard& operator=(const BoggleBoard& orig); // Copy assignment operator
    ~BoggleBoard() { _dict = nullptr; } // Destructor
    
    // Method to begin playing the game.
    void play(ostream& outputStream);
    // Method to print the board.
    void printBoard(ostream& outputStream) const;

private:
    /* === Constants === */
    const unsigned int COLS = 5;
    const unsigned int ROWS = 5;

    /* === Private variables === */
    int                   _frequencySum;
    const Dictionary*     _dict;
    vector<int>           _frequencyTable;
    vector<vector<char>>  _board;

};
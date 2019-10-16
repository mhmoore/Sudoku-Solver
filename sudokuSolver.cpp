#include <iostream>
#include <fstream>
using namespace std;

void import_puzzle(int puzzle[9][9], string filename) {
    ifstream ifs(filename);
    for (int row = 0; row < 9; row++) {
        for (int column = 0; column < 9; column++) {
            ifs >> puzzle[row][column];
        }
    }
}

void print_puzzle(int puzzle[9][9]) {
    cout << endl;
    for (int row = 0; row < 9; row++) {
        for (int column = 0; column < 9; column++) {
            cout << puzzle[row][column] << "  ";
        }
        cout << endl << endl;
    }
    cout << endl;
}

bool row_conflict(int row, int number, int puzzle[9][9]) { // searches rows and reports if a number has already been used

    for (int column = 0; column < 9; column++) {
        if (puzzle[row][column] == number) {
            return true;
        }
    }
    return false;
}

bool column_conflict(int column, int number, int puzzle[9][9]) {  // searches columns and reports if a number as already been used
    
    for (int row = 0; row < 9; row++) {
        if (puzzle[row][column] == number) {
            return true;
        }
    }
    return false;
}

/*
 * takes the row and column of the cell being checked, and sets the index at the upper left cell of that 3x3 box.
 * It then iterates through each cell (column by column, row by row)
 * checks to see if the number you're trying to place is already in the 3x3 box
 */
bool box_conflict(int row, int column, int number, int puzzle[9][9]) { // searches box and reports if a number as already been used
    int index_row = row - (row % 3);
    int index_col = column - (column % 3);

    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 3; column++) {
            if (puzzle[row + index_row][column + index_col] == number) {
                return true;
            }
        }
    }

    return false;
}

bool valid_move(bool row_conflict, bool column_conflict, bool box_conflict) { // checks if number has already been used in row, column, and box
    if (row_conflict || column_conflict || box_conflict) {
        return false;
    }
    return true;
}

struct empty_cell {
    int r;
    int c;
};

empty_cell check_empty_cell(int puzzle[9][9]) {
    empty_cell ec;
    ec.r = -1;
    ec.c = -1;
    for (int row = 0; row < 9; row++){
        for (int column = 0; column < 9; column++){
            if (puzzle[row][column] == 0) {
                ec.r = row;
                ec.c = column;
                return ec;
            }
        }
    }
    return ec;
}

bool solve(int puzzle[9][9]) {

    empty_cell ec = check_empty_cell(puzzle);

    if (ec.r == -1 && ec.r == -1) {
        return true;
    }

    int row = ec.r;
    int column = ec.c;

    for (int number = 1; number < 10; number++) {
        if (valid_move(row_conflict(row, number, puzzle), column_conflict(column, number, puzzle), box_conflict(row, column, number, puzzle))) {
            puzzle[row][column] = number;  // if the move is valid, assign temporary number
            ofstream pout("stepByStepSolution.txt", ios::app);
                for (int row = 0; row < 9; row++) {
                    for (int column = 0; column < 9; column++) {
                        pout << puzzle[row][column] << "  ";
                    }
                    pout << endl << endl;
                    }
                pout << endl;
               
            if (solve(puzzle)) { // call function again to find the next empty cell, and place an another number
                return true;
            }
            puzzle[row][column] = 0;
        }        
    }
    return false;
}

int main () {
    int sudoku_puzzle[9][9];
    import_puzzle(sudoku_puzzle, "hard0.txt");
    print_puzzle(sudoku_puzzle); // print initial puzzle

    if (solve(sudoku_puzzle)) {
        cout << "The solution:" << endl;
        print_puzzle(sudoku_puzzle); // print solved puzzle
    } else {
        cout << "This puzzle has no solution." << endl;
    }

    return 0;
}
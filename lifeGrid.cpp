//
//  lifeGrid.cpp
//  life
//
//  Created by Vince Mansel on 9/29/11.
//  Copyright 2011 Wave Ocean Networks. All rights reserved.
//


#include <iostream>

#include "genlib.h"
#include "extgraph.h"
#include "simpio.h"
#include "strutils.h"
#include "grid.h"

#include "lifeGrid.h"
#include "lifeGraphics.h"

/*
 * Simulation Update Speed Note:
 *  SimSpeed of 1 calls pause with 0    seconds
 *              2                  0.1
 *              3                  0.5    
 */
const double FrameRateMuliplier = 0.1; // Fast Simulation
//Nullifies the fast sim when turtle mode (3 - 1 = 2) selected. 2 * 0.25 = 0.5 second updates.
const double FrameRateAdjustment = (1 / FrameRateMuliplier) * 0.25;

void ShowGrid(gridLifeT &gridLife);
bool IsDirectionOccupied(gridLifeT &gridLife, int row, int col, int simMode,int dRow, int dCol, int nRows, int nCols);
int CheckForNeighbors(gridLifeT &gridLife, int row, int col, int simMode, int nRows, int nCols);
bool UpdateGrid(gridLifeT &gridLife, int simMode);

void DrawGridX(gridLifeT &gridLife) {
    int rows = gridLife.numRows();
    int cols = gridLife.numCols();
    InitGraphics(); // Slightly faster
    //InitLifeGraphics(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            //if (gridLife[i][j] != 0) {  // DrawCellAt is an expensive operation, CPU-wise.
                                        // Only use for cells with life, not empty cells.
                DrawCellAt(i, j, gridLife[i][j]);
            //}
        }
    //ShowGrid(gridLife);
}


void DrawGrid1(gridLifeT &gridLife) {
    int rows = gridLife.numRows();
    int cols = gridLife.numCols();
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            DrawCellAt(i, j, gridLife[i][j]);
        }
}

void DrawGrid(gridLifeT &gridLife) {
    int rows = gridLife.numRows();
    int cols = gridLife.numCols();
    InitGraphics(); // Flushes the graphics palette
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            if (gridLife[i][j] != 0) {  
                // DrawCellAt is an expensive operation, CPU-wise.
                // Only use for cells with life, not empty cells.
                DrawCellAt(i, j, gridLife[i][j]);
            }
        }
}


void RunLifeSim(int simMode, int simSpeed, gridLifeT &gridLife) {
    //cout << "Stub for RunLifeSim(" << simSpeed << ", " << simMode << ", GRID= " << gridLife.size.row << ":" << gridLife.size.col << ")" << endl;
    cout << endl;
    
    if (simSpeed == 4) {
        while (true) {
            cout << "RETURN to continue (or \"quit\" to end simulation): "; 
            string line = ConvertToLowerCase(GetLine());
            if (line[0] == 'q') {
                break;
            }
            bool isStabilized = UpdateGrid(gridLife, simMode);
            DrawGrid(gridLife);
            if (isStabilized) {
                cout << "Colony stablized." << endl;
                break;
            }
        }
    }
    else {
        double frameRate = (simSpeed - 1) * FrameRateMuliplier * ((simSpeed == 3) ? FrameRateAdjustment : 1);
        cout << "Click and hold the mouse button on the graphics window to end the simulation." << endl;
        while (true) {
            if (MouseButtonIsDown()) {
                break;
            }
            bool isStabilized = UpdateGrid(gridLife, simMode);
            DrawGrid(gridLife);
            if (isStabilized) {
                cout << "Colony stablized." << endl;
                break;
            }
            Pause(frameRate);
        }
    }
}

void PrintCell(int point) {
    cout << " " << point;
}

void ShowGrid(gridLifeT &gridLife) {
    //gridLife.lifeGrid.mapAll(PrintCell);
    cout << endl;
    for (int i = 0; i < gridLife.numRows(); i++) {
        cout << " ";
        for (int j = 0; j < gridLife.numCols(); j++) {
            cout << gridLife[i][j];
            string spacer = ((gridLife[i][j]) > 9) ? " " : "  ";
            cout << spacer;
        }
        cout << endl;
    }
}

/*
 * Calculates life for the next step and updates the Grid by reference based on the given simMode.
 * Also, checks for stability of colony on-going during the update process.
 */


bool UpdateGrid(gridLifeT &gridLife, int simMode) {
    gridLifeT gridLife2 = gridLife;
    bool isStabilized = true;
    int rows = gridLife.numRows();
    int cols = gridLife.numCols();
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int neighbors = CheckForNeighbors(gridLife2, i, j, simMode, rows, cols);
            int age = gridLife[i][j];
            switch (neighbors) {
                case 2:
                    if (age < MaxAge) {
                        gridLife[i][j] += (age != 0) ? 1 : 0;
                    }
                    break;
                case 3:
                    if (age < MaxAge) {
                        gridLife[i][j] += 1;
                    }
                    break;
                default:
                    gridLife[i][j] = 0;
                    break;
            }
            age = gridLife[i][j];
            if (isStabilized && (age > 0 && age < MaxAge)) {
                //cout << " UpdatedGrid: isStabilize false at: " << i << ":" << j << endl;
                isStabilized = false;
            }
        }
    }
    return isStabilized;
}

int CheckForNeighbors(gridLifeT &gridLife, int row, int col, int simMode, int nRows, int nCols) {
    int count = 0;
    
    for (int dRow = -1; dRow <= 1; dRow++) {
        for (int dCol = -1; dCol <= 1; dCol++) {
            count += IsDirectionOccupied(gridLife, row, col, simMode, dRow, dCol, nRows, nCols);
        }
    }
    return count;
}

/**
 * Function: OnBoard
 * (modified from: 08-Velociraptor-Safety.pdf, Stanford Univerity)
 * -----------------
 * OnBoard is a predicate function that returns true if and only if the
 * specified coordinate indexes a legal entry of the lifeGrid.
 */

bool OnBoard(int row, int col, int nRows, int nCols) {
    return (row >= 0 && row < nRows &&
            col >= 0 && col < nCols);
}

/**
 * Function: IsDirectionOccupied
 * (modified from: 08-Velociraptor-Safety.pdf, Stanford Univerity)
 * -------------------------
 * IsDirectionOccupied decides whether or not a neighbor can be seen
 * by looking from the specified (row, col) coordinate in a particular
 * direction--specified by arguments four and five in the form of exactly
 * what values should be added to (row, col) to look in a specified
 * direction.
 *
 * Assuming that the origin (0,0) coincides with the lower left corner of
 * then board, IsDirectionOccupied must examine the coordinates * (row + dRow, col + dCol), (row + 2 * dRow, col + 2 * dCol)
 * to see if neighbors are adjacent.
 *
 * In Donut Mode, IsDirectionOccupied wraps the grid around like a torus to check "neighborship".
 *
 * In Mirror Mode, off the edge cells are a relection of what is on the grid.
 */

bool IsDirectionOccupied(gridLifeT &gridLife, int row, int col, int simMode,int dRow, int dCol, int nRows, int nCols) {    
    if ((dRow == 0) && (dCol == 0)) return false; // protect against bad call
    row += dRow;
    col += dCol;
    if (simMode == 0 ) { //|| row != 0 || col != 0) { // Plateau
        return (OnBoard(row, col, nRows, nCols) && gridLife[row][col] > 0);
    }
    else if (simMode == 1) { // Donut
        if (row < 0) {
            row = nRows - 1;
        }
        else if (row == nRows) {
            row = 0;
        }
        if (col < 0) {
            col = nCols - 1;
        }
        else if (col == nCols) {
            col = 0;
        }
    }
    else if (simMode == 2) { // Mirror
        if (row < 0) {
            row = 0;
        }
        else if (row == nRows) {
            row = nRows - 1;
        }
        if (col < 0) {
            col = 0;
        }
        else if (col == nCols) {
            col = nCols - 1;
        }
    }
    else {
        Error("Bad simMode detected");
    }
    return (gridLife[row][col] > 0);
}






//
//  lifeGrid.h
//  life
//
//  Created by Vince Mansel on 9/29/11.
//  Copyright 2011 Wave Ocean Networks. All rights reserved.
//

/*
 * Defines routines the interact directly with graphics window,
 * and runs the life simulation
 *
 */

#ifndef life_lifeGrid_h
#define life_lifeGrid_h

struct gridSizeT {
    int row;
    int col;
};

typedef Grid<int> gridLifeT;

/*
 * Draws a fresh grid unto the graphics window
 */

void DrawGrid(gridLifeT &gridLife);

/* 
 * Handles the various modes of the game and kicks off the life simulation
 */

void RunLifeSim(int simMode, int simSpeed, gridLifeT &gridLife);

#endif

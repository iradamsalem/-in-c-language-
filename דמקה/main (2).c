#pragma once
#include "Trees.h"
#include "Lists.h"
#include "Game.h"

// Final Project

// name:moshe yifrah id:208153346
// name:irad amsalem id:209363639



void main()
{



	Player player = 'B';  //choosen player

	Board board;

	buildBoard(&board); // build the started board

	PlayGame(board, player);


}
#pragma once
#include "Trees.h"
#include "Lists.h"


// Function declarations

// Function to handle a player's turn
void Turn(Board board, Player player);

// Function to copy the contents of one board to another
void copyBoard(Board board, Board boardtocopy);

// Function to update the board state based on a move
void changeBoard(Board board, bool capture, checkersPos* lastmove, checkersPos* nextmove);

// Function to play the game
void PlayGame(Board board, Player starting_player);

// Function to check if the opponent still has any pieces on the board
bool checkOpponent(Board board, Player player);


// Function to check if the current player has reached the opponent's end
bool playerInEnd(Board board, Player player);

// Function to initialize the board with the starting configuration
void buildBoard(Board* board);

//Function to print the board
void printBoard(Board board);
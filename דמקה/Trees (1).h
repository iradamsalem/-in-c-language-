#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>
#define BOARD_SIZE 8

typedef struct _checkersPos
{
    char row, col;
} checkersPos;

typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];
typedef unsigned char Player;

typedef struct _SingleSourceMovesTreeNode {
    Board board;
    checkersPos* pos;
    unsigned short total_captures_so_far;
    struct _SingleSourceMovesTreeNode* next_move[2];
} SingleSourceMovesTreeNode;

typedef struct _SingleSourceMovesTree
{
    SingleSourceMovesTreeNode* source;
} SingleSourceMovesTree;



// Function declarations


// Find all possible moves for a single source position on the board
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src);

// Build the tree of possible moves for a single source position (T player)
SingleSourceMovesTree buildTTreeMoves(SingleSourceMovesTree trOptions, Board board, int row, int col);

// Allocate memory for a single source moves tree node
void allocatecell(SingleSourceMovesTreeNode** cell);

// Check if memory allocation was successful
void checkAllocation(void* ptr);

// Build the tree of possible moves for a single source position (B player)
SingleSourceMovesTree buildBTreeMoves(SingleSourceMovesTree trOptions, Board board, int row, int col);

// Free the memory allocated for a single source moves tree
void freeTree(SingleSourceMovesTreeNode* root);

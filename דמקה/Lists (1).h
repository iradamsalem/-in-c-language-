#pragma once
#include "Trees.h"

// Structure for a single cell in a SingleSourceMovesList
typedef struct _SingleSourceMovesListCell
{
    checkersPos* position;
    unsigned short captures;
    struct _SingleSourceMovesListCell* next;
} SingleSourceMovesListCell;

// Structure for a SingleSourceMovesList
typedef struct _SingleSourceMovesList
{
    SingleSourceMovesListCell* head;
    SingleSourceMovesListCell* tail;
} SingleSourceMovesList;

// Structure for a cell in a MultipleSourceMovesList
typedef struct _multipleSourceMovesListCell
{
    SingleSourceMovesList* single_source_moves_list;
    struct _multipleSourceMovesListCell* next;
} MultipleSourceMovesListCell;

// Structure for a MultipleSourceMovesList
typedef struct _multipleSourceMovesList
{
    MultipleSourceMovesListCell* head;
    MultipleSourceMovesListCell* tail;
} MultipleSourceMovesList;

// Function declarations

// Find the optimal move in a SingleSourceMovesTree
SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree);

// Initialize an empty SingleSourceMovesList
void makeEmptyList(SingleSourceMovesList* lst);

// Recursively find the optimal move in a SingleSourceMovesTree
SingleSourceMovesList FindSingleSourceOptimalMoverec(SingleSourceMovesTreeNode* tree);

// Get the length of a SingleSourceMovesList
int listLength(SingleSourceMovesList* list);

// Insert a SingleSourceMovesTreeNode at the beginning of a SingleSourceMovesList
void insertToBeginningList(SingleSourceMovesList* lst, SingleSourceMovesTreeNode node);

// Find all possible moves for a player on the board
MultipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player);

// Initialize an empty MultipleSourceMovesList
void makeEmptyMultipleList(MultipleSourceMovesList* lst);

// Insert a SingleSourceMovesList at the end of a MultipleSourceMovesList
void insertDataToEndMultipleList(MultipleSourceMovesList* lst, SingleSourceMovesList node);

// Free the memory allocated for a SingleSourceMovesList
void freeList(SingleSourceMovesList* lst);

// Free the memory allocated for a MultipleSourceMovesList
void freeMultipleList(MultipleSourceMovesList* lst);

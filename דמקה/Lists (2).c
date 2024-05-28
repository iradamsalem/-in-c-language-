#pragma once
#include "Trees.h"
#include "Lists.h"
#include "Game.h"


// Find the optimal move in a SingleSourceMovesTree
SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree)
{
    SingleSourceMovesList* list;
    list = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
    checkAllocation(list);

    *list = FindSingleSourceOptimalMoverec(moves_tree->source);

    return list;

}

// Recursively find the optimal move in a SingleSourceMovesTree
SingleSourceMovesList FindSingleSourceOptimalMoverec(SingleSourceMovesTreeNode* tree)
{
    SingleSourceMovesList left, right, main;
    makeEmptyList(&main);  // Initialize the main list

    if (tree == NULL)
    {
        return main;
    }
    if (tree->next_move[0] == NULL && tree->next_move[1] == NULL)
    {
        insertToBeginningList(&main, *(tree));
        return main;
    }
    if (tree->next_move[0] == NULL && tree->next_move[1] != NULL)
    {
        right = FindSingleSourceOptimalMoverec(tree->next_move[1]);
        insertToBeginningList(&right, *(tree));
        return right;
    }
    if (tree->next_move[0] != NULL && tree->next_move[1] == NULL)
    {
        left = FindSingleSourceOptimalMoverec(tree->next_move[0]);
        insertToBeginningList(&left, *(tree));
        return left;
    }
    if (tree->next_move[0] != NULL && tree->next_move[1] != NULL)
    {
        left = FindSingleSourceOptimalMoverec(tree->next_move[0]);
        right = FindSingleSourceOptimalMoverec(tree->next_move[1]);

        // Return the longest list between left and right
        if (left.tail->captures > right.tail->captures)
        {
            insertToBeginningList(&left, *(tree));


            return left;
        }
        else if (left.tail->captures < right.tail->captures)
        {
            insertToBeginningList(&right, *(tree));


            return right;
        }

        else if (listLength(&left) > listLength(&right))
        {
            insertToBeginningList(&left, *(tree));


            return left;
        }
        else
        {
            insertToBeginningList(&right, *(tree));


            return right;
        }


    }

    return main;
}


// Insert a SingleSourceMovesList at the end of a MultipleSourceMovesList
void insertDataToEndMultipleList(MultipleSourceMovesList* lst, SingleSourceMovesList node)
{
    MultipleSourceMovesListCell* newNode = (MultipleSourceMovesListCell*)malloc(sizeof(MultipleSourceMovesListCell));
    checkAllocation(newNode);

    newNode->single_source_moves_list = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
    checkAllocation(newNode->single_source_moves_list);

    *(newNode->single_source_moves_list) = node;

    newNode->next = NULL;

    if (lst->head == NULL)
    {
        lst->head = newNode;
        lst->tail = newNode;
    }
    else
    {
        lst->tail->next = newNode;
        lst->tail = newNode;
    }
}

// Initialize an empty SingleSourceMovesList
void makeEmptyList(SingleSourceMovesList* lst)
{
    lst->head = NULL;
    lst->tail = NULL;
}

// Initialize an empty MultipleSourceMovesList
void makeEmptyMultipleList(MultipleSourceMovesList* lst)
{
    lst->head = NULL;
    lst->tail = NULL;
}

// Get the length of a SingleSourceMovesList
int listLength(SingleSourceMovesList* list)
{
    int length = 0;
    SingleSourceMovesListCell* current = list->head;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}

// Insert a SingleSourceMovesTreeNode at the beginning of a SingleSourceMovesList
void insertToBeginningList(SingleSourceMovesList* lst, SingleSourceMovesTreeNode node)
{
    SingleSourceMovesListCell* newNode = (SingleSourceMovesListCell*)malloc(sizeof(SingleSourceMovesListCell));
    checkAllocation(newNode);
    newNode->position = (checkersPos*)malloc(sizeof(checkersPos));
    checkAllocation(newNode->position);
    newNode->position->col = node.pos->col;
    newNode->position->row = node.pos->row;
    newNode->captures = node.total_captures_so_far;
    newNode->next = lst->head;
    lst->head = newNode;
    if (lst->tail == NULL)
    {
        lst->tail = newNode;
    }
}



// Find all possible moves for a player on the board
MultipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player)
{
    MultipleSourceMovesList* multiplelst = (MultipleSourceMovesList*)malloc(sizeof(MultipleSourceMovesList));
    checkAllocation(multiplelst);
    makeEmptyMultipleList(multiplelst);
    checkersPos src;
    int row, col;

    if (player == 'T')
    {
        for (row = BOARD_SIZE - 1; row >= 0; row--)
        {
            for (col = BOARD_SIZE - 1; col >= 0; col--)
            {
                if ((row % 2 == 0 && col % 2 != 0) || (row % 2 != 0 && col % 2 == 0))
                {
                    if (board[row][col] == player)
                    {
                        src.row = row + '0';
                        src.col = col + '0';
                        SingleSourceMovesTree* tr = FindSingleSourceMoves(board, &src);
                        SingleSourceMovesList* lst = FindSingleSourceOptimalMove(tr);
                        freeTree(tr->source);

                        if (lst->head)
                            insertDataToEndMultipleList(multiplelst, *lst);

                    }
                }
            }

        }
    }
    if (player == 'B')
    {

        for (row = 0; row < BOARD_SIZE; row++)
        {
            for (col = 0; col < BOARD_SIZE; col++)
            {
                if ((row % 2 == 0 && col % 2 != 0) || (row % 2 != 0 && col % 2 == 0))
                {

                    if (board[row][col] == player)
                    {

                        src.row = row + '0';
                        src.col = col + '0';
                        SingleSourceMovesTree* tr = FindSingleSourceMoves(board, &src);
                        SingleSourceMovesList* lst = FindSingleSourceOptimalMove(tr);
                        freeTree(tr->source);
                        if (lst->head)
                            insertDataToEndMultipleList(multiplelst, *lst);
                    }
                }
            }


        }
    }
    return multiplelst;
}


// Free the memory allocated for a SingleSourceMovesList
void freeList(SingleSourceMovesList* lst)
{
    SingleSourceMovesListCell* current = lst->head;

    while (current != NULL)
    {
        free(current->position);
        SingleSourceMovesListCell* next = current->next;
        free(current);
        current = next;
    }

    free(lst);
}


// Free the memory allocated for a MultipleSourceMovesList
void freeMultipleList(MultipleSourceMovesList* lst)
{
    MultipleSourceMovesListCell* current = lst->head;

    while (current != NULL)
    {
        freeList(current->single_source_moves_list);
        MultipleSourceMovesListCell* next = current->next;
        free(current);
        current = next;
    }

    free(lst);
}
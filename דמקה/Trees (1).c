#include "Trees.h"
#include "Lists.h"
#include "Game.h"

// Find all possible moves for a single source position on the board
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src)
{


    SingleSourceMovesTree* treetoreturn;
    treetoreturn = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree));
    checkAllocation(treetoreturn);
    int col = (int)src->col - '0';
    int row = (int)src->row - '0';
    SingleSourceMovesTree trOptions;
    allocatecell(&trOptions.source);
    copyBoard(trOptions.source->board, board);



    if (board[row][col] == 'T')
        trOptions = buildTTreeMoves(trOptions, board, row, col);

    else if (board[row][col] == 'B')
        trOptions = buildBTreeMoves(trOptions, board, row, col);
    else
    {
        free(trOptions.source);
        free(treetoreturn);

        return NULL;
    }
    *(treetoreturn) = trOptions;

    return treetoreturn;
}


// Build the tree of possible moves for a single source position (T player)
SingleSourceMovesTree buildTTreeMoves(SingleSourceMovesTree trOptions, Board board, int row, int col)
{
    SingleSourceMovesTree right, left;

    trOptions.source->pos->col = col;
    trOptions.source->pos->row = row;
    if (row == 7)
        return trOptions;
    if (col + 1 == 8)
        trOptions.source->next_move[1] = NULL;
    if (col - 1 < 0)
        trOptions.source->next_move[0] = NULL;


    if ((board[row + 1][col + 1] == '*' || board[row + 1][col - 1] == '*') && trOptions.source->total_captures_so_far == 0)
    {
        if (board[row + 1][col + 1] == '*')
        {
            allocatecell(&trOptions.source->next_move[1]);
            trOptions.source->next_move[1]->pos->col = col + 1;
            trOptions.source->next_move[1]->pos->row = row + 1;
            copyBoard(trOptions.source->next_move[1]->board, board);
            changeBoard(trOptions.source->next_move[1]->board, false, trOptions.source->next_move[1]->pos, trOptions.source->pos);
        }
        if (board[row + 1][col - 1] == '*')
        {
            allocatecell(&trOptions.source->next_move[0]);
            trOptions.source->next_move[0]->pos->col = col - 1;
            trOptions.source->next_move[0]->pos->row = row + 1;
            copyBoard(trOptions.source->next_move[0]->board, board);
            changeBoard(trOptions.source->next_move[0]->board, false, trOptions.source->next_move[0]->pos, trOptions.source->pos);
        }

        if (board[row + 1][col + 1] == '*' && board[row + 1][col - 1] == '*')
            return trOptions;


    }
    if (board[(row)+1][(col)+1] == '*' && board[(row)+1][(col)-1] == 'T' && trOptions.source->total_captures_so_far == 0)
    {
        allocatecell(&trOptions.source->next_move[1]);
        trOptions.source->next_move[1]->pos->col = col + 1;
        trOptions.source->next_move[1]->pos->row = row + 1;
        copyBoard(trOptions.source->next_move[1]->board, board);
        changeBoard(trOptions.source->next_move[1]->board, false, trOptions.source->next_move[1]->pos, trOptions.source->pos);
        trOptions.source->next_move[0] = NULL;
        return trOptions;

    }
    if (board[(row)+1][(col)+1] == 'T' && board[(row)+1][(col)-1] == '*' && trOptions.source->total_captures_so_far == 0)
    {
        allocatecell(&trOptions.source->next_move[0]);
        trOptions.source->next_move[0]->pos->col = col - 1;
        trOptions.source->next_move[0]->pos->row = row + 1;
        copyBoard(trOptions.source->next_move[0]->board, board);
        changeBoard(trOptions.source->next_move[0]->board, false, trOptions.source->next_move[0]->pos, trOptions.source->pos);
        trOptions.source->next_move[1] = NULL;
        return trOptions;
    }
    if (board[(row)+1][(col)+1] == 'T' && board[(row)+1][(col)-1] == 'T')
    {
        trOptions.source->next_move[0] = NULL;
        trOptions.source->next_move[1] = NULL;
        return trOptions;

    }
    if ((board[(row)+1][(col)+1] == 'B' || board[(row)+1][(col)-1] == 'B'))
    {

        if (board[(row)+2][(col)+2] == 'B' || board[(row)+2][(col)+2] == 'T')
        {
            trOptions.source->next_move[0] = NULL;
        }
        if (board[(row)+2][(col)-2] == 'B' || board[(row)+2][(col)-2] == 'T')
        {
            trOptions.source->next_move[1] = NULL;
        }
        if (board[(row)+2][(col)-2] == '*' && board[(row)+1][(col)-1] == 'B')
        {
            allocatecell(&trOptions.source->next_move[0]);
            left.source = trOptions.source->next_move[0];
            trOptions.source->next_move[0]->total_captures_so_far += trOptions.source->total_captures_so_far + 1;
            trOptions.source->next_move[0]->pos->row = row + 2;
            trOptions.source->next_move[0]->pos->col = col - 2;
            copyBoard(trOptions.source->next_move[0]->board, board);
            changeBoard(trOptions.source->next_move[0]->board, true, trOptions.source->next_move[0]->pos, trOptions.source->pos);
            trOptions.source->next_move[0] = buildTTreeMoves(left, left.source->board, left.source->pos->row, left.source->pos->col).source;

        }
        if (board[(row)+2][(col)+2] == '*' && board[(row)+1][(col)+1] == 'B')
        {
            allocatecell(&trOptions.source->next_move[1]);

            right.source = trOptions.source->next_move[1];
            trOptions.source->next_move[1]->total_captures_so_far += trOptions.source->total_captures_so_far + 1;
            trOptions.source->next_move[1]->pos->row = row + 2;
            trOptions.source->next_move[1]->pos->col = col + 2;
            copyBoard(trOptions.source->next_move[1]->board, board);
            changeBoard(trOptions.source->next_move[1]->board, true, trOptions.source->next_move[1]->pos, trOptions.source->pos);
            trOptions.source->next_move[1] = buildTTreeMoves(right, right.source->board, right.source->pos->row, right.source->pos->col).source;

        }

        return trOptions;


    }
    return trOptions;
}


// Build the tree of possible moves for a single source position (B player)
SingleSourceMovesTree buildBTreeMoves(SingleSourceMovesTree trOptions, Board board, int row, int col)//build B player tree moves recursively
{

    SingleSourceMovesTree right, left;
    trOptions.source->pos->col = col;
    trOptions.source->pos->row = row;
    if (row == 0)
        return trOptions;
    if (col + 1 == 8)
        trOptions.source->next_move[0] = NULL;
    if (col - 1 < 0)
        trOptions.source->next_move[1] = NULL;


    if ((board[row - 1][col + 1] == '*' || board[row - 1][col - 1] == '*') && trOptions.source->total_captures_so_far == 0)
    {
        if (board[row - 1][col + 1] == '*')
        {
            allocatecell(&trOptions.source->next_move[0]);

            trOptions.source->next_move[0]->pos->col = col + 1;
            trOptions.source->next_move[0]->pos->row = row - 1;
            copyBoard(trOptions.source->next_move[0]->board, board);
            changeBoard(trOptions.source->next_move[0]->board, false, trOptions.source->next_move[0]->pos, trOptions.source->pos);
        }
        if (board[row - 1][col - 1] == '*')
        {


            allocatecell(&trOptions.source->next_move[1]);

            trOptions.source->next_move[1]->pos->col = col - 1;
            trOptions.source->next_move[1]->pos->row = row - 1;
            copyBoard(trOptions.source->next_move[1]->board, board);
            changeBoard(trOptions.source->next_move[1]->board, false, trOptions.source->next_move[1]->pos, trOptions.source->pos);
        }

        if (board[row - 1][col + 1] == '*' && board[row - 1][col - 1] == '*')
        {

            return trOptions;
        }


    }
    if (board[(row)-1][(col)+1] == '*' && board[(row)-1][(col)-1] == 'B' && trOptions.source->total_captures_so_far == 0)
    {

        allocatecell(&trOptions.source->next_move[0]);

        trOptions.source->next_move[0]->pos->col = col + 1;
        trOptions.source->next_move[0]->pos->row = row - 1;
        copyBoard(trOptions.source->next_move[0]->board, board);
        changeBoard(trOptions.source->next_move[0]->board, false, trOptions.source->next_move[0]->pos, trOptions.source->pos);
        trOptions.source->next_move[1] = NULL;
        return trOptions;

    }
    if (board[(row)-1][(col)+1] == 'B' && board[(row)-1][(col)-1] == '*' && trOptions.source->total_captures_so_far == 0)
    {

        allocatecell(&trOptions.source->next_move[1]);

        trOptions.source->next_move[1]->pos->col = col - 1;
        trOptions.source->next_move[1]->pos->row = row - 1;
        copyBoard(trOptions.source->next_move[1]->board, board);
        changeBoard(trOptions.source->next_move[1]->board, false, trOptions.source->next_move[1]->pos, trOptions.source->pos);
        trOptions.source->next_move[0] = NULL;
        return trOptions;
    }
    if (board[(row)-1][(col)+1] == 'B' && board[(row)-1][(col)-1] == 'B')
    {

        trOptions.source->next_move[0] = NULL;
        trOptions.source->next_move[1] = NULL;
        return trOptions;

    }
    if (board[(row)-1][(col)+1] == 'T' || board[(row)-1][(col)-1] == 'T')
    {

        if (board[(row)-2][(col)+2] == 'B' || board[(row)-2][(col)+2] == 'T')
        {

            trOptions.source->next_move[1] = NULL;
        }
        if (board[(row)-2][(col)-2] == 'B' || board[(row)-2][(col)-2] == 'T')
        {

            trOptions.source->next_move[0] = NULL;
        }
        if (board[(row)-2][(col)-2] == '*' && board[(row)-1][(col)-1] == 'T')
        {

            allocatecell(&trOptions.source->next_move[1]);
            trOptions.source->next_move[1]->total_captures_so_far += trOptions.source->total_captures_so_far + 1;
            right.source = trOptions.source->next_move[1];
            trOptions.source->next_move[1]->pos->row = row - 2;
            trOptions.source->next_move[1]->pos->col = col - 2;
            copyBoard(trOptions.source->next_move[1]->board, board);
            changeBoard(trOptions.source->next_move[1]->board, true, trOptions.source->next_move[1]->pos, trOptions.source->pos);
            trOptions.source->next_move[1] = buildBTreeMoves(right, right.source->board, right.source->pos->row, right.source->pos->col).source;

        }
        if (board[(row)-2][(col)+2] == '*' && board[(row)-1][(col)+1] == 'T')
        {
            allocatecell(&trOptions.source->next_move[0]);
            trOptions.source->next_move[0]->total_captures_so_far += trOptions.source->total_captures_so_far + 1;


            left.source = trOptions.source->next_move[0];


            trOptions.source->next_move[0]->pos->row = row - 2;
            trOptions.source->next_move[0]->pos->col = col + 2;
            copyBoard(trOptions.source->next_move[0]->board, board);
            changeBoard(trOptions.source->next_move[0]->board, true, trOptions.source->next_move[0]->pos, trOptions.source->pos);
            trOptions.source->next_move[0] = buildBTreeMoves(left, left.source->board, left.source->pos->row, left.source->pos->col).source;

        }


        return trOptions;


    }
    return trOptions;





}

// Allocate memory for a single source moves tree node
void allocatecell(SingleSourceMovesTreeNode** cell)
{
    *(cell) = (SingleSourceMovesTreeNode*)malloc(sizeof(SingleSourceMovesTreeNode));
    checkAllocation(*cell);
    (*cell)->next_move[0] = NULL;
    (*cell)->next_move[1] = NULL;
    (*cell)->pos = (checkersPos*)malloc(sizeof(checkersPos));
    checkAllocation((*cell)->pos);

    (*cell)->total_captures_so_far = 0;

}




// Check if memory allocation was successful
void checkAllocation(void* ptr)
{
    if (ptr == NULL)
    {
        printf("Memory Failed");
        exit(1);
    }
}


// Free the memory allocated for a single source moves tree
void freeTree(SingleSourceMovesTreeNode* root)
{
    if (root == NULL)
    {
        return;
    }
    if (root->next_move[0])
    {
        freeTree(root->next_move[0]);
    }
    if (root->next_move[1])
    {
        freeTree(root->next_move[1]);
    }
    free(root->pos);
    free(root);

}
#include "Lists.h"
#include "Trees.h"
#include "Game.h"

// Function to handle a player's turn
void Turn(Board board, Player player)
{
	int maxcap = 0;
	int countmax = 1;

	// Find all possible moves for the player
	MultipleSourceMovesList* lst = FindAllPossiblePlayerMoves(board, player);
	MultipleSourceMovesListCell* multiplecurr = lst->head;
	SingleSourceMovesList* bestlist = NULL;

	// Iterate over the moves and find the one with the maximum captures
	while (multiplecurr)
	{
		if (multiplecurr->single_source_moves_list->tail->captures > maxcap)
		{
			countmax = 1;
			maxcap = multiplecurr->single_source_moves_list->tail->captures;
			bestlist = multiplecurr->single_source_moves_list;
		}
		multiplecurr = multiplecurr->next;

		// If multiple moves have the same maximum captures, count them
		if (multiplecurr && multiplecurr->single_source_moves_list->tail->captures == maxcap)
		{
			countmax++;
		}
	}

	// Handle cases with multiple moves with the same maximum captures
	if (countmax >= 2)
	{
		MultipleSourceMovesListCell* multiplecurr = lst->head;

		// Find a the best move with maxcap
		while (multiplecurr)
		{
			if (multiplecurr->single_source_moves_list->tail->captures == maxcap && multiplecurr->single_source_moves_list->head != multiplecurr->single_source_moves_list->tail)
			{
				bestlist = multiplecurr->single_source_moves_list;
				break;
			}
			multiplecurr = multiplecurr->next;
		}
	}
	// Apply the selected best move to the board
	SingleSourceMovesListCell* currbest = bestlist->head;
	while (currbest->next)
	{
		bool iscapture = false;

		// Check if the current move is a capture move
		if (currbest->next->captures > currbest->captures)
			iscapture = true;
		changeBoard(board, iscapture, currbest->position, currbest->next->position);
		currbest = currbest->next;
	}

	// Print the selected move
	printf("\n\n%c%d -> %c%d\n\n", bestlist->head->position->row + 'A', bestlist->head->position->col, bestlist->tail->position->row + 'A', bestlist->tail->position->col);

	// Update the board state after the move
	if (player == 'B')
	{
		if (bestlist->tail->captures > board[0][0] - '0')
			board[0][0] = bestlist->tail->captures + '0';
		board[0][4]++;
	}

	if (player == 'T')
	{
		if (bestlist->tail->captures > board[0][2] - '0')
			board[0][2] = bestlist->tail->captures + '0';
		board[0][6]++;
	}

	// Free the memory allocated for the move list
	freeMultipleList(lst);
}

// Function to copy the contents of one board to another
void copyBoard(Board board, Board boardtocopy)
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			board[i][j] = boardtocopy[i][j];
		}
	}
}

// Function to update the board state based on a move
void changeBoard(Board board, bool capture, checkersPos* lastmove, checkersPos* nextmove)
{
	char temp;

	// Swap the positions of the last move and the next move on the board
	if (!capture)
	{
		temp = board[lastmove->row][lastmove->col];
		board[lastmove->row][lastmove->col] = board[nextmove->row][nextmove->col];
		board[nextmove->row][nextmove->col] = temp;
	}
	else
	{
		temp = board[lastmove->row][lastmove->col];
		board[lastmove->row][lastmove->col] = board[nextmove->row][nextmove->col];
		board[nextmove->row][nextmove->col] = temp;

		int row, col;

		// Find the captured piece's position and mark it as captured
		row = (lastmove->row + nextmove->row) / 2;
		col = (lastmove->col + nextmove->col) / 2;

		board[row][col] = '*';
	}
}
// Function to play the game
void PlayGame(Board board, Player starting_player)
{
	// Initialize move and capture counters
	board[0][6] = 0;  // count moves for T
	board[0][4] = 0; // count moves for B
	board[0][2] = 0; // saved max cap for T
	board[0][0] = 0; // saved max cap for B

	bool gameover = false;
	Player startplayer = starting_player, nextplayer, temp;
	if (startplayer == 'B')
		nextplayer = 'T';
	else
		nextplayer = 'B';
	printf("%c's turn\n", startplayer);
	printBoard(board);

	while (!gameover)
	{
		// Handle a player's turn
		Turn(board, startplayer);

		// Check if the opponent has any remaining pieces or if the current player has reached the opponent's end
		if (checkOpponent(board, startplayer) == false || playerInEnd(board, startplayer) == true)
			gameover = true;

		// Switch players
		temp = startplayer;
		startplayer = nextplayer;
		nextplayer = temp;

		printf("%c's turn\n", startplayer);
		printBoard(board);
	}

	char maxmoves;
	if (nextplayer == 'T')
		maxmoves = board[0][6];
	else
		maxmoves = board[0][4];

	printf("%c' wins!\n", nextplayer);
	printf("%c performed %d moves.\n", nextplayer, (int)maxmoves);

	if (board[0][0] > board[0][2])
		printf("B performed the highest number of captures in a single move- %d\n", board[0][0] - '0');
	else
		printf("T performed the highest number of captures in a single move- %d\n", board[0][2] - '0');
}


// Function to check if the opponent still has any pieces on the board
bool checkOpponent(Board board, Player player)
{
	int row, col;
	Player opponent;

	if (player == 'B')
		opponent = 'T';
	else
		opponent = 'B';

	for (row = 0; row < BOARD_SIZE; row++)
	{
		for (col = 0; col < BOARD_SIZE; col++)
		{
			if (board[row][col] == opponent)
				return true;
		}
	}

	return false;
}

// Function to check if the current player has reached the opponent's end
bool playerInEnd(Board board, Player player)
{
	int i;

	if (player == 'T')
	{
		for (i = 0; i < BOARD_SIZE; i++)
		{
			if (board[7][i] == 'T')
				return true;
		}
	}

	if (player == 'B')
	{
		for (i = 0; i < BOARD_SIZE; i++)
		{
			if (board[0][i] == 'B')
				return true;
		}
	}

	return false;
}

// Function to initialize the board with the starting configuration
void buildBoard(Board* board)
{
	int i, j;

	// Initializing the empty board
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			(*board)[i][j] = ' ';
		}
	}

	// Placing the pieces on the board
	(*board)[0][1] = 'T';
	(*board)[0][3] = 'T';
	(*board)[0][5] = 'T';
	(*board)[0][7] = 'T';
	(*board)[1][0] = 'T';
	(*board)[1][2] = 'T';
	(*board)[1][4] = 'T';
	(*board)[1][6] = 'T';
	(*board)[2][1] = 'T';
	(*board)[2][3] = 'T';
	(*board)[2][5] = 'T';
	(*board)[2][7] = 'T';
	(*board)[7][0] = 'B';
	(*board)[7][2] = 'B';
	(*board)[7][4] = 'B';
	(*board)[7][6] = 'B';
	(*board)[6][1] = 'B';
	(*board)[6][3] = 'B';
	(*board)[6][5] = 'B';
	(*board)[6][7] = 'B';
	(*board)[5][0] = 'B';
	(*board)[5][2] = 'B';
	(*board)[5][4] = 'B';
	(*board)[5][6] = 'B';
	(*board)[3][0] = '*';
	(*board)[3][2] = '*';
	(*board)[3][4] = '*'; //changed
	(*board)[3][6] = '*';
	(*board)[4][1] = '*';
	(*board)[4][3] = '*';
	(*board)[4][5] = '*';
	(*board)[4][7] = '*';


}

//funtion to print the board
void printBoard(Board board)
{
	int i, j;
	printf("\n");
	printf("  0 1 2 3 4 5 6 7\n");
	printf("-----------------\n");
	for (i = 0; i < 8; i++) {
		printf("%c|", i + 'A');
		for (j = 0; j < 8; j++)
		{
			if ((i == 0) && (j == 0 || j == 2 || j == 4 || j == 6))
			{
				printf(" |");
			}
			else if (board[i][j] == '*')
			{
				printf(" |");
			}
			else
			{
				printf("%c|", board[i][j]);

			}
		}
		printf("\n-----------------\n");
	}

}


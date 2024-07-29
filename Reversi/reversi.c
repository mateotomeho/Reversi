//
// Author: Mateo Tomeho
//

#include <stdio.h>
#include "reversi.h"


bool ValidMoves(char board [][26], int n, char colour, int row, int col);
void checkValidAndFlip(char board[][26], int n, int row, int col, char colour, bool *flip);
void computerMove(char board[][26], int n, char colour);
bool moveAvailable(char board[][26], int n, char colour);
void findWinner(char board[][26], int n, char computer);
int makeMove(const char board[][26], int n, char turn, int *row, int *col);

int minimax1branch(const char board[][26], int n, int depth, char colour, int* bestRow, int* bestCol);
int minimax(char board[][26], int n, int depth, char colour, bool maximizingPlayer, bool GameOver, int alpha, int beta);
int eval(char board[][26], int n, char computer);


int main(void) {
  //Declare the variables
  int n;
  int turnDraw = 4;
  bool flip = false;
  char turn, oppositeTurn, computer, human;
  char humanRow, humanCol;
  int computerRow, computerCol;
  bool gameOver = false;


  //Prompt user for the board dimension 
  printf("Enter the board dimension: ");
  scanf("%d", &n);
  char board [26][26];

  //Assigning all tiles to U
  for (int row = 0; row < n; row++){
    for (int col = 0; col < n; col++){
      board[row][col] = 'U';
    }
  }
  //Modify the tiles in the center for starting positions
  board[(n-1)/2][(n-1)/2] = 'W';
  board[(n/2)][(n/2)] = 'W';
  board[(n-1)/2][(n/2)] = 'B';
  board[(n/2)][(n-1)/2] = 'B';

  
  //Start the code gradually
  printf("Computer plays (B/W): ");
  scanf(" %c", &computer);

  printBoard(board, n);

  if (computer == 'W'){
    human = 'B';
  } else {
    human = 'W';
  }

  turn = 'B';
  
  while (gameOver == false){
    
    if (turnDraw < n*n){
      turnDraw++;
    } else {
      gameOver = true;
      findWinner(board, n, computer);
    }
    
    if (gameOver == false){
      if (turn == computer){
        makeMove(board, n, computer, &computerRow, &computerCol);
        //Flip the tiles and print the message
        checkValidAndFlip(board, n, computerRow, computerCol, computer, &flip);
        printf("Computer places %c at %c%c.\n", computer, computerRow + 'a', computerCol + 'a');
      } else { 
        printf("Enter move for colour %c (RowCol): ", human);
        scanf(" %c%c", &humanRow, &humanCol);

        if (positionInBounds(n, humanRow - 'a', humanCol - 'a') == true && board[humanRow - 'a'][humanCol - 'a'] == 'U' && ValidMoves(board, n, human, humanRow -'a', humanCol - 'a')){ //'U'
          checkValidAndFlip(board, n, humanRow -'a', humanCol - 'a', human, &flip);
        } else {
          printf("Invalid move.\n");
          printf("%c player wins.\n", computer);
          gameOver = true;
          
        }
      }
    }
    
    if (gameOver == false){
      printBoard(board, n);
    }

    if (turn == 'B'){
      oppositeTurn = 'W';
    }else {
      oppositeTurn = 'B';
    }
    
    if (gameOver == false){
      if (moveAvailable(board, n, oppositeTurn)){
      turn = oppositeTurn;
      } else if (moveAvailable(board, n, turn)){
        if (turn == computer){
          printf("%c player has no valid move.\n", human);
        } else {
          printf("%c player has no valid move.\n", computer);
        }
      } else {
        gameOver = true;
        findWinner(board, n, computer);
      }
    }
  }

  return 0;
}

////////Functions implementations: ///////////////////////////////////////////////////

void printBoard(char board[][26], int n) {
  //Declare variables
  char letter; 
  int count = 1;

  //Print the board alphabet
  printf("  ");
  for (letter = 'a'; letter <= 'z' && count <= n; letter ++){
    printf("%c", letter);
    count ++;
  }

  //Print the initial board + alphabet at the beginning of the row
  letter  = 'a';
  for (int row = 0; row < n; row++){
    printf("\n");
    printf("%c ", letter);
    letter++;
    for (int col = 0; col < n; col++){
      printf("%c", board[row][col]);
    }
  }
  printf("\n");
}


/////////////////////////////////////////////////////////////////////////
bool positionInBounds(int n, int row, int col){
  return (row >= 0 && row < n && col >= 0 && col < n);
}



/////////////////////////////////////////////////////////////////////////
bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol){
  char oppositeCol;

  if (colour == 'W'){
    oppositeCol = 'B';
  } else {
    oppositeCol = 'W';
  }

  if (positionInBounds(n, row+deltaRow, col+deltaCol) != true || board[row+deltaRow][col+deltaCol] != oppositeCol){
      return false;
    }

    if ((positionInBounds(n, row+deltaRow, col+deltaCol) == true) && (board[row+deltaRow][col+deltaCol] == oppositeCol)){
      int i = 1;
      while (positionInBounds(n, row + i*deltaRow, col + i*deltaCol) != false && (board[row + i*deltaRow][col + i*deltaCol]) != 'U' && (board[row + i*deltaRow][col + i*deltaCol]) != colour){
          i++;
      }
      if (positionInBounds(n, row + i*deltaRow, col + i*deltaCol) == false || (board[row + i*deltaRow][col + i*deltaCol]) == 'U'){
        return false;
      } else if ((board[row + i*deltaRow][col + i*deltaCol]) == colour){
        return true;   
      }
    }
return false;
}     
        
/////////////////////////////////////////////////////////////////////////
bool ValidMoves(char board [][26], int n, char colour, int row, int col){
  int deltaRow = 0;
  int deltaCol = 0;
  for (int dir = 0; dir < 8; dir++){                    
    if (dir == 0){ //South-East
      deltaRow = 1;
      deltaCol = 1;
    }else if (dir == 1){ //South
      deltaRow = 1;
      deltaCol = 0;
    }else if (dir == 2){ //South-West
      deltaRow = 1;
      deltaCol = -1;
    }else if (dir == 3){ //West
      deltaRow = 0;
      deltaCol = -1;
    }else if (dir == 4){ //North-West
      deltaRow = -1;
      deltaCol = -1;
    }else if (dir == 5){ //North
      deltaRow = -1;
      deltaCol = 0;
    }else if (dir == 6){ //North-East
      deltaRow = -1;
      deltaCol = 1;
    }else if (dir == 7){ //East
      deltaRow = 0;
      deltaCol = 1;
    }
    if (board[row][col] == 'U'){
      if (checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)) {
        return true;
        //printf("%c%c\n", row + 'a', col + 'a');
        row=n;
        col=n;
        dir=8;
        //Write statement to avoid to print twice if the position is available in two different places
      }
    }else {
      return false;
    }
  }
  return false;
}


/////////////////////////////////////////////////////////////////////////
bool moveAvailable(char board[][26], int n, char colour){
  //Check every position on the board and see if this specific position is a valid moves
  int row, col;
  bool Availability = false;

  for (row = 0 ; row < n; row++){
    for (col = 0; col < n; col++){
      if (ValidMoves(board, n, colour, row, col)){
        Availability = true;
      } 
    }
  }
  return Availability;
}


 ///////////////////////////////////////////////////////////////////////// 
 void checkValidAndFlip(char board[][26], int n, int row, int col, char colour, bool *flip){
  int oppositeCol;

  if (colour == 'W'){
    oppositeCol = 'B';
  } else {
    oppositeCol = 'W';
  }
  
  int deltaRow = 0;
  int deltaCol = 0;
  for (int dir = 0; dir < 8; dir++){                    
    if (dir == 0){ //South-East
      deltaRow = 1;
      deltaCol = 1;
    }else if (dir == 1){ //South
      deltaRow = 1;
      deltaCol = 0;
    }else if (dir == 2){ //South-West
      deltaRow = 1;
      deltaCol = -1;
    }else if (dir == 3){ //West
      deltaRow = 0;
      deltaCol = -1;
    }else if (dir == 4){ //North-West
      deltaRow = -1;
      deltaCol = -1;
    }else if (dir == 5){ //North
      deltaRow = -1;
      deltaCol = 0;
    }else if (dir == 6){ //North-East
      deltaRow = -1;
      deltaCol = 1;
    }else if (dir == 7){ //East
      deltaRow = 0;
      deltaCol = 1;
    }

    if (checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)){
      *flip = true;     
      for (int i = 1; board[row + i*deltaRow][col + i*deltaCol] != colour; i++){
        if (board[row + i*deltaRow][col + i*deltaCol] == oppositeCol){
          board[row + i*deltaRow][col + i*deltaCol] = colour;
        }
      } 
    board[row][col] = colour;
    }
  }
}

/////////////////////////////////////////////////////////////////////////
void computerMove(char board[][26], int n, char colour){
  //Declaring variables
  int score = 0, bestScore = 0;
  int bestRow = 0, bestCol = 0;
  int deltaRow = 0;
  int deltaCol = 0;
  bool flip = false; 

  //Going into the board to calculate the score
  for (int row = 0; row < n; row++){
    for (int col = 0; col < n; col++) {
      score = 0;
      if (board[row][col] == 'U' && ValidMoves(board, n, colour, row, col)){
        for (int dir = 0; dir < 8; dir++){                    
          if (dir == 0){ //South-East
            deltaRow = 1;
            deltaCol = 1;
          }else if (dir == 1){ //South
            deltaRow = 1;
            deltaCol = 0;
          }else if (dir == 2){ //South-West
            deltaRow = 1;
            deltaCol = -1;
          }else if (dir == 3){ //West
            deltaRow = 0;
            deltaCol = -1;
          }else if (dir == 4){ //North-West
            deltaRow = -1;
            deltaCol = -1;
          }else if (dir == 5){ //North
            deltaRow = -1;
            deltaCol = 0;
          }else if (dir == 6){ //North-East
            deltaRow = -1;
            deltaCol = 1;
          }else if (dir == 7){ //East
            deltaRow = 0;
            deltaCol = 1;
          }

          if (checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)){
            for (int i = 1; board[row + i*deltaRow][col + i*deltaCol] != colour; i++){
              score ++;
            }
          }
          if (score > bestScore){
          bestScore = score;
          bestRow = row;
          bestCol = col;
          }
        }
      }
    }
  }
  //Flip the tiles and print the message
  checkValidAndFlip(board, n, bestRow, bestCol, colour, &flip);
  printf("Computer places %c at %c%c.\n", colour, bestRow + 'a', bestCol + 'a');
}

/////////////////////////////////////////////////////////////////////////
void findWinner(char board[][26], int n, char computer){
  int humanScore = 0, computerScore = 0;
  char human;

  if (computer == 'W'){
    human = 'B';
  } else {
    human = 'W';
  }

  //Going into the board to look for each tiles and the matching colour and giving points
  for (int row = 0; row < n; row++){
    for (int col = 0; col < n; col++){
      if (board[row][col]== computer){
        computerScore++;
      } else if (board[row][col]== human){
        humanScore++;
      }
    }
  }
  //Determine the winner base of the points for each colour.
  if (computerScore > humanScore){
    printf("%c player wins.\n", computer);
    return;
  } else if (humanScore > computerScore){
    printf("%c player wins.\n", human);
    return;
  } else {
    printf("Draw!\n");
    return;
  }
}
/////////////////////////////////
int makeMove(const char board[][26], int n, char turn, int *row, int *col){
  int bestRow = 0, bestCol = 0;
  
  // Call the minimax1branch function to find the best move
  minimax1branch(board, n, 4, turn, &bestRow, &bestCol);

  //Assign the value of the row and col to what the minimax fucntion found;
  *row = bestRow;
  *col = bestCol;
  return 0;
}
////////////////////////////////////////
int minimax1branch(const char board[][26], int n, int depth, char colour, int* bestRow, int* bestCol){
  int bestEval = -1000;
  int alpha = -10000, beta = 10000;
  bool flip = false;

  if (moveAvailable(board, n, colour) == false){
    return 0;
  }

  // Iterate through each valid move
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (ValidMoves(board, n, colour, row, col)) {
        char newBoard[26][26]; //Create a copy of the board
        for (int i = 0; i < n; i++) {
          for (int j = 0; j < n; j++) {
            newBoard[i][j] = board[i][j];
          }
        }

        //Simulate the move and flip
        checkValidAndFlip(newBoard, n, row, col, colour, &flip);


        // Recursively call minimax with the new board state
        int temp = minimax(newBoard, n, depth - 1, colour, false, false, alpha, beta); 
        
        if (temp > bestEval){
          *bestRow = row;
          *bestCol = col;
          bestEval = temp;
        }
      }
    }
  }
return bestEval;
}

////////////////////////////////////////
int minimax(char board[][26], int n, int depth, char colour, bool maximizingPlayer, bool GameOver, int alpha, int beta){
  bool isAvailable;
  char oppositeCol;
  bool flip = false;

  if (colour == 'W'){
    oppositeCol = 'B';
  } else {
    oppositeCol = 'W';
  }

  if (depth == 0){
    return eval(board, n, colour);
  }

  if (maximizingPlayer){
    isAvailable = moveAvailable(board, n, colour);
    if (isAvailable == false && GameOver == false){ //Switch to minimize and Game is over
      return minimax(board, n, depth, colour, false, true, alpha, beta);
    }
    if (isAvailable == false && GameOver == true){ //No more turn the game is over
      return eval(board, n, colour);
    }

    int maxEval = -2000;

    // Iterate through each valid move
    for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++) {
        if (ValidMoves(board, n, colour, row, col)) {
          char newBoard[26][26]; //Create a copy of the board
          for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
              newBoard[i][j] = board[i][j];
            }
          }

          //Simulate the move and flip
          checkValidAndFlip(newBoard, n, row, col, colour, &flip);
          // Recursively call minimax with the new board state
          int temp = minimax(newBoard, n, depth - 1, colour, false, false, alpha, beta); //Switch to minimize and Game is NOT over
           // Find the maximum evaluation value
            if (temp > maxEval){
              maxEval = temp;
            }
            // Find the updated alpha for pruning
            if (maxEval < alpha){
              alpha = maxEval;
            }
          // Prune the branch if necessary
          if (beta <= alpha){
            break;
          }
        }
      }
    }
    return maxEval;

  } else { //FOR THE MINIMIZE

      isAvailable = moveAvailable(board, n, oppositeCol);
      if (isAvailable == false && GameOver == false){ //Switch to maximize and Game is over
        return minimax(board, n, depth, colour, true, true, alpha, beta);
      }
      if (isAvailable == false && GameOver == true){ //No more turn the game is over
        return eval(board, n, colour);
      }

      int minEval = 2000;

      // Iterate through each valid move
      for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
          if (ValidMoves(board, n, oppositeCol, row, col)) {
            char newBoard[26][26]; //Create a copy of the board
            for (int i = 0; i < n; i++) {
              for (int j = 0; j < n; j++) {
                newBoard[i][j] = board[i][j];
              }
            }

            //Simulate the move and flip
            checkValidAndFlip(newBoard, n, row, col, oppositeCol, &flip);
            // Recursively call minimax with the new board state
            int temp = minimax(newBoard, n, depth - 1, colour, true, false, alpha, beta); //Switch to maximize and Game is NOT over
            // Find the maximum evaluation value
            if (temp < minEval){
              minEval = temp;
            }
            // Find the updated alpha for pruning
            if (minEval > beta){
              beta = minEval;
            }
            // Prune the branch if necessary
            if (beta <= alpha){
              break;
            }
          }
        }
      }

      return minEval;
  }
}



////////////////////////////////////////////////////
int eval(char board[][26], int n, char computer){
  int score = 0;
  int weight = 0;

  for (int row = 0; row < n; row++){
    for (int col = 0; col < n; col++){
      // Check coordinates and assign weights accordingly
      if ((row == 0 && (col == 0 || col == 7)) ||  // (0,0) or (0,7)
          (row == 7 && (col == 0 || col == 7))) {  // (7,0) or (7,7)
          weight = 100;
      } else if ((row == 0 && (col == 1 || col == 6)) ||  // (0,1) or (0,6)
                  (row == 1 && (col == 0 || col == 7)) ||  // (1,0) or (1,7)
                  (row == 6 && (col == 0 || col == 7)) ||  // (6,0) or (6,7)
                  (row == 7 && (col == 1 || col == 6))) {  // (7,1) or (7,6)
          weight = -8;
      } else if ((row == 1 && (col == 1 || col == 6)) ||   // (1,1) or (1,6)
                  (row == 6 && (col == 1 || col == 6))) {   // (6,1) or (6,6)
          weight = -24;
      } else if ((row == 0 && (col == 2 || col == 5)) ||   // (0,2) or (0,5)
                  (row == 2 && (col == 0 || col == 7)) ||   // (2,0) or (2,7)
                  (row == 5 && (col == 0 || col == 7)) ||   // (5,0) or (5,7)
                  (row == 7 && (col == 2 || col == 5))) {   // (7,2) or (7,5)
          weight = 15;
      } else if ((row == 0 && (col == 3 || col == 4))||    // (0,3) or (0,4)
                  (row == 7 && (col == 3 || col == 4)) ||   // (7,3) or (7,4)
                  (row == 3 && (col == 0 || col == 7)) ||   // (3,0) or (3,7)
                  (row == 4 && (col == 0 || col == 7))) {   // (4,0) or (4,7)
          weight = 6;
      } else if ((row == 1 && (col == 2 || col == 5))||    // (1,2) or (1,5)
                (row == 2 && (col == 1 || col == 6)) ||   // (2,1) or (2,6)
                (row == 5 && (col == 1 || col == 6)) ||   // (5,1) or (5,6)
                (row == 6 && (col == 2 || col == 5))) {   // (6,2) or (6,5)
        weight = -4;
      } else if ((row == 1 && (col == 3 || col == 4))||    // (1,3) or (1,4)
                (row == 3 && (col == 1 || col == 6)) ||   // (3,1) or (3,6)
                (row == 4 && (col == 1 || col == 6)) ||   // (4,1) or (4,6)
                (row == 6 && (col == 3 || col == 4))) {   // (6,3) or (6,4)
        weight = -3;
      } else if ((row == 2 && (col == 3 || col == 4))||    // (2,3) or (2,4)
                (row == 3 && (col == 2 || col == 5)) ||   // (3,2) or (3,5)
                (row == 4 && (col == 2 || col == 5)) ||   // (4,2) or (4,5)
                (row == 5 && (col == 3 || col == 4))) {   // (5,3) or (5,4)
        weight = 4;
      } else if ((row == 2 && (col == 2 || col == 5)) ||   // (2,2) or (2,5)
                  (row == 5 && (col == 2 || col == 5))) {   // (5,2) or (5,5)
          weight = 7;
      } else{
          weight = 1; 
      }

      if (board[row][col] == computer){
        score = score+weight;
      } //else if (board[row][col] != computer && board[row][col] != 'U'){
        //score = score - weight;
      //}
    }
  }
  return score;
}

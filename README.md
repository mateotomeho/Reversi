# Reversi Game

## Introduction

Welcome to the Reversi game! This project is a terminal-based implementation of the classic board game Reversi. The game allows you to play against a computer AI with advanced algorithms. Below you'll find instructions on how to start a game and interesting facts about the computer AI.

## Game Rules

1. **Setup**: The game is played on an 8x8 checkered board. Initially, the board has 4 tiles placed in the center in an alternating pattern:
   - Two black tiles and two white tiles form a square in the center.
   
2. **Valid Moves**:
   - A valid move involves placing a tile such that in any of the 8 possible directions (N, NE, E, SE, S, SW, W, NW), there are 1 or more opposite-colored tiles followed by one of your own tiles.
   - After placing a tile, all the opposite-colored tiles along the valid paths are flipped to your color.

3. **Turn Rules**:
   - If one player has no moves, the other player continues to play until the original player can make a move.
   - The game ends when the board is completely filled or both players have no moves.

4. **Winning**:
   - The winner is the player with the most tiles on the board at the end of the game.

## How to Play

1. **Download and Compile**:
   - Download the game folder from the repository.
   - Navigate to the `Reversi` folder, then compile and run the game.

2. **Game Setup**:
   - Choose the board size (default is 8x8).
   - Select the color for the computer (B/W).

3. **Playing the Game**:
   - Follow the on-screen prompts to make your moves.
   - The computer will automatically make its moves using an advanced AI.

## Interesting Facts About the Computer AI

- **AI Algorithms**:
  - The computer AI initially used the Minimax algorithm but now incorporates Alpha-Beta Pruning for improved performance.
  - The AI uses heuristics with a weighted board with corner positions weighted significantly due to their strategic value.

- **Depth of Search**:
  - The AI evaluates up to 4 moves ahead to determine the best move. You can adjust the search depth by modifying the `depth` variable set to 4 at the line 399 in the code.

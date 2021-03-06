/* ******************************************************
 * CS368 FINAL PROJECT: CLI_Checkers
 * Main File: checkers.cpp
 * Names: Matt Thomas, Aleks Lesiewicz, and Parker Breene
 *******************************************************/

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//Designates current state of the game
class Board {
public:
    
    string black = "\033[34;m⬓\033[0m";
    string red = "\033[31;m⬕\033[0m";
    string blackKing = "\033[34;m⬒\033[0m";
    string redKing = "\033[31;m⬔\033[0m";
    string space = "⧠";
    string board[8][8];
    string winner = "";
    int noActionTurns = 0;
    
    //Verifies that the piece to move belongs to the player at turn
    bool validPiece(int x, int y, bool playerOne) {
        if(playerOne) {
            if(board[invert(y)][x] != black && (board[invert(y)][x] != blackKing)) {
                return false;
            }
        }
        else {
            if((board[invert(y)][x] != red) && (board[invert(y)][x] != redKing)) {
                return false;
            }
        }
        return true;
    }

    //Verifies that the user-requested move is valid
    bool checkValid(int thisX, int thisY, int nextX, int nextY) {
        //Y-coordinates corrected for top-down row access of array
        int vThisY = invert(thisY);
        int vNextY = invert(nextY);
        //Identifies the current player / piece type and the opponent's
        string player = board[vThisY][thisX];
        string other = "";
        string otherK = "";
        if((player == black) || (player == blackKing)) {
            other = red;
            otherK = redKing;
        }
        else {
            other = black;
            otherK = blackKing;
        }
        //Checks if move is within the bounds of the board
        if(thisX < 0 || thisX > 7 || thisY < 0 || thisY > 7) {
            return false;
        }
        //Checks if move tile is unoccupied
        if(board[vNextY][nextX] != space) {
            return false;
        }
        //Varaibles to designate deviation from current position
        int absX = abs(thisX - nextX);
        int absY = abs(thisY - nextY);
        //Checks if move tile is diagonally located w/ respect to current position
        if((absX != absY) || absX == 0) {
            return false;
        }
        //Checks if move is valid in direction allotted by piece type
        if((player == black) && (vThisY < vNextY)) {
            return false;
        }
        if((player == red) && (vThisY > vNextY)) {
            return false;
        }
        //Permits single-tile movement
        if(absX == 1) {
            return true;
        }
        //Checks if move distance is a multiple of two tiles
        if(absX % 2 != 0) {
            return false;
        }
        //Variable to track increment span to move via jump(s)
        int inc = 2;
        //Four loops currated for 1 of 4 directions; checks if jump space(s) are open / there exist opponent's piece(s) to be jumped
        if(nextX < thisX && nextY < thisY) {
            while(inc <= absX) {
                if((!((board[vNextY + inc - 1][nextX - inc + 1]) == other) && !((board[vNextY + inc - 1][nextX - inc + 1]) == otherK)) && (board[vNextY + inc][nextX - inc] == space)) {
                    return false;
                }
                inc += 2;
            }
        }
        else if(nextX > thisX && nextY < thisY) {
            while(inc <= absX) {
                if((!((board[vNextY + inc - 1][nextX + inc - 1]) == other) && !((board[vNextY + inc - 1][nextX + inc - 1]) == otherK)) && (board[vNextY + inc][nextX + inc] == space)) {
                    return false;
                }
                inc += 2;
            }
        }
        else if(nextX < thisX && nextY > thisY) {
            while(inc <= absX) {
                if((!((board[vNextY - inc + 1][nextX - inc + 1]) == other) && !((board[vNextY - inc + 1][nextX - inc + 1]) == otherK)) && (board[vNextY - inc][nextX - inc] == space)) {
                    return false;
                }
                inc += 2;
            }
        }
        else {
            while(inc <= absX) {
                if((!((board[vNextY - inc + 1][nextX + inc - 1]) == other) && !((board[vNextY - inc + 1][nextX + inc - 1]) == otherK)) && (board[vNextY - inc][nextX + inc] == space)) {
                    return false;
                }
                inc += 2;
            }
        }
        return true;
    }
    
    //Updates char contents of array following checkValid returning true for given move
    void move(int thisX, int thisY, int nextX, int nextY) {
        //Identifies the piece type at turn; updates to king if necessary
        string player = board[invert(thisY)][thisX];
        if((player == black) && (nextY == 7)) {
            player = blackKing;
	    noActionTurns = 0;
        }
        else if((player == red) && (nextY == 0)) {
            player = redKing;
	    noActionTurns = 0;
        }

        //Clears piece's initial tile
        board[invert(thisY)][thisX] = space;
        //Varaible to designate deviation from current position
        int absX = abs(thisX - nextX);
        //Updates both tiles for single-unit move
        if(absX == 1) {
            board[invert(nextY)][nextX] = player;
	    noActionTurns++;
            return;
        }
	noActionTurns = 0;
        //Y-coordinates corrected for top-down row access of array
        int vThisY = invert(thisY);
        int vNextY = invert(nextY);
        //Variable to track increment span to move via jump(s)
        int inc = 2;

        //Four loops currated for 1 of 4 directions; updates symbols long movement's path
        if(nextX < thisX && nextY < thisY) {
            while(inc <= absX) {
                board[vThisY + inc - 1][thisX - inc + 1] = space;
                if(inc == absX) {
                    board[vThisY + inc][thisX - inc] = player;
                    return;
                }
                else {
                    board[vThisY + inc][thisX - inc] = space;
                }
                inc += 2;
            }
        }
        else if(nextX > thisX && nextY < thisY) {
            while(inc <= absX) {
                board[vThisY + inc - 1][thisX + inc - 1] = space;
                if(inc == absX) {
                    board[vThisY + inc][thisX + inc] = player;
                    return;
                }
                else {
                    board[vThisY + inc][thisX + inc] = space;
                }
                inc += 2;
            }
        }
        else if(nextX < thisX && nextY > thisY) {
            while(inc <= absX) {
                board[vThisY - inc + 1][thisX - inc + 1] = space;
                if(inc == absX) {
                    board[vThisY - inc][thisX - inc] = player;
                    return;
                }
                else {
                    board[vThisY - inc][thisX - inc] = space;
                }
                inc += 2;
            }
        }
        else {
            while(inc <= absX) {
                board[vThisY - inc + 1][thisX + inc - 1] = space;
                if(inc == absX) {
                    board[vThisY - inc][thisX + inc] = player;
                    return;
                }
                else {
                    board[vThisY - inc][thisX + inc] = space;
                }
                inc += 2;
            }
        }
        return;
    }
    
    //Checks both players' piece count for 0, indicating end-game state
    bool checkEnd() {
        bool b = false;
        bool r = false;
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                if(board[i][j] == black) {
                    b = true;
                }
                else if(board[i][j] == red) {
                    r = true;
                }
                if(b && r) {
                    return false;
                }
            }
        }
        if(!b) {
            winner = "Red";
        }
        else {
            winner = "Black";
        }
        return true;
    }
    
    //Access method for winner global
    string getWinner() {
        return winner;
    }
    
    //Array traversed to check for tie-game state
    bool checkTie() {
        if(noActionTurns == 40) {
            return true;
        }
        else {
            return false;
        }
    }
    
    //Populates the 2D array with the starting piece state configuration
    void newBoard() {
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                //If i s even
                if(i == 3 || i == 4) {
                    board[i][j] = space;
                }
                if(i % 2 != 1) {
                    if(j % 2 == 1) {
                        if(i < 3) {
                            board[i][j] = red;
                        }
                        else if(i > 4) {
                            board[i][j] = black;
                        }
                    }
                    else {
                        board[i][j] = space;
                    }
                }
                //If i is odd
                else {
                    if(j % 2 != 1) {
                        if(i < 3) {
                            board[i][j] = red;
                        }
                        else if(i > 4) {
                            board[i][j] = black;
                        }
                    }
                    else {
                        board[i][j] = space;
                    }
                }
            }
        }
    }
    
    //Prints current state of the game board
    string updateBoard() {
        string tiles = "";
        for(int i = 0; i < 8; i++) {
            if(i == 0) {
                tiles += "  1 2 3 4 5 6 7 8";
                tiles += "\n";
            }
            tiles += to_string(8-i);
            tiles += " ";
            	for(int j = 0; j < 8; j++) {
                    tiles += board[i][j];
                    tiles += " ";
            	}
            	tiles += "\n";
        	}
        	return "\n" + tiles + "\n";
    	}
   
    //Inverts a y-coordinate for use in operations on the array
    int invert(int n) {
        return (7-n);
    }
    
    //AI acting as red player; sequential executes first valid move found
    void AI() {
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                for(int k = 0; k < 8; k++) {
                    if(validPiece(i, j, false)) {
                    //Check for the first valid 1 or 2 space move for the first red or red king piece
                    if(k == 0) {
                        if(checkValid(i, j, i+1, j+1)) {
                            move(i, j, i+1, j+1);
                            return;
                        }
                    }
                    if(k == 1) {
                        if(checkValid(i, j, i+2, j+2)) {
                            move(i, j, i+2, j+2);
                            return;
                        }
                    }
                    if(k == 2) {
                        if(checkValid(i, j, i-1, j+1)) {
                            move(i, j, i-1, j+1);
                            return;
                        }
                    }
                    if(k == 3) {
                        if(checkValid(i, j, i-2, j+2)) {
                            move(i, j, i-2, j+2);
                            return;
                        }
                    }
                    if(k == 4) {
                        if(checkValid(i, j, i+1, j-1)) {
                            move(i, j, i+1, j-1);
                            return;
                        }
                    }
                    if(k == 5) {
                        if(checkValid(i, j, i+2, j-2)) {
                            move(i, j, i+2, j-2);
                            return;
                        }
                    }
                    if(k == 6) {
                        if(checkValid(i, j, i-1, j-1)) {
                            move(i, j, i-1, j-1);
                            return;
                        }
                    }
                    if(k == 7) {
                        if(checkValid(i, j, i-2, j-2)) {
                            move(i, j, i-2, j-2);
                            return;
                        }
                    }
                    }
                }
            }
        }
    }
    
};

//Handles singleplayer / multiplayer gameplay loops
int main(int argc, char**argv)
{
    Board game;
    bool multi = false;
    bool select = false;
    string input = "";
    bool playerOne = true;
    printf("\nCheckers\n=========================\nEnter 1 for 1P or 2 for 2P: ");
    while(!select) {
        cin >> input;
        if(input == "1") {
            select = true;
        }
        else if(input == "2") {
            multi = true;
            select = true;
        }
        if(!select) {
            printf("\nInvalid input. Enter 1 for 1P or 2 for 2P: ");
        }
    }
    if(!multi) {
        printf("\nCheckers for one: Player 1 - \033[34;m⬓\033[0m/\033[34;m⬒\033[0m  CPU - \033[31;m⬕\033[0m/\033[31;m⬔\033[0m\n");
    }
    else {
        printf("\nCheckers for two: Player 1 - ⬓/⬒  Player 2 - ⬕/⬔\n");
    }
    int thisX = 0;
    int thisY = 0;
    int nextX = 0;
    int nextY = 0;
    input = "";
    game.newBoard();
    if(multi) {
    //Overarching game loop for 2 players
    while(!game.checkEnd() && !game.checkTie()) {
        cout << "\033[2J\033[1;1H";
        cout << game.updateBoard();
        while(!game.validPiece(thisX - 1, thisY - 1, playerOne)) {
            while((input != "1") && (input != "2") && (input != "3") && (input != "4") && (input != "5") && (input != "6") && (input != "7") && (input != "8")) {
                if(multi && !playerOne) {
                    printf("P2: Enter the column # of piece to move: ");
                }
                else {
                    printf("P1: Enter the column # of piece to move: ");
                }
                
                cin >> input;
                printf("\n");
            }
            thisX = stoi(input);
            input = "";
            while((input != "1") && (input != "2") && (input != "3") && (input != "4") && (input != "5") && (input != "6") && (input != "7") && (input != "8")) {
                if(multi && !playerOne) {
                    printf("P2: Enter the row # of piece to move: ");
                }
                else {
                    printf("P1: Enter the row # of piece to move: ");
                }
                cin >> input;
                printf("\n");
            }
            thisY = stoi(input);
            input = "";
            if(!game.validPiece(thisX - 1, thisY - 1, playerOne)) {
                printf("That tile is not occupied by one of your pieces. ");
            }
        }

        input = "";
        while(!game.checkValid(thisX - 1, thisY - 1, nextX - 1, nextY - 1)) {
            while((input != "1") && (input != "2") && (input != "3") && (input != "4") && (input != "5") && (input != "6") && (input != "7") && (input != "8")) {
                if(multi && !playerOne) {
                    printf("P2: Enter the column # of space to move to: ");
                }
                else {
                    printf("P1: Enter the column # of space to move to: ");
                }
                cin >> input;
                printf("\n");
            }
            nextX = stoi(input);
            input = "";
            while((input != "1") && (input != "2") && (input != "3") && (input != "4") && (input != "5") && (input != "6") && (input != "7") && (input != "8")) {
                if(multi && !playerOne) {
                    printf("P2: Enter the row # of space to move to: ");
                }
                else {
                    printf("P1: Enter the row # of space to move to: ");
                }
                cin >> input;
                printf("\n");
            }
            nextY = stoi(input);
            if(!game.checkValid(thisX - 1, thisY - 1, nextX - 1, nextY - 1)) {
                printf("That is not a valid move. ");
            }
        }
        game.move(thisX - 1, thisY - 1, nextX - 1, nextY - 1);
        if(playerOne) {
            playerOne = false;
        }
        else {
            playerOne = true;
        }
        thisX = 0;
        thisY = 0;
        nextX = 0;
        nextY = 0;
        input = "";
    }
    }
    
    //Overarching game loop for a single player against the AI
    else {
        
        while(!game.checkEnd() && !game.checkTie()) {
            cout << "\033[2J\033[1;1H";
            cout << game.updateBoard();
            if(!playerOne) {
                printf("AI:\n");
                game.AI();
                playerOne = true;
                continue;
            }
            while(!game.validPiece(thisX - 1, thisY - 1, playerOne)) {
                while((input != "1") && (input != "2") && (input != "3") && (input != "4") && (input != "5") && (input != "6") && (input != "7") && (input != "8")) {

                        printf("P1: Enter the column # of piece to move: ");
                    
                    
                    cin >> input;
                    printf("\n");
                }
                thisX = stoi(input);
                input = "";
                while((input != "1") && (input != "2") && (input != "3") && (input != "4") && (input != "5") && (input != "6") && (input != "7") && (input != "8")) {


                    printf("P1: Enter the row # of piece to move: ");

                    cin >> input;
                    printf("\n");
                }
                thisY = stoi(input);
                input = "";
                if(!game.validPiece(thisX - 1, thisY - 1, playerOne)) {
                    printf("That tile is not occupied by one of your pieces. ");
                }
            }
            
            input = "";
            while(!game.checkValid(thisX - 1, thisY - 1, nextX - 1, nextY - 1)) {
                while((input != "1") && (input != "2") && (input != "3") && (input != "4") && (input != "5") && (input != "6") && (input != "7") && (input != "8")) {
 

                    printf("P1: Enter the column # of space to move to: ");
                    
                    cin >> input;
                    printf("\n");
                }
                nextX = stoi(input);
                input = "";
                while((input != "1") && (input != "2") && (input != "3") && (input != "4") && (input != "5") && (input != "6") && (input != "7") && (input != "8")) {

                    printf("P1: Enter the row # of space to move to: ");
                    
                    cin >> input;
                    printf("\n");
                }
                nextY = stoi(input);
                if(!game.checkValid(thisX - 1, thisY - 1, nextX - 1, nextY - 1)) {
                    printf("That is not a valid move. ");
                }
            }
            game.move(thisX - 1, thisY - 1, nextX - 1, nextY - 1);
            if(playerOne) {
                playerOne = false;
            }
            else {
                playerOne = true;
            }
            thisX = 0;
            thisY = 0;
            nextX = 0;
            nextY = 0;
            input = "";
        }
        
    }
    //Final conditionals to determine end game state / winner
    if(game.checkEnd()) {
        printf("\n" , game.getWinner().c_str(), "wins!");
    }
    else {
        printf("\nTie game!");
    }
    
        
    return 0;
}

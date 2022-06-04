/*
 * tic_tac_toe.c
 *
 *  Created on: Oct 5, 2020
 *      Author: Ragotham Senthilkumar
 */

#include "tic_tac_toe.h"
#define VOLT_0P6 745
#define VOLT_2P70 3351

//User(X = 1 or O = 2 )
int board[3][3] = {{0,0,0}, {0,0,0}, {0,0,0}};

//Positioning X,Y
int x_pos[3] = {LEFT_COL, CENTER_COL, RIGHT_COL};
int y_pos[3] = {LOWER_ROW, CENTER_ROW, UPPER_ROW};

//Check which player won
bool X_player = false;
bool O_player = false;

//Positioning and index
bool up = false, down = false, left = false, right = false;
int x = 1;
int y = 1;

//Checks the joy stick if in the center.
bool center = true;

//Check Player turn
int player = 1;

//Win variable
bool win = false;

//Toggle
bool toggle = 1;

/*******************************************************************************
* Function Name: tic_tac_toe_hw_init
********************************************************************************
* Summary: Initializes all the hardware resources required for tic tac toe game
*          (IO Pins, ADC14, Timer32_1, Timer32_2).
* Returns:
*  Nothing
*******************************************************************************/
void tic_tac_toe_hw_init(void)
{
    timer32_1_interupt();
    timer32_2_interupt();
    ece353_ADC14_Config();
}

/*******************************************************************************
* Function Name: tic_tac_toe_example_board
********************************************************************************
* Summary: Prints out an example of what the tic-tac-toe board looks like
* Returns:
*  Nothing
*******************************************************************************/
void tic_tac_toe_example_board(void)
{
    // Horizontal Lines
    lcd_draw_rectangle(SCREEN_CENTER_COL, UPPER_HORIZONTAL_LINE_Y, LINE_LENGTH,LINE_WIDTH,LCD_COLOR_BLUE);
    lcd_draw_rectangle(SCREEN_CENTER_COL,LOWER_HORIZONTAL_LINE_Y, LINE_LENGTH,LINE_WIDTH,LCD_COLOR_BLUE);

    //Vertical Lines
    lcd_draw_rectangle(LEFT_HORIZONTAL_LINE_X,SCREEN_CENTER_ROW, LINE_WIDTH, LINE_LENGTH,LCD_COLOR_BLUE);
    lcd_draw_rectangle(RIGHT_HORIZONTAL_LINE_X,SCREEN_CENTER_ROW, LINE_WIDTH,LINE_LENGTH,LCD_COLOR_BLUE);

    // Top Row
    lcd_draw_image(LEFT_COL,UPPER_ROW,O_WIDTH,O_HEIGHT,Bitmaps_O,LCD_COLOR_RED,LCD_COLOR_BLACK);
    lcd_draw_image(CENTER_COL,UPPER_ROW,X_WIDTH,X_HEIGHT,Bitmaps_X,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
    lcd_draw_image(RIGHT_COL,UPPER_ROW,O_WIDTH,O_HEIGHT,Bitmaps_O,LCD_COLOR_GREEN,LCD_COLOR_BLACK);

    // Center Row
    lcd_draw_image(LEFT_COL,CENTER_ROW,O_WIDTH,O_HEIGHT,Bitmaps_X,LCD_COLOR_BLUE,LCD_COLOR_BLACK);
    lcd_draw_image(CENTER_COL,CENTER_ROW,X_WIDTH,X_HEIGHT,Bitmaps_O,LCD_COLOR_ORANGE,LCD_COLOR_BLACK);
    lcd_draw_image(RIGHT_COL,CENTER_ROW,O_WIDTH,O_HEIGHT,Bitmaps_X,LCD_COLOR_CYAN,LCD_COLOR_BLACK);

    // Lower Row
    lcd_draw_image(LEFT_COL,LOWER_ROW,O_WIDTH,O_HEIGHT,Bitmaps_O,LCD_COLOR_MAGENTA,LCD_COLOR_BLACK);
    lcd_draw_image(CENTER_COL,LOWER_ROW,X_WIDTH,X_HEIGHT,Bitmaps_X,LCD_COLOR_GRAY,LCD_COLOR_BLACK);
    lcd_draw_image(RIGHT_COL,LOWER_ROW,O_WIDTH,O_HEIGHT,Bitmaps_O,LCD_COLOR_BROWN,LCD_COLOR_BLACK);
}

/*******************************************************************************
* Function Name: tic_tac_toe_play_game
********************************************************************************
* Summary: Enters the code that plays the tic_tac_toe game.  Once called, this
*          function should never return!
* Returns:
*  Nothing
*******************************************************************************/
void tic_tac_toe_play_game(void)
{

    //Calling the tic_tac_toe_hw_init()
    tic_tac_toe_hw_init();

    // Horizontal Lines
    lcd_draw_rectangle(SCREEN_CENTER_COL, UPPER_HORIZONTAL_LINE_Y, LINE_LENGTH,LINE_WIDTH,LCD_COLOR_BLUE);
    lcd_draw_rectangle(SCREEN_CENTER_COL,LOWER_HORIZONTAL_LINE_Y, LINE_LENGTH,LINE_WIDTH,LCD_COLOR_BLUE);

    //Vertical Lines
    lcd_draw_rectangle(LEFT_HORIZONTAL_LINE_X,SCREEN_CENTER_ROW, LINE_WIDTH, LINE_LENGTH,LCD_COLOR_BLUE);
    lcd_draw_rectangle(RIGHT_HORIZONTAL_LINE_X,SCREEN_CENTER_ROW, LINE_WIDTH,LINE_LENGTH,LCD_COLOR_BLUE);

    //Set the active square to center and O is the start
    lcd_draw_rectangle(CENTER_COL, CENTER_ROW, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GREEN);
    lcd_draw_image(CENTER_COL,CENTER_ROW, O_WIDTH, O_HEIGHT,Bitmaps_O,LCD_COLOR_BLACK, LCD_COLOR_GREEN);

    while(1){
        //Checking the win condition
        if(win_condition()){
            leaving_condition(x, y); //updates the screen to change the color of the last placed.
            if(O_player){
                lcd_O_wins(); // O_wins prints
            } else if(X_player){
                lcd_X_wins();  //X_wins prints
            }
            win = true;       // sets win to true
            center = false;   //Stops the movement
        } else if(Check_Tie()){ //Checking the tie condition
            leaving_condition(x, y); //updates the color of the last placed.
            lcd_tie();  //prints tied condition
            win = true;  //sets win to true
            center = false; //stops movement after game ended
        }


        //If game is done then win condition is true
        while(win){
            if(S2_PRESSED){ //if S2_Pressed then the game restarts
                S2_PRESSED = false;
                re_start();
            }
        }

        //Positioning checks
        if(center){
            if(PS2_X_DIR > VOLT_2P70){ //right
                right = true;
            } else if(PS2_X_DIR < VOLT_0P6){ // left
                left = true;
            } else if(PS2_Y_DIR > VOLT_2P70){ //up
                up = true;
            } else if(PS2_Y_DIR < VOLT_0P6){ //down
                down = true;
            }
        }

        //Checks if the joy stick in the center
        if((PS2_X_DIR < VOLT_2P70) & (PS2_X_DIR > VOLT_0P6) & (PS2_Y_DIR < VOLT_2P70) & (PS2_Y_DIR > VOLT_0P6)){
            center = true;
        }

        if(LCD_UPDATE){
            if(right){ //right

                leaving_condition(x, y); //Checks the condition when leaving the square
                x++; //change position
                if(x == 3){  // goes past the edge check
                    x = 0;
                }
                entering_condition(x, y, player); //Checks the exit condition when entering the square
                right = false; //set to false after moved to right
                center = false; // needs to be centered

            } else if(left){ //left

                leaving_condition(x, y); //Checks the condition when leaving the square
                x--; //change position
                if(x == -1){  // goes past the edge check
                    x = 2;
                }
                entering_condition(x, y, player); //Checks the condition when entering the square
                left = false; //set to false after moved to left
                center = false; // needs to be centered

            } else if (down){ //bottom

                leaving_condition(x, y); //Checks the condition when leaving the square
                y--; //change position
                if(y == -1){  // goes past the edge check
                    y = 2;
                }
                entering_condition(x, y, player); //Checks the condition when entering the square
               down = false; //set to false after moved down
               center = false; // needs to be centered

            } else if (up){ //up

                leaving_condition(x, y); //Checks the condition when leaving the square
                y++; //change position
                if(y == 3){
                    y = 0;
                }
                entering_condition(x, y, player); //Checks the condition when entering the square
                up = false; //set to false after moved to up
                center = false; // needs to be centered

            }

            if(S2_PRESSED){ //checks if button is pressed
                S2_PRESSED = false; //Sets the global variable to fasle
                if(board[x][y] == 0){ //Checks if its an empty position
                    if(player == 1){ //Checks is its player O who pressed the button
                        lcd_draw_rectangle(x_pos[x], y_pos[y], SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_RED);
                        lcd_draw_image(x_pos[x], y_pos[y], O_WIDTH, O_HEIGHT,Bitmaps_O,LCD_COLOR_BLACK, LCD_COLOR_RED);
                        board[x][y] = 1; //sets it to 1 in the position for history
                        player = 2;  // Switches to next player

                    } else if(player == 2) { //Checks if it is player X who pressed the button
                        lcd_draw_rectangle(x_pos[x], y_pos[y], SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_RED);
                        lcd_draw_image(x_pos[x], y_pos[y], X_WIDTH, X_HEIGHT,Bitmaps_X,LCD_COLOR_BLACK, LCD_COLOR_RED);
                        board[x][y] = 2; //sets it to 1 in the position for history
                        player = 1; //Switches to next player
                    }
                }
            }
        }
    }
}

/*
 * Method for setting the drawing setting for X, O and empty when leaving
 * for the next spot based on what was there.
 */
void leaving_condition(int x, int y)
{
    if(board[x][y] == 0){ // Checks if leaving an empty square and sets its conditions
    lcd_draw_rectangle(x_pos[x], y_pos[y], SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_BLACK);
    } else if(board[x][y] == 1){// Checks if leaving a square that held an O
     lcd_draw_rectangle(x_pos[x], y_pos[y], SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_BLACK);
     lcd_draw_image(x_pos[x], y_pos[y], O_WIDTH, O_HEIGHT,Bitmaps_O,LCD_COLOR_CYAN, LCD_COLOR_BLACK);
    } else { // Checks if the left square was an X
        lcd_draw_rectangle(x_pos[x], y_pos[y], SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_BLACK);
        lcd_draw_image(x_pos[x], y_pos[y], X_WIDTH, X_HEIGHT,Bitmaps_X,LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
    }
}

/*
 * Method for setting the drawing setting for X, O and empty when entering
 * for the next spot based on what was there.
 */
void entering_condition(int x, int y, int player)
{
    if (board[x][y] == 1){ //if the new square being entered is O sets its conditions and is taken by O
        lcd_draw_rectangle(x_pos[x], y_pos[y], SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_RED);
        lcd_draw_image(x_pos[x], y_pos[y], O_WIDTH, O_HEIGHT,Bitmaps_O,LCD_COLOR_BLACK, LCD_COLOR_RED);
    } else if(board[x][y] == 2){ //if the new square being entered is X sets its conditions and is taken by X
              lcd_draw_rectangle(x_pos[x], y_pos[y], SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_RED);
              lcd_draw_image(x_pos[x], y_pos[y], O_WIDTH, X_HEIGHT,Bitmaps_X,LCD_COLOR_BLACK, LCD_COLOR_RED);
    } else{
          if(player == 1){ //if the new square being entered is X sets its conditions and is empty.
               lcd_draw_rectangle(x_pos[x], y_pos[y], SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GREEN);
               lcd_draw_image(x_pos[x], y_pos[y], O_WIDTH, O_HEIGHT,Bitmaps_O,LCD_COLOR_BLACK, LCD_COLOR_GREEN);
           } else { //if the new square being entered is O sets its conditions and is empty.
               lcd_draw_rectangle(x_pos[x], y_pos[y], SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GREEN);
               lcd_draw_image(x_pos[x], y_pos[y], X_WIDTH, X_HEIGHT, Bitmaps_X,LCD_COLOR_BLACK, LCD_COLOR_GREEN);
           }
    }
}

/**
 * Logic and the Win condition are being tested in this method
 */
bool win_condition(void)
{
    //Checking the row condition
    int r;
    for(r = 0; r < 3; r++){
        if((board[r][0]== board[r][1]) && (board[r][1]== board[r][2]) && board[r][0] !=0){
            if(board[r][0] == 1){O_player = true;} //sets O to true if row has 1's
            if(board[r][0] == 2){X_player = true;} //sets X to true if row has 2's
            return true;
        }
    }
    r = 0;

    //checking the column condition for win
    int c;
    for(c = 0; c < 3; c++){
        if((board[0][c]== board[1][c]) && (board[1][c] == board[2][c]) && board[0][c] !=0){
            if(board[0][c] == 1){O_player = true;} //sets O to true if column has 1's
            if(board[0][c] == 2){X_player = true;} //sets X to true if column has 2's
            return true;
        }
    }
    c = 0;

    //checking diagonal for win
    if((board[0][0] == board[1][1]) && (board[1][1] == board[2][2]) && board[0][0] != 0){
          if(board[0][0] == 1){O_player = true;} //sets O to true if diagonal has 1's
          if(board[0][0] == 2){X_player = true;} //sets X to true if diagonal has 2's
          return true;
    } else if ((board[0][2] == board[1][1]) && (board[1][1]== board[2][0]) && board[1][1] !=0){
          if(board[2][0] == 1){O_player = true;}
          if(board[2][0] == 2){X_player = true;}
          return true;
    }

    //if no condition has been met return false
    return false;
}

/*
 * Checks for a tie
 */
bool Check_Tie(void)
{
    int row;
    int col;
    for(row = 0; row < 3; row++){
        for(col = 0; col < 3; col++){
            if(board[row][col] == 0){ // goes through and checks if the spots are empty
                return false;
            }
        }
    }

    return true;
}


/**
 * Restarts the games after the game has been played with intial game conditions
 */
void re_start(void){


    lcd_draw_rectangle(CENTER_COL, CENTER_ROW, SCREEN_X, SCREEN_Y, LCD_COLOR_BLACK);

    // Horizontal Lines
    lcd_draw_rectangle(SCREEN_CENTER_COL, UPPER_HORIZONTAL_LINE_Y, LINE_LENGTH,LINE_WIDTH,LCD_COLOR_BLUE);
    lcd_draw_rectangle(SCREEN_CENTER_COL,LOWER_HORIZONTAL_LINE_Y, LINE_LENGTH,LINE_WIDTH,LCD_COLOR_BLUE);

    //Vertical Lines
    lcd_draw_rectangle(LEFT_HORIZONTAL_LINE_X,SCREEN_CENTER_ROW, LINE_WIDTH, LINE_LENGTH,LCD_COLOR_BLUE);
    lcd_draw_rectangle(RIGHT_HORIZONTAL_LINE_X,SCREEN_CENTER_ROW, LINE_WIDTH,LINE_LENGTH,LCD_COLOR_BLUE);

    //Set the active square to center and toggles which won is supposed to start.
    if(toggle == 0){
        lcd_draw_rectangle(CENTER_COL, CENTER_ROW, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GREEN);
        lcd_draw_image(CENTER_COL,CENTER_ROW, O_WIDTH, O_HEIGHT,Bitmaps_O,LCD_COLOR_BLACK, LCD_COLOR_GREEN);
        toggle = 1;
        player = 1;
    } else if(toggle == 1){
        lcd_draw_rectangle(CENTER_COL, CENTER_ROW, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GREEN);
        lcd_draw_image(CENTER_COL,CENTER_ROW, X_WIDTH, X_HEIGHT,Bitmaps_X, LCD_COLOR_BLACK, LCD_COLOR_GREEN);
        toggle = 0;
        player = 2;
    }

    //Positioning and index
    up = false, down = false, left = false, right = false;
    x = 1;
    y = 1;

    //Checks the joy stick if in the center.
    center = true;

    //Win variable
    win = false;

    //User(X = 1 or O = 2 )
    int r;
    int c;
    for(r = 0; r < 3; r++){
        for(c = 0; c < 3; c++){
            board[r][c] = 0;
        }
    }

    //Check which player won
    X_player = false;
    O_player = false;

}


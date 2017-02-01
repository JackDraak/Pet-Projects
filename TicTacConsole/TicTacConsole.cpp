/*
    Tic - Tac - Console

    by: @JackDraak - Playing around: just a humble hobbyist
                     tinkerer, messing with C++ to have fun
                     and maybe even learn something.

    This is  my first working version of a one-player
    take on the the classic pen and paper (or should I
    say, "stick and dirt"?) game: tic - tac - toe

    [designed to now be played on the Windows Console.]

    Where to go from here:
        - improve formatting, use colour(?)
        - optimize source-code
            - separate into MFC class and Game class, use a header file....
*/

// Used for Input and Output.
#include <iostream>
#include <sstream>

// Used by A.I. player.
#include <random> 

// Used to treat the console display as a textbox.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// For quality randomness, use this Entropy.
std::mt19937 Entropy = std::mt19937{ std::random_device{}() };

// The 9 cells of the traditional game grid will be box objects in memory.
class box
{
private:
    char    value;
    char    identity;
    int     screenColumn;
    int     screenRow;

public:
    box();
    char    GetValue();
    char    GetID();
    int     GetScreenColumn();
    int     GetScreenRow();
    void    Reset();
    void    SetValue(char);
    void    SetID(char);
    void    SetScreenColumn(int);
    void    SetScreenRow(int);
};

// Default initializations for a new box object.
box::box() { value = 'U'; identity = '0'; screenColumn = 0; screenRow = 0; return; }

// Box getters and setters.
char    box::GetValue()                     { return value; }
char    box::GetID()                        { return identity; }
int     box::GetScreenColumn()              { return screenColumn; }
int     box::GetScreenRow()                 { return screenRow; }
void    box::Reset()                        { value = identity; return; }
void    box::SetValue(char newValue)        { value = newValue; return; }
void    box::SetID(char newID)              { identity = newID; return; }
void    box::SetScreenColumn(int sColumn)   { screenColumn = sColumn;  return; }
void    box::SetScreenRow(int sRow)         { screenRow = sRow;  return; }

// Global variables.
char const winSets[8][3] = {
    { '1','2','3' }, // 0 row 1
    { '4','5','6' }, // 1 row 2
    { '7','8','9' }, // 2 row 3
    { '1','4','7' }, // 3 column 1
    { '2','5','8' }, // 4 column 2
    { '3','6','9' }, // 5 column 3
    { '1','5','9' }, // 6 backslash
    { '7','5','3' }, // 7 foreslash
};

box gameBoard[3][3];

char    activePlayer;
char    blockingPlay;
char    winingSet;
char    winner;
bool    playerXturn = false;
int     blockingSet;

// Method prototypes.

bool    CheckForBlock();
bool    CheckForWin();
bool    Continue();
bool    ValidatePlay(char);
char    GetBlockingPlay(int);
char    GetPlay();
char    GetBoxValue(char);
char    Shuffle(char, char);
int     Shuffle(int);
int     Shuffle(int, int);
void    Home();
void    Home(bool);
void    Home(int, int);
void    InitBoard();
void    PauseForInput();
void    PrintEmptyBoard();
void    PrintValues();
void    SwitchPlayer();

// Switch active player from X to O or vice versa.
void SwitchPlayer()
{
    playerXturn = !playerXturn;
    if (playerXturn) activePlayer = 'X';
    else activePlayer = 'O';
}

// Print the game box::value's into the game-grid on screen.
void PrintValues()
{
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            int boxRow = gameBoard[row][column].GetScreenRow();
            int boxColumn = gameBoard[row][column].GetScreenColumn();
            char boxValue = gameBoard[row][column].GetValue();

            // Output value to designated screen position.
            Home(boxRow, boxColumn);
            std::cout << boxValue;
        }
    }
    Home(10, 0);
    return;
}

// Initialize gameBoard array with content (numbered boxes) and position information.
void InitBoard()
{
    // Assign game grid rows and columns to on-screen coordinates.
    int const R1 = 4;
    int const R2 = 6;
    int const R3 = 8;
    int const C1 = 17;
    int const C2 = 21;
    int const C3 = 25;
    char thisValue = '1';
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            // Set initial value and reset ID <thisValue> of each box.
            gameBoard[row][column].SetID(thisValue);
            gameBoard[row][column].Reset();

            // Set designated on-screen row of each box.
            if (row == 0) gameBoard[row][column].SetScreenRow(R1);
            else if (row == 1) gameBoard[row][column].SetScreenRow(R2);
            else if (row == 2) gameBoard[row][column].SetScreenRow(R3);

            // Set designated on-screen column of each box.
            if (column == 0) gameBoard[row][column].SetScreenColumn(C1);
            else if (column == 1) gameBoard[row][column].SetScreenColumn(C2);
            else if (column == 2) gameBoard[row][column].SetScreenColumn(C3);

            thisValue++;
        }
    } return;
}

// Get a boxID <char 1-9> from the console user.
char GetPlay()
{
    std::string userInput;
    char position;
    bool outOfRange = true;
    do
    {
        getline(std::cin, userInput);
        std::stringstream thisStream(userInput);
        Home(15, 0); std::cout << "                                  ";
        if (thisStream >> position) { break; }
        Home(15, 0); std::cout << "   Invalid entry...";
        if (position >= '1' && position <= '9') outOfRange = false;
    } while (outOfRange);
    return position;
}

// Clear and home the console, and put up an empty game board.
void PrintEmptyBoard()
{
    Home(true);
    std::cout << /// 10        20        30        4
                 ///0123456789-123456789-123456789-123456///0
        "\n            -------------------"     ///1
        "\n            Tic - Tac - Console"     ///2
        "\n"                                    ///3
        "\n                   |   |   "         ///4
        "\n                ---|---|---"         ///5
        "\n                   |   |   "         ///6
        "\n                ---|---|---"         ///7
        "\n                   |   |   "         ///8
        "\n";                                   ///9
    Home(10, 0);
    return;
}

// Use this function to "Home" the cursor to the top left of the console.
void Home()
{
    HANDLE	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD	coordScreen = { 0, 0 };
    SetConsoleCursorPosition(hConsole, coordScreen);
    return;
}

// Use this function overload to "Home" the cursor to a specific row:column.
void Home(int row, int column)
{
    short thisRow = row;
    short thisColumn = column;
    HANDLE	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD	coordScreen = { thisColumn, thisRow };
    SetConsoleCursorPosition(hConsole, coordScreen);
    return;
}

// Use this overload with a true or false argument for a "Clear and Home" effect in the console.
void Home(bool clear)
{
    HANDLE	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD	coordScreen = { 0, 0 };
    BOOL	bSuccess;
    DWORD	cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO	csbi;
    DWORD	dwConSize;
    bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
    if (!bSuccess) return;
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    bSuccess = FillConsoleOutputCharacter(hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten);
    if (!bSuccess) return;
    bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
    if (!bSuccess) return;
    bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    if (!bSuccess) return;
    bSuccess = SetConsoleCursorPosition(hConsole, coordScreen);
    if (!bSuccess) return;
}

// Return a random int from 0-max.
int Shuffle(int max)
{
    std::uniform_int_distribution<> zIndexDist(0, max);
    return zIndexDist(Entropy);
}

// Return a random int from min-max.
int Shuffle(int min, int max)
{
    std::uniform_int_distribution<> zIndexDist(min, max);
    return zIndexDist(Entropy);
}

// Return a random char from begin to end.
char Shuffle(char begin, char end)
{
    std::uniform_int_distribution<> zIndexDist(begin, end);
    return zIndexDist(Entropy);
}

// Delay for console input.
void PauseForInput()
{
    std::string userInput;
    Home(10, 0);  std::cout << "   Please hit <Enter> to proceed...                         ";
    getline(std::cin, userInput);
    return;
}

// True if continued play is requested, false if exit requested.
bool Continue()
{
    bool validInput = false;
    bool proceed = false;
    do
    {
        std::string userInput;
        Home(10, 0); std::cout << "   Please enter (P)lay or (Q)uit then <Enter>, thank you. ";
        getline(std::cin, userInput);
        if (userInput[0] == 'p' || userInput[0] == 'P')
        {
            validInput = true;
            proceed = true;
        }
        else  if (userInput[0] == 'q' || userInput[0] == 'Q')
        {
            validInput = true;
        }
    } while (!validInput);
    Home(10, 0); std::cout << "                                                               ";
    return proceed;
}

// Take a box ID and attempt to claim it for the activePlayer.
bool ValidatePlay(char id)
{
    bool validity = false;
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            if (id == gameBoard[row][column].GetID())
            {
                char boxValue = gameBoard[row][column].GetValue();
                if (boxValue == id)
                {
                    gameBoard[row][column].SetValue(activePlayer);
                    validity = true;
                }
                else if (boxValue == activePlayer && activePlayer == 'X')
                {
                    Home(12, 0); std::cout << "                                                                 ";
                    Home(12, 0); std::cout << "   Box " << id << "?!? You've already claimed that one! ";
                    Home(10, 0);
                }
                else if (activePlayer == 'X' && boxValue != activePlayer)
                {
                    Home(12, 0); std::cout << "                                                                 ";
                    Home(12, 0); std::cout << "   Box " << id << "?!? Your opponent already claimed that one! ";
                    Home(10, 0);
                }
            }
        }
    } return validity;
}

// Return the box::value of a box by box-ID.
char GetBoxValue(char id)
{
    char value = '-';
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            if (gameBoard[row][column].GetID() == id)
            {
                value = gameBoard[row][column].GetValue();
                return value;
            }
        }
    } return value;
}

// Check winSets against gameBoard.
bool CheckForWin()
{
    bool won = false;
    char thisSet[3];
    for (int winSetRow = 0; winSetRow <= 7; winSetRow++)
    {
        for (int member = 0; member <= 2; member++)
        {
            thisSet[member] = winSets[winSetRow][member];
        }
        int xScore = 0;
        int oScore = 0;
        for (int member = 0; member <= 2; member++)
        {
            char thisValue = thisSet[member];
            char thisBoxValue = GetBoxValue(thisValue);
            if (thisBoxValue != thisValue)
            {
                if (thisBoxValue == 'X') xScore++;
                else oScore++;
            }
        }
        if (xScore == 3 || oScore == 3)
        {
            Home(16, 0); std::cout << "   Congratulations, player " << activePlayer << " for the win! (WinPatternID:" << winSetRow << ")";
            won = true;
        }
    } return won;
}

// Check gameBoard array, looking for any prescribed blocking play; when true, follow with GetBlockingPlay() on the identified set.
bool CheckForBlock()
{
    bool block = false;
    char thisSet[3];
    blockingSet = -1;
    for (int winSetRow = 0; winSetRow <= 7; winSetRow++)
    {
        for (int member = 0; member <= 2; member++)
        {
            thisSet[member] = winSets[winSetRow][member];
        }
        int xScore = 0;
        int oScore = 0;
        blockingPlay = 'U';
        for (int member = 0; member <= 2; member++)
        {
            char thisValue = thisSet[member];
            char thisBoxValue = GetBoxValue(thisValue);
            if (thisBoxValue != thisValue)
            {
                if (thisBoxValue == 'X') xScore++;
                else oScore++;
            }
        }
        if ((xScore == 2 && oScore == 0) || (xScore == 0 && oScore == 2))
        {
            blockingSet = winSetRow;
            block = true;
        }
    } return block;
}

// To be used if CheckForBlock identifies a blockingSet in order to return the blockingPlay.
char GetBlockingPlay(int blockingSet)
{
    char block = 'E';
    char thisSet[3];
    char thisValue;
    for (int member = 0; member <= 2; member++)
    {
        thisSet[member] = winSets[blockingSet][member];
        thisValue = thisSet[member];
        char thisBoxValue = GetBoxValue(thisValue);
        if (thisBoxValue == thisValue) block = thisValue;
    } return block;
}

// Application entry point.
int main()
{
    // Play (at least one) game of Tic-Tac-Toe
    do {
        PrintEmptyBoard();
        InitBoard();
        PrintValues();
        SwitchPlayer();
        if (activePlayer == 'O') SwitchPlayer(); // For simplicity, start each game with console player as X.
        blockingPlay = 'U';
        winner = '-';
        bool gameInProgress = true;
        int turnNumber = 1;
        do {
            // Player X plays.
            bool validMove = false;
            do {
                Home(10, 0); std::cout << "                                                        ";
                Home(10, 0); std::cout << "          Make a move, player " << activePlayer << ": ";
                char thisPlay = GetPlay();
                validMove = ValidatePlay(thisPlay);
            } while (!validMove);
            PrintValues();
            gameInProgress = (!CheckForWin());
            if (turnNumber == 9 && gameInProgress) { winner = '-';  gameInProgress = false; }
            else
            {
                if (CheckForBlock()) blockingPlay = GetBlockingPlay(blockingSet);
            }
            turnNumber++;
            SwitchPlayer();
            // Player O plays.
            if (gameInProgress)
            {
                bool validRandom;
                if (!ValidatePlay(blockingPlay)) validRandom = false; else validRandom = true;
                if (!validRandom) do {
                    char randomPlay = Shuffle('1', '9');
                    validRandom = ValidatePlay(randomPlay);
                } while (!validRandom);
                PrintValues();
                gameInProgress = (!CheckForWin());
                turnNumber++;
                SwitchPlayer();
            }
            // Repeat X play then O play as needed.
        } while (turnNumber < 10 && gameInProgress);
        // Would you like to play a game? -War Games
    } while (Continue());
    // End Of Line -Tron
    return 0;
}

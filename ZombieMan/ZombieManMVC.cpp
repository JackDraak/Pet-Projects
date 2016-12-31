/*
                ....
               C C  ?
              /<   %
 ___ __________\&__/
/(- /(\_\_______   \
\ ) \ )_     \o     \
/|\ /|\      |'     |
            /o   __\
           / '  /  |
          /_/\_____|
         (   _(    <
          \    \    \
           \    \    |
            \____\____\
            %___\_%__\_\

    While contemplating things I might code in order
    to practice and learn about C++, I remembered a
    childhood two-person game called Hangman. 

    Here I will attempt a game I'll call ZombieMan,
    modeled on the same mechanic as hangman: a secret
    word is selected... letters are guessed one at a 
    time... valid letters help reveal the secret word,
    but invalid letters are eaten by the ZombieMan to
    help him re-assemble his scattered parts... Once
    ZombieMan has enough discarded letters to get his
    head on straight: Game Over, you're lunch!

    (TODO, play against computer opponent?)

    @JackDraak (2016)
*/

// Used for Input and Output.
#include <iostream>
#include <sstream>

// Used to be random!
#include <random> 

// Used to treat the console display as a textbox.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

std::string zombieMan[15] = {
     "                ....     ",
     "               C C  ?     ",
     "              /<   %     ",
     " ___ __________\\&__/     ",
     "/(- /(\\_\\_______   \\     ",
     "\\ ) \\ )_     \\o     \\     ",
     "/|\\ /|\\      |'     |          ",
     "            /o   __\\     ",
     "           / '  /  |     ",
     "          /_/\\_____|     ",
     "         (   _(    <     ",
     "          \\    \\    \\     ",
     "           \\    \\    |     ",
     "            \\____\\____\\     ",
     "            %___\\_%__\\_\\     ",
};

bool Continue();
char GetChar();
int GetPercent();
int Shuffle(int, int);
void PauseForInput();
void PrintZombieMan(int);
void Home();
void Home(int, int);

// For quality randomness, use this Entropy.
std::mt19937 Entropy = std::mt19937{ std::random_device{}() };

// Output a percentage of the ZombieMan.
void PrintZombieMan(int percent)
{
    int const HEADER = 3;
    int const MARGIN = 15;
    int const ENDROW = 14;
    float thisPercent = (percent / 100) * ENDROW; // TODO fix this
Home(22, 0); std::cout << thisPercent << "-thisPercent.  ";
    int beginRow = (ENDROW - thisPercent);
std::cout << beginRow << "-beginRow ";
    for (int thisRow = beginRow; thisRow <= ENDROW; thisRow++)
    {
        Home((thisRow + HEADER), MARGIN);
        std::cout << zombieMan[thisRow];
    }
    return;
}

// Get a <char> from the console user.
char GetChar()
{
    std::string userInput;
    char position;
    bool outOfRange = true;
    do
    {
        getline(std::cin, userInput);
        std::stringstream thisStream(userInput);
        Home(21, 0); std::cout << "                                  ";
        if (thisStream >> position) { break; }
        Home(21, 0); std::cout << "   Invalid entry...";
        if (position >= '1' && position <= '9') outOfRange = false; // TODO complete this
    } while (outOfRange);
    return position;
}

// Get a percentage (0-100) from the console user.
int GetPercent()
{
    std::string userInput;
    int newPercent;
    bool outOfRange = true;
    do
    {
        getline(std::cin, userInput);
        std::stringstream thisStream(userInput);
        Home(21, 0); std::cout << "                                  ";
        if (thisStream >> newPercent) { break; }
        Home(21, 0); std::cout << "   Invalid entry...";
        if (newPercent <= 100 && newPercent >= 0) outOfRange = false;
    } while (outOfRange);
    return newPercent;
}

// Return a random int from min-max.
int Shuffle(int min, int max)
{
    std::uniform_int_distribution<> zIndexDist(min, max);
    return zIndexDist(Entropy);
}

// Use this fucntion for a "Clear and Home" effect in the console.
void Home()
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

// Delay for console input.
void PauseForInput()
{
    std::string userInput;
    Home(20, 0);  std::cout << "   Please hit <Enter> to proceed...                         ";
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
        Home(20, 0); std::cout << "   Please enter (P)lay or (Q)uit then <Enter>, thank you. ";
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

int main()
{
    do
    {
        Home();
        Home(20, 0); std::cout << "   Enter percentage of ZombieMan to appear: ";
        int printPercent = GetPercent();
        PrintZombieMan(printPercent);
    } while (Continue());

    return 0;
}

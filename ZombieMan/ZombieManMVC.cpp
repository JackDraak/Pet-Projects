/*
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

    (TODO, play against computer opponent? But maybe
    first get it.... well, up and running!)

    @JackDraak (2016)
*/

// Includes for I/O.
#include <iostream>
#include <sstream>

// Include in _order_ to be random!
#include <random> 

// Include in _order_ to sort!
#include <algorithm>

// Includes to treat the console display as a textbox.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// CLASS LetterBox -- Container to store characters submitted during a phase of play.
class LetterBox {
public:
    std::string GetLetters() const;

    void Reset();
    bool SubmitLetter(char);

private:
    std::string BoxOfLetters;
};

// Instantiate a LetterBox object for the game.
LetterBox activeLetterBox;

// Global variables.

std::string zombieMan[15] = {
     "               ....     ",
     "              C C  ?     ",
     "             /<   %     ",
     " ___ ________\\&__/     ",
     "/(- /(\\_\\_____   \\     ",
     "\\ ) \\ )_   \\o     \\     ",
     "/|\\ /|\\    |'     |          ",
     "           /o   __\\     ",
     "          / '  /  |     ",
     "         /_/\\_____|     ",
     "        (   _(    <     ",
     "         \\    \\    \\     ",
     "          \\    \\    |     ",
     "           \\____\\____\\     ",
     "           %___\\_%__\\     ",
};

std::string secretWord = "example"; // TODO get a dictionary up and going

// Function prototypes.

bool Continue();
char GetLetter();
int GetPercent();
int Shuffle(int, int);
std::string GetString();
void PauseForInput();
void PrintLetterBox();
bool PrintAndEvaluateSecret();
void PrintZombieMan(int);
void Home();
void Home(int, int);

// For quality randomness, use this Entropy.
std::mt19937 Entropy = std::mt19937{ std::random_device{}() };

// Output a percentage of the ZombieMan.
void PrintZombieMan(int percent)
{
    if (percent > 0)
    {
        int const HEADER = 3;
        int const MARGIN = 50;
        int const ENDROW = 14;
        float thisPercent = ((percent / 100.0f) * ENDROW);
        int thisFraction = floor(thisPercent + 0.5); /// simulate rounding-off to get back to an integer; ignore data-loss warning.
        int beginRow = (ENDROW - thisFraction);
        for (int thisRow = beginRow; thisRow <= ENDROW; thisRow++)
        {
            Home((thisRow + HEADER), MARGIN);
            std::cout << zombieMan[thisRow];
        }
    } return;
}

// Output the secret word clue. True when player has guessed all the correct letters.
bool PrintAndEvaluateSecret()
{
    bool gameOver = false;
    bool secret;
    int letterScore = 0;
    int wordSize = secretWord.size();
    std::string userLetters = activeLetterBox.GetLetters();
    int userLettersSize = userLetters.size();
    Home(7, 0); 
    std::cout << "     Guess the secret word, before\n"
              << "     the ZombieMan eats enough of\n"
              << "     your scraps to put himself back\n"
              << "     together again, allowing him to\n"
              << "     eat your BrAiNs!";
    Home(5, 0);
    std::cout << "     Secret Word:  ";
    for (int letterPos = 0; letterPos < wordSize; letterPos++)
    {
        secret = true;
        for (int userLetter = 0; userLetter < userLettersSize; userLetter++)
        {
            if (userLetters[userLetter] == secretWord[letterPos])
            {
                secret = false;
                letterScore++;
                if (letterScore == wordSize) gameOver = true;
            }
        }
        if (secret) std::cout << "_ "; else std::cout << secretWord[letterPos] << " ";
    } 
    return gameOver;
}

// Output the current LetterBox.
void PrintLetterBox()
{
    // TODO decide if the template row stays or goes...
    // Print a template row... 
    ///    Home(0, 14); for (char thisLetter = 'a'; thisLetter <= 'z'; thisLetter++) { std::cout << thisLetter << " "; } 

    // ...and beaneath it the queue.
    Home(1, 14);
    std::string theseLetters = activeLetterBox.GetLetters();
    int boxMember = 0;
    for (char thisLetter = 'a'; thisLetter <= 'z'; thisLetter++)
    {
        char testLetter = theseLetters[boxMember];
        if (testLetter == thisLetter)
        {
            boxMember++;
            std::cout << thisLetter << " ";
        }
        else if (testLetter != thisLetter) {
            std::cout << "  ";
        }
    } return;
}

// Get a <char a-z> from the console user. 
char GetLetter()
{
    std::string userInput;
    char position;
    bool outOfRange = true;
    Home(21, 0); std::cout << "                                  ";
    do
    {
        getline(std::cin, userInput);
        Home(21, 0); std::cout << "                                  ";
        std::stringstream thisStream(userInput);
        if (thisStream >> position) { break; }
        Home(21, 0); std::cout << "   Invalid entry...";
        if (position >= 'a' && position <= 'z') outOfRange = false;
    } while (outOfRange);
    Home(21, 0); std::cout << "                                  ";
    return position;
}

// Get a string from the console user. 
std::string GetString()
{
    std::string userInput;
    getline(std::cin, userInput);
    return userInput;
}

// Get a percentage (0-100) from the console user.
int GetPercent()
{
    std::string userInput;
    int newPercent = -1;
    bool outOfRange = true;
    Home(21, 0); std::cout << "                   ";
    do
    {
        Home(20, 44);
        getline(std::cin, userInput);
        Home(21, 0); std::cout << "                                  ";
        std::stringstream thisStream(userInput);
        if (!(thisStream >> newPercent)) { Home(21, 0); std::cout << "   Invalid entry..."; }
        if (newPercent >= 0 && newPercent <= 100) outOfRange = false;
    } while (outOfRange);
    Home(21, 0); std::cout << "                                  ";
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
    Home(20, 0);  std::cout << "] Please hit <Enter> to proceed...                       ";
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
        Home(20, 0); std::cout << "] Please enter (P)lay or (Q)uit then <Enter>, thank you. ";
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
    Home(10, 0); std::cout << "                                                         ";
    return proceed;
}

// ----- Letter-Box Methods ----- //

// Return a string of any letters stored in the letterbox.
std::string LetterBox::GetLetters() const { return BoxOfLetters; }

// Reset (i.e.: empty) the letterbox contents.
void LetterBox::Reset() { BoxOfLetters = ""; return; }

// Ensure that the character argument is in the letterbox. ('True' when submitted letter is also in the secret word).
bool LetterBox::SubmitLetter(char letter)
{
    int boxSize = BoxOfLetters.length();
    if (BoxOfLetters == "") { BoxOfLetters += letter; }
    else
    {
        bool novelChar = false;
        int letterMatches = 0;

        for (int iSlot = 0; iSlot < boxSize; iSlot++)
        {
            if (!(BoxOfLetters[iSlot] == letter))
            {
                letterMatches++; novelChar = true;
            }
        }
        if (novelChar && (letterMatches == boxSize)) {
            BoxOfLetters += letter;
        }
    }
    std::sort(BoxOfLetters.begin(), BoxOfLetters.end());

    // check if Zombie gets a meal...
    int wordSize = secretWord.size();
    bool inWord = false;
   for (int letterPos = 0; letterPos < wordSize; letterPos++)
        {
            if (letter == secretWord[letterPos])
            {
                inWord = true;
            }
    } return inWord;
}

// Application entry point.
int main()
{
    do
    {
        int zombieLevel = 0;
        bool zombieSafe = true;
        bool success;
        activeLetterBox.Reset();
        do
        {
            Home();
            PrintLetterBox();
            PrintZombieMan(zombieLevel); // Shuffle(0, 100)); // TODO placeholder for something more meaningful ||  PrintZombieMan(zombieLevel);
            success = PrintAndEvaluateSecret();
            if (zombieLevel >= 100)
            {
                success = true;
                Home(20, 42); std::cout << "              ";
                Home(20, 0); std::cout << "] Here comes ZombieMan, intent on eating your BrAiNs!\n  (hit <Enter> to continue)";
                std::string thisPlay = GetString();
            }
            else if (!success) {
                Home(20, 42); std::cout << "              ";
                Home(20, 0); std::cout << "] Please enter the letter you would like to risk: ";
                std::string thisPlay = GetString();
                bool zombieSafe = (activeLetterBox.SubmitLetter(thisPlay[0]));
                if (!zombieSafe)
                {
                    zombieLevel += 20;
                }
            }
        } while (!success);
        if (PrintAndEvaluateSecret())
        {
            Home(20, 42); std::cout << "              ";
            Home(20, 0); std::cout << "] You escaped with the secret word before ZombieMan attacked. Congratulations!\n  (hit <Enter> to continue)";
            std::string thisPlay = GetString();
        }
    } while (true); //  Continue());

    return 0;
}

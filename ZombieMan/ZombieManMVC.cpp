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
// Includes to treat the console display as a textbox.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Includes for I/O.
#include <iostream>
#include <sstream>

// Include in _order_ to sort!
#include <algorithm>

// Include in _order_ to map relations!
#include <map>

// Include in _order_ to be random!
#include <random> 

// For quality randomness, use this Entropy.
std::mt19937 Entropy = std::mt19937{ std::random_device{}() };

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

// Function declarations.

bool Continue();
char GetLetter();
int GetPercent();
int Shuffle(int, int);
std::string GetString();
std::string SetZombieWord(int, int);
void PauseForInput();
void PrintLetterBox();
bool PrintAndEvaluateSecret();
void PrintZombieMan(int);
void Home();
void Home(int, int);

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
    /// Print a template row... 
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
    std::string tempString;
    char thisChar;
    bool outOfRange = true;
    Home(20, 0); std::cout << "                                 ";
    do
    {
        Home(19, 50);
        getline(std::cin, userInput);
        std::stringstream thisStream(userInput);
        thisStream >> tempString;
        thisChar = tempString[0];
        if (thisChar >= 'a' && thisChar <= 'z') outOfRange = false;
        else
        {
            Home(20, 0); std::cout << "Invalid entry, please re-try.";
        }
    } while (outOfRange);
    return thisChar;
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
        Home(19, 44);
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
    Home(19, 0);  std::cout << "] Please hit <Enter> to proceed...                       ";
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
        Home(19, 0); std::cout << "] Please enter (P)lay or (Q)uit then <Enter>, thank you. ";
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

// Respond with # of guesses allowed based on length of challenge word (& difficulty setting?).
int GetMaxGuesses(int wordSize) 
{
    int maxGuesses;

    if (true) /// (difficultyFactor == 1) // Easy difficulty map.
    {
        std::map <int, int> mWordSizeToGuessCount{
            { 2,11 },{ 3,10 },{ 4,9 },{ 5,8 },{ 6,7 },{ 7,6 },{ 8,5 },{ 9,4 },
            { 10,3 },{ 11,3 },{ 12,3 },{ 13,3 },{ 14,3 },{ 15,3 },{ 16,3 },{ 17,3 }
        };
        maxGuesses = mWordSizeToGuessCount[wordSize];
    } return maxGuesses;
}

// Respond with next challenge-word;
std::string SetZombieWord(int minSize, int maxSize)
{
    if (maxSize < 3) { maxSize = 3; }
    if (maxSize > 11) { maxSize = 11; }
    if (minSize < 2) { minSize = 2; }
    if (minSize > 10) { minSize = 10; }

    // Dictionary of (mostly) isogram challenge words
    std::vector<std::string> dictionary = {
        // random words, may not be isograms
        "that", "ranger", "endanger", "entangle", "arrest", "arresting", "arrested", "envelope", "raspberry", "cherry", 
        "gross", "cheese", "homesteading", "homestead", "pioneer", "pioneering", "evaluate", "evaluating", "evaluated",
        "risks", "mitigation", "enshrine", "enshrined", "allowed", "maulers", "brawlers", "rookies", "provision", "revision",
        "envision", "envisioning", "envisioned", "challenge", "arrange", "arranged", "alabaster", "inevitable", "glorious",
        "endangered", "whistle", "criminal", "justice", "system", "moment", "before", "momentary", "realize", "realizing",

        // 2
        "as", "am", "an", "at", "be", "do", "go", "he", "id", "is", "it", "me", "no", "on", "so", "to", "we",

        // 3
        "age", "art", "bag", "bat", "boy", "bye", "car", "coy", "cry", "cut", "day", "dim", "din", "elf", "fit",
        "fly", "for", "hit", "hug", "jam", "kit", "lab", "lax", "lie", "lit", "lot", "mar", "may", "mit", "net",
        "nip", "par", "pay", "pen", "pie", "pit", "pot", "put", "rag", "ray", "rot", "say", "sea", "sin", "tag",
        "tax", "tea", "the", "try", "vye", "was", "win", "won", "yam", "zip",

        // 4
        "acre", "ages", "army", "aunt", "bail", "bale", "belt", "care", "cave", "cure", "daft", "damn", "dart",
        "dime", "expo", "fair", "fast", "fawn", "felt", "fish", "from", "hair", "hare", "harp", "hear", "help",
        "hide", "huge", "jail", "july", "june", "lair", "land", "lend", "lint", "love", "lynx", "lyre", "name",
        "nape", "navy", "nose", "oven", "pair", "pure", "quit", "raid", "rail", "same", "sail", "sand", "sock",
        "soda", "sold", "soup", "suit", "talk", "tiny", "this", "tone", "tray", "tuna", "walk", "wasp", "wave",
        "yarn",

        // 5
        "angel", "anger", "angry", "ankle", "april", "blast", "braid", "brain", "bread", "brick", "brush", "clear",
        "cloth", "drain", "dregs", "dwarf", "empty", "enact", "envoy", "facts", "fairy", "faith", "farce", "farms",
        "gravy", "hares", "large", "laugh", "light", "lingo", "litre", "lives", "loads", "locks", "march", "marsh",
        "miles", "money", "nasty", "nurse", "pious", "query", "rages", "roads", "roast", "sable", "scone", "shock",
        "sight", "snout", "stand", "stick", "super", "swift", "table", "talks", "tango", "thick", "thing", "tiger",
        "timer", "toads", "tongs", "trash", "trick", "tuber", "tuner", "vegan", "yeast",

        // 6
        "abound", "around", "baster", "belfry", "bounty", "bricks", "bright", "bronze", "buster", "cables",
        "crawls", "crowns", "curled", "custom", "dancer", "danger", "dearly", "design", "fables", "fabric",
        "factor", "fairly", "famine", "farces", "friday", "gizmos", "guitar", "hybrid", "jaunts", "loving",
        "market", "monday", "muster", "myrtle", "mythos", "nachos", "nailed", "nebula", "nickel", "orgasm",
        "phrase", "placid", "quaint", "quartz", "sailor", "sample", "sculpt", "second", "slight", "sucker",
        "sunday", "tackle", "tongue", "travel", "tricks", "turkey", "wealth", "zealot",

        // 7
        "anxiety", "apricot", "caribou", "cliquey", "clothes", "consume", "crazily", "dolphin", "domains",
        "exclaim", "fabrics", "factory", "haircut", "imports", "isogram", "jackpot", "jaybird", "jockeys",
        "jukebox", "lockjaw", "mindful", "monarch", "nostril", "polearm", "pulsing", "quicken", "quicker",
        "quickly", "scarlet", "schlump", "scholar", "scourge", "sketchy", "smoking", "sparkle", "strange",
        "stymied", "subject", "treason", "turbine", "wrongly", "wyverns", "yoghurt", "ziplock",

        // 8 --
        "authored", "bankrupt", "blasting", "bunghole", "burliest", "bushland", "bushmeat", "busywork",
        "butchery", "buxomest", "cogently", "crumpets", "emblazon", "exhaling", "exoplasm", "explains",
        "exploits", "finagled", "frontage", "goldfish", "gumption", "handgrip", "hardiest", "hasteful",
        "hospital", "imported", "informed", "jockular", "megalith", "megatons", "merciful", "minerals",
        "monarchy", "pronated", "questing", "question", "rambling", "sculptor", "speaking", "spectrum",
        "thursday",

        // 9
        "authorize", "binocular", "constable", "dangerous", "dragonfly", "inspector", "introduce", "nightmare",
        "racheting", "ramblings", "redaction", "revolting", "solarized", "solemnity", "traveling", "tribunals",
        "uncharted", "unclamped", "veracious", "verbosity", "vouchsafe", "vulcanism", "vulgarity", "watchdogs",
        "waveforms", "wavefront", "welcoming", "windstorm", "wolfsbane", "womanizer", "wordgames", "workmates",
        "workplace", "workspace", "worshiped", "yachtsmen",

        // 10
        "abductions", "afterglows", "aftershock", "agonizedly", "algorithms", "authorizes", "background",
        "behaviours", "benchmarks", "binoculars", "birthnames", "campground", "capsulized", "chalkstone",
        "chipboards", "chlorinate", "destroying", "duplicates", "exfoliants", "exhaustion", "farsighted",
        "gobsmacked", "graciously", "greyhounds", "hospitable", "infamously", "nightmares", "shockingly",
        "subjectify", "sympathize",

        // 11
        "abolishment", "atmospheric", "backgrounds", "backlighted", "birthplaces", "bodysurfing", "cabinetwork",
        "countryside", "dangerously", "decryptions", "delusionary", "demographic", "designatory", "discernably",
        "documentary", "exculpatory", "factorylike", "flavourings", "francophile", "palindromes", "personality",
        "playgrounds", "precautions", "regulations", "speculation", "subcategory", "sympathizer", "trampolines",
        "workmanship",

        // 12
        "counterplays", "demographics", "demonstrably", "descrambling", "discountable", "discrepantly", "exhaustingly",
        "lexicography", "locksmithery", "malnourished", "multipronged", "nightwalkers", "outsparkling", "overstudying",
        "packinghouse", "productively", "pyromagnetic", "questionably", "stenographic", "stickhandler", "subnormality",
        "subvocalized", "thunderclaps", "unforgivable", "unprofitable", "upholstering",

        // 13
        "chimneyboards", "consumptively", "copyrightable", "documentarily", "draughtswomen", "flamethrowing",
        "flowchartings", "lycanthropies", "metalworkings", "misconjugated", "muckspreading", "multibranched",
        "musicotherapy", "pneumogastric", "salpingectomy", "subordinately", "unmaledictory", "unpredictably",
        "unproblematic", "unsympathized",

        // 14
        "ambidextrously", "computerizably", "dermatoglyphic", "hydromagnetics", "hydropneumatic", "pseudomythical",
        "subformatively", "undiscoverably",

        // 15
        "uncopyrightable",

        // 17
        "subdermatoglyphic"
    };
    int numberOfWords = size(dictionary);

    // Select the challenge word randomly.
    std::string selectionString;
    int randomNumber;
    int selectionLength;

    do {
        std::uniform_int_distribution<> zIndexDist(0, numberOfWords - 1);
        randomNumber = zIndexDist(Entropy);
        selectionString = dictionary[randomNumber];
        selectionLength = selectionString.length();
    } while (selectionLength < minSize || selectionLength > maxSize);
    return selectionString; // Break and watch this variable to cheat,^H^H^H^H^H^H^H^H^Herr for play-testing.
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
        secretWord = SetZombieWord(3, 11);
        int guessLimit = GetMaxGuesses(secretWord.size());
        int zombieBooster = (99.0f / guessLimit);

        activeLetterBox.Reset();
        do
        {
            Home();
            PrintLetterBox();
            PrintZombieMan(zombieLevel); /// Shuffle(0, 100));
            success = PrintAndEvaluateSecret();
            if (zombieLevel >= 100)
            {
                success = true;
                Home(19, 42); std::cout << "              ";
                Home(19, 0); std::cout << "] Here comes ZombieMan, intent on eating your BrAiNs!\n  (hit <Enter> to continue)";
                std::string thisPlay = GetString();
            }
            else if (!success) {
                Home(19, 42); std::cout << "              ";
                Home(19, 0); std::cout << "] Please enter the letter you would like to risk: ";
                char thisPlay = GetLetter();
                bool zombieSafe = (activeLetterBox.SubmitLetter(thisPlay));
                if (!zombieSafe)
                {
                    zombieLevel += zombieBooster;
                }
            }
        } while (!success);
        if (PrintAndEvaluateSecret()) // TODO should this have more evaluators? seems to be leading to the printing of the serect word on losses...
        {
            Home(19, 42); std::cout << "              ";
            Home(19, 0); std::cout << "] You escaped with the secret word before ZombieMan attacked. Congratulations!\n  (hit <Enter> to continue)";
            std::string thisPlay = GetString();
        }
    } while (true); ///  Continue());

    return 0;
}

#pragma once
#include "IsogramChallenge.h"

IsogramEngine::IsogramEngine() { Reset(); return; }

bool IsogramEngine::bIsGuessMatch()           const { return bDoesGuessMatch; }
FString IsogramEngine::sGetIsogram()          const { return sIsogram; }
int32 IsogramEngine::iGetCurrentGuessNum()    const { return iCurrentGuess; }
int32 IsogramEngine::iGetDifficulty()         const { return iDifficultyFactor; }
int32 IsogramEngine::iGetIsogramLength()      const { return sIsogram.length(); }
int32 IsogramEngine::iGetLossCount()          const { return iLossCount; }
int32 IsogramEngine::iGetPhaseScore()         const { return iPhaseScore; }
int32 IsogramEngine::iGetRunningGuesses()     const { return iRunningGuesses; }
int32 IsogramEngine::iGetRunningScore()       const { return iRunningScore; }
int32 IsogramEngine::iGetWinCount()           const { return iWinCount; }

void IsogramEngine::FudgeGuesses() { iCurrentGuess--; return; }
void IsogramEngine::IncrementGuess() { iCurrentGuess++; return; }
void IsogramEngine::IncrementLoss() { iLossCount++; return; }
void IsogramEngine::SetEasy() { iDifficultyFactor = 1; return; }
void IsogramEngine::SetHard() { iDifficultyFactor = 3; return; }
void IsogramEngine::SetNormal() { iDifficultyFactor = 2; return; }

// Initialize new game or setup for next phase.
void IsogramEngine::Reset()
{
    if (!bInitialized)
    {
        bDisplayClues = true;
        bDisplayLetterbox = true;
        bInitialized = true;
        bValidated = false;
        bValidDictionary = false;
        iDifficultyFactor = 2; // Difficulty: 1 = easy, 2 = normal, 3 = hard.
        iLossCount = 0;
        iPhaseScore = 0;
        iWinCount = 0;
    }
    iCurrentGuess = 1;
    sIsogram = sSelectIsogram(iGetChallengeSize()); // Select challenge-word [size] based on score.
    return;
}

// Update total score, & zero phase-score. (Also track total guesses).
void IsogramEngine::Tally()
{
    iRunningGuesses += iGetCurrentGuessNum();
    iRunningScore += iPhaseScore;
    iPhaseScore = 0;
    return;
}

// Respond with # of guesses allowed based on length of challenge word (& difficulty setting).
int32 IsogramEngine::iGetMaxGuesses() const
{
    int32 iWordSize;

    if (iDifficultyFactor == 1) // Easy difficulty map.
    {
        std::map <int32, int32> mWordSizeToGuessCount{
            { 2,10 },{ 3,9 },{ 4,10 },{ 5,11 },{ 6,11 },{ 7,11 },{ 8,11 },{ 9,11 },
            { 10,10 },{ 11,9 },{ 12,8 },{ 13,7 },{ 14,6 },{ 15,5 },{ 16,4 },{ 17,4 }
        };
        iWordSize = mWordSizeToGuessCount[sGetIsogram().length()];
    }
    else if (iDifficultyFactor == 3) // Hard difficulty map.
    {
        std::map <int32, int32> mWordSizeToGuessCount{
            { 2,5 },{ 3,4 },{ 4,4 },{ 5,5 },{ 6,6 },{ 7,6 },{ 8,6 },{ 9,5 },
            { 10,5 },{ 11,4 },{ 12,4 },{ 13,3 },{ 14,3 },{ 15,2 },{ 16,2 },{ 17,2 }
        };
        iWordSize = mWordSizeToGuessCount[sGetIsogram().length()];
    }
    else if (iDifficultyFactor == 2) // Normal difficulty map.
    {
        std::map <int32, int32> mWordSizeToGuessCount{
            { 2,7 },{ 3,6 },{ 4,7 },{ 5,8 },{ 6,9 },{ 7,9 },{ 8,9 },{ 9,8 },
            { 10,7 },{ 11,6 },{ 12,5 },{ 13,4 },{ 14,3 },{ 15,3 },{ 16,3 },{ 17,3 }
        };
        iWordSize = mWordSizeToGuessCount[sGetIsogram().length()];
    }
    return iWordSize;
}

// Respond with maximum challenge-word size. Higher total scores give bigger challenges (longer words).
int32 IsogramEngine::iGetChallengeSize() const
{
    if (iRunningScore < 45) { return 3; }
    else if (iRunningScore < 45 * 3) { return 4; }
    else if (iRunningScore < 45 * 5) { return 5; }
    else if (iRunningScore < 45 * 9) { return 6; }
    else if (iRunningScore < 45 * 17) { return 7; }
    else if (iRunningScore < 45 * 33) { return 8; }
    else if (iRunningScore < 45 * 65) { return 9; }
    else if (iRunningScore < 45 * 129) { return 10; }
    else if (iRunningScore < 45 * 257) { return 11; }
    else if (iRunningScore < 45 * 513) { return 12; }
    else if (iRunningScore < 45 * 1025) { return 13; }
    else if (iRunningScore < 45 * 2049) { return 14; }
    else if (iRunningScore < 45 * 4097) { return 15; }
    else if (iRunningScore < 45 * 8193) { return 16; }
    else { return 17; }
}

// Update the active game Analysis <struct>, comparing challenge word aganist submitted guess.
Analysis IsogramEngine::AnalyzeGuess(FString sGuess)
{
    Analysis zAnalysis;

    int32 iIsogramLength = sIsogram.length();
    zAnalysis.sPositionHint = FString(iIsogramLength, '-');
    zAnalysis.sLetterHint = zAnalysis.sPositionHint;

    // Compare guess against challenege word, letter for letter & position for position.
    for (int32 iGuessLetter = 0; iGuessLetter < iIsogramLength; iGuessLetter++) {
        for (int32 iIsogramLetter = 0; iIsogramLetter < iIsogramLength; iIsogramLetter++) {
            if (sGuess[iGuessLetter] == sIsogram[iIsogramLetter]) {
                bool bLetterScore = false;
                bool bPositionScore = false;

                if (iGuessLetter == iIsogramLetter)
                {
                    bPositionScore = true;
                    zAnalysis.iPositionMatches++;
                    zAnalysis.sPositionHint[iGuessLetter] = sGuess[iGuessLetter];
                }
                else
                {
                    bLetterScore = true;
                    zAnalysis.iLetterMatches++;
                    zAnalysis.sLetterHint[iGuessLetter] = sGuess[iGuessLetter];
                }
                // Setup a score multiplier considering: difficulty, use of clues, use of letterbox/
                int32 iClueMulti = 1;
                int32 iLetterMulti = 1;
                int32 iLetterScore = 0;
                int32 iMultiplier = 1;

                if (!bDisplayClues) { iClueMulti = 2; };
                if (!bDisplayLetterbox) { iLetterMulti = 2; };
                iMultiplier = iDifficultyFactor * iClueMulti * iLetterMulti;

                if (!bLetterScore && bPositionScore) { iLetterScore = 3 * iMultiplier; }
                else if (bLetterScore && !bPositionScore) { iLetterScore = 1 * iMultiplier; }
                bLetterScore = false;
                bPositionScore = false;
                // Update global phase-score variable with any applicable letter score.
                iPhaseScore = iPhaseScore + iLetterScore;
            }
        }
    }
    if (zAnalysis.iPositionMatches == iIsogramLength)
    {
        bDoesGuessMatch = true;
        iWinCount++;
    }
    else {
        bDoesGuessMatch = false;
    }
    return zAnalysis;
}

// Set and respond with next challenge-word; required argument INT indicates maximum word length.
FString IsogramEngine::sSelectIsogram(int32 iChallengeNum)
{
    if (iChallengeNum < 3) { iChallengeNum = 3; }

    // Dictionary of isogram challenge words
    std::vector<FString> aDictionary = {
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

        // 15 -- categories of one word are not very replayable.....  buuuut it's unlikely anyone will ever play this long....
        "uncopyrightable",

        // 17
        "subdermatoglyphic"
    };
    int32 iNumberOfIsograms = size(aDictionary);

    // Validate dictionary -- ONCE ONLY --
    if (!bValidated && !bValidDictionary)
    {
        bool bReportingMode = true;
        bValidated = true;
        bValidDictionary = true;
        FString sTestString;

        for (int32 iIndex = 0; iIndex < iNumberOfIsograms; iIndex++)
        {
            sTestString = aDictionary[iIndex];
            if (!bIsIsogram(sTestString))
            {
                bValidDictionary = false;
                // Output list of any invalidated words detected, or silently proceed.
                if (bReportingMode)
                {
                    std::cout << "E R R O R . V A L I D A T I N G -" << iNumberOfIsograms << "- W O R D S\n";
                    std::cout << "INTERNAL ERROR z-13: Dictionary validation failures detected:\n";
                    bReportingMode = false;
                }
                std::cout << " -- Dictionary[" << iIndex << "] = \"" << sTestString << "\"\n";
            }
        }
        if (!bReportingMode)
        {
            std::cout << "\nWARNING: Any words listed above will be ignored as unplayable.\n\n";
        }
    }

    // Select the challenge word randomly.
    /*       High quality entropy required, because this loop can get hammered.
    NOTE: use of Random will give the same result for a full second,
    no matter how many times it is queried (if the word selected is the
    wrong size, it re-queries until satisfied, so it's a bit thrashy to
    continue supplying it with the same number...) This is why we need
    an Entropy that can give us a new number every time we query it,
    even if we only asked it for a number a millisecond ago.                */
    FString sSelection;
    int32 iSelection;
    int32 iSelectionLength;

    do {
        do {
            std::uniform_int_distribution<> zIndexDist(0, iNumberOfIsograms - 1);
            iSelection = zIndexDist(Entropy);
            sSelection = aDictionary[iSelection];
        } while (!bIsIsogram(sSelection));
        iSelectionLength = sSelection.length();
    } while (iSelectionLength > iChallengeNum);
    return sSelection; // Break and watch this variable to cheat,^H^H^H^H^H^H^H^H^Herr for play-testing.
}

// Boolean test if string argument is an isogram.
bool IsogramEngine::bIsIsogram(FString sTestString)
{
    sTestString = sStringToLower(sTestString);
    std::map<char, bool> mObservedLetterTF;

    for (auto cLetter : sTestString) {
        if (!mObservedLetterTF[cLetter]) { mObservedLetterTF[cLetter] = true; }
        else return false;
    } return true;
}

// Convert string argument to lower-case.
FString IsogramEngine::sStringToLower(FString sConvertString)
{
    int32 iLength = sConvertString.length();

    for (int32 iPosition = 0; iPosition < iLength; iPosition++)
    {
        sConvertString[iPosition] = tolower(sConvertString[iPosition]);
    }
    return sConvertString;
}

// ----- Letter-Box Methods ----- //

// Return a string of any letters stored in the letterbox.
FString LetterBox::sGetLetters() const { return sBoxOfLetters; }

// Reset (i.e.: empty) the letterbox contents.
void LetterBox::Reset() { sBoxOfLetters = ""; return; }

// Ensure that the character argument is in the letterbox. 
void LetterBox::SubmitLetter(char cLetter)
{
    if (sBoxOfLetters == "") { sBoxOfLetters += cLetter; }
    else
    {
        bool bNovelChar = false;
        int32 iBoxSize = sBoxOfLetters.length();
        int32 iLetterMatches = 0;

        for (int32 iSlot = 0; iSlot < iBoxSize; iSlot++)
        {
            if (!(sBoxOfLetters[iSlot] == cLetter))
            {
                iLetterMatches++; bNovelChar = true;
            }
        }
        if (bNovelChar && (iLetterMatches == iBoxSize)) {
            sBoxOfLetters += cLetter;
        }
    }
    std::sort(sBoxOfLetters.begin(), sBoxOfLetters.end());
    return;
}

#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

// Substitutions to maintain UnrealEngine compatability.
using FString = std::string;
using int32 = int;

// Detailed analysis comparing player guess vs. challenge word.
struct Analysis
{
    FString     sLetterHint = "";
    FString     sPositionHint = "";
    int32       iLetterMatches = 0;
    int32       iPositionMatches = 0;
};

// CLASS IsogramEngine -- Functions that manage the core of the Isogram Game.
class IsogramEngine {
    std::mt19937 Entropy = std::mt19937{ std::random_device{}() };

public:
    IsogramEngine(); // Constructor.

    bool    bIsGuessMatch()         const;
    FString sGetIsogram()           const;
    int32   iGetChallengeSize()     const;
    int32   iGetCurrentGuessNum()   const;
    int32   iGetIsogramLength()     const;
    int32   iGetLossCount()         const;
    int32   iGetMaxGuesses()        const;
    int32   iGetPhaseScore()        const;
    int32   iGetRunningGuesses()    const;
    int32   iGetRunningScore()      const;
    int32   iGetWinCount()          const;
    int32   iGetDifficulty()        const;

    Analysis    AnalyzeGuess(FString);
    bool        bDisplayLetterbox;
    bool        bDisplayClues;
    bool        bIsIsogram(FString);
    FString     sSelectIsogram(int32);
    FString     sStringToLower(FString);
    void        FudgeGuesses();
    void        IncrementGuess();
    void        IncrementLoss();
    void        Reset();
    void        SetEasy();
    void        SetHard();
    void        SetNormal();
    void        Tally();

    // See IsogramEngine::IsogramEngine() definition for initialization. [found in IsogramEngine.cpp]
private:
    bool        bDoesGuessMatch;
    bool        bInitialized;
    bool        bValidated;
    bool        bValidDictionary;
    FString     sIsogram;
    int32       iCurrentGuess;
    int32       iDifficultyFactor;
    int32       iLossCount;
    int32       iMaxGuesses;
    int32       iPhaseScore;
    int32       iRunningGuesses;
    int32       iRunningScore;
    int32       iWinCount;
};

// CLASS LetterBox -- Container to store characters submitted during a phase of play.
class LetterBox {
public:
    FString sGetLetters() const;

    void Reset();
    void SubmitLetter(char);

private:
    FString sBoxOfLetters;
};

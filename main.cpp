#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <utility>
//For random seeding, srand(static_cast<unsigned int>(time(0)));
struct RoundResult {
    int roundNumber;
    int prizeDoor;
    int playerPick;
    int revealedDoor;
    int switchPick;
    bool stayWin;
    bool switchWin;
};


RoundResult simulateMontyHallRound(int roundNumber){
    // Setup The 3 doors: 0 , 1 , 2
    std::vector<int> doors{1,2,3};
    
    // Randomly choose the Prize Door and Players Choice
    int prizeDoor = rand() % 3;
    int playerPick = rand() % 3;

    // Puts Hosts Options(not winner not chosen) In a new vector
    std::vector<int> revealOptions;
    for(int door : doors){
        if(door != playerPick && door != prizeDoor) {
            revealOptions.push_back(door);
        }
    }

    // Host randomly picks through his options
    int revealedDoor = revealOptions[rand() % revealOptions.size()];

    // Decide the Door The player could switch to
    int switchPick = -1;
    for (int door : doors) {
        if (door!= playerPick && door != revealedDoor) {
            switchPick = door;
            break;
        }
    }

    //Determine win or lose if you switch / stay with choice
    bool stayWin = (playerPick == prizeDoor);
    bool switchWin = (switchPick == prizeDoor);

    
    return RoundResult{
        roundNumber,
        prizeDoor,
        playerPick,
        revealedDoor,
        switchPick,
        stayWin,
        switchWin
    };
}
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed RNG

    const int trials = 1000000;
    std::vector<RoundResult> results;
    int stayWins = 0;
    int switchWins = 0;

    for (int i = 1; i <= trials; ++i) {
        RoundResult result = simulateMontyHallRound(i);
        results.push_back(result);
        if (result.stayWin) stayWins++;
        if (result.switchWin) switchWins++;
    }

    // Write to CSV
    std::ofstream outFile("monty_hall_results.csv");
    if (!outFile) {
        std::cerr << "Failed to open file for writing.\n";
        return 1;
    }

    outFile << "Round,PrizeDoor,PlayerPick,RevealedDoor,SwitchPick,StayWin,SwitchWin\n";
    for (const RoundResult& r : results) {
        outFile << r.roundNumber << ","
                << r.prizeDoor << ","
                << r.playerPick << ","
                << r.revealedDoor << ","
                << r.switchPick << ","
                << (r.stayWin ? "true" : "false") << ","
                << (r.switchWin ? "true" : "false") << "\n";
    }

    outFile.close();

    std::cout << "Simulation Complete!\n";
    std::cout << "Stay Wins: " << stayWins << " (" << (stayWins * 100.0 / trials) << "%)\n";
    std::cout << "Switch Wins: " << switchWins << " (" << (switchWins * 100.0 / trials) << "%)\n";

    return 0;
}

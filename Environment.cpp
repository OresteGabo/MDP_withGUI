//
// Created by oreste on 01/04/24.
//

#include <cstdlib>
#include <iostream>
#include "Environment.h"

Environment::Environment() : current_state(0) {
    initialiseTerminalState();
    for(int i=0; i < NumStates - 1; i++)
        reward_table[i] = -1;
    reward_table[NumStates - 1] = 100; // Give a positive reward for reaching terminal state
}

int Environment::getCurrentState() const {
    return current_state;
}

bool Environment::isTerminal() const {
    return current_state == terminalState;;
}


Reward Environment::takeAction(Action action) {
    const int goal_state = NumStates - 1;
    int old_dist = std::abs(current_state/4 - goal_state/4) + std::abs(current_state%4 - goal_state%4);

    switch (action) {
        case MoveUp:     if (current_state >= 4) current_state -= 4; break;
        case MoveDown:   if (current_state < 12) current_state += 4; break;
        case MoveLeft:   if (current_state % 4 != 0) current_state -= 1; break;
        case MoveRight:  if (current_state % 4 != 3) current_state += 1; break;
        default:         std::cerr << "Invalid action!" << std::endl;
    }

    int new_dist = std::abs(current_state/4 - goal_state/4) + std::abs(current_state%4 - goal_state%4);
    double reward = 0;

    if (new_dist < old_dist) {  // This means action was good
        reward = 100 * (1 - (double)new_dist / (double)old_dist);
    } else if (new_dist > old_dist) {   // This means action was not optimal
        reward = 0;
    } else {  // This means the action made no progress (either good or bad)
        reward = 50;
    }


    return reward;
}

int Environment::getTerminalState() const {
    return terminalState;
}

void Environment::initialiseTerminalState() {
    srand(time(0)); // use current time as seed for random generator
    terminalState = rand() % NumStates;
}


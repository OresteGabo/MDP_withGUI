//
// Created by oreste on 01/04/24.
//

#ifndef MDP_GUI_ENVIRONMENT_H
#define MDP_GUI_ENVIRONMENT_H

enum Action { MoveUp = 0, MoveDown, MoveLeft, MoveRight };

typedef int Reward;
using namespace std;
class Environment {
private:

    int current_state;
    int terminalState;

public:
    Environment();
    [[nodiscard]] int getCurrentState() const;
    bool isTerminal() const;
    Reward takeAction(Action action);
    static constexpr int NumStates = 16;
    void initialiseTerminalState();

    int getTerminalState() const;

private :
    Reward reward_table[NumStates]; // Rewards for being in each state.
};


#endif //MDP_GUI_ENVIRONMENT_H

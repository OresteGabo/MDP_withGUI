//
// Created by oreste on 01/04/24.
//

#ifndef MDP_GUI_AGENT_H
#define MDP_GUI_AGENT_H


#include <random>
#include <array>
#include "Environment.h"
using namespace std;
class Agent {
private:
    mt19937 gen;//{std::random_device{}()};
    double learning_rate = 5;
    double discount_factor = 0.95;
    static const int NumOfActions = 4;
    double getMaxQValue(int state);

    std::array<std::array<double, Agent::NumOfActions>, Environment::NumStates> Q_table{};
public:
    Agent();
    Action choose_action(int state);
    void learn(Reward reward, int old_state, int action, int new_state);
    Action getMaxQAction(int state);

};


#endif //MDP_GUI_AGENT_H

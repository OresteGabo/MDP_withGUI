//
// Created by oreste on 01/04/24.
//

#include <algorithm>
#include "Agent.h"

Agent::Agent() {
    gen = std::mt19937(std::random_device{}());
    for (auto &row : Q_table)
        row.fill(0.0);
}




Action Agent::choose_action(int state) {
    std::uniform_real_distribution<> dis(0, 1);
    if (dis(gen) < 0.1) {
        // Random choice
        return static_cast<Action>(std::uniform_int_distribution<>(0, NumOfActions - 1)(gen));
    } else {
        // Greedy choice
        return getMaxQAction(state);
    }
}

void Agent::learn(Reward reward, int old_state, int action, int new_state) {
    auto old_value = Q_table[old_state][static_cast<int>(action)];
    auto max_future_value = getMaxQValue(new_state);
    double new_value = (1 - learning_rate) * old_value + learning_rate * (reward + discount_factor * max_future_value);
    Q_table[old_state][static_cast<int>(action)] = new_value;
}
// Returns the action associated with the maximum Q value for a given state.
Action Agent::getMaxQAction(int state) {
    return static_cast<Action>(std::max_element(Q_table[state].begin(), Q_table[state].begin() + NumOfActions) - Q_table[state].begin());
}
// Returns the maximum Q value for a given state.
double Agent::getMaxQValue(int state) {
    return *std::max_element(Q_table[state].begin(), Q_table[state].begin() + NumOfActions);
}
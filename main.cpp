#include <QApplication>
#include <QPushButton>
#include <random>
#include <iostream>

typedef int Reward;
enum Action { MoveUp = 0, MoveDown, MoveLeft, MoveRight };
class Environment {
private:
    static const int NumStates = 16;
    int current_state;
    Reward reward_table[NumStates]; // Rewards for being in each state.
public:
    Environment();
    [[nodiscard]] int getCurrentState() const;
    bool isTerminal() const;
    Reward takeAction(Action action); // Modify to return reward
};




class Agent {
private:
    static std::mt19937 gen;
    std::array<std::array<double, 4>, 16> QTable{};  // Initialize Q-Table with zeros
    double learning_rate = 0.1;
    double discount_factor = 0.95;
public:
    Agent();
    Action choose_action(int state);
    void learn(Reward reward, int old_state, Action action, int new_state);
};

Agent::Agent() {
    for (auto &row : QTable)
        row.fill(0.0);
}

int toInt(Action action) {
    return static_cast<int>(action);
}

Action toAction(int action) {
    return static_cast<Action>(action);
}

std::mt19937 Agent::gen(std::random_device{}());

Action Agent::choose_action(int state) {
    std::uniform_real_distribution<> dis(0, 1);
    if (dis(gen) < 0.1) {
        // Random choice
        return toAction(std::uniform_int_distribution<>(0, 3)(gen));
    } else {
        // Greedy choice
        return toAction(std::max_element(QTable[state].begin(), QTable[state].end()) - QTable[state].begin());
    }
}

void Agent::learn(Reward reward, int old_state, Action action, int new_state) {
    auto old_value = QTable[old_state][toInt(action)];
    auto max_future_value = *std::max_element(QTable[new_state].begin(), QTable[new_state].end());
    // Apply the Q-learning formula
    double new_value = old_value + learning_rate * (reward + discount_factor * max_future_value - old_value);
    QTable[old_state][toInt(action)] = new_value;
}


Environment::Environment() : current_state(0) {
    // Fill reward_table with some reward values, for example:
    for(int i=0; i < NumStates -1; i++) {
        reward_table[i] = -1;
    }
    reward_table[NumStates - 1] = 100; // give a positive reward for reaching terminal state
}

int Environment::getCurrentState() const {
    return current_state;
}

bool Environment::isTerminal() const {
    return current_state == NumStates - 1;
}

Reward Environment::takeAction(Action action) {
    switch (action) {
        case MoveUp:     if (current_state >= 4) current_state -= 4; break;
        case MoveDown:   if (current_state < 12) current_state += 4; break;
        case MoveLeft:   if (current_state % 4 != 0) current_state -= 1; break;
        case MoveRight:  if (current_state % 4 != 3) current_state += 1; break;
        default:         std::cerr << "Invalid action!" << std::endl;
    }
    return reward_table[current_state];
}

void runEpisode(Environment& env, Agent& agent) {
    int state = env.getCurrentState();
    int episode = 0;
    while (!env.isTerminal()) {
        Action action = agent.choose_action(state);
        int reward = env.takeAction(action);
        int new_state = env.getCurrentState();
        agent.learn(reward, state, action, new_state);
        state = new_state;
        std::cout << "Episode " << episode << ", State: " << state << ", Reward: " << reward << std::endl;
        episode++;
    }
}

int main() {
    Environment env;
    Agent agent;
    for (int i = 0; i < 10; ++i) {
        runEpisode(env, agent);
        env = Environment(); // Once an episode finished, recreate the environment for the next one
    }
    return 0;
}
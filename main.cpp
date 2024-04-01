#include <QApplication>
#include <QPushButton>
#include <random>
#include <iostream>
const int MAX_STEPS_PER_EPISODE = 1000;

typedef int Reward;
enum Action { MoveUp = 0, MoveDown, MoveLeft, MoveRight };
std::ostream& operator<<(std::ostream& os, const Action action) {
    switch (action) {
        case MoveUp:     os << "MoveUp"; break;
        case MoveDown:   os << "MoveDown"; break;
        case MoveLeft:   os << "MoveLeft"; break;
        case MoveRight:  os << "MoveRight"; break;
    }
    return os;
}
class Environment {
private:

    int current_state;

public:
    Environment();
    [[nodiscard]] int getCurrentState() const;
    bool isTerminal() const;
    Reward takeAction(Action action);
    static constexpr int NumStates = 16;
private :
    Reward reward_table[NumStates]; // Rewards for being in each state.
};




class Agent {
private:
    std::mt19937 gen;//{std::random_device{}()};
    double learning_rate = 0.1;
    double discount_factor = 0.95;
    static const int NumOfActions = 4;
    double getMaxQValue(int state);

    std::array<std::array<double, Agent::NumOfActions>, Environment::NumStates> Q_table{};
    //std::array<std::array<double, NumOfActions>, Environment::NumStates> Q_table{};
public:
    Agent();
    Action choose_action(int state);
    void learn(Reward reward, int old_state, Action action, int new_state);
    Action getMaxQAction(int state);

};

Agent::Agent() {
    gen = std::mt19937(std::random_device{}());
    for (auto &row : Q_table)
        row.fill(0.0);
}

int toInt(Action action) {
    return static_cast<int>(action);
}

Action toAction(int action) {
    return static_cast<Action>(action);
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

void Agent::learn(Reward reward, int old_state, Action action, int new_state) {
    auto old_value = Q_table[old_state][static_cast<int>(action)];
    auto max_future_value = getMaxQValue(new_state);
    double new_value = (1 - learning_rate) * old_value + learning_rate * (reward + discount_factor * max_future_value);
    Q_table[old_state][static_cast<int>(action)] = new_value;
}
Environment::Environment() : current_state(0) {
    for(int i=0; i < NumStates - 1; i++)
        reward_table[i] = -1;
    reward_table[NumStates - 1] = 100; // Give a positive reward for reaching terminal state
}

// Returns the action associated with the maximum Q value for a given state.
Action Agent::getMaxQAction(int state) {
    return static_cast<Action>(std::max_element(Q_table[state].begin(), Q_table[state].begin() + NumOfActions) - Q_table[state].begin());
}

// Returns the maximum Q value for a given state.
double Agent::getMaxQValue(int state) {
    return *std::max_element(Q_table[state].begin(), Q_table[state].begin() + NumOfActions);
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
    int stepCount = 0;

    while (true) {
        Action action = agent.choose_action(state);
        int reward = env.takeAction(action);
        int new_state = env.getCurrentState();
        agent.learn(reward, state, action, new_state);


        // Outputting the state, action, reward, and new state in the terminal
        std::cout << "State: " << state
                  << ", Action: " << action
                  << ", Reward: " << reward
                  << ", New State: " << new_state << std::endl;

        state = new_state;

        if (env.isTerminal() || stepCount >= MAX_STEPS_PER_EPISODE) {
            break;
        }

        stepCount++;
    }
}

int main() {
    Environment env;
    Agent agent;
    for (int i = 0; i < 10; ++i) {
        runEpisode(env, agent);
        env = Environment();  // Recreate the environment for the next episode
    }
    return 0;
}
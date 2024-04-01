#include <QApplication>
#include <QPushButton>
#include <random>
#include <iostream>
#include "Environment.h"
#include "Agent.h"
#include "Widget.h"

const int MAX_STEPS_PER_EPISODE = 1000;

typedef int Reward;
std::ostream& operator<<(std::ostream& os, const Action action) {
    switch (action) {
        case MoveUp:     os << "MoveUp"; break;
        case MoveDown:   os << "MoveDown"; break;
        case MoveLeft:   os << "MoveLeft"; break;
        case MoveRight:  os << "MoveRight"; break;
    }
    return os;
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

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Widget widget;
    widget.show();

    return app.exec();
}
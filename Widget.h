//
// Created by oreste on 01/04/24.
//

#ifndef MDP_GUI_WIDGET_H
#define MDP_GUI_WIDGET_H


#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <sstream>
#include "Environment.h"
#include "Agent.h"

class Widget : public QWidget
{
    Q_OBJECT

private:
    QVector<QPushButton*> buttons;
    QLabel* stateLabel;
    QTextEdit* textEdit;
    Environment env;
    Agent agent;
    QTimer* timer;

public:
    explicit Widget(QWidget *parent = nullptr)
            : QWidget(parent),
              stateLabel(new QLabel(this)),
              textEdit(new QTextEdit(this)),
              timer(new QTimer(this))
    {
        QVBoxLayout* mainLayout = new QVBoxLayout;
        QGridLayout* gridLayout = new QGridLayout;
        mainLayout->addLayout(gridLayout);
        for (int i = 0; i < Environment::NumStates; ++i) {
            QPushButton* button = new QPushButton(QString::number(i), this);
            button->setEnabled(false); // initially all buttons are disabled
            buttons.append(button);
            gridLayout->addWidget(button, i / 4, i % 4);  // assuming a 4x4 grid
        }
        mainLayout->addWidget(stateLabel);
        mainLayout->addWidget(textEdit);

        QPushButton* runButton = new QPushButton("Run", this);
        connect(runButton, &QPushButton::clicked, this, &Widget::startLearning);
        mainLayout->addWidget(runButton);

        timer->setInterval(1); // adjust interval as needed
        connect(timer, &QTimer::timeout, this, &Widget::runStep);

        setLayout(mainLayout);
    }

private slots:
    void startLearning() {
        env.initialiseTerminalState();
        timer->start();
    }

    void runStep() {
        int state = env.getCurrentState();

        // change button color to indicate current state
        for (int i = 0; i < Environment::NumStates; ++i) {
            if (i == state)
                buttons[i]->setStyleSheet("background-color: green; border-width: 3px;");
            else if (i == env.getTerminalState())
                buttons[i]->setStyleSheet("border-width: 3px; border-style: solid; border-color: red");
            else
                buttons[i]->setStyleSheet("border-width: 1px;");  // restore normal border when not in terminal state
        }

        Action action = agent.choose_action(state);
        int reward = env.takeAction(action);
        int new_state = env.getCurrentState();
        agent.learn(reward, state, action, new_state);

        std::ostringstream oss;
        oss << "State: " << state
            << ", Action: " << action
            << ", Reward: " << reward
            << ", New State: " << new_state;
        stateLabel->setText(QString::fromStdString(oss.str()));
        textEdit->append(stateLabel->text());


        if (env.isTerminal()) {
            timer->stop();
        }
    }
};


#endif //MDP_GUI_WIDGET_H

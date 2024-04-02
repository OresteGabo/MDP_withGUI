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
    bool isRunning;

public:
    explicit Widget(QWidget *parent = nullptr)
            : QWidget(parent),
              stateLabel(new QLabel(this)),
              textEdit(new QTextEdit(this)),
              timer(new QTimer(this)),
              isRunning(false)
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

        auto runButton = new QPushButton("Run", this);
        connect(runButton, &QPushButton::clicked, this, &Widget::onRunSimulation);

        auto stopButton = new QPushButton("Stop", this);
        connect(stopButton, &QPushButton::clicked, this, &Widget::onStopSimulation);
        mainLayout->addWidget(runButton);
        mainLayout->addWidget(stopButton);

        timer->setInterval(1); // adjust interval as needed
        connect(timer, &QTimer::timeout, this, &Widget::runStep);

        setLayout(mainLayout);
        this->resize(1000, 800);

    }

private slots:

    void onRunSimulation() {
        if (!isRunning) {
            isRunning = true;
            startLearning();
        }
    }
    void onStopSimulation() {
        if (isRunning) {
            isRunning = false;
            timer->stop();
        }
    }
    void startLearning() {
        env.initialiseTerminalState();
        timer->start();
    }

    void runStep() {
        std::map<int, std::string> actionToString = {
                {0, "Up"},
                {1, "Down"},
                {2, "Right"},
                {3, "Left"}
        };
        int state = env.getCurrentState();


        Action action = agent.choose_action(state);
        int reward = env.takeAction(action);
        int new_state = env.getCurrentState();
        agent.learn(reward, state, action, new_state);


        // change button color to indicate current state
        for (int i = 0; i < Environment::NumStates; ++i) {
            if (i == state) {
                buttons[i]->setStyleSheet("background-color: green; border-width: 3px;");
            } else if (i == env.getTerminalState()) {
                buttons[i]->setStyleSheet("background-color: green; border-width: 3px;");
                buttons[i]->setStyleSheet("border-width: 3px; border-style: solid; border-color: red");
            } else {
                buttons[i]->setStyleSheet("border-width: 1px;");  // restore normal border when not in terminal state
            }



        }

        if(new_state==env.getTerminalState()){
            state=new_state;

            for (int i = 0; i < Environment::NumStates; ++i) {
                buttons[i]->setStyleSheet("border-width: 1px;");
            }
            buttons[new_state]->setStyleSheet("border-width: 3px; border-style: solid; border-color: black");
        }

        std::ostringstream oss;
        oss << "Etat: " << state
            << ", Action: " << actionToString[action]
            << ", Récompense: " << reward
            << ", Nouvel etat: " << new_state;

        QFont font = stateLabel->font();
        font.setPointSize(14); // Set font size to 14
        font.setBold(true);    // Set font style to be bold
        stateLabel->setFont(font); // Apply the font to stateLabel
        stateLabel->setText(QString::fromStdString(oss.str()));

        textEdit->append(stateLabel->text());
        if (env.isTerminal()) {
            timer->stop();
        }
    }
};


#endif //MDP_GUI_WIDGET_H

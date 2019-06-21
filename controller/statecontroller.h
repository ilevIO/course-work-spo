#ifndef STATECONTROLLER_H
#define STATECONTROLLER_H

#include <QAudioInput>
#include <QAudioOutput>

class StateController
{

    int frequencies[10];
    char chosen_frequency;

    //QAudioInput * audioInput;
    //QAudioOutput * audioOutput;

    bool is_in_config;

    bool is_setting_up;

    bool is_turned_on;

    char step_of_setting_up;

    int is_config_on;

    bool ant_launched;
    bool pit_launched;
    bool tel_launched;
    

    bool szum;
    bool pomiechi;

public:

    struct TableInfo {
        char field[6];

        static TableInfo frequencyToTable(int frequency);
        static void frequencyToTable(TableInfo& info, int frequency);
    };
    StateController();

    TableInfo getTable();
    bool isReadyToRecieve();

    int numberButtonPressed(char number);
    void clsPresed();
    void powerIsPressed();
    float pomiechiIsPressed();
    char powerLevelIsPressed();
    char szumIsPressed();
    void tableSwitcher();
    void pitSwitcher();
    void antSwitcher();
    void telSwitcher();

    int getCurrentFrequency();
};

#endif // STATECONTROLLER_H

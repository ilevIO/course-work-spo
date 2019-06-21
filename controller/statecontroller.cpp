#include "statecontroller.h"
#include <cmath>
#include <QDebug>

StateController::StateController()
{

    for (int i = 0; i < 10; i++)
        this->frequencies[i] = 30000;

    this->chosen_frequency = 1;

    this->is_turned_on = false;

    this->is_in_config = false;

    this->is_setting_up = false;

    this->is_config_on = -1;

    this->ant_launched = false;
    this->tel_launched = false;
    this->pit_launched = false;

    this->pomiechi = false;
    this->szum = false;

}

StateController::TableInfo StateController::getTable()
{
    TableInfo info;

    if(!this->is_turned_on || !this->pit_launched)
    {
        for (int i = 0; i < 6; i++) {
            info.field[i] = -1;
        }
    }
    else if (!this->is_in_config)
    {
        for (int i = 1; i < 6; i++)
            info.field[i] = -1;
        info.field[0] = this->chosen_frequency;
    }
    else if (!this->is_setting_up)
    {
        info.field[0] = this->chosen_frequency;
        TableInfo::frequencyToTable(info, this->frequencies[this->chosen_frequency]);
    }
    else
    {
        info.field[0] = this->chosen_frequency;
        TableInfo::frequencyToTable(info, this->frequencies[chosen_frequency]);
        for(int i = 0; i < 5 - step_of_setting_up; i++)
        {
            info.field[5-i] = -1;
        }
    }

    return info;
}

bool StateController::isReadyToRecieve()
{
    if (this->is_config_on != -1 && this->is_turned_on &&
            ant_launched && tel_launched)
        return true;
    else
        return false;
}

int StateController::numberButtonPressed(char number)
{
    qDebug() << "pressed " << ((int)number);

    int to_ret = 0;

    if (this->is_turned_on && this->pit_launched)
    {
        if (!this->is_setting_up)
        {
            if(!this->is_in_config)
            {
                this->is_config_on = this->frequencies[number];
                to_ret = 1;
            }
            this->chosen_frequency = number;
        }
        else
        {
            switch(step_of_setting_up)
            {
            case 0:
                if (number < 3 || number > 7)
                    return 0;
                break;
            case 1:
                if (frequencies[chosen_frequency] == 70000 && number > 5)
                    return 0;
                break;
            }
            if(step_of_setting_up == 0)
                frequencies[chosen_frequency] = 0;
            frequencies[chosen_frequency] += number * pow(10, 4 - step_of_setting_up);
            step_of_setting_up++;
            if (step_of_setting_up == 5)
            {
                this->is_setting_up = false;
            }
        }
    }
    return to_ret;
}

void StateController::clsPresed()
{
     qDebug() << "pressed cls";
     if (this->is_turned_on && this->is_in_config)// && !this->is_setting_up)
     {
         this->is_setting_up = true;
         this->step_of_setting_up = 0;
     }
}

void StateController::powerIsPressed()
{
    this->is_turned_on = !this->is_turned_on;
    this->is_setting_up = false;
    qDebug() << "pressed power";
}

float StateController::pomiechiIsPressed()
{
    qDebug() << "pressed pomiechi";

    this->pomiechi = !this->pomiechi;

    if (pomiechi && szum)
        return 0.f;
    else if (pomiechi || szum)
        return 0.1f;
    else
        return 0.5f;
}

char StateController::powerLevelIsPressed()
{
    static bool little = false;
    qDebug() << "level is pressed " << !little;

    if (little == false)
    {
        little = true;
        return 50;
    }
    else
    {
        little = false;
        return 100;
    }
}

char StateController::szumIsPressed()
{
    qDebug() << "szum is pressed";

    this->szum = !this->szum;

    if (pomiechi && szum)
        return 0;
    else if (pomiechi || szum)
        return 1;
    else
        return 2;
}

void StateController::tableSwitcher()
{
    qDebug() << "table switcher is pressed";
    this->is_in_config = !this->is_in_config;
    if (this->is_setting_up)
    {
        //TODO save temp frequency
    }
    this->is_setting_up = false;
}

void StateController::pitSwitcher()
{
    pit_launched = !pit_launched;
}

void StateController::antSwitcher()
{
    ant_launched = !ant_launched;
}

void StateController::telSwitcher()
{
    tel_launched = !tel_launched;
}

int StateController::getCurrentFrequency()
{
    return this->is_config_on;
}

StateController::TableInfo StateController::TableInfo::frequencyToTable(int frequency)
{
    TableInfo info;
    TableInfo::frequencyToTable(info, frequency);
    return info;
}

void StateController::TableInfo::frequencyToTable(StateController::TableInfo &info, int frequency)
{
    for (int i = 0; i < 5; i++)
    {
        info.field[5-i] = frequency % 10;
        frequency/=10;
    }
}

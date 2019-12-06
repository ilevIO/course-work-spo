#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "view/r173.h"
#include "view/r123.h"
#include <regex>
#include <string>
#include <QMessageBox>
#include "controller/broadcastnetworkcontroller.h"
using namespace std;

MainMenu::MainMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}
/*
class MainController {
public:
    NetworkController *networkController;
    static MainController *shared;
private:
    MainController() {
        shared = new MainController();
    }
};
*/
void MainMenu::on_launchServer_clicked()
{
    regex regular("^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$");
    if (!regex_match(this->ui->ipEdit->text().toStdString().c_str(), regular))
    {
        QMessageBox::warning(this, "Ошибка ввода", "Вы ввели неправильный ip адресс");
    }
    else
    {

        //MainController mainController;
        NetworkController * networkController = new NetworkController(false, ui->ipEdit->text());
        networkController->start();
        if (this->ui->R173_select->isChecked())
        {
            R173 *w = new R173(this->ui->ipEdit->text(), false, networkController);
            w->show();
        }
        else
        {
            R123 *w = new R123(this->ui->ipEdit->text(), false, networkController);
            w->show();
        }
        this->close();
    }
}

void MainMenu::on_pushButton_clicked()
{
    QMessageBox::information(this, "Авторы", "Бейда А.И.\nБурак А.А.\nДудак М.Н.\nМакатерчик А.В.\nУтин Л.Л.");
}

void MainMenu::on_createServer_clicked()
{
    NetworkController * networkController = new NetworkController(true, "");
    networkController->start();
    if (this->ui->R173_select->isChecked())
    {
        R173 *w = new R173(this->ui->ipEdit->text(), false, networkController);
        w->show();
    }
    else
    {
        R123 *w = new R123(this->ui->ipEdit->text(), false, networkController);
        w->show();
    }
    this->close();
}

void MainMenu::on_ipEdit_returnPressed()
{
    on_launchServer_clicked();
}

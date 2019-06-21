#include "r159.h"
#include "ui_r159.h"

R159::R159(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::R159)
{
    ui->setupUi(this);
}

R159::~R159()
{
    delete ui;
}


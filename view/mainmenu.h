#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>

namespace Ui {
class MainMenu;
}

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();


private slots:
    void on_launchServer_clicked();

    void on_pushButton_clicked();

    void on_createServer_clicked();

    void on_ipEdit_returnPressed();

private:
    Ui::MainMenu *ui;
};

#endif // MAINMENU_H

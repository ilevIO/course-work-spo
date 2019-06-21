#ifndef R159_H
#define R159_H

#include <QMainWindow>

namespace Ui {
class R159;
}

class R159 : public QMainWindow
{
    Q_OBJECT

public:
   explicit R159(QWidget *parent = nullptr);
    ~R159();

private:
    Ui::R159 *ui;
};

#endif // R159_H

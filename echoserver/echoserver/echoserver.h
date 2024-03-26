#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class echoserver;
}
QT_END_NAMESPACE

class echoserver : public QMainWindow
{
    Q_OBJECT

public:
    echoserver(QWidget *parent = nullptr);
    ~echoserver();

private:
    Ui::echoserver *ui;
};
#endif // ECHOSERVER_H

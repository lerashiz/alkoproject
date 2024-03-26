#include "echoserver.h"
#include "ui_echoserver.h"

echoserver::echoserver(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::echoserver)
{
    ui->setupUi(this);
}

echoserver::~echoserver()
{
    delete ui;
}

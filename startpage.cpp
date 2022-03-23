#include "startpage.h"
#include "ui_startpage.h"
#include <QDebug>
#include "widget.h"

StartPage::StartPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartPage)
{
    ui->setupUi(this);
}

StartPage::~StartPage()
{
    delete ui;
}

void StartPage::on_startButton_clicked()
{
    Widget* w = new Widget(this);
    w->show();
}


void StartPage::on_exitButton_clicked()
{
    QApplication::exit();
}


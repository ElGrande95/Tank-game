#include "startpage.h"
#include "ui_startpage.h"
#include <QDebug>
#include "widget.h"
#include "highscoretable.h"
#include "QPixmap"
#include <QGraphicsOpacityEffect>
StartPage::StartPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartPage)
{

    QPixmap bgImages(":/Imgs/Images/tank.jpg");
    bgImages = bgImages.scaled(600, 600);
    QPalette bgPalette = this->palette();
    bgPalette.setBrush(QPalette::Background,bgImages);
    this->setPalette(bgPalette);

    ui->setupUi(this);
    ui->startButton->setFlat(true);
    ui->startButton->setStyleSheet("QPushButton { background-color: transparent;"
                                   "border: 5px solid darkkhaki;"
                                   "padding: 5px;"
                                   "border-radius: 3px;"
                                   "opacity: 200;"
                                   "font: bold 32px;}");
    ui->highscoreButton->setStyleSheet("QPushButton { background-color: transparent;"
                                   "border: 5px solid darkkhaki;"
                                   "padding: 5px;"
                                   "border-radius: 3px;"
                                   " opacity: 200;"
                                   "font: bold 32px;}");
    ui->exitButton->setStyleSheet("QPushButton { background-color: transparent;"
                                   "border: 5px solid darkkhaki;"
                                   "padding: 5px;"
                                   "border-radius: 3px;"
                                   " opacity: 200;"
                                   "font: bold 32px;}");

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


void StartPage::on_highscoreButton_clicked()
{
    HighscoreTable* w = new HighscoreTable(this);
    w->show();
}


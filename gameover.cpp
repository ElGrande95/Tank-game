#include "gameover.h"
#include "ui_gameover.h"
#include <QDebug>

GameOver::GameOver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameOver)
{
    ui->setupUi(this);

    db = new HighScore("../Tank-game/highscore");
    db->connectOpen();
}

GameOver::~GameOver()
{
    delete ui;
}

void GameOver::setDestroyLabel(int destroyLabel)
{
    QFont f = QFont();
    f.setPixelSize(28);
    ui->destroyLabel->setFont(f);
    ui->destroyLabel->setText("Destroyed: " +QString::number(destroyLabel));
}

void GameOver::setTimeLabel(double timeLabel)
{
    QFont f = QFont();
    f.setPixelSize(28);
    ui->timeLabel->setFont(f);
    ui->timeLabel->setText("Time: " +QString::number(timeLabel));
}

void GameOver::setDestroy(int newDestroy)
{
    destroy = newDestroy;
}

void GameOver::setTime(double newTime)
{
    time = newTime;
}

void GameOver::showScore()
{
    ui->tableView->setModel(db->modelBase());
    ui->topTen->hide();
}

void GameOver::on_menuButton_clicked()
{
    db->connectClose();

    delete this;
}


void GameOver::on_confirmButton_clicked()
{
    db->insertInBase(ui->userLineEdit->text(), destroy, time);
    showScore();

}


#include "gameover.h"
#include "ui_gameover.h"
#include <QDebug>

GameOver::GameOver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameOver)
{
    ui->setupUi(this);

    setPage();

    db = new HighScore("../Tank-game/highscore");
    db->connectOpen();

    showScore();
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

    for(int i=0; i<ui->tableView->model()->columnCount(); i++) {
        ui->tableView->setColumnWidth(i,182);
    }

    for(int i=0; i<ui->tableView->model()->rowCount(); i++) {
        ui->tableView->setRowHeight(i,40);
    }
}

void GameOver::setPage()
{
    QPixmap bgImages(":/Imgs/Images/tank2.jpeg");
    bgImages = bgImages.scaled(600, 600);

    ui->label->setPixmap(bgImages);

    ui->tableView->setStyleSheet("QTableView { background-color: transparent;"
                                 "color: white;"
                                 "opacity: 200;"
                                 "font: bold 24px;};");

    ui->menuButton->setStyleSheet("QPushButton { background-color: transparent;"
                                  "border: 2px solid white;"
                                  "color: white;"
                                  "padding: 5px;"
                                  "border-radius: 7px;"
                                  "opacity: 200;"
                                  "font: bold 32px;}");

    ui->destroyLabel->setStyleSheet("QLabel { background-color: transparent;"
                                  "color: white;"
                                  "padding: 5px;"
                                  "border-radius: 7px;"
                                  "opacity: 200;}");

    ui->timeLabel->setStyleSheet("QLabel { background-color: transparent;"
                                  "color: white;"
                                  "padding: 5px;"
                                  "border-radius: 7px;"
                                 "opacity: 200;;}");
}

void GameOver::hideUserScore()
{
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
    hideUserScore();
    ui->tableView->viewport()->update();
}


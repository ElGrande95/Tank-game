#include "highscoretable.h"
#include "ui_highscoretable.h"
#include <QGraphicsOpacityEffect>
#include <QTableWidget>

HighscoreTable::HighscoreTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HighscoreTable)
{
    this->resize(600,600);
    this->setFixedSize(600,600);

    ui->setupUi(this);

    QPixmap bgImages(":/Imgs/Images/tank2.jpeg");
    bgImages = bgImages.scaled(600, 600);

    ui->label->setPixmap(bgImages);

    ui->tableView->setStyleSheet("QTableView { background-color: transparent;"
                                 "color: white;"
                                 "opacity: 200;"
                                 "font: bold 24px;};");

    ui->pushButton->setStyleSheet("QPushButton { background-color: transparent;"
                                  "border: 5px solid white;"
                                  "color: white;"
                                  "padding: 5px;"
                                  "border-radius: 7px;"
                                  "opacity: 200;"
                                  "font: bold 32px;}");

    db = new HighScore("../Tank-game/highscore");
    db->connectOpen();

    ui->tableView->setModel(db->modelBase());

    for(int i=0; i<3; i++) {
        ui->tableView->setColumnWidth(i,187);
    }

    for(int i=0; i<12; i++) {
        ui->tableView->setRowHeight(i,41);
    }

    db->connectClose();
}

HighscoreTable::~HighscoreTable()
{
    delete ui;
}

void HighscoreTable::on_pushButton_clicked()
{
    this->deleteLater();
}


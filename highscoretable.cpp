#include "highscoretable.h"
#include "ui_highscoretable.h"

HighscoreTable::HighscoreTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HighscoreTable)
{
    ui->setupUi(this);

    db = new HighScore("../Tank-game/highscore");
    db->connectOpen();

    ui->tableView->setModel(db->modelBase());
    for(int i=0; i<3; i++) {
        ui->tableView->setColumnWidth(i,189);
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


#ifndef HIGHSCORETABLE_H
#define HIGHSCORETABLE_H
#include "highscore.h"
#include <QWidget>

namespace Ui {
class HighscoreTable;
}

class HighscoreTable : public QWidget
{
    Q_OBJECT

public:
    explicit HighscoreTable(QWidget *parent = nullptr);
    ~HighscoreTable();

private slots:
    void on_pushButton_clicked();

private:
    Ui::HighscoreTable *ui;
    HighScore* db;
};

#endif // HIGHSCORETABLE_H

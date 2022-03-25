#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <QWidget>
#include "highscore.h"

namespace Ui {
class GameOver;
}

class GameOver : public QWidget
{
    Q_OBJECT

public:
    explicit GameOver(QWidget *parent = nullptr);
    ~GameOver();

    void setDestroyLabel(int destroyLabel );
    void setTimeLabel(double timeLabel );

    void setDestroy(int newDestroy);

    void setTime(double newTime);

    void showScore();
    void setPage();
    void hideUserScore();

private slots:
    void on_menuButton_clicked();

    void on_confirmButton_clicked();

private:
    Ui::GameOver *ui;
    HighScore* db;
    int destroy;
    double time;
};

#endif // GAMEOVER_H

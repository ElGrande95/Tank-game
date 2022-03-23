#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QMainWindow>

namespace Ui {
class StartPage;
}

class StartPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartPage(QWidget *parent = nullptr);
    ~StartPage();

private slots:
    void on_startButton_clicked();

    void on_exitButton_clicked();

    void on_highscoreButton_clicked();

private:
    Ui::StartPage *ui;
};

#endif // STARTPAGE_H

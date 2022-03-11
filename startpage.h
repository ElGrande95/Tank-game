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

private:
    Ui::StartPage *ui;
};

#endif // STARTPAGE_H

#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQueryModel>

class HighScore : public QObject
{
    Q_OBJECT
public:
    explicit HighScore(const QString &path = "");

    void deleteLastFromBase();
    void insertInBase(const QString& username, int destroy, double time);
    bool checkBase(int destroy, double time);
    QSqlQueryModel* modelBase() const;

    void connectClose();
    bool connectOpen();

private:
    QString mPath;
    QSqlDatabase db;
};

#endif // HIGHSCORE_H

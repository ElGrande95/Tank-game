#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QStandardItemModel>

class HighScore : public QObject
{
    Q_OBJECT
public:
    explicit HighScore(const QString &path = "");

    void deleteLastFromBase();
    void insertInBase(const QString& username, int destroy, double time);
    bool checkBase(int destroy, double time);
    QStandardItemModel* modelBase() const;

    void connectClose();
    bool connectOpen();

private:
    QString mPath;
    QSqlDatabase db;
};

#endif // HIGHSCORE_H

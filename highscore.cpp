#include "highscore.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QString>

HighScore::HighScore(const QString &path)
    :mPath(path)
{
}

void HighScore::deleteLastFromBase()
{
    QString username = "";
    int destroy = 0;
    double time = 0;

    QSqlQuery query ("SELECT * FROM highscore "
                     "order by Destroy desc, Time desc");
    int indicator = 0;

    while (query.next()) {

        indicator++;

        if(indicator == 10) {
            username = query.value(0).toString();
            destroy = query.value(1).toInt();
            time = query.value(2).toDouble();
            break;
        }
    }
    query.clear();
    query.prepare("delete from highscore where Username =  '"+username+"' and "
                  "Destroy = '"+QString::number(destroy)+"' and Time = '"+QString::number(time)+"' ");

    if(!query.exec()) {
        qDebug() << "nije brisalo";
    }
}

void HighScore::insertInBase(const QString& username, int destroy, double time)
{
    QSqlQuery query("INSERT INTO highscore (Username, Destroy, Time) "
                    "VALUES (:Username, :Destroy, :Time)");

    query.bindValue(0, username);
    query.bindValue(1, destroy);
    query.bindValue(2, time);

    if(!query.exec()) {
        qDebug() << "notInsert";
    }
}

bool HighScore::checkBase(int destroy, double time)
{
    int indicator = 0;

    QSqlQuery query("SELECT * FROM highscore "
                    "order by Destroy desc, Time desc");

    while (query.next()) {
        indicator++;

        if(indicator == 10){

            if ( query.value(1).toInt() < destroy || (query.value(1).toInt() == destroy && query.value(2).toDouble() < time))
            {

                deleteLastFromBase();
                return true;
            }
        }
    }
    if(indicator < 10)
        return true;

    return false;
}

QSqlQueryModel* HighScore::modelBase() const
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM highscore "
                    "order by Destroy desc, Time desc");
    model->setHeaderData(0, Qt::Horizontal, tr("Username"));
    model->setHeaderData(1, Qt::Horizontal, tr("Destroy"));
    model->setHeaderData(2, Qt::Horizontal, tr("Time"));

    return model;

}

void HighScore::connectClose()
{
    db.close();
    db.removeDatabase(QSqlDatabase::defaultConnection);
}

bool HighScore::connectOpen()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(mPath);
    if(!db.open()){
        return false;
    }
    else {
        return true;
    }
}

#ifndef AIRCRAFTTABLE_H
#define AIRCRAFTTABLE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QTimer>
#include <QSqlQuery>
#include <QSettings>
#include "aircrafts.h"


class aircraftTable : public QObject
{
    Q_OBJECT
public:
    explicit aircraftTable(QObject *parent = 0);
    ~aircraftTable();

signals:

private:
     QSqlDatabase m_db;
     QSqlQuery *qry;
     QSqlQuery *qry_del;
     QTimer *timer;
     QSettings settings;

private:
     void CountPos(QSqlQuery qry);
     void writeinFile(Aircrafts Acs);


private slots:
     void PrintCount();
};

#endif // AIRCRAFTTABLE_H

#ifndef DATAREADER_H
#define DATAREADER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QTimer>
#include <QSqlQuery>
#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "aircrafts.h"

class dataReader : public QObject
{
    Q_OBJECT
public:
    explicit dataReader(QObject *parent = 0);
    ~dataReader();

private:
     QSqlDatabase m_db;
     QSqlQuery *qry;
     QSqlQuery *qry_del;
     QSettings settings;

public:
     Aircrafts getFlights();

public slots:
};

#endif // DATAREADER_H

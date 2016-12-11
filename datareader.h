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
#include <QtConcurrent>

#include "aircrafts.h"
#include "datasender.h"

class dataReader : public QObject
{
    Q_OBJECT
public:
    explicit dataReader(QObject *parent = 0);
    ~dataReader();

private:
    void startsender();

private:
     QSqlDatabase m_db;
     QSqlQuery *qry;
     QSqlQuery *qry_del;
     QSettings settings;
     dataSender sender;
     unsigned int sec;
     Aircrafts previousValues;

public:
     Aircrafts getFlights();

public slots:
};

#endif // DATAREADER_H

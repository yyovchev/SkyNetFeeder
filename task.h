#ifndef TASK_H
#define TASK_H

#include "localreader.h"
#include <QObject>
#include <QDebug>

class Task: public QObject
{
Q_OBJECT
public:
    Task();
    ~Task();

public:
    localreader *m;

public slots:
    void CreateReader();

signals:
    void workFinished();
};

#endif // TASK_H

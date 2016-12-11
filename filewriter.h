#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QObject>

class fileWriter : public QObject
{
    Q_OBJECT
public:
    explicit fileWriter(QObject *parent = 0);

signals:

public slots:
};

#endif // FILEWRITER_H
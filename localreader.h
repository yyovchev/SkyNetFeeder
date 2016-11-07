#ifndef LOCALREADER_H
#define LOCALREADER_H

///=======================================================================
/// This class read the messages from local tcp server port 30002
/// hosted by dump1090. It removes the unnecessary parts of every message
/// and send it to binaryparser do decode it.
///


#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QString>
#include <QTcpServer>
#include <QTimer>
#include <QDataStream>
#include <QHashIterator>
#include <QThread>
#include <QtConcurrent>
#include <QTime>
#include <QFile>
#include <QTextStream>

#include "binaryparser.h"

#define STATE_WAIT 0
#define STATE_COLLECT 1
#define MAX_LINE_LENGTH 50



class localreader : public QObject
{
    Q_OBJECT
public:
    QString n_str;
    const char *charset;

private:
    static void writeFile(int count);

private:
    QTcpSocket *socket;
    BinaryParser parser;
    int state;
    char line[MAX_LINE_LENGTH+1];
    unsigned char bin_data[MAX_LINE_LENGTH/2];
    size_t read_pos;
    QTimer *timer;                              //when its disconnect, chek every 1.5s for connection
    QTimer *timer_wfile;
    int msgCount;

public:
    explicit localreader(QObject *parent = 0);
    int hex2bin(const char *input, unsigned int count, unsigned char *output);
    ~localreader();

private slots:
    void ReadData();
    void Disconnected();
    void TryToConnect();
    void wFile();

signals:

public slots:


};

#endif // LOCALREADER_H

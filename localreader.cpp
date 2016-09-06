#include "localreader.h"
#include <QDataStream>
#include <QHashIterator>
#include <QThread>

localreader::localreader(QObject *parent) : QObject(parent)
{

    state = STATE_WAIT;
    read_pos = 0;
    n_str="";
    socket = new QTcpSocket(this);
    socket->connectToHost("localhost",30002);

    timer = new QTimer();
    if (socket->waitForConnected(3000))
    {
        qDebug()<<"Connected to "+ socket->peerName();
        qDebug()<<"Port: "+ QString::number(socket->peerPort());
    }
    else
    {
        qDebug()<<"Not connected ...";
        timer->start(1500);
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(ReadData()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(Disconnected()));


    connect(timer,SIGNAL(timeout()),this,SLOT(TryToConnect()));



}

localreader::~localreader()
{
    socket->close();
    delete socket;
}

void localreader::ReadData()
{
    QByteArray data = socket->readAll();

       for (int i=0; i<data.size(); i++) {
           char c=data.at(i);
           if (state == STATE_WAIT) {
               if (c == '@' || c=='*') {
                   read_pos=0;
                   state = STATE_COLLECT;
               }
           } else if (state == STATE_COLLECT) {
               if (c == ';') {
                   line[read_pos]=0;
                   int offset = 6;

//                   qDebug()<<line;

                   hex2bin(line, read_pos, bin_data);

//                   parser.parseData(&bin_data[offset]);   old version of dump1090 ()
                   parser.parseData(bin_data);

                   state=STATE_WAIT;
               } else {
                   line[read_pos++]=c;
                   if (read_pos > MAX_LINE_LENGTH) {
                       qDebug() << "Error max length reached";
                       state=STATE_WAIT;
                   }
               }
           }
       }
}

int localreader::hex2bin(const char *input, unsigned int count, unsigned char *output) {
    unsigned int i;
    unsigned char c1,c2;
    if (count & 1) return 0; // count is odd

    for (i=1; i<count; i+=2) {
        c1 = input[i-1];
        c2 = input[i];

        if (c1 >= '0' && c1 <= '9') c1 -= 48;
        else if (c1 >= 'A' && c1 <= 'F') c1 -= 55;
        else if (c1 >= 'a' && c1 <= 'f') c1 -= 87;
        else return 0;

        if (c2 >= '0' && c2 <= '9') c2 -= 48;
        else if (c2 >= 'A' && c2 <= 'F') c2 -= 55;
        else if (c2 >= 'a' && c2 <= 'f') c2 -= 87;
        else return 0;

        output[i/2] = ((c1<<4) & 0xf0) | (c2 & 0x0f);
    }
    return count/2;
}

void localreader::Disconnected()
{
    qDebug()<<"Disconnected";
    timer->start(1500);

}

void localreader::TryToConnect()
{
    socket->connectToHost("localhost",30002);

    if (socket->waitForConnected(3000))
    {
        qDebug()<<"Connected to "+ socket->peerName();
        qDebug()<<"Port: "+ QString::number(socket->peerPort());
        timer->stop();
    }
}






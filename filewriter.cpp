#include "filewriter.h"

fileWriter::fileWriter(QObject *parent) : QObject(parent)
{
    filename = "/sourcers/qrysource.txt";
    file.setFileName(filename);
}

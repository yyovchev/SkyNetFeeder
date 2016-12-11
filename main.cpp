#include <QApplication>
#include <QCoreApplication>
#include <QThread>
#include <QObject>
#include <QSettings>
#include <iostream>
#include <QPoint>
#include <QEventLoop>
#include <QFile>

#include "mainwindow.h"
#include "task.h"
#include "locations.h"
#include "datasender.h"

using namespace std;



int main(int argc, char *argv[])
{

    //programs settings: OrganizationName, ApplicationName
    QApplication a(argc, argv);// set Application OrganizationName and ApplicationName for settings
    QCoreApplication::setOrganizationName("Skynet");
    QCoreApplication::setApplicationName("SkynetFeeder");

    // temp file

    // set settings
    QSettings settings;
    settings.setValue("Radar_Range",200);

    // Open thread which decode and store msgs in sqlite db
    QThread *thread = new QThread();
    Task *task = new Task();
    task->moveToThread(thread);
    QObject::connect(thread,SIGNAL(started()),task,SLOT(CreateReader()));
    QObject::connect(task, SIGNAL(workFinished()), thread, SLOT(quit()));

    QObject::connect( thread, SIGNAL(finished()), task, SLOT(deleteLater()));
    QObject::connect( thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

    //      start mainwindows
    MainWindow w;
    w.show();

    return a.exec();
}






#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QThread>
#include <QObject>
#include <QSettings>

#include "aircrafttable.h"
#include "task.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /// set Application OrganizationName and ApplicationName for settings
    QCoreApplication::setOrganizationName("YMY");
    QCoreApplication::setApplicationName("PlaneRadar02");

    // set settings
    // myLat myLong - Sliven's coordinates
    QSettings settings;
    settings.setValue("Radar_Range",150);
    settings.setValue("myLong",24.70);
    settings.setValue("myLat",42.50);

    // start mainwindows
    MainWindow w;
    w.show();

    // Open thread which decode and store msgs
    QThread *thread = new QThread();
    Task *task = new Task();
    task->moveToThread(thread);
    QObject::connect(thread,SIGNAL(started()),task,SLOT(CreateReader()));
    QObject::connect(task, SIGNAL(workFinished()), thread, SLOT(quit()));

    QObject::connect( thread, SIGNAL(finished()), task, SLOT(deleteLater()));
    QObject::connect( thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

    // Open thread which read db and del the old inf
    // put info in file which radar reads
    QThread *threadTab = new QThread();
    Task *task_T = new Task();
    task_T->moveToThread(threadTab);
    QObject::connect(threadTab,SIGNAL(started()),task_T,SLOT(CreateTab()));
    QObject::connect(task_T, SIGNAL(workFinished()), threadTab, SLOT(quit()));

    QObject::connect( threadTab, SIGNAL(finished()), task_T, SLOT(deleteLater()));
    QObject::connect( threadTab, SIGNAL(finished()), threadTab, SLOT(deleteLater()));
    threadTab->start();



    return a.exec();
}

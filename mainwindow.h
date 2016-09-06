#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTimer>
#include <QSqlQuery>
#include <QSettings>
#include "aircrafts.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

private:
    Ui::MainWindow *ui;
    QSettings settings;
};

#endif // MAINWINDOW_H

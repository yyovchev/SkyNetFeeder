#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QTimer>
#include <QString>

#include "aircrafts.h"
#include "datareader.h"


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
    void SetSources();

private:
    Ui::MainWindow *ui;
    QSettings settings;
    QTimer *timer;
    dataReader *DB_reader;

};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DB_reader = new dataReader(this);

    timer = new QTimer(this);
    timer->start(1000);

    connect(timer,SIGNAL(timeout()),this,SLOT(SetSources()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetSources()
{
    Aircrafts mid;

     mid = DB_reader->getFlights();
     ui->radar->setSources(mid);
}


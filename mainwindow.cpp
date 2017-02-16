#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    this->setFixedSize(300,200);

    // Tray icon
    _trayIcon = new QSystemTrayIcon(this);
    _trayIcon->setIcon(QIcon("://myappico.png"));
    _trayIcon->setVisible(true);
    connect(_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(show()));
    connect(_trayIcon, SIGNAL(messageClicked()),this, SLOT(show()));


    // Tray actions
    _actionExit = new QAction(this);
    _actionExit->setText(QString("Exit"));

    _actionPauseUnpause = new QAction(this);
    _actionPauseUnpause->setText(QString("Pause timer"));

    connect(_actionExit,SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(_actionPauseUnpause,SIGNAL(triggered(bool)), this, SLOT(pauseUnpause()));

    _trayIconMenu = new QMenu(this);

    _trayIconMenu->addAction(_actionPauseUnpause);
    _trayIconMenu->addAction(_actionExit);
    _trayIcon->setContextMenu(_trayIconMenu);


    // Timer

    _interval = 10;

    _timer = new QTimer(this);
    _timer->setTimerType(Qt::TimerType::CoarseTimer);
    _timer->start(_interval * 1000);


    connect(_timer, SIGNAL(timeout()), this, SLOT(showMessage()));

    _tick = new QTimer(this);
    _tick->start(1000);
    connect(_tick, SIGNAL(timeout()), this, SLOT(tickUpdate()));

    _time = new QTime();
    _time->start();

    _ui->spinBox_Interval->setValue(_interval);
    _ui->slider_interval->setValue(_interval);

    connect(_ui->button_stopTimer, SIGNAL(clicked()), this, SLOT(stopTimer()));
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::pauseUnpause()
{
    if(_timer->isActive())
    {
        int remaining = _timer->remainingTime();
        _timer->stop();
        _timer->setInterval(remaining);
        _actionPauseUnpause->setText(QString("Start timer"));
    }
    else
    {
        _timer->start(_interval);
        _time->start();
        _actionPauseUnpause->setText(QString("Pause timer"));
    }
}

void MainWindow::stopTimer()
{
    _timer->stop();
    _timerIsActive = false;
}

void MainWindow::showMessage()
{
    _trayIcon->showMessage( QString("Time to stretch!"),QString(""),QSystemTrayIcon::NoIcon, 1000);
    _time->start();
}

void MainWindow::tickUpdate()
{
    if(_timerIsActive)
    {
        int rem = (_timer->interval()-_time->elapsed()) / 1000 + 1;
        _ui->label_timeLeft->setText(QString::number(rem));
    }
    else
    {
        _ui->label_timeLeft->setText(QString("Timer is stopped."));
    }
}

void MainWindow::setTimer()
{
    _timer->stop();
    _timer->start(_interval * 1000);
    _time->start();
    _timerIsActive = true;
}


void MainWindow::on_spinBox_Interval_valueChanged(int val)
{
    _interval = val;
    _ui->slider_interval->setValue(val);
}

void MainWindow::on_slider_interval_valueChanged(int val)
{
    _interval = val;
    _ui->spinBox_Interval->setValue(val);
}

void MainWindow::on_button_setTimer_clicked()
{
    setTimer();
}

void MainWindow::on_button_Cancel_clicked()
{
    this->hide();
}

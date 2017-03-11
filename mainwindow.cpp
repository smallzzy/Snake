#include "mainwindow.h"

#include <QGraphicsView>
#include <QTimer>
#include <qaction.h>
#include <qmenubar.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <QIcon>
#include <QDebug>
#include <QThread>

#include <iostream>

#include "constants.h"
#include "gamecontroller.h"
#include "settingsdialog.h"
#include "neurosky.h"

MainWindow::MainWindow(QWidget *parent):
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    settings = new SettingsDialog;

    // main window size
    resize(900, 900);
    setWindowIcon(QIcon(":/images/snake_ico"));

    // fix graphicsview size on mainwindow
    ui->graphicsView->setFixedSize(500,500);

    // disable scrolling
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff );
    ui->graphicsView->viewport()->installEventFilter(this);

    // force scene view point
    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);
    scene->setSceneRect(0, 0, 200, 200);
    // view rect
//    QRect viewRect(0, 0, 2000, 2000);
//   ui->graphicsView->setSceneRect(0, 0, 200, 200);
//    qDebug()<<scene->sceneRect();

//       qDebug()<<ui->graphicsView->mapFromScene(100,100);
//      qDebug()<<ui->graphicsView->sceneRect();


 //  ui->graphicsView->centerOn(349,349);
   // ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

  // ui->graphicsView->scale(500,500);

	game = new GameController(*scene, this);

	serialThread = new QThread;

    connectActions();

    //initScene();
    initSceneBackground();


    QTimer::singleShot(0, this, SLOT(adjustViewSize()));
}

MainWindow::~MainWindow()
{
	//closeSerialPort();
	delete settings;
	delete ui;
}

void MainWindow::adjustViewSize()
{
	ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
}

void MainWindow::connectActions()
{
	//newGameAction = new QAction(tr("&New Game"), this);
	//newGameAction->setShortcuts(QKeySequence::New);
	//newGameAction->setStatusTip(tr("Start a new game"));
	connect(ui->actionNew_Game, &QAction::triggered, this, &MainWindow::newGame);

	//exitAction = new QAction(tr("&Exit"), this);
	//exitAction->setShortcut(tr("Ctrl+Q"));
	//exitAction->setStatusTip(tr("Exit the game"));
	connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

	//pauseAction = new QAction(tr("&Pause"), this);
	//pauseAction->setStatusTip(tr("Pause..."));
	connect(ui->actionPause, &QAction::triggered, game, &GameController::pause);

	//resumeAction = new QAction(tr("&Resume"), this);
	//resumeAction->setStatusTip(tr("Resume..."));
	connect(ui->actionResume, &QAction::triggered, game, &GameController::resume);

	//gameHelpAction = new QAction(tr("Game &Help"), this);
	//gameHelpAction->setShortcut(tr("Ctrl+H"));
	//gameHelpAction->setStatusTip(tr("Help on this game"));
	connect(ui->actionGame_Help, &QAction::triggered, this, &MainWindow::gameHelp);

	//aboutAction = new QAction(tr("&About"), this);
	//aboutAction->setStatusTip(tr("Show the application's about box"));
	connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);


	connect(ui->actionAbout_Qt, &QAction::triggered, qApp, QApplication::aboutQt);

    connect(ui->actionConfigure, &QAction::triggered, settings, &MainWindow::show);

    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);

	connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);

	ui->actionDisconnect->setEnabled(false);

}



void MainWindow::initSceneBackground()
{
    QPixmap bg(TILE_SIZE, TILE_SIZE);
    QPainter p(&bg);
    p.setBrush(QBrush(Qt::gray));
    p.drawRect(0, 0, TILE_SIZE, TILE_SIZE);

	ui->graphicsView->setBackgroundBrush(QBrush(bg));
}

void MainWindow::newGame()
{
	QTimer::singleShot(0, game, SLOT(gameOver()));
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("About this Game"), tr("<h2>Snake Game</h2>"
		"<p>Copyright &copy; XXX."
		"<p>This game is a small Qt application. It is based on the demo in the GitHub written by Devbean."));
}

void MainWindow::gameHelp()
{
	QMessageBox::about(this, tr("Game Help"), tr("Using direction keys to control the snake to eat the food"
		"<p>Space - pause & resume"));
}

// disable graphicview scroll
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->graphicsView->viewport() && event->type() == QEvent::Wheel) {
        //qDebug() << "SCroll";
        return true;
    }
    return false;
}


void MainWindow::openSerialPort()
{
	SettingsDialog::Settings p = settings->settings();
	std::string portName = "\\\\.\\";
	portName += p.name.toUtf8().constData();

	worker = new Neurosky(portName);
	worker->moveToThread(serialThread);
	
	connect(worker, SIGNAL(signalStatus(QString)), this, SLOT(updateStatus(QString)));
	connect(serialThread, SIGNAL(started()), worker, SLOT(process()));
	connect(worker, SIGNAL(finished()), serialThread, SLOT(quit()));
	connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
	connect(serialThread, SIGNAL(finished()), serialThread, SLOT(deleteLater()));
	serialThread->start();

	ui->actionConnect->setEnabled(false);
	ui->actionDisconnect->setEnabled(true);
}

void MainWindow::closeSerialPort()
{
	QTimer::singleShot(0, worker, SLOT(quit()));
}

void MainWindow::updateStatus(QString status)
{
	ui->statusbar->showMessage(status);
}
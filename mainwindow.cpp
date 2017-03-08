#include <QGraphicsView>
#include <QTimer>
#include <qaction.h>
#include <qmenubar.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <QIcon>

#include "constants.h"
#include "gamecontroller.h"
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent):
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	scene = new QGraphicsScene(this);
	ui->graphicsView->setScene(scene);

	
	game = new GameController(*scene, this);
    setCentralWidget(ui->graphicsView);
//    resize(600, 600);
    setFixedSize(600, 600);
    setWindowIcon(QIcon(":/images/snake_ico")); 

	createActions();

    initScene();
    initSceneBackground();

    QTimer::singleShot(0, this, SLOT(adjustViewSize()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::adjustViewSize()
{
	ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
}

void MainWindow::createActions()
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

	//aboutQtAction = new QAction(tr("About &Qt"), this);
	//aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
	connect(ui->actionAbout_Qt, &QAction::triggered, qApp, QApplication::aboutQt);
}



void MainWindow::initScene()
{
    scene->setSceneRect(-100, -100, 200, 200);
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

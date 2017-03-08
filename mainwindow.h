#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>

#include <QMainWindow>

#include "ui_mainwindow.h"


QT_BEGIN_NAMESPACE


class QLabel;
class QGraphicsScene;
class QGraphicsView;

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class GameController;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = nullptr);
	//MainWindow(QWidget * parent);
	~MainWindow();

private slots:
    void adjustViewSize();
	void newGame();
	void gameHelp();
	void about();

private:
	void createActions();

    void initScene();
    void initSceneBackground();

	Ui::MainWindow *ui;
    QGraphicsScene *scene;
    //QGraphicsView *view;

    GameController *game;

	//QAction *newGameAction;
	//QAction *pauseAction;
	//QAction *resumeAction;
	//QAction *exitAction;
	//QAction *gameHelpAction;
	//QAction *aboutAction;
	//QAction *aboutQtAction;
};

#endif // MAINWINDOW_H

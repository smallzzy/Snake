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
class SettingsDialog;
class Neurosky;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = nullptr);
	//MainWindow(QWidget * parent);
	~MainWindow();

protected:
     bool eventFilter(QObject *object, QEvent *event);

private slots:
    void openSerialPort();
	void closeSerialPort();

    void adjustViewSize();
	void updateStatus();
	void newGame();
	void gameHelp();
	void about();

private:
    void connectActions();

    void initSceneBackground();

	Ui::MainWindow *ui;
    QGraphicsScene *scene;

    SettingsDialog *settings;
    GameController *game;

	QThread* threadNeurosky;
	Neurosky* objectNeurosky;

	bool serialRunning;

};


#endif // MAINWINDOW_H

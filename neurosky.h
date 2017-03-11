#ifndef NEUROSKY_H
#define NEUROSKY_H

#include <QObject>
#include "NSK_Algo.h"


QT_BEGIN_NAMESPACE

class QObject;

QT_END_NAMESPACE

class Neurosky : public QObject {
    Q_OBJECT
 
public:
	Neurosky(std::string portName);
	~Neurosky();
	void sendStatus(QString status);

 
public slots:
	void quit();
    void process();
 
signals:
	void signalStatus(QString status);
    void finished();
   // void error(QString err);

protected:


private:
	std::string comPortName;
	int   connectionId = -1;

};

#endif

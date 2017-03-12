#ifndef NEUROSKY_H
#define NEUROSKY_H

#include <QObject>
#include <QMutex>
#include <QTimer>
#include <QWaitCondition>

#include "NSK_Algo.h"


QT_BEGIN_NAMESPACE

class QObject;


QT_END_NAMESPACE

class Neurosky : public QObject {
    Q_OBJECT
 
public:
	Neurosky(std::string portName);
	~Neurosky();
	friend void AlgoSdkCallback(sNSK_ALGO_CB_PARAM param);
	void quit2();

	struct Output
	{
		QString signalQuality;
		int attValue = 0;
		bool updated = false;
	};

	Output output() const;


public slots:
	//void quit();
    void process();
	void read();
 
signals:
	void signalUpdate();
    void signalFinished();
	void signalDestroyed();
	void signalError();

protected:


private:
	std::string comPortName;
	int   connectionId = -1;

	// Neurosky Running status
	bool bConnected = false;
	bool bInited = false;
	bool bRunning = false;

	bool bInterrupt = false;
	QMutex mutex;
	//QTimer *timer;
	QWaitCondition condition;

	Output currentOutput;

	int rawCount = 0;
	wchar_t buffer[100];
	short rawData[512] = { 0 };
	int lastRawCount = 0;

};

#endif
